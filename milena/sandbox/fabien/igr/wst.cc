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
#include <mln/value/label_16.hh>
#include <mln/value/label_32.hh>

#include <mln/morpho/watershed/flooding.hh>



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
  using value::label_16;
  using value::label_32;
  typedef label_16 L;

  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " <ima.dcm> <dimensions>"
	      << std::endl;
    return 1;
  }

  unsigned dim = atoi(argv[2]);
  if (dim != 2 && dim != 3)
  {
    std::cout << "<dimensions> invalid" << std::endl;
    return 1;
  }

  L nbasins;

  if (dim == 2)
  {
    image2d<int_u12> input;
    io::dump::load(input, argv[1]);
    image2d<L> wshed = morpho::watershed::flooding(input, c4(), nbasins);
    io::dump::save(wshed, "wst.dump");
  }
  else
  {
    image3d<int_u12> input;
    io::dump::load(input, argv[1]);
    image3d<L> wshed = morpho::watershed::flooding(input, c6(), nbasins);
    io::dump::save(wshed, "wst.dump");
  }

  std::cout << nbasins << std::endl;

  return 0;
}
