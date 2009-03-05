#include <mln/core/concept/image.hh>
#include <mln/core/image/image2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/dicom/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/literal/colors.hh>

#include <mln/level/transform.hh>
#include <mln/fun/v2b/threshold.hh>

#include <mln/data/fill.hh>
#include <mln/pw/all.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dcm output.pbm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  image2d<int_u8> input;
  io::dicom::load(input, argv[1]);

  image2d<bool> ima = level::transform(input, fun::v2b::threshold<int_u8>(1));
  io::pbm::save(ima, argv[2]);
}
