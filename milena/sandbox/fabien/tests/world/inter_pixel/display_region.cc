#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>

#include <mln/data/stretch.hh>
#include <mln/literal/colors.hh>

#include <mln/world/inter_pixel/display_region.hh>


using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_8;
using value::label_16;
using value::rgb8;





int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump wst.dump" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
    return usage(argv[0]);


  // Initialization.
  typedef int_u12 V;
  typedef label_16 L;
  image3d<V> input;
  io::dump::load(input, argv[1]);
  image2d<L> wst;
  io::dump::load(wst, argv[2]);

  image2d<V> source = duplicate(slice(input, 10));

  io::ppm::save(world::inter_pixel::display_region(data::stretch(int_u8(), source), wst, literal::red), "regions.ppm");


  return 0;
}
