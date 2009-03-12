#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/io/dicom/load.hh>
#include <mln/io/dump/save.hh>

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

  if (dim == 2)
  {
    image2d<int_u12> dcm2;
    io::dicom::load(dcm2, argv[1]);
    image2d<int_u12> grad2 = morpho::elementary::gradient(dcm2, c4());
    io::dump::save(grad2, "grad.dump");
  }
  else
  {
    image3d<int_u12> dcm3;
    io::dicom::load(dcm3, argv[1]);
    image3d<int_u12> grad3 = morpho::elementary::gradient(dcm3, c6());
    io::dump::save(grad3, "grad.dump");
  }

  return 0;
}
