#include <mln/core/concept/image.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/io/dicom/load.hh>
#include <mln/io/pgm/save.hh>



int usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dcm output.dump" << std::endl;
  std::cerr << "\t work for 2D images encoded in int_u8" << std::endl;
  return 1;
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;

  if (argc != 3)
    return usage(argv);

  image2d<int_u8> ima;
  io::dicom::load(ima, argv[1]);
  io::pgm::save(ima, argv[2]);

  return 0;
}
