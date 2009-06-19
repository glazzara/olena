#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>

#include <mln/convert/from_to.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/data/stretch.hh>
#include <mln/pw/all.hh>
#include <mln/literal/colors.hh>


using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_8;
using value::label_16;
using value::rgb8;



int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump wst.dump output.dump" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 4)
    return usage(argv[0]);


  // Initialization.
  typedef int_u8 V;
  typedef label_8 L;
  image3d<V> input;
  io::dump::load(input, argv[1]);
  image3d<L> wst;
  io::dump::load(wst, argv[2]);
  image3d<rgb8> output;
  initialize(output, input);

  mln_piter_(image3d<V>) p(input.domain());
  for_all(p)
    convert::from_to(input(p), output(p));
  data::fill((output | pw::value(wst) == pw::cst(0)).rw(), literal::red);

  io::dump::save(output, argv[3]);

  return 0;
}
