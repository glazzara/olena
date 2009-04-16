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

#include <mln/io/dump/all.hh>
#include <mln/io/dicom/load.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_32.hh>

#include <mln/accu/mean.hh>
#include <mln/accu/median_h.hh>
#include <mln/labeling/relabel.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/level/compute.hh>
#include <mln/make/image3d.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>
#include <mln/util/set.hh>



using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;
using value::label_32;



///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  typedef label_32 L;

  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " <ima.dump> <ima.dcm>"
	      << std::endl;
    return 1;
  }

  L nlabels = 0;

  image3d<L> ima_labels;
  io::dump::load(ima_labels, argv[1]);
  image3d<int_u12> ima;
  io::dicom::load(ima, argv[2]);
  util::array<image2d<int_u12> > arr;
  mln_VAR(dilate, morpho::elementary::dilation(extend(ima_labels | (pw::value(ima_labels) == 0u), ima_labels), c6()));
  data::fill((ima_labels | (pw::value(ima_labels) == 0u)).rw(), dilate);

  for (unsigned i = 0; i < ima.nslices(); ++i)
  {
    image2d<int_u12> sli = duplicate(slice(ima, i));
    image2d<L> sli_labels = duplicate(slice(ima_labels, i));
    image2d<L> labels = labeling::relabel(sli_labels, nlabels);
    mln_VAR(mean_slice, labeling::mean_values(sli, labels, nlabels));
    arr.append(mean_slice);
  }

  image3d<int_u12> vol = make::image3d(arr);
  io::dump::save(vol, "mean_slices.dump");

  return 0;
}
