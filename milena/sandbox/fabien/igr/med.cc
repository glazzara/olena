#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/image_if.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/dicom/load.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_32.hh>

#include <mln/core/var.hh>
#include <mln/core/routine/extend.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/median_h.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/level/compute.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>



///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::label_16;
  using value::label_32;
  typedef label_32 L;

  if (argc != 5)
  {
    std::cout << "Usage: " << argv[0] << " <ima.dump> <dimensions> <ima.dcm> <nbasins>"
	      << std::endl;
    return 1;
  }

  unsigned dim = atoi(argv[2]);
  L nbasins = atoi(argv[4]);
  if (dim != 2 && dim != 3)
  {
    std::cout << "<dimensions> invalid" << std::endl;
    return 1;
  }

  if (dim == 2)
  {
    image2d<L> labels;
    io::dump::load(labels, argv[1]);
    image2d<int_u12> dcm;
    io::dicom::load(dcm, argv[3]);

    mln_VAR(wst_dilate, morpho::elementary::dilation(extend(labels | (pw::value(labels) == 0u), labels), c8()));
    data::fill((labels | (pw::value(labels) == 0u)).rw(), wst_dilate);
    mln_VAR(wst_mean, labeling::mean_values(dcm, labels, nbasins));

    accu::mean<float> accu_mean;
    util::array<float> means = level::compute(accu_mean, wst_mean);

    // FIXME: Take median value of means

    io::dump::save(wst_mean, "med.dump");
    std::cout << median << std::endl;
  }
  else
  {
    // FIXME
  }

  return 0;
}
