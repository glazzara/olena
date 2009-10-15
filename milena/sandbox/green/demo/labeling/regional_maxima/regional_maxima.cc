// DEMO ON REGIONAL MAXIMA

#include <mln/clustering/kmean2d.hh>

#include <iostream>
#include <sstream>

#include <mln/img_path.hh>
#include <mln/pw/value.hh>

#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/accu/stat/histo3d_rgb.hh>
#include <mln/data/compute.hh>

#include <mln/arith/revert.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/routine/initialize.hh>
#include <mln/literal/colors.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/opening/volume.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/data/stretch.hh>

#include <mln/display/display_histo.hh>
#include <mln/labeling/mean_values.hh>

mln::image2d<mln::value::label_8>
label_image(const mln::image2d<mln::value::rgb<5> >& input,
	    const mln::image3d<mln::value::label_8>& label)
{
  mln::image2d<mln::value::label_8> output(input.nrows(),
					   input.ncols());

  mln_piter_(mln::image2d<mln::value::label_8>) po(output.domain());
  mln_piter_(mln::image2d<mln::value::rgb<5> >) pi(input.domain());

  for_all_2(po, pi)
  {
    output(po) = label(mln::point3d(input(pi).blue(),
				    input(pi).red(),
				    input(pi).green()));
  }

  return output;
}



void print_count(const mln::image3d<unsigned>&            histo,
		 const mln::image3d<mln::value::label_8>& label,
		 const unsigned                           n_labels)
{
  unsigned count[255];
  unsigned red[255];
  unsigned green[255];
  unsigned blue[255];

  for (unsigned i = 0; i < n_labels; ++i)
  {
    count[i] = 0;
    red[i]   = 0.0;
    green[i] = 0.0;
    blue[i]  = 0.0;
  }

  mln_piter_(mln::image3d<unsigned>)            ph(histo.domain());
  mln_piter_(mln::image3d<mln::value::label_8>) pl(label.domain());

  for_all_2(ph, pl)
  {
    count[label(pl)] += histo(ph);
    red[label(pl)]   += histo(ph) * pl.row();
    green[label(pl)] += histo(ph) * pl.col();
    blue[label(pl)]  += histo(ph) * pl.sli();
  }

  std::cout << std::endl;

  for (unsigned i = 0; i < n_labels; ++i)
  {
    red[i]   = red[i]   / count[i];
    green[i] = green[i] / count[i];
    blue[i]  = blue[i]  / count[i];

    std::cout << "count[" << i      << "]("
	      << red[i]   << ", "   << green[i] << ", "
	      << blue[i]  << ") = " << count[i] << std::endl;
  }

  std::cout << std::endl;
}

mln::image2d<mln::value::rgb<5> >
merge(const mln::image2d<mln::value::rgb<5> >& input,
      const mln::image3d<mln::value::label_8>& label)
{
  mln::image2d<mln::value::rgb<5> > output;

  mln::initialize(output, input);

  mln_piter_(mln::image2d<mln::value::rgb<5> >) pi(input.domain());
  mln_piter_(mln::image2d<mln::value::rgb<5> >) po(output.domain());

  for_all_2(pi, po)
  {
    if (0 < label(mln::point3d(input(pi).blue(),
			       input(pi).red(),
			       input(pi).green())))
    {
      output(po).red()   = input(pi).red();
      output(po).green() = input(pi).green();
      output(po).blue()  = input(pi).blue();
    }
    else
      output(po) = mln::literal::red;
    //output(po) = mln::literal::black;
  }

  return output;
}


//
// Regional maxima image processing chain.
// RGB8
//
void do_demo(const std::string& image)
{
  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<5>                          t_rgb5;
  typedef mln::image3d<t_lbl8>                        t_image3d_lbl8;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgb5>                        t_image2d_rgb5;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::image2d<unsigned>                      t_histo2d;
  typedef mln::fun::v2v::rgb8_to_rgbn<5>              t_rgb8_to_rgbn;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_rgb5                                      input_rgb5;
  t_image2d_rgb5                                      output_rgb5;
  t_image2d_rgb5                                      mean_rgb5;
  t_histo3d                                           histo;
  t_image2d_int_u8                                    projected;
  t_image2d_int_u8                                    filtered;
  t_histo3d                                           opened;
  t_image3d_lbl8                                      label;
  t_image2d_lbl8                                      label_img;
  t_image3d_lbl8                                      dilated;
  t_lbl8                                              n_labels;
  t_rgb5                                              value_rgb5;


  // IMAGE LOADING PHASE
  std::cout << "Image loading phase ..." << std::endl;
  mln::io::ppm::load(input_rgb8, image.c_str());
  input_rgb5 = mln::data::transform(input_rgb8, t_rgb8_to_rgbn());
  mln::io::ppm::save(input_rgb5, "input_rgb5.ppm");


  // HISTO COMPUTING PHASE
  std::cout << "Histo computing phase ..." << std::endl;
  histo     = mln::data::compute(t_histo3d_fun(), input_rgb5);
  projected = mln::display::display_histo3d_unsigned(histo);

  mln::io::pgm::save(projected, "histo.pgm");
  //mln::io::plot::save_image_sh(histo, "histo.sh");

  // HISTO FILTERING PHASE
  std::cout << "Histo filtering phase ..." << std::endl;;
  opened = mln::morpho::opening::volume(histo, mln::c6(), 1000);
  mln::io::plot::save_image_sh(opened, "opened.sh");
  filtered = mln::display::display_histo3d_unsigned(opened);
  mln::io::pgm::save(filtered, "filtered.pgm");


  // HISTO LABELING PHASE
  std::cout << "Histo labeling phase ..." << std::endl;
  label    = mln::labeling::regional_maxima(opened, mln::c6(), n_labels);
  mln::io::plot::save_image_sh(label, "label.sh");


  // HISTO DILATING PHASE
  std::cout << "Histo dilating phase ..." << std::endl;
  dilated  = mln::morpho::elementary::dilation(label, mln::c18());
  mln::io::plot::save_image_sh(dilated, "dilated.sh");

  // PRINTING PHASE
  std::cout << "Labels : " << n_labels << std::endl;
  print_count(histo, dilated, n_labels);

  // OUTPUT PHASE
  std::cout << "Output phase ..." << std::endl;
  output_rgb5 = merge(input_rgb5, dilated);
  //mln::io::ppm::save(output_rgb5,  "output_rgb5.ppm");
  mln::io::plot::save_image_sh(output_rgb5,  "output_rgb5.sh");


  // LABELING IMG OUTPUT
  std::cout << "Labeling img output phase ..." << std::endl;
  label_img = label_image(input_rgb5, dilated);
  mln::io::pgm::save(label_img, "label_img.pgm");


  // BUILDING MEAN VALUES
  std::cout << "Building mean values phase ..." << std::endl;
  mean_rgb5 = mln::labeling::mean_values(input_rgb5, label_img, n_labels);
  mln::io::ppm::save(mean_rgb5, "mean.ppm");
}


void demo(const std::string& image     = SCRIBO_PPM_IMG_PATH"/mp00082c_50p.ppm",
	  //const std::string& image     = OLENA_IMG_PATH"/house.ppm",
	  const unsigned     k_center  = 2,
	  //const unsigned     k_center  = 3,
	  const unsigned     n_times   = 10,
	  const unsigned     watch_dog = 10)
{
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Launching the demo with these parameters" << std::endl;
  std::cout << "image     : " << image                    << std::endl;
  std::cout << "k_center  : " << k_center                 << std::endl;
  std::cout << "n_times   : " << n_times                  << std::endl;
  std::cout << "watch_dog : " << watch_dog                << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  do_demo(image);
}

void usage(const int argc, const char *args[])
{
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "argc    : " << argc                       << std::endl;

  for (int i = 0; i < argc; ++i)
    std::cout << "args[" << i << "] : " << args[i] << std::endl;

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "usage: kmean2d [image [k_center [n_times [watch_dog]]]]"
	    << std::endl;
  std::cout << "pbm image          (points to work with)" << std::endl;
  std::cout << "unsigned k_center  (number of centers)"   << std::endl;
  std::cout << "unsigned n_times   (number of launching)" << std::endl;
  std::cout << "unsigned watch_dog (convergence loop)"    << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

bool char_to_unsigned(const bool status, const char *arg, unsigned& val)
{
  bool result = false;

  if (status)
  {
    std::istringstream arg_stream(arg);

    arg_stream >> val;

    result = !arg_stream.fail();
  }

  return result;
}

bool char_to_string(const bool status, const char *arg, std::string& val)
{
  bool result = false;

  if (status)
  {
    std::istringstream arg_stream(arg);

    arg_stream >> val;

    return !arg_stream.fail();
  }

  return result;
}

int main(const int argc, const char *args[])
{
  std::string image("top");
  unsigned    k_center;
  unsigned    watch_dog;
  unsigned    n_times;
  bool        status = true;

  switch (argc)
  {
  case 5:  status = char_to_unsigned(status, args[4], watch_dog);
  case 4:  status = char_to_unsigned(status, args[3], n_times);
  case 3:  status = char_to_unsigned(status, args[2], k_center);
  case 2:  status = char_to_string(status, args[1], image); break;
  case 1:  status = true; break;
  default: status = false;
  }

  if (status)
  {
    switch (argc)
    {
    case 1: demo(); break;
    case 2: demo(image); break;
    case 3: demo(image, k_center); break;
    case 4: demo(image, k_center, n_times); break;
    case 5: demo(image, k_center, n_times, watch_dog); break;
    }
  }
  else
    usage(argc, args);

  return 0;
}
