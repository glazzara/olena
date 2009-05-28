#include <sstream>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/var.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/plot/save.hh>

#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>

#include <mln/pw/all.hh>
#include <mln/util/array.hh>


using namespace mln;
using value::int_u12;
using value::label_16;




int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump wst.dump label" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 4)
    return usage(argv[0]);

  unsigned label = atoi(argv[3]);


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

  image2d<label_16> ima_labels;
  io::dump::load(ima_labels, argv[2]);

  // Plot points.
  int origin_x = ima_arr.bbox().pmin()[1];
  int origin_y = ima_arr.bbox().pmin()[0];

  mln_VAR(ima_if, ima_arr | pw::value(ima_labels) == pw::cst(label));
  mln_piter_(ima_if_t) p(ima_if.domain());
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
