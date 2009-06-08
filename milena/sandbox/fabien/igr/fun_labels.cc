#include <iostream>
#include <sstream>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/slice_image.hh>
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
//#include <mln/io/dicom/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>

#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/median_h.hh>
#include <mln/data/fill.hh>
#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>
#include <mln/make/box3d.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>
#include <mln/util/set.hh>



using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;


////////////////
//	      //
// Plot label //
//	      //
////////////////

template <typename I, typename L, typename V>
inline
void plot_label(Image<I>& ima, Image<L>& ima_labels, V lbl)
{
  mln_VAR(vol_label, ima | pw::value(ima_labels) == pw::cst(lbl));
  util::array<float> arr;
  int sli_min = geom::bbox(vol_label).pmin().sli();
  int sli_max = geom::bbox(vol_label).pmax().sli();

  for (int i = geom::bbox(ima).pmin().sli(); i < sli_min; ++i)
    arr.append(0.0);

  int min_row = geom::bbox(vol_label).pmin().row();
  int min_col = geom::bbox(vol_label).pmin().col();
  int max_row = geom::bbox(vol_label).pmax().row();
  int max_col = geom::bbox(vol_label).pmax().col();
  for (int sli = sli_min; sli <= sli_max; ++sli)
  {
    box3d slice = make::box3d(sli, min_row, min_col,
			      sli, max_row, max_col);
    mln_VAR(slice_label, vol_label | slice);
    accu::stat::mean<mln_value(I)> accu_mean;
    float mean = data::compute(accu_mean, slice_label);
    arr.append(mean);
  }

  for (int i = sli_max + 1; i <= geom::bbox(ima).pmax().sli(); ++i)
    arr.append(0.0);

  std::ostringstream slabel;
  slabel << "label_";
  if (lbl < 100)
    slabel << "0";
  if (lbl < 10)
    slabel << "0";
  // FIXME: insert label location
  slabel << lbl << ".plot";
  io::plot::save(arr, slabel.str());
}

template <typename I, typename L>
inline
void plot_all_labels(Image<I>& ima, Image<L>& ima_labels, unsigned nlabels)
{
  util::array<util::array<float> > arrays(nlabels);
  accu::stat::mean<int_u12> accu_mean;

  label_16 l = 0;
  int max_slice = ima.domain().pmax().sli();
  for (int sli = ima.domain().pmin().sli(); sli < max_slice; ++sli)
  {
    for (unsigned l = 0; l < nlabels; ++l)
    {
      image2d<int_u12> ima_slice = duplicate(slice(ima, sli));
      image2d<label_16> lbl_slice = duplicate(slice(ima_labels, sli));
      float mean = data::compute(accu_mean, ima_slice | pw::value(lbl_slice) == pw::cst(l));
      arrays[l].append(mean);
    }
  }

  for (unsigned l = 0; l < nlabels; ++l)
  {
    std::ostringstream slabel;
    slabel << "label_";
    if (l < 100)
      slabel << "0";
    if (l < 10)
      slabel << "0";
    // FIXME: insert label location
    slabel << l << ".plot";
    io::plot::save(arrays[l], slabel.str());
  }
}


////////////////
//	      //
// Plot point //
//	      //
////////////////

template <typename L>
inline
void save_label_plot(int count, const char* desc, L prev_lbl, util::array<float> arr, int start)
{
  std::ostringstream slabel;
  if (count < 10)
    slabel << desc << "_0" << count << "_label_" << prev_lbl << ".plot";
  else
    slabel << desc << "_" << count << "_label_" << prev_lbl << ".plot";
  io::plot::save(arr, slabel.str(), start);
}

template <typename I, typename L>
inline
void plot_point(Image<I>& ima, Image<L>& ima_labels, point2d point, const char* desc)
{
  util::array<float> arr;
  mln_value(L) prev_lbl;
  int start = 0;
  int count = 1;
  int nslices = geom::nslis(ima);

  for (unsigned sli = 0; sli < nslices; ++sli)
  {
    image2d<mln_value(I)> ima_slice = duplicate(slice(ima, sli));
    image2d<mln_value(L)> lbl_slice = duplicate(slice(ima_labels, sli));
    if (sli == 0)
      prev_lbl = lbl_slice(point);
    if (lbl_slice(point) != prev_lbl)
    {
      save_label_plot(count, desc, prev_lbl, arr, start);
      ++count;
      arr.clear();
      start = sli + 1;
    }
    // Taking the median value of the region.
    accu::stat::mean<mln_value(I)> accu_mean;
    mln_value(I) mean = data::compute(accu_mean, ima_slice | pw::value(lbl_slice) == pw::cst(lbl_slice(point)));
    arr.append(mean);
    prev_lbl = lbl_slice(point);

    // Saving a image of the selected label in the current slice for debug.
    /*data::fill((ima_slice | pw::value(lbl_slice) == pw::cst(prev_lbl)).rw(), 1750);
    std::ostringstream str_ima;
    str_ima << "debug_" << desc << "_";
    if (sli < 100)
      str_ima << "0";
    if (sli < 10)
      str_ima << "0";
    str_ima << sli << ".pgm";
    io::pgm::save(data::stretch(int_u8(), ima_slice), str_ima.str());*/
  }

  if (!arr.is_empty())
    save_label_plot(count, desc, prev_lbl, arr, start);
}


///////////////////
//               //
// Main Function //
//               //
///////////////////


int main(int argc, char *argv[])
{
  typedef label_16 L;
  typedef float I;

  if (argc != 4)
  {
    std::cout << "Usage: " << argv[0] << " <ima.dump> <source.dump> <nbasins>"
	      << std::endl;
    return 1;
  }

  L nlabels = atoi(argv[3]);

  point2d p_tumeur(163, 114);
  point2d p_air(54, 94);
  point2d p_poumon(122, 115);
  image3d<L> ima_labels;
  io::dump::load(ima_labels, argv[1]);
  image3d<float> ima;
  io::dump::load(ima, argv[2]);
  mln_VAR(dilate, morpho::elementary::dilation(extend(ima_labels | (pw::value(ima_labels) == 0u), ima_labels), c6()));
  data::fill((ima_labels | (pw::value(ima_labels) == 0u)).rw(), dilate);

  //plot_all_labels(ima, ima_labels, nlabels);

  /*L l = 0;
  for (unsigned i = 0; i < nlabels; ++i, l = i)
    plot_label(ima, ima_labels, l);*/

  io::dump::save(ima_labels, "labels.dump");

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

  return 0;
}
