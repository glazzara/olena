#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

//#include <mln/io/dicom/load.hh>
#include <mln/io/dump/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>

#include <mln/morpho/elementary/gradient.hh>



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

  if (argc != 4)
  {
    std::cout << "Usage: " << argv[0] << " input dim output" << std::endl;
    std::cout << "  input:  input image in dump format encoded in int_u8" << std::endl;
    std::cout << "  dim:    number of dimensions of the input image {2, 3}" << std::endl;
    std::cout << "  output: output image in dump format encoded in int_u8" << std::endl;
    return 1;
  }

  unsigned dim = atoi(argv[2]);
  if (dim != 2 && dim != 3)
  {
    std::cout << "<dimensions> invalid" << std::endl;
    return 1;
  }

  if (dim == 2)
  {
    image2d<input_type> dcm2;
    io::dump::load(dcm2, argv[1]);
    image2d<input_type> grad2 = morpho::elementary::gradient(dcm2, c4());
    io::dump::save(grad2, argv[3]);
  }
  else
  {
    image3d<input_type> dcm3;
    io::dump::load(dcm3, argv[1]);
    image3d<input_type> grad3 = morpho::elementary::gradient(dcm3, c6());
    io::dump::save(grad3, argv[3]);
  }

  return 0;
}
