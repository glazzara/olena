#include <iostream>
#include <sstream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/var.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/geom/nslis.hh>
#include <mln/geom/bbox.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_32.hh>

#include <mln/accu/mean.hh>
#include <mln/accu/median_h.hh>
#include <mln/data/fill.hh>
#include <mln/level/compute.hh>
#include <mln/level/stretch.hh>
#include <mln/make/box3d.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>
#include <mln/util/set.hh>

using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;



template <typename I, typename L>
inline
void label2gif(I ima, L ima_labels, label_16 label)
{
  util::array<float> arr;

  for (unsigned sli = 0; sli < geom::nslis(ima); ++sli)
  {
    image2d<int_u12> ima_slice = duplicate(slice(ima, sli));
    image2d<label_16> lbl_slice = duplicate(slice(ima_labels, sli));

    // Saving a image of the selected label in the current slice for debug.
    data::fill((ima_slice | pw::value(lbl_slice) == pw::cst(label)).rw(), 1750);
    std::ostringstream str_ima;
    str_ima << "debug_label_" << label << "_";

    if (sli < 100)
      str_ima << "0";
    if (sli < 10)
      str_ima << "0";

    str_ima << sli << ".pgm";
    io::pgm::save(level::stretch(int_u8(), ima_slice), str_ima.str());
  }
}


///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  typedef label_16 L;

  if (argc != 5)
  {
    std::cout << "Usage: " << argv[0] << " <labels.dump> <source.dump> <nbasins> <label>"
	      << std::endl;
    return 1;
  }

  unsigned nlabels = atoi(argv[3]);
  L label = atoi(argv[4]);

  image3d<L> ima_labels;
  io::dump::load(ima_labels, argv[1]);
  image3d<int_u12> ima;
  io::dump::load(ima, argv[2]);

  label2gif(ima, ima_labels, label);

  return 0;
}

