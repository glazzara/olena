#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/var.hh>
#include <mln/pw/all.hh>

#include <mln/io/dump/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>

#include <mln/binarization/threshold.hh>
#include <mln/data/convert.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/geom/bbox.hh>
#include <mln/labeling/blobs.hh>
#include <mln/labeling/fill_holes.hh>
#include <mln/labeling/n_max.hh>
#include <mln/literal/colors.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/opening/volume.hh>
#include <mln/morpho/elementary/gradient_external.hh>
#include <mln/util/array.hh>

#include <mln/accu/transform.hh>
#include <mln/accu/maj_h.hh>

#include <mln/win/disk2d.hh>


using namespace mln;
using value::int_u8;
using value::label_16;
using value::rgb8;


int main (int argc, char* argv[])
{
  if (argc != 6)
  {
    std::cout << "Usage: " << argv[0] << " input closing threshold opening size" << std::endl;
    std::cout << "  input:     input 3D image in dump format encoded in int_u8" << std::endl;
    std::cout << "  closing:   closing lambda" << std::endl;
    std::cout << "  threshold: threshold value" << std::endl;
    std::cout << "  opening:   opening lambda" << std::endl;
    std::cout << "  size:      size of the disk 2D window for filling holes" << std::endl;
    return 1;
  }

  unsigned lambda = atoi(argv[2]);
  unsigned threshold = atoi(argv[3]);
  unsigned opening_lambda = atoi(argv[4]);
  unsigned size = atoi(argv[5]);

  image3d<int_u8> input;
  io::dump::load(input, argv[1]);
  image3d<rgb8> output = data::convert(rgb8(), input);

  image3d<int_u8> closing = morpho::closing::volume(input, c6(), lambda);
  io::dump::save(closing, "closing.dump");

  image3d<bool> thres = binarization::threshold(closing, threshold);
  io::dump::save(thres, "threshold.dump");

  image3d<bool> opening = morpho::opening::volume(thres, c6(), opening_lambda);
  io::dump::save(opening, "opening.dump");

  box<point3d> b = geom::bbox(opening);
  label_16 nlabels;
  accu::maj_h<bool> maj;
  for (int slice_index = b.pmin().sli(); slice_index <= b.pmax().sli(); ++slice_index)
  {
    mln_VAR(slice_image, slice(opening, slice_index));
    mln_VAR(slice_out, slice(output, slice_index));
    mln_VAR(slice_blobs, labeling::blobs(slice_image, c4(), nlabels));
    util::array<label_16> nmax = labeling::n_max(slice_blobs, nlabels, 2);
    data::fill((slice_image | pw::value(slice_blobs) != pw::cst(nmax[1])).rw(), false);

    // Filtering.
    image2d<bool> maj_slice = accu::transform(slice_image, maj, win::disk2d(size));

    mln_VAR(slice_blobs2, labeling::blobs(maj_slice, c4(), nlabels));
    mln_VAR(slice_noholes, labeling::fill_holes(duplicate(maj_slice), c4(), nlabels));
    data::fill((slice_image | pw::value(slice_noholes) == true).rw(), true);

    mln_VAR(gradient, morpho::elementary::gradient_external(slice_image, c4()));

    data::fill((slice_out | pw::value(gradient) == true).rw(), literal::red);
  }

  io::dump::save(opening, "maj.dump");
  io::dump::save(output, "output.dump");

  return 0;
}
