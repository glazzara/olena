#include <iostream>
#include <sstream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/window3d.hh>

#include <mln/geom/nslis.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/dicom/load.hh>
#include <mln/io/plot/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/label_32.hh>

#include <mln/accu/mean.hh>
#include <mln/accu/median_h.hh>
#include <mln/level/compute.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>
#include <mln/util/set.hh>



using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;
using value::label_32;


template <typename I, typename L, typename V>
inline
void plot_label(I ima, L ima_labels, V lbl)
{
  I ima_spe = duplicate(ima | pw::value(ima_labels) == pw::cst(lbl));
  util::array<float> arr;
  for (int i = 0; i < ima_spe.domain().pmin().sli(); ++i)
    arr.append(0.0);
  for (int sli = ima_spe.domain().pmin().sli(); sli <= ima_spe.domain().pmax().sli(); ++sli)
  {
    image2d<int_u12> tmp_slice = duplicate(slice(ima_spe, sli));
    accu::mean<int_u12> accu_mean;
    float mean = level::compute(accu_mean, tmp_slice);
    arr.append(mean);
  }
  for (int i = ima_spe.domain().pmax().sli() + 1; i <= ima.domain().pmax().sli(); ++i)
    arr.append(0.0);
  std::ostringstream slabel;
  slabel << "label_" << lbl << ".plot";
  io::plot::save(arr, slabel.str());
}


template <typename I, typename L>
inline
void plot_point(I ima, L ima_labels, point2d point, const char* desc)
{
  util::array<float> arr;
  label_32 prev_lbl;
  int start = 0;
  int count = 1;
  for (unsigned sli = 0; sli < geom::nslis(ima); ++sli)
  {
    image2d<int_u12> ima_slice = duplicate(slice(ima, sli));
    image2d<label_32> lbl_slice = duplicate(slice(ima_labels, sli));
    if (sli == 0)
      prev_lbl = lbl_slice(point);
    if (lbl_slice(point) != prev_lbl)
    {
      std::ostringstream slabel;
      slabel << desc << "_" << count << "_label_" << prev_lbl << ".plot";
      count++;
      io::plot::save(arr, slabel.str(), start);
      arr.clear();
      start = sli + 1;
    }
    // Taking the mean value of the region.
    accu::mean<int_u12> accu_mean;
    float mean = level::compute(accu_mean, ima_slice | pw::value(lbl_slice) == pw::cst(lbl_slice(point)));
    arr.append(mean);
    prev_lbl = lbl_slice(point);
  }
  if (!arr.is_empty())
  {
    std::ostringstream slabel;
    slabel << desc << "_" << count << "_label_" << prev_lbl << ".plot";
    io::plot::save(arr, slabel.str(), start);
  }
}


///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  typedef label_32 L;

  if (argc != 5)
  {
    std::cout << "Usage: " << argv[0] << " <ima.dump> <dimensions> <ima.dcm> <nbasins>"
	      << std::endl;
    return 1;
  }

  unsigned dim = atoi(argv[2]);
  if (dim != 2 && dim != 3)
  {
    std::cout << "<dimensions> invalid" << std::endl;
    return 1;
  }

  L nlabels = atoi(argv[4]);

  if (dim == 2)
  {
    // FIXME
  }
  else
  {
    point2d p_tumeur(156, 114);
    point2d p_air(34, 94);
    point2d p_poumon(122, 115);
    image3d<L> ima_labels;
    io::dump::load(ima_labels, argv[1]);
    image3d<int_u12> ima;
    io::dicom::load(ima, argv[3]);

    plot_point(ima, ima_labels, p_tumeur, "tumeur");
    plot_point(ima, ima_labels, p_air, "air");
    plot_point(ima, ima_labels, p_poumon, "poumon");
    /*util::set<L> lbl_set;
    for (int sli = 0; sli < geom::nslis(ima_labels); ++sli)
    {
      image2d<L> tmp_slice = duplicate(slice(ima_labels, sli));
      lbl_set.insert(tmp_slice(p_tumeur));
      lbl_set.insert(tmp_slice(p_air));
      lbl_set.insert(tmp_slice(p_poumon));
    }

    util::set_fwd_iter<L> siter(lbl_set);
    for_all(siter)
    {
      plot_label(ima, ima_labels, siter.element());
    }*/
  }

  return 0;
}
