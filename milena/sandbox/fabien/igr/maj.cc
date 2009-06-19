#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/io/dump/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>

#include <mln/morpho/watershed/flooding.hh>

#include <mln/accu/transform.hh>
#include <mln/accu/maj_h.hh>

#include <mln/win/sphere3d.hh>



///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::label_8;
  using value::label_16;
  typedef label_8 L;
  typedef int_u8 V;

  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " wst.dump output.dump"
	      << std::endl;
    return 1;
  }

  L nbasins;

  image3d<L> wst;
  io::dump::load(wst, argv[1]);
  accu::maj_h<L> maj;
  image3d<L> output = accu::transform(wst, maj, win::sphere3d(7));

  io::dump::save(output, argv[2]);

  return 0;
}
