#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/make/image2d.hh>
#include <mln/debug/println.hh>
#include <mln/io/pgm/load.hh>
#include <mln/data/compare.hh>

#include "io/icdar/save.hh"
#include "io/icdar/load.hh"

int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace mln::value;

  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <input.pgm>" << std::endl;
    return 1;
  }

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);
  io::icdar::save(input, "icdar.raw");

  image2d<int_u8> output;
  io::icdar::load(output, "icdar.raw", 48, 44);

  mln_assertion(output.domain() == input.domain());
  mln_assertion(output == input);

  /// Small test.
//  {
//    int_u8 val[4][5] = { { 2, 3, 6, 7, 8 },
//			 { 1, 2, 3, 4, 5 },
//			 { 1, 2, 3, 4, 5 },
//			 { 1, 2, 3, 4, 5 } };
//
//    image2d<int_u8> ima = make::image(val);
//
//    debug::println(ima);
//
//    io::icdar::save(ima, "icdar.raw");
//
//    image2d<int_u8> ima_l;
//
//    io::icdar::load(ima_l, "icdar.raw", 4, 5);
//
//    debug::println(ima_l);
//  }
}
