#include <iostream>
#include <mln/debug/println.hh>
#include <mln/literal/all.hh>

#include <mln/io/ppm/load.hh>
#include <mln/value/int_u8.hh>

#include "../../mln/histo/compute_histo_rgb.hh"

#include <vector>

int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::value;

  // build test image
  std::cout << "  => loading " << argv[1] << "..." << std::endl;
  image2d<rgb8>ima;
  io::ppm::load(ima, argv[1]);

  std::cout << "  => computing histo..." << std::endl;
//  image3d<int_u8> out = histo::compute_histo_rgb<value::int_u8>(ima);
  std::vector<unsigned> red(256, 0);
  std::vector<unsigned> green(256, 0);
  std::vector<unsigned> blue(256, 0);
  mln_fwd_piter_(image2d<rgb8>) p(ima.domain());
  for_all(p)
  {
    ++red[ima(p).red()];
    ++green[ima(p).green()];
    ++blue[ima(p).blue()];
  }

  for (unsigned i = 0; i < 256; ++i)
  {
    // unsigned acc0 = 0;
    // for (unsigned j = 0; j < 256; ++j)
    //   for (unsigned k = 0; k < 256; ++k)
    // 	acc0 += out(point3d(i, j, k));
    // unsigned acc1 = 0;
    // for (unsigned j = 0; j < 256; ++j)
    //   for (unsigned k = 0; k < 256; ++k)
    // 	acc1 += out(point3d(j, i, k));
    // unsigned acc2 = 0;
    // for (unsigned j = 0; j < 256; ++j)
    //   for (unsigned k = 0; k < 256; ++k)
    // 	acc2 += out(point3d(j, k, i));
    // std::cout << i << " " << acc0 << " " << acc1 << " " << acc2 << std::endl;
    std::cout << i << " "
	      << red[i] << " "
	      << green[i] << " "
	      << blue[i] << std::endl;
  }

}
