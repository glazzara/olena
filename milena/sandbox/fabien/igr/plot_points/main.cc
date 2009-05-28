#include <sstream>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/plot/save.hh>

#include <mln/value/int_u12.hh>
#include <mln/util/array.hh>


using namespace mln;
using value::int_u12;




int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 2)
    return usage(argv[0]);


  // Initialization.
  typedef int_u12 V;
  image3d<V> input;
  io::dump::load(input, argv[1]);
  typedef image2d<util::array<V> > I;
  I ima_arr;
  initialize(ima_arr, slice(input, 0));
  for (unsigned int i = 0; i < input.nslices(); ++i)
  {
    image2d<V> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<V>) p(tmp_slice.domain());
    for_all(p)
      ima_arr(p).append(tmp_slice(p));
  }


  // Plot points.
  int origin_x = ima_arr.bbox().pmin()[1];
  int origin_y = ima_arr.bbox().pmin()[0];

  mln_piter_(I) p (ima_arr.domain());
  for_all(p)
  {
    int x = p[1] - origin_x;
    int y = p[0] - origin_y;
    std::ostringstream slabel;
    slabel << "x";
    if (x < 100)
      slabel << "0";
    if (x < 10)
      slabel << "0";
    slabel << x << "_y";
    if (y < 100)
      slabel << "0";
    if (y < 10)
      slabel << "0";
    slabel << y << ".plot";

    io::plot::save(ima_arr(p), slabel.str());
  }

  return 0;
}
