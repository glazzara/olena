#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/pbm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_32.hh>

#include <mln/binarization/threshold.hh>



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

  if (argc != 4)
  {
    std::cout << "Usage: " << argv[0] << " input dim threshold" << std::endl;
    std::cout << "  input:     input image in dump format encoded in int_u12" << std::endl;
    std::cout << "  dim:       number of dimensions of the input image {2, 3}" << std::endl;
    std::cout << "  threshold: threshold value" << std::endl;
    return 1;
  }

  unsigned dim = atoi(argv[2]);
  unsigned threshold = atoi(argv[3]);
  if (dim != 2 && dim != 3)
  {
    std::cout << "<dimensions> invalid" << std::endl;
    return 1;
  }

  if (dim == 2)
  {
    image2d<int_u12> input;
    io::dump::load(input, argv[1]);
    image2d<bool> bin_result = binarization::threshold(input, threshold);
    io::dump::save(bin_result, "result.dump");
  }
  else
  {
    image3d<int_u12> input;
    io::dump::load(input, argv[1]);
    image3d<bool> bin_result = binarization::threshold(input, threshold);
    io::dump::save(bin_result, "result.dump");
  }

  return 0;
}
