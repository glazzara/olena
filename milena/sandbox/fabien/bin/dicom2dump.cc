#include <mln/core/concept/image.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/io/dicom/load.hh>
#include <mln/io/dump/save.hh>



int usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dcm output.dump" << std::endl;
  std::cerr << "\t work for 3D images encoded in int_u12" << std::endl;
  return 1;
}


/*int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u12;

  if (argc != 3)
    return usage(argv);

  image3d<int_u12> ima;
  io::dicom::load(ima, argv[1]);
  io::dump::save(ima, argv[2]);

  return 0;
}*/

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;

  if (argc != 3)
    return usage(argv);

  image2d<int_u8> ima;
  io::dicom::load(ima, argv[1]);
  io::dump::save(ima, argv[2]);

  return 0;
}
