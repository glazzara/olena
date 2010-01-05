// TOOLS ==> regmax on histo

#include <iostream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/macros.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/data/compute.hh>

#include <mln/debug/println.hh>
#include <mln/display/display_histo.hh>

#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/labeling/regional_maxima.hh>

#include <mln/morpho/opening/volume.hh>

#include <mln/value/label_8.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

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

void mk_regmax(const std::string& input,
	       const std::string& quant,
	       const std::string& histo,
	       const std::string& label,
	       const std::string& output)
{
  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgbn                            t_rgbn;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::algebra::vec<3,float>                  t_v3f;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image3d<t_lbl8>                        t_image3d_lbl8;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
  typedef mln::accu::stat::mean<t_v3f,t_v3f,t_v3f>    t_mean;
  typedef mln::util::array<t_v3f>                     t_mean_array;

  t_image2d_rgb8                                      i0_input; // input img
  t_image2d_rgbn                                      i1_input; // quant img
  t_histo3d                                           h2_input; // opened histo
//  t_image2d_int_u8                                    p2_label; // histo proj
  t_image2d_lbl8                                      p2_label; // histo proj
//  t_image2d_rgb8                                      p2_label; // histo proj
  t_image3d_lbl8                                      l2_histo; // label histo
  t_mean_array                                        m2_label; // palette

  t_lbl8                                              n_lbl;    // nb class

  // BEGIN LOADING
  mln::io::ppm::load(i0_input, input.c_str());
  mln::io::ppm::load(i1_input, quant.c_str());
  mln::io::dump::load(h2_input, histo.c_str());
  // END LOADING

  // BEGIN IMAGE PROCESSING
  l2_histo  = mln::labeling::regional_maxima(h2_input, mln::c6(), n_lbl);
  // END IMAGE PROCESSING

  // BEGIN SAVING
  mln::debug::println(h2_input);
  mln::io::dump::save(l2_histo, labeled.c_str());

  l2_input = mln::data::transform(i1_input, t_labeling_rgbn<n>(l2_histo));
  m2_label = mln::labeling::compute(t_mean(), i0_input, l2_input, n_labels);
  p2_label =mln::display::display3_histo3d_unsigned(h2_input,l2_histo,m2_label);

//  mln::io::pgm::save(p2_label, output.c_str());
  mln::io::ppm::save(p2_label, output.c_str());
  std::cout << "Nb classes : " << n_lbl << std::endl;
  // END SAVING
}


void usage()
{
  std::cout                                                       << std::endl;
  std::cout << "regmax input.dump out.dump out.ppm"               << std::endl;
  std::cout << "where"                                            << std::endl;
  std::cout << "input.dump is opened histo"                       << std::endl;
  std::cout << "out.pgm is the r/g proj of the opened histogram"  << std::endl;
  std::cout << "out.dump is the labeled histogram"                << std::endl;
  std::cout << std::endl;
}

int main(int argc, char* args[])
{
  if (4 == argc)
  {
    const std::string input(args[1]);
    const std::string output(args[2]);
    const std::string labeled(args[3]);

    mk_regmax(input, output, labeled);
  }
  else
    usage();

  return 0;
}
