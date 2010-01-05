// TOOLS ==> histogram filtering

#include <iostream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/macros.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/data/compute.hh>

#include <mln/display/display_histo.hh>

#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/morpho/opening/volume.hh>

#include <mln/value/int_u8.hh>

void mk_opening(const std::string& input,
		const unsigned     min_vol,
		const std::string& output,
		const std::string& opened)
{
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;

  // START OF IMAGE PROCESSING CHAIN
  t_histo3d                                           h1_input; // histo input
  t_histo3d                                           h2_input; // histo input
  t_image2d_int_u8                                    p1_histo; // histo proj

  mln::io::dump::load(h1_input, input.c_str());
  h2_input = mln::morpho::opening::volume(h1_input, mln::c6(), min_vol);
  // END OF IMAGE PROCESSING CHAIN

  // BEGIN DUMPING
  p1_histo = mln::display::display_histo3d_unsigned(h2_input);
  mln::io::dump::save(h2_input, opened.c_str());
  mln::io::pgm::save(p1_histo, output.c_str());
  // END DUMPING
}


void usage()
{
  std::cout                                                       << std::endl;
  std::cout << "opening input.dump v out.dump out.ppm"            << std::endl;
  std::cout << "where"                                            << std::endl;
  std::cout << "input.dump is the 3d color input histo"           << std::endl;
  std::cout << "v is the minimum size of each composant"          << std::endl;
  std::cout << "out.pgm is the r/g proj of the opened histogram"  << std::endl;
  std::cout << "out.dump is the opened histogram"                 << std::endl;
  std::cout << std::endl;
}

int main(int argc, char* args[])
{
  if (5 == argc)
  {
    const std::string input(args[1]);
    const unsigned    min_vol = atoi(args[2]);
    const std::string output(args[3]);
    const std::string opened(args[4]);

    mk_opening(input, min_vol, output, opened);
  }
  else
    usage();

  return 0;
}
