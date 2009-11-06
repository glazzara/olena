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





#include <mln/canvas/morpho/attribute_filter.hh>
#include <mln/morpho/attribute/volume.hh>



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
  typedef int_u8 input_type;

  if (argc != 5)
  {
    std::cout << "Usage: " << argv[0] << " input dim lambda output" << std::endl
	      << "  input:  input image in dump format encoded in int_u8" << std::endl
	      << "  dim:    number of dimensions of the input image {2, 3}" << std::endl
	      << "  lambda: parameter for the volume closing" << std::endl
	      << "  output: output image in dump format encoded in int_u8" << std::endl;
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
    image2d<input_type> input;
    io::dump::load(input, argv[1]);
    image2d<input_type> clo = morpho::closing::volume(input, c4(), closure_lambda);
    io::dump::save(clo, argv[4]);
  }
  else
  {
    typedef image3d<input_type> I;

    I input;
    io::dump::load(input, argv[1]);

    I clo = morpho::closing::volume(input, c6(), closure_lambda);
    io::dump::save(clo, argv[4]);
  }

  return 0;
}
