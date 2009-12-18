// DEMO ON REGIONAL MAXIMA

#include <iostream>
#include <sstream>
#include <boost/format.hpp>

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

// #include <mln/debug/println.hh>
#include <mln/display/display_histo.hh>

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

#include <mln/transform/influence_zone_geodesic.hh>
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

// Version optimisée de mean_values

struct t_mean_lbl8_with_rgb8 : mln::Function_v2v< t_mean_lbl8_with_rgb8 >
{
  typedef mln::value::rgb8                     t_rgb8;
  typedef mln::algebra::vec<3,float>           t_vec3f;
  typedef mln::value::label_8                  t_lbl8;
  typedef t_lbl8                               argument;
  typedef t_rgb8                               result;
  typedef mln::image3d<t_lbl8>                 t_label;
  typedef mln::util::array<t_vec3f>            t_mean_array;

  const t_mean_array& _mean_array;

  t_mean_lbl8_with_rgb8(const t_mean_array& mean_array) :
    _mean_array(mean_array) {}

  result operator()(const argument& c) const
  {
    t_rgb8 tmp = _mean_array[c];

    return tmp;
  }
};


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
  typedef mln::value::rgb<n>                   t_rgbn;
  typedef mln::value::rgb8                     t_rgb8;
  typedef mln::value::label_8                  t_lbl8;
  typedef t_rgb8                               argument;
  typedef t_rgb8                               result;
  typedef mln::image3d<t_lbl8>                 t_label;
  typedef mln::fun::v2v::rgb8_to_rgbn<n>       t_rgb8_to_rgbn;

  const t_label&       _label;
  const t_rgb8_to_rgbn _convert;

  t_merge_lbl8_with_rgbn(const t_label& label) : _label(label) {}

  result operator()(const argument& c) const
  {
    t_rgb8 tmp_rgb8 = c;
    t_rgbn tmp_rgbn = _convert(c);

    if (0==mln::opt::at(_label,tmp_rgbn.blue(),tmp_rgbn.red(),tmp_rgbn.green()))
    {
      // tmp = mln::literal::black;
      // tmp = mln::literal::yellow;
      tmp_rgb8 = mln::literal::red;
    }

    return tmp_rgb8;
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

void compute_stats(const mln::image2d<mln::value::rgb8>&    i_input_rgb8,
		   const mln::image2d<mln::value::label_8>& l_input_lbl8,
		   const mln::image3d<unsigned>&            h_histo_rgbn,
		   const mln::image3d<mln::value::label_8>& l_histo_lbl8,
		   const mln::value::label_8&               n_labels)
{
  typedef mln::algebra::vec<3,float>                        t_vec3f;
  typedef mln::accu::math::sum<unsigned,unsigned>           t_sum;
  typedef mln::accu::stat::mean<t_vec3f,t_vec3f,t_vec3f>    t_mean;
  typedef mln::util::array<unsigned>                        t_count_array;
  typedef mln::util::array<t_vec3f>                         t_mean_array;

  mln::util::array<float>  abs((unsigned)(n_labels)+1);
  mln::util::array<float>  rel((unsigned)(n_labels)+1);
  unsigned                 nb = 0;

  for (unsigned i = 0; i <= n_labels; ++i)
  {
    abs[i] = 0.0;
    rel[i] = 0.0;
  }

  // COMPUTE THE SUM
  t_count_array count = mln::labeling::compute(t_sum(),
					       h_histo_rgbn,
					       l_histo_lbl8,
					       n_labels);

  // COMPUTE THE TOTAL
  for (unsigned i = 0; i <= n_labels; ++i)
  {
    unsigned c = count[i];
    nb += c;
  }

  // COMPUTE THE PERCENTAGES
  for (unsigned i = 0; i <= n_labels; ++i)
    if (0 < count[i])
    {
      abs[i] = ((float)count[i] / nb)*100.0;
      rel[i] = ((float)count[i] / (nb - count[0]))*100.0;
    }

  // COMPUTE THE MEAN

  t_mean_array mean = mln::labeling::compute(t_mean(),
					     i_input_rgb8,
					     l_input_lbl8,
					     n_labels);

  // CORRECT 0 LABEL STATS
  rel[0] = 0;
  mean[0][0] = 255.0;
  mean[0][1] = 255.0;
  mean[0][2] = 0.0;

  // PRINT STATS
  for (unsigned i = 0; i <= n_labels; ++i)
  {
    const t_vec3f& mean_v = mean[i];

    std::cout << boost::format("%2i|"
			       "r = %6.2f, g = %6.2f, b = %6.2f |"
			       "c = %7i, %%i = %5.2f, %%c = %5.2f")
      % i
      % mean_v[0]
      % mean_v[1]
      % mean_v[2]
      % count[i]
      % abs[i]
      % rel[i]
	      << std::endl;
    /*
    std::cout << i << "|("
	      << "r = " << unquant<n>(mean_v[0]) << ", "
	      << "g = " << unquant<n>(mean_v[1]) << ", "
	      << "b = " << unquant<n>(mean_v[2]) << ")|("
	      << "c = " << count[i]              << ", "
	      << "%i= " << abs[i]                << "%, "
	      << "%c= " << rel[i]                << "%)"
	      << std::endl;
    */
  }

  std::cout << std::endl << std::endl;

}

void save_histo(const mln::image2d<mln::value::rgb8>& input_rgb8,
		const std::string&                    name)
{
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;

  t_histo3d        histo   = mln::data::compute(t_histo3d_fun(), input_rgb8);
  t_image2d_int_u8 display = mln::display::display_histo3d_unsigned(histo);
  mln::io::pgm::save(display, name);
}


// n < 8, n is the degree of quantification
template <unsigned n>
void demo(const std::string& image, const unsigned min_vol)
{
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

  // START OF IMAGE PROCESSING CHAIN
  t_image2d_rgb8                                      i0_input; // input rgb8
  t_histo3d                                           h0_input; // histo input

  t_image2d_rgbn                                      i1_input; // input rgbn
  t_histo3d                                           h1_input; // histo input

  t_histo3d                                           h2_input; // histo opened
  t_image3d_lbl8                                      l2_histo; // label histo
  t_image2d_lbl8                                      l2_input; // label image
  t_image2d_rgb8                                      i2_merge; // merge label
  t_image2d_rgb8                                      i2_mean;  // mean label

  t_image3d_lbl8                                      l3_histo; // label dilated
  t_image2d_lbl8                                      l3_input; // label image
  t_image2d_rgb8                                      i3_merge; // merge label
  t_image2d_rgb8                                      i3_mean;  // mean label

  t_image3d_lbl8                                      l4_histo; // label iz
  t_image2d_lbl8                                      l4_input; // label image
  t_image2d_rgb8                                      i4_merge; // merge label
  t_image2d_rgb8                                      i4_mean;  // mean label

  t_image3d_lbl8                                      l5_histo; // label iz 2
  t_image2d_lbl8                                      l5_input; // label image
  t_image2d_rgb8                                      i5_merge; // merge label
  t_image2d_rgb8                                      i5_mean;  // mean label

  t_lbl8                                              n_lbl;    // nb class

  mln::io::ppm::load(i0_input, image.c_str());
  i1_input  = mln::data::transform(i0_input, t_rgb8_to_rgbn());
  h1_input  = mln::data::compute(t_histo3d_fun(), i1_input);
  h2_input  = mln::morpho::opening::volume(h1_input, mln::c6(), min_vol);
  l2_histo  = mln::labeling::regional_maxima(h2_input, mln::c6(), n_lbl);
  l3_histo  = mln::morpho::elementary::dilation(l2_histo, mln::c26());
  l4_histo  = mln::transform::influence_zone_geodesic(l2_histo, mln::c26());
  l5_histo  = mln::transform::influence_zone_geodesic(l2_histo, mln::c26(), 2);

  // END OF IMAGE PROCESSING CHAIN

// i2_mean = mln::data::transform(l0_input,t_mean_lbl8_with_rgb8(mean_array));

  // BEGIN DUMPING
  save_histo(i0_input, "h0_input.pgm");

  // without anything
  l2_input = mln::data::transform(i1_input, t_labeling_rgbn<n>(l2_histo));
  i2_mean  = mln::labeling::mean_values(i0_input, l2_input, n_lbl);
  i2_merge = mln::data::transform(i0_input,t_merge_lbl8_with_rgbn<n>(l2_histo));

  compute_stats(i0_input, l2_input, h1_input, l2_histo, n_lbl);

  mln::io::pgm::save(l2_input, "l2_input.pgm");
  mln::io::ppm::save(i2_mean,  "i2_mean.ppm");
  mln::io::ppm::save(i2_merge, "i2_merge.ppm");

  save_histo(i2_mean,  "h2_mean.pgm");
  save_histo(i2_merge, "h2_merge.pgm");

  // with dilation
  l3_input = mln::data::transform(i1_input, t_labeling_rgbn<n>(l3_histo));
  i3_mean  = mln::labeling::mean_values(i0_input, l3_input, n_lbl);
  i3_merge = mln::data::transform(i0_input,t_merge_lbl8_with_rgbn<n>(l3_histo));

  compute_stats(i0_input, l3_input, h1_input, l3_histo, n_lbl);

  mln::io::pgm::save(l3_input, "l3_input.pgm");
  mln::io::ppm::save(i3_mean,  "i3_mean.ppm");
  mln::io::ppm::save(i3_merge, "i3_merge.ppm");

  save_histo(i3_mean,  "h3_mean.pgm");
  save_histo(i3_merge, "h3_merge.pgm");

  // with geodesic influence
  l4_input = mln::data::transform(i1_input, t_labeling_rgbn<n>(l4_histo));
  i4_mean  = mln::labeling::mean_values(i0_input, l4_input, n_lbl);
  i4_merge = mln::data::transform(i0_input,t_merge_lbl8_with_rgbn<n>(l4_histo));

  compute_stats(i0_input, l4_input, h1_input, l4_histo, n_lbl);

  mln::io::pgm::save(l4_input, "l4_input.pgm");
  mln::io::ppm::save(i4_mean,  "i4_mean.ppm");
  mln::io::ppm::save(i4_merge, "i4_merge.ppm");

  save_histo(i4_mean,  "h4_mean.pgm");
  save_histo(i4_merge, "h4_merge.pgm");

  // with restricted geodesic influence
  l5_input = mln::data::transform(i1_input, t_labeling_rgbn<n>(l5_histo));
  i5_mean  = mln::labeling::mean_values(i0_input, l5_input, n_lbl);
  i5_merge = mln::data::transform(i0_input,t_merge_lbl8_with_rgbn<n>(l5_histo));

  compute_stats(i0_input, l5_input, h1_input, l5_histo, n_lbl);

  mln::io::pgm::save(l5_input, "l5_input.pgm");
  mln::io::ppm::save(i5_mean,  "i5_mean.ppm");
  mln::io::ppm::save(i5_merge, "i5_merge.ppm");

  save_histo(i5_mean,  "h5_mean.pgm");
  save_histo(i5_merge, "h5_merge.pgm");
  // END DUMPING

/*
  std::ostringstream				      name;
  std::ostringstream				      name2;
  std::ostringstream				      name3;

  name  << "input_rgb"  << n <<  ".ppm";
  name2 << "output_rgb" << n <<  ".ppm";
  name3 << "label_img"  << n <<  ".pgm";

  std::cout << "Labels  : " << n_labels << std::endl;
  std::cout << "Name    : " << name.str() << std::endl;

//  mln::io::ppm::save(input_rgbn,  name.str());

  std::cout << "timer2 : " << timer2.read() << std::endl;
  output_rgbn = mln::data::transform(input_rgbn,
				     t_merge_lbl8_with_rgbn<n>(label));
  // output_rgbn = merge<n>(input_rgbn, dilated);
  mln::io::ppm::save(output_rgbn, name2.str());

  label_img = mln::data::transform(input_rgbn,
				   t_labeling_rgbn<n>(label));
  // label_img = label_image<n>(input_rgbn, dilated);
  mln::io::pgm::save(label_img, name3.str());
*/

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


void usage()
{
  std::cout << std::endl;
  std::cout << "regional_maxima"       << std::endl;
  std::cout << "image          <name>" << std::endl;
  std::cout << "quantification {5,6}"  << std::endl;
  std::cout << "min_volume      1000"  << std::endl;

  // 3 paramètres en entré:
  // * l'image
  // * la quantification utilisée
  // * le volume minimale laissé pour les pics de l'histo

  // sorties:
  // * fichier de stats [etiquette|couleur moyenne|% image|% classé]
  // * image de labels
  // * image reconstruite avec la moyenne de chaque classe [i_mean]
  // * image reconstruite avec les couleurs de chaque classe [i_merge]
  // * histogramme original de l'image d'entrée [h_input]
  // * histogramme projeté de mean [h_mean]
  // * histogramme projeté de merge [h_merge]

  // *
}

int main(int argc, char* args[])
{
  if (argc != 3)
  {
    // const std::string& image     = OLENA_IMG_PATH"/fly.ppm";
    const std::string& image     = SCRIBO_PPM_IMG_PATH"/mp00082c_50p.ppm";
    // const std::string& image     = OLENA_IMG_PATH"/tiny.ppm";
    // const std::string image      = OLENA_IMG_PATH"/tiny.ppm";
    const unsigned    min_volume = 1000;
    // const unsigned    min_volume = atoi(args[2]);

    switch(args[2][0])
    {
      case '2': demo<2>(image, min_volume); break;
      case '3': demo<3>(image, min_volume); break;
      case '4': demo<4>(image, min_volume); break;
      case '5': demo<5>(image, min_volume); break;
      case '6': demo<6>(image, min_volume); break;
      case '7': demo<7>(image, min_volume); break;
      case '8': demo<8>(image, min_volume); break;
      default:  demo<5>(image, min_volume); break;
      // default: usage(); break;
    }
  }
  else
    usage();
}
