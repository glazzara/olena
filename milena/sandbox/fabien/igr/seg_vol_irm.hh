// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#include <mln/core/concept/image.hh>
#include <mln/core/concept/neighborhood.hh>
#include <mln/core/alias/neighb1d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/neighb3d.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/all.hh>
#include <mln/io/cloud/all.hh>
#include <mln/io/dump/all.hh>
#include <mln/io/dicom/load.hh>

#include <mln/labeling/flat_zones.hh>
#include <mln/labeling/background.hh>
#include <mln/literal/colors.hh>
#include <mln/norm/l1.hh>

#include <mln/geom/bbox.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/foreground.hh>
#include <mln/labeling/fill_holes.hh>
#include <mln/labeling/n_max.hh>

#include <mln/data/compare.hh>
#include <mln/data/compute.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/data/transform.hh>

#include <mln/fun/internal/selector.hh>

#include <mln/fun/v2b/threshold.hh>
#include <mln/data/transform.hh>

#include <mln/accu/count.hh>
#include <mln/accu/center.hh>
#include <mln/accu/max.hh>
#include <mln/accu/sum.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/stat/deviation.hh>

#include <mln/histo/compute.hh>

#include <mln/set/compute.hh>
#include <mln/value/label_16.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>

#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/morpho/approx/dilation.hh>
#include <mln/morpho/approx/erosion.hh>

#include <mln/win/disk2d.hh>
#include <mln/win/sphere3d.hh>

#include <mln/math/diff_abs.hh>

#include <mln/convert/from_to.hh>

#include <mln/metal/int.hh>

#include <iostream>
#include <fstream>
#include <mln/debug/println.hh>

using namespace mln;
using value::int_u8;
using value::int_u12;
using value::int_u16;
using value::rgb8;
using value::label_16;



template <typename T>
struct L_to_int_u8
: mln::fun::internal::selector_<int_u8, T, L_to_int_u8<T> >::ret
{
  typedef int_u8 result;
  int_u8 operator()(const T& t) const;
};



template <typename T>
inline
int_u8
L_to_int_u8<T>::operator()(const T& t) const
{
  return static_cast<int_u8>(t == 0 ?  0 : 1 + ((unsigned) t - 1) % 255);
}



template <typename I>
I
close_threshold(const Image<I>& input, metal::int_<2>, int dil, int ero)
{
  return morpho::erosion(morpho::dilation(input, win::disk2d(dil)), win::disk2d(ero));
}



template <typename I>
I
close_threshold(const Image<I>& input, metal::int_<3>, int dil, int ero)
{
  return morpho::approx::erosion(morpho::approx::dilation(input, win::sphere3d(dil)), win::sphere3d(ero));
}



template <typename I>
void
save_regions_color(const Image<I>& ima, metal::int_<2>)
{
  io::ppm::save(ima, "regions_color.ppm");
}



template <typename I>
void
save_regions_color(const Image<I>& ima, metal::int_<3>)
{
  io::dump::save(ima, "regions_color.dump");
}



template <typename I, typename N>
unsigned
find_threshold_value(const Image<I>& input, const Neighborhood<N>& nbh)
{
  int bg_thres = 20;
  int obj_thres = 10;

  mln_ch_value(I, bool) ima_bg;
  initialize(ima_bg, input);
  data::fill(ima_bg, false);

  mln_ch_value(I, bool) ima_obj;
  initialize(ima_obj, input);
  data::fill(ima_obj, false);

  unsigned result = 0;

  // We remove the 0 value because it is not part of the image.
  histo::array<mln_value(I)> arr_histo = histo::compute(input | pw::value(input) != 0);
  image1d<unsigned> ima_histo;
  convert::from_to(arr_histo, ima_histo);

  std::ofstream fout("histo.plot");
  fout << "0 0" << std::endl;
  for (unsigned int i = 1; i < ima_histo.nelements(); ++i)
  {
    fout << i << " " << ima_histo(point1d(i)) << std::endl;
    ima_histo(point1d(i)) += ima_histo(point1d(i - 1));
  }
  accu::max<unsigned> max_accu;
  unsigned max = data::compute(max_accu, ima_histo);
  bool low_done = false;
  bool high_done = false;
  for (unsigned int i = 0; i < ima_histo.nelements(); ++i)
  {
    if (!low_done && ((ima_histo(point1d(i)) * 100) / max) > bg_thres)
    {
      data::fill((ima_bg | pw::value(input) < pw::cst(i)).rw(), true);
      low_done = true;
    }
    if (!high_done && ((ima_histo(point1d(i)) * 100) / max) > (100 - obj_thres))
    {
      data::fill((ima_obj | pw::value(input) > pw::cst(i)).rw(), true);
      high_done = true;
    }
  }

  // Debug output images
  if (I::site::dim == 2)
  {
    io::pbm::save(ima_bg, "bg.pbm");
    io::pbm::save(ima_obj, "obj.pbm");
  }
  if (I::site::dim == 3)
  {
    io::dump::save(ima_bg, "bg.dump");
    io::dump::save(ima_obj, "obj.dump");
  }

  ima_bg = close_threshold(ima_bg, metal::int_<I::site::dim>(), 9, 15);
  ima_obj = close_threshold(ima_obj, metal::int_<I::site::dim>(), 9, 11);

  // Labeling
  /*label_16 nlabels = 0;

  mln_ch_value(I, label_16) bg_labels = labeling::foreground(ima_bg, nbh, nlabels);
  accu::count<int_u8> ab_;
  util::array<unsigned> arr_b_label = labeling::compute(ab_, ima_bg, bg_labels, nlabels);
  util::array<label_16> arr_b_big = labeling::n_max<label_16>(arr_b_label, 1);
  data::fill((ima_bg | (pw::value(bg_labels) != pw::cst(arr_b_big[1]))).rw(), false);

  mln_ch_value(I, label_16) obj_labels = labeling::foreground(ima_obj, nbh, nlabels);
  accu::count<int_u8> ao_;
  util::array<unsigned> arr_o_label = labeling::compute(ao_, ima_obj, obj_labels, nlabels);
  util::array<label_16> arr_o_big = labeling::n_max<label_16>(arr_o_label, 1);
  data::fill((ima_obj | (pw::value(obj_labels) != pw::cst(arr_o_big[1]))).rw(), false);*/

  // Debug output images
  mln_ch_value(I, rgb8) out = data::convert(rgb8(), level::stretch(int_u8(), input));
  data::fill((out | pw::value(morpho::elementary::gradient_internal(ima_bg, nbh)) == true).rw(), literal::red);
  data::fill((out | pw::value(morpho::elementary::gradient_internal(ima_obj, nbh)) == true).rw(), literal::green);
  save_regions_color(out, metal::int_<I::site::dim>());
  if (I::site::dim == 2)
  {
    io::pbm::save(ima_bg, "bg_closed.pbm");
    io::pbm::save(ima_obj, "obj_closed.pbm");
  }
  if (I::site::dim == 3)
  {
    io::dump::save(ima_bg, "bg_closed.dump");
    io::dump::save(ima_obj, "obj_closed.dump");
  }

  // Histo
  histo::array<mln_value(I)> bg_histo = histo::compute(input | pw::value(ima_bg) == true);
  histo::array<mln_value(I)> obj_histo = histo::compute(input | pw::value(ima_obj) == true);

  accu::sum<unsigned> sum_accu;
  image1d<unsigned> ima_bg_histo;
  convert::from_to(bg_histo, ima_bg_histo);
  ima_bg_histo(point1d(0)) = 0;
  unsigned bg_sum = data::compute(sum_accu, ima_bg_histo);
  std::ofstream fout_bg("bg_histo.plot");
  std::ofstream fout_p_bg("bg_histo_norm.plot");
  for (unsigned int i = 0; i < ima_bg_histo.nelements(); ++i)
  {
    fout_bg << i << " " << ima_bg_histo(point1d(i)) << std::endl;
    ima_bg_histo(point1d(i)) *= 10000;
    ima_bg_histo(point1d(i)) /= bg_sum;
    fout_p_bg << i << " " << ima_bg_histo(point1d(i)) << std::endl;
  }

  image1d<unsigned> ima_obj_histo;
  convert::from_to(obj_histo, ima_obj_histo);
  unsigned obj_sum = data::compute(sum_accu, ima_obj_histo);
  std::ofstream fout_obj("obj_histo.plot");
  std::ofstream fout_p_obj("obj_histo_norm.plot");
  for (unsigned int i = 0; i < ima_obj_histo.nelements(); ++i)
  {
    fout_obj << i << " " << ima_obj_histo(point1d(i)) << std::endl;
    ima_obj_histo(point1d(i)) *= 10000;
    ima_obj_histo(point1d(i)) /= obj_sum;
    fout_p_obj << i << " " << ima_obj_histo(point1d(i)) << std::endl;
  }

  // Search for the index with the min distance between histogrammes.
  unsigned min = math::diff_abs<unsigned>(ima_bg_histo(point1d(1)), ima_obj_histo(point1d(1)));
  for (unsigned int i = 1; i < ima_bg_histo.nelements(); ++i)
  {
    if (math::diff_abs<unsigned>(ima_bg_histo(point1d(i)), ima_obj_histo(point1d(i))) < min)
    {
      min = math::diff_abs<unsigned>(ima_bg_histo(point1d(i)), ima_obj_histo(point1d(i)));
      result = i;
    }
  }

  return result;
}



template <typename I, typename N>
unsigned
find_threshold_mean(const Image<I>& input, const Neighborhood<N>& nbh)
{
  unsigned coef = 1;

  accu::mean<unsigned> mean_accu;
  unsigned mean = data::compute(mean_accu, (input | (pw::value(input) != 0)));

  accu::stat::deviation<unsigned, unsigned, float> dev_accu(mean);
  float deviation = data::compute(dev_accu, (input | pw::value(input) != 0));

  std::cout << "[mean = " << mean  << " | deviation = " << deviation << "]";
  return floor(mean + coef * deviation);
}



/*template <typename I, typename N, typename L>
mln_ch_value(I, bool)
igr_seg(const Image<I>& input_, const Neighborhood<N>& nbh_, L& nlabels)
{
  const I& input = exact(input_);
  const N& nbh = exact(nbh_);

  // Threshold.

  mln_ch_value(I, bool) ima_thres;
  initialize(ima_thres, input);
  data::fill(ima_thres, false);
  std::cout << "double threshold value = " << find_threshold_value(input, nbh) << std::endl;
  unsigned threshold_value = find_threshold_mean(input, nbh);
  std::cout << " deviation threshold value = " << threshold_value << std::endl;
  data::fill((ima_thres | pw::value(input) < pw::cst(threshold_value)).rw(), true);

  return ima_thres;
}*/
