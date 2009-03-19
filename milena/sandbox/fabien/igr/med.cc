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
#include <mln/histo/array.hh>
#include <mln/histo/compute.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/level/compute.hh>
#include <mln/morpho/elementary/dilation.hh>
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

  unsigned median = 0;
  int bg = 0;
  int fg = 0;

  if (dim == 2)
  {
    image2d<L> labels;
    io::dump::load(labels, argv[1]);
    image2d<int_u12> dcm;
    io::dicom::load(dcm, argv[3]);

    mln_VAR(wst_dilate, morpho::elementary::dilation(extend(labels | (pw::value(labels) == 0u), labels), c8()));
    data::fill((labels | (pw::value(labels) == 0u)).rw(), wst_dilate);
    mln_VAR(wst_mean, labeling::mean_values(dcm, labels, nbasins));

    histo::array<int_u12> histogram = histo::compute(wst_mean);
    int j = 0;
    int k = 0;
    for (int i = 0; i < histogram.nvalues(); ++i)
    {
      if (histogram[i])
	histogram[i] = i;
    }
    image1d<unsigned> ima_histo;
    convert::from_to(histogram, ima_histo);
    accu::median_h<int_u12> accu_med;
    median = level::compute(accu_med, ima_histo | pw::value(ima_histo) != pw::cst(0));

    for (int i = 0; i < histogram.nvalues(); ++i)
    {
      if (histogram[i])
      {
	if (histogram[i] > median / 2)
	  ++fg;
	else ++bg;
      }
    }

    io::dump::save(wst_mean, "med.dump");
  }
  else
  {
    image3d<L> labels;
    io::dump::load(labels, argv[1]);
    image3d<int_u12> dcm;
    io::dicom::load(dcm, argv[3]);

    mln_VAR(wst_dilate, morpho::elementary::dilation(extend(labels | (pw::value(labels) == 0u), labels), c26()));
    data::fill((labels | (pw::value(labels) == 0u)).rw(), wst_dilate);
    mln_VAR(wst_mean, labeling::mean_values(dcm, labels, nbasins));

    histo::array<int_u12> histogram = histo::compute(wst_mean);
    int j = 0;
    int k = 0;
    for (int i = 0; i < histogram.nvalues(); ++i)
    {
      if (histogram[i])
	histogram[i] = i;
    }
    image1d<unsigned> ima_histo;
    convert::from_to(histogram, ima_histo);
    accu::median_h<int_u12> accu_med;
    median = level::compute(accu_med, ima_histo | pw::value(ima_histo) != pw::cst(0));

    for (int i = 0; i < histogram.nvalues(); ++i)
    {
      if (histogram[i])
      {
	if (histogram[i] > median / 2)
	  ++fg;
	else ++bg;
      }
    }

    io::dump::save(wst_mean, "med.dump");
  }

  std::cout << median << std::endl;
  std::cerr << "    [ " << bg << " <" << median / 2 << "> " << fg << " ]" << std::endl;

  return 0;
}
