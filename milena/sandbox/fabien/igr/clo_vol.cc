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

#include <mln/morpho/closing/volume.hh>



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

  if (argc != 4)
  {
    std::cout << "Usage: " << argv[0] << " <input.dump> <dimensions> <closure_lambda>"
	      << std::endl;
    return 1;
  }

  unsigned closure_lambda = atoi(argv[3]);
  unsigned dim = atoi(argv[2]);
  if (dim != 2 && dim != 3)
  {
    std::cout << "<dimensions> invalid" << std::endl;
    return 1;
  }

  if (dim == 2)
  {
    image2d<int_u12> input;
    io::dump::load(input, argv[1]);
    image2d<int_u12> clo = morpho::closing::volume(input, c4(), closure_lambda);
    io::dump::save(clo, "clo_vol.dump");
  }
  else
  {
    image3d<int_u12> input;
    io::dump::load(input, argv[1]);
    image3d<int_u12> clo = morpho::closing::volume(input, c6(), closure_lambda);
    io::dump::save(clo, "clo_vol.dump");
  }

  return 0;
}
