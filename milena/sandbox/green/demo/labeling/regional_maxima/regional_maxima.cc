// DEMO ON REGIONAL MAXIMA

#include <iostream>
#include <sstream>

#include <mln/img_path.hh>

#include <mln/accu/math/sum.hh>
#include <mln/accu/stat/histo3d_rgb.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>

#include <mln/algebra/vec.hh>

// #include <mln/arith/revert.hh>
#include <mln/arith/diff_abs.hh>

#include <mln/core/macros.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>
// #include <mln/data/stretch.hh>
#include <mln/data/transform.hh>

// #include <mln/display/display_histo.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
// #include <mln/io/plot/save_image_sh.hh>

#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/labeling/compute.hh>

#include <mln/literal/colors.hh>

// #include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/opening/volume.hh>
#include <mln/morpho/elementary/dilation.hh>

#include <mln/opt/at.hh>

// #include <mln/pw/value.hh>

#include <mln/util/array.hh>
#include <mln/util/timer.hh>

#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rgb.hh>
#include <mln/value/int_u.hh>


template <unsigned n, unsigned ch>
struct t_channel : mln::Function_v2v< t_channel<n,ch> >
{
  typedef mln::value::rgb<n>    t_rgbn;
  typedef mln::value::int_u<n>  t_int_un;
  typedef t_rgbn                argument;
  typedef t_int_un              result;

  result operator()(const argument& c) const
  {
    result tmp;

    switch(ch)
    {
      case 0: tmp = c.red();   break;
      case 1: tmp = c.green(); break;
      case 2: tmp = c.blue();  break;
    }

    return tmp;
  }
};


// version optimisée de labeling

template <unsigned n>
struct t_labeling_rgbn : mln::Function_v2v< t_labeling_rgbn<n> >
{
  typedef mln::value::rgb<n>    t_rgbn;
  typedef mln::value::label_8   t_lbl8;
  typedef t_rgbn                argument;
  typedef t_lbl8                result;
  typedef mln::image3d<t_lbl8>  t_label;

  const t_label& _label;

  t_labeling_rgbn(const t_label& label) : _label(label) {}

  result operator()(const argument& c) const
  {
    t_lbl8 tmp = mln::opt::at(_label, c.blue(), c.red(), c.green());

    return tmp;
  }
};


// version non optimisée de label

template <unsigned n>
mln::image2d<mln::value::label_8>
label_image(const mln::image2d< mln::value::rgb<n> >& input,
	    const mln::image3d< mln::value::label_8>& label)
{
  mln::image2d<mln::value::label_8> output;

  initialize(output, input);

  mln_piter(mln::image2d< mln::value::label_8 >) po(output.domain());
  mln_piter(mln::image2d< mln::value::rgb<n>  >) pi(input.domain());

  for_all_2(po, pi)
  {
    const mln::value::rgb<n>& vi = input(pi);

    output(po) = mln::opt::at(label, vi.blue(), vi.red(), vi.green());
  }

  return output;
}

template <unsigned n>
unsigned unquant(const float& value)
{
  unsigned size   = pow(2,(8-n));
  unsigned result = value * size;

  return result;
}

template <unsigned n>
void print_count2(const mln::image2d<mln::value::rgb<n> >& input_rgbn,
		  const mln::image3d<unsigned>&            histo,
		  const mln::image3d<mln::value::label_8>& label,
		  const unsigned                           n_labels)
{
  typedef mln::value::label_8                               t_lbl8;
  typedef mln::value::rgb<n>                                t_rgbn;
  typedef mln::value::int_u<n>                              t_int_un;
  typedef mln::algebra::vec<3,float>                        t_vec3f;
  typedef mln::accu::math::sum<unsigned,unsigned>           t_sum;
  typedef mln::accu::stat::mean<t_vec3f,t_vec3f,t_vec3f>    t_mean;
  typedef mln::accu::math::sum<t_vec3f,t_vec3f>             t_diff;
  typedef mln::accu::stat::variance<float,float,float>      t_var;
  typedef mln::image2d<t_lbl8>                              t_image2d_lbl8;
  typedef mln::image2d<t_rgbn>                              t_image2d_rgbn;
  typedef mln::image2d<t_int_un>                            t_image2d_int_un;

  mln::util::array<t_mean> mean((unsigned)(n_labels)+1);
//   mln::util::array<t_diff> diff((unsigned)(n_labels)+1);
  mln::util::array<t_var> var_red((unsigned)(n_labels)+1);
  mln::util::array<t_var> var_green((unsigned)(n_labels)+1);
  mln::util::array<t_var> var_blue((unsigned)(n_labels)+1);
  mln::util::array<t_sum>  count((unsigned)(n_labels)+1);
  mln::util::array<float>  abs((unsigned)(n_labels)+1);
  mln::util::array<float>  rel((unsigned)(n_labels)+1);
  unsigned                 nb = 0;

  for (unsigned i = 0; i <= n_labels; ++i)
  {
    count(i).init();
    mean(i).init();
//     diff(i).init();
    var_red(i).init();
    var_green(i).init();
    var_blue(i).init();
    abs[i] = 0.0;
    rel[i] = 0.0;
  }

  mln::labeling::compute(count, histo, label, n_labels);

  for (unsigned i = 0; i <= n_labels; ++i)
  {
    unsigned c = count[i];
    nb += c;
  }

  for (unsigned i = 0; i <= n_labels; ++i)
    if (0 < count[i])
    {
      abs[i] = ((float)count[i] / nb)*100.0;
      rel[i] = ((float)count[i] / (nb - count[0]))*100.0;
    }

  t_image2d_lbl8 label_img = mln::data::transform(input_rgbn,
						  t_labeling_rgbn<n>(label));

  mln::labeling::compute(mean, input_rgbn, label_img, n_labels);


//   t_image2d_rgbn mean_rgbn = mln::labeling::mean_values(input_rgbn,
// 						       label_img,
// 						       n_labels);

//   t_image2d_int_un mean_red  =mln::data::transform(mean_rgbn,t_channel<n>());
//   t_image2d_int_un mean_green=mln::data::transform(mean_rgbn,t_channel<n>());
//   t_image2d_int_un mean_blue =mln::data::transform(mean_rgbn,t_channel<n>());

  t_image2d_int_un input_red  =mln::data::transform(input_rgbn,
						    t_channel<n,0>());
  t_image2d_int_un input_green=mln::data::transform(input_rgbn,
						    t_channel<n,1>());
  t_image2d_int_un input_blue =mln::data::transform(input_rgbn,
						    t_channel<n,2>());

// FIXME VARIANCE NEGATIVE DANS LES RESULTATS !!

//   mln::labeling::compute(var_red,   input_rgbn, label_img, n_labels);
//   mln::labeling::compute(var_green, input_rgbn, label_img, n_labels);
//   mln::labeling::compute(var_blue,  input_rgbn, label_img, n_labels);

//   t_image2d_rgbn diff_rgbn = mln::arith::diff_abs(input_rgbn, mean_rgbn);

  std::cout <<  mln::labeling::compute(var_red,   input_red,   label_img, n_labels)(29) << std::endl;
//   mln::labeling::compute(t_var(),   input_red,   label_img, n_labels);
  mln::labeling::compute(var_green, input_green, label_img, n_labels);
  mln::labeling::compute(var_blue,  input_blue,  label_img, n_labels);

  for (unsigned i = 0; i <= n_labels; ++i)
  {
    if (5.0 < abs[i] && 10.0 < rel[i])
    {
      const t_vec3f& mean_v = mean[i];
//       const t_vec3f& diff_v = diff[i];

      std::cout << i << " :[" << unquant<n>(mean_v[0])
// 		<< "("        << diff_v[0]
		<< "("        << var_red[i]
		<< "),"       << unquant<n>(mean_v[1])
// 		<< "("        << diff_v[1]
		<< "("        << var_green[i]
		<< "),"       << unquant<n>(mean_v[2])
// 		<< "("        << diff_v[2]
		<< "("        << var_blue[i]
		<< ")]- "     << count[i]
		<< " - "      << abs[i]
		<< " - "      << rel[i]
		<< std::endl;
    }
  }
}

void print_count(const mln::image3d<unsigned>&            histo,
		 const mln::image3d<mln::value::label_8>& label,
		 const unsigned                           n_labels)
{
  unsigned count[255];
  unsigned red[255];
  unsigned green[255];
  unsigned blue[255];
  unsigned nb = 0;
  unsigned tmp = 0;

  for (unsigned i = 0; i <= n_labels; ++i)
  {
    count[i] = 0;
    red[i]   = 0;
    green[i] = 0;
    blue[i]  = 0;
  }

  mln_piter_(mln::image3d<unsigned>) p(histo.domain());

  for_all(p)
  {
    count[label(p)] += histo(p);
    red[label(p)]   += histo(p) * p.row();
    green[label(p)] += histo(p) * p.col();
    blue[label(p)]  += histo(p) * p.sli();
    nb              += histo(p);
    ++tmp;
  }

  std::cout << std::endl;

  std::cout << "nb  : " << nb  << std::endl;
  std::cout << "tmp : " << tmp << std::endl;

  std::cout << std::endl;

  for (unsigned i = 0; i <= n_labels; ++i)
    if (0 < count[i])
    {
      float percentage_abs = ((float)count[i] / nb)*100.0;
      float percentage_rel = ((float)count[i] / (nb - count[0]))*100.0;

      red[i]   = red[i]   / count[i];
      green[i] = green[i] / count[i];
      blue[i]  = blue[i]  / count[i];

      std::cout << "count[" << i      << "]("
		<< red[i]   << ", "   << green[i] << ", "
		<< blue[i]  << ") = " << count[i] << "("
		<< percentage_abs     << "%)";

      if (0 < i)
	std::cout << "[" << percentage_rel << "%]";

      std::cout << std::endl;
    }

  std::cout << std::endl;
}

// Version optimisée de merge

template <unsigned n>
struct t_merge_lbl8_with_rgbn : mln::Function_v2v< t_merge_lbl8_with_rgbn<n> >
{
  typedef mln::value::rgb<n>    t_rgbn;
  typedef mln::value::label_8   t_lbl8;
  typedef t_rgbn                argument;
  typedef t_rgbn                result;
  typedef mln::image3d<t_lbl8>  t_label;

  const t_label& _label;

  t_merge_lbl8_with_rgbn(const t_label& label) : _label(label) {}

  result operator()(const argument& c) const
  {
    t_rgbn tmp = c;

    if (0 == mln::opt::at(_label, c.blue(), c.red(), c.green()))
    {
      tmp = mln::literal::black;
    }

    return tmp;
  }
};

// version non optimisée de merge

template <unsigned n>
mln::image2d< mln::value::rgb<n> >
merge(const mln::image2d< mln::value::rgb<n> >&  input,
      const mln::image3d< mln::value::label_8 >& label)
{
  mln::image2d<mln::value::rgb<n> > output;

  mln::initialize(output, input);
  // mln::data::fill(output, mln::literal::green);

  mln_piter(mln::image2d< mln::value::rgb<n> >) pi(input.domain());
  mln_piter(mln::image2d< mln::value::rgb<n> >) po(output.domain());

  for_all_2(pi, po)
  {
    const mln::value::rgb<n>& vi = input(pi);
    mln::value::rgb<n>&       vo = output(po);

    if (0 < mln::opt::at(label,vi.blue(),vi.red(),vi.green()))
    {
      vo.red()   = vi.red();
      vo.green() = vi.green();
      vo.blue()  = vi.blue();
    }
    else
      vo = mln::literal::black;
  }

  return output;
}


//
// Theo regional maxima image processing chain.
//

// FIXME C'est la dilatation qui fait apparaître des classes < min_volume.
// Une couleur se dilate au détriment du fond et des autres couleurs.

int main2()
{
  const unsigned     min_volume = 1000;
  //const std::string& image      = OLENA_IMG_PATH"/fly.ppm";
  const std::string& image      = SCRIBO_PPM_IMG_PATH"/mp00082c_50p.ppm";
  //const std::string& image = OLENA_IMG_PATH"/tiny.ppm";

  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<5>                          t_rgb5;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgb5>                        t_image2d_rgb5;
  typedef mln::image3d<t_lbl8>                        t_image3d_lbl8;
  typedef mln::image3d<unsigned>                      t_image3d_unsigned;
  typedef mln::fun::v2v::rgb8_to_rgbn<5>              t_rgb8_to_rgb5;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
  typedef mln::accu::math::sum<unsigned,unsigned>     t_sum;

  mln::util::timer timer;

  // START IMAGE PROCESSING CHAIN
  timer.start();

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_rgb5                                      input_rgb5;
  t_image3d_unsigned                                  histo;
  t_image3d_unsigned                                  opened;
  t_image3d_lbl8                                      label;
  t_image3d_lbl8                                      dilated;
  t_lbl8                                              n_labels;


  mln::io::ppm::load(input_rgb8, image.c_str());
  input_rgb5 = mln::data::transform(input_rgb8, t_rgb8_to_rgb5());
  histo      = mln::data::compute(t_histo3d_fun(), input_rgb5);
  opened     = mln::morpho::opening::volume(histo,   mln::c6(), min_volume);
  label      = mln::labeling::regional_maxima(opened, mln::c6(), n_labels);
  dilated    = mln::morpho::elementary::dilation(label, mln::c26());

  mln::util::array<t_sum> length((unsigned)(n_labels)+1);

  for (unsigned i = 0; i <= n_labels; ++i)
    length(i).init();

  mln::labeling::compute(length, histo, dilated, n_labels);

  timer.stop();
  // STOP IMAGE PROCESSING CHAIN

  std::cout << "Done in " << timer.read() << " ms" << std::endl;
  std::cout << "n_labels : " << n_labels << std::endl;

  for (unsigned i = 0; i <= n_labels; ++i)
  {
    std::cout << "count[" << i << "] = " << length[i] << std::endl;
  }

  print_count(histo,label,n_labels);

  //mln::io::plot::save_image_sh(histo, "histo.sh");
  //mln::io::plot::save_image_sh(histo, "opened.sh");
  //mln::io::plot::save_image_sh(label, "label.sh");

  return 0;
}

// n < 8, n is the degree of quantification
template <unsigned n>
void demo()
{
  const unsigned     min_volume = 1000;
  //const std::string& image      = OLENA_IMG_PATH"/fly.ppm";
  const std::string& image      = SCRIBO_PPM_IMG_PATH"/mp00082c_50p.ppm";
  //const std::string& image = OLENA_IMG_PATH"/tiny.ppm";

  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::image3d<t_lbl8>                        t_image3d_lbl8;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::image2d<unsigned>                      t_histo2d;
  typedef mln::fun::v2v::rgb8_to_rgbn<n>              t_rgb8_to_rgbn;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;

  mln::util::timer timer;

  // START OF IMAGE PROCESSING CHAIN
  timer.start();

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_rgbn                                      input_rgbn;
  t_image2d_rgbn                                      output_rgbn;
  // t_image2d_rgbn                                      mean_rgb5;
  t_histo3d                                           histo;
  // t_image2d_int_u8                                    projected;
  // t_image2d_int_u8                                    filtered;
  t_histo3d                                           opened;
  t_image3d_lbl8                                      label;
  t_image2d_lbl8                                      label_img;
  t_image3d_lbl8                                      dilated;
  t_lbl8                                              n_labels;
  // t_rgbn                                              value_rgbn;

  mln::io::ppm::load(input_rgb8, image.c_str());
  input_rgbn = mln::data::transform(input_rgb8, t_rgb8_to_rgbn());
  histo      = mln::data::compute(t_histo3d_fun(), input_rgbn);
  opened     = mln::morpho::opening::volume(histo, mln::c6(), min_volume);
  label      = mln::labeling::regional_maxima(opened, mln::c6(), n_labels);
  dilated    = mln::morpho::elementary::dilation(label, mln::c26());

  timer.stop();
  // END OF IMAGE PROCESSING CHAIN

  std::ostringstream				      name;
  std::ostringstream				      name2;
  std::ostringstream				      name3;

  name  << "input_rgb"  << n <<  ".ppm";
  name2 << "output_rgb" << n <<  ".ppm";
  name3 << "label_img"  << n <<  ".pgm";

  std::cout << "Done in : " << timer.read() << " s" << std::endl;
  std::cout << "Labels  : " << n_labels << std::endl;
  std::cout << "Name    : " << name.str() << std::endl;

  mln::io::ppm::save(input_rgbn,  name.str());

  mln::util::timer timer2;

  timer2.start();
  print_count2(input_rgbn, histo, dilated, n_labels);
  timer2.stop();
  std::cout << "timer2 : " << timer2.read() << std::endl;
  output_rgbn = mln::data::transform(input_rgbn,
				     t_merge_lbl8_with_rgbn<n>(label));
  // output_rgbn = merge<n>(input_rgbn, dilated);
  mln::io::ppm::save(output_rgbn, name2.str());

  label_img = mln::data::transform(input_rgbn,
				   t_labeling_rgbn<n>(label));
  // label_img = label_image<n>(input_rgbn, dilated);
  mln::io::pgm::save(label_img, name3.str());


  // localiser les couleurs sur l'image (fond en black, le reste)

  // La dilatation englobe beaucoup plus de couleur, mais celles-ci ne
  // sont pas forcément présentes dans l'image. Du coup, les classes ne
  // bougent pas démeusurément.

//   mln::io::ppm::save(input_rgb5, "input_rgb5.ppm");
//   mln::io::plot::save_image_sh(input_rgb8, "input_rgb8.sh");
//   mln::io::plot::save_image_sh(input_rgb5, "input_rgb5.sh");
//   projected = mln::display::display_histo3d_unsigned(histo);
//   mln::io::pgm::save(projected, "histo.pgm");
//   mln::io::plot::save_image_sh(histo, "histo.sh");
//   mln::io::plot::save_image_sh(opened, "opened.sh");
//   filtered = mln::display::display_histo3d_unsigned(opened);
//   mln::io::pgm::save(filtered, "filtered.pgm");
//   mln::io::plot::save_image_sh(label, "label.sh");
  // mln::io::plot::save_image_sh(dilated, "dilated.sh");

  /*
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
  */
}


int main()
{
  demo<2>(); //  2.26 s
  demo<3>(); //  2.29 s
  demo<4>(); //  2.29 s
  demo<5>(); //  2.37 s
  demo<6>(); //  3.19 s
  demo<7>(); // 11.43 s
  demo<8>(); // 96.19 s
}
