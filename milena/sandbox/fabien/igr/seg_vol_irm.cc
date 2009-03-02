// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
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
#include <mln/labeling/fill_holes.hh>
#include <mln/labeling/n_max.hh>

#include <mln/level/compare.hh>
#include <mln/level/compute.hh>
#include <mln/level/transform.hh>

#include <mln/fun/internal/selector.hh>

#include <mln/fun/v2b/threshold.hh>
#include <mln/level/transform.hh>

#include <mln/accu/count.hh>
#include <mln/accu/center.hh>
#include <mln/accu/max.hh>
#include <mln/accu/sum.hh>

#include <mln/histo/compute.hh>

#include <mln/set/compute.hh>
#include <mln/value/label_16.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>

#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/morpho/closing.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>

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
close_threshold(const Image<I>& input, metal::int_<2>)
{
  return morpho::erosion(morpho::dilation(input, win::disk2d(5)), win::disk2d(7));
}


template <typename I>
I
close_threshold(const Image<I>& input, metal::int_<3>)
{
  return morpho::erosion(morpho::dilation(input, win::sphere3d(5)), win::sphere3d(7));
}


template <typename I>
unsigned
find_threshold_value(const Image<I>& input_)
{
  const I& input = exact(input_);
  int bg_thres = 30;
  mln_ch_value(I, bool) ima_bg;
  initialize(ima_bg, input);
  data::fill(ima_bg, false);
  int obj_thres = 50;
  mln_ch_value(I, bool) ima_obj;
  initialize(ima_obj, input);
  data::fill(ima_obj, false);
  unsigned result = 0;

  histo::array<mln_value(I)> arr_histo = histo::compute(input);

  image1d<unsigned> ima_histo;
  convert::from_to(arr_histo, ima_histo);

  // We remove the 0 value because it is not part of the image.
  ima_histo(point1d(0)) = 0;

  for (unsigned int i = 1; i < ima_histo.nelements(); ++i)
  {
    ima_histo(point1d(i)) += ima_histo(point1d(i - 1));
  }
  accu::max<unsigned> max_accu;
  unsigned max = level::compute(max_accu, ima_histo);
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

  io::dump::save(ima_bg, "bg.pbm");
  io::dump::save(ima_obj, "obj.pbm");

  ima_bg = close_threshold(ima_bg, metal::int_<I::site::dim>());
  ima_obj = close_threshold(ima_obj, metal::int_<I::site::dim>());

  io::dump::save(ima_bg, "bg_closed.pbm");
  io::dump::save(ima_obj, "obj_closed.pbm");

  histo::array<mln_value(I)> bg_histo = histo::compute(input | pw::value(ima_bg) == true);
  histo::array<mln_value(I)> obj_histo = histo::compute(input | pw::value(ima_obj) == true);

  accu::sum<unsigned> sum_accu;
  image1d<unsigned> ima_bg_histo;
  convert::from_to(bg_histo, ima_bg_histo);
  ima_bg_histo(point1d(0)) = 0;
  unsigned bg_sum = level::compute(sum_accu, ima_bg_histo);
  // We remove the 0 value because it is not part of the image.
  std::ofstream fout_bg("bg_histo_norm.plot");
  for (unsigned int i = 0; i < ima_bg_histo.nelements(); ++i)
  {
    ima_bg_histo(point1d(i)) *= 10000;
    ima_bg_histo(point1d(i)) /= bg_sum;
    fout_bg << i << " " << ima_bg_histo(point1d(i)) << std::endl;
  }

  image1d<unsigned> ima_obj_histo;
  convert::from_to(obj_histo, ima_obj_histo);
  unsigned obj_sum = level::compute(sum_accu, ima_obj_histo);
  std::ofstream fout_obj("obj_histo_norm.plot");
  for (unsigned int i = 0; i < ima_obj_histo.nelements(); ++i)
  {
    ima_obj_histo(point1d(i)) *= 10000;
    ima_obj_histo(point1d(i)) /= obj_sum;
    fout_obj << i << " " << ima_obj_histo(point1d(i)) << std::endl;
  }

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


template <typename I, typename N, typename L>
mln_ch_value(I, bool)
igr(const Image<I>& input_, const mln::Neighborhood<N>& nbh_, L& nlabels)
{
  const I& input = exact(input_);
  const N& nbh = exact(nbh_);

  // Threshold.

  mln_ch_value(I, bool) ima_thres;
  initialize(ima_thres, input);
  data::fill(ima_thres, false);
  unsigned threshold_value = find_threshold_value(input);
  data::fill((ima_thres | pw::value(input) < pw::cst(threshold_value)).rw(), true);

  return ima_thres;
}


int main(int argc, char* argv[])
{
  //trace::quiet = false;

  label_16 nlabels;

  /*std::cout << "Processing IM_0049..." << std::endl;
  image2d<int_u12> im49;
  io::dicom::load(im49, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0049.dcm");
  io::pbm::save(igr(im49, c6(), nlabels), "IM_0049.pbm");

  std::cout << "Processing IM_0052..." << std::endl;
  image3d<int_u12> im52;
  io::dicom::load(im52, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm");
  io::dump::save(igr(im52, c6(), nlabels), "IM_0052.dump");

  std::cout << "Processing IM_0055..." << std::endl;
  image2d<int_u12> im55;
  io::dicom::load(im55, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0055.dcm");
  io::pbm::save(igr(im55, c4(), nlabels), "IM_0055.pbm");

  std::cout << "Processing IM_0058..." << std::endl;
  image2d<int_u12> im58;
  io::dicom::load(im58, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm");
  io::pbm::save(igr(im58, c4(), nlabels), "IM_0058.pbm");

  std::cout << "Processing IM_0061..." << std::endl;
  image3d<int_u12> im61;
  io::dicom::load(im61, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0061.dcm");
  io::dump::save(igr(im61, c6(), nlabels), "IM_0061.dump");*/

  std::cout << "Processing IM_0064..." << std::endl;
  image3d<int_u12> im64;
  io::dicom::load(im64, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0064.dcm");
  io::dump::save(igr(im64, c6(), nlabels), "IM_0064.dump");

  return 0;
}
