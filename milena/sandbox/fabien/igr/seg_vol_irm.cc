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

#include <mln/histo/compute.hh>

#include <mln/set/compute.hh>
#include <mln/value/label_16.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>

#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/morpho/opening_area.hh>

#include <mln/convert/from_to.hh>

//FIXME: remove
#include <mln/essential/2d.hh>
#include <iostream>
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
unsigned
find_threshold_value(const Image<I>& input_)
{
  const I& input = exact(input_);

  histo::array<mln_value(I)> arr_histo = histo::compute(input);

  image1d<unsigned> ima_histo;
  convert::from_to(arr_histo, ima_histo);

  ima_histo(point1d(0)) = 0;

  //ima_histo = morpho::opening_area(ima_histo, c2(), 5);
  //mln_fwd_piter(image1d<unsigned>) p(ima_histo.domain());
  for (unsigned int i = 1; i < ima_histo.nelements(); ++i)
  {
    ima_histo(point1d(i)) += ima_histo(point1d(i - 1));
  }
  accu::max<unsigned> max_accu;
  unsigned max = level::compute(max_accu, ima_histo);
  for (unsigned int i = 0; i < ima_histo.nelements(); ++i)
  {
    //std::cout << "[" << i << "] => " << ((ima_histo(point1d(i)) * 100) / max) << std::endl;
    if (((ima_histo(point1d(i)) * 100) / max) > 70)
      return i;
  }
}


template <typename I, typename N, typename L>
mln_ch_value(I, bool)
igr(const Image<I>& input_, const mln::Neighborhood<N>& nbh_, L& nlabels)
{
  const I& input = exact(input_);
  const N& nbh = exact(nbh_);

  // Threshold.

  unsigned threshold_value = find_threshold_value(input_);
  mln_ch_value(I, bool) ima_thres;
  initialize(ima_thres, input);
  data::fill((ima_thres | pw::value(input) < pw::cst(threshold_value)).rw(), true);

  return ima_thres;

  // Labeling.

  /*mln_ch_value(I, L) labels = labeling::flat_zones(threshold, nbh, nlabels);
  accu::count<int_u8> a_;
  util::array<unsigned> a = labeling::compute(a_, threshold, labels, nlabels);

  // We keep the third and second biggest object.

  mln_ch_value(I, bool) big_second;
  initialize(big_second, input);
  data::fill(big_second, false);

  util::array<L> arr_big = labeling::n_max<L>(a, 3);
  data::fill((big_second | (pw::value(labels) == arr_big[2])).rw(), true);
  mln_VAR(big_third, threshold | pw::value(labels) == arr_big[3]);

  // Fill holes.

  big_second = labeling::fill_holes(big_second, nbh, nlabels);

  // Gradient.

  mln_ch_value(I, bool) gradient = morpho::elementary::gradient_internal(big_second, nbh);
  mln_VAR(gradient_map, gradient | pw::value(gradient) == true);

  mln_ch_value(I, rgb8) result = level::convert(rgb8(), input);
  data::fill((result | gradient_map.domain()).rw(), literal::red);

  // Center.

  accu::center<mln_site(I)> center_;
  mln_site(I) center = set::compute(center_, big_third.domain());
  result(center) = literal::blue;

  // Distance.

  mln_fwd_piter(gradient_map_t) p(gradient_map.domain());
  p_array<mln_site(I)> arr;
  for_all(p)
  {
    if (mln::norm::l1_distance(p.to_site().to_vec(), center.to_vec()) < 200)
    {
      result(p) = literal::green;

      arr.append(p);
    }
  }

  // Save the cloud in a file.

  io::cloud::save(arr, "cloud.txt");

  return result;*/
}

int main()
{
  //trace::quiet = false;

  label_16 nlabels;

  std::cout << "Processing IM_0043..." << std::endl;
  image3d<int_u12> im43;
  io::dicom::load(im43, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0043.dcm");
  io::dump::save(igr(im43, c6(), nlabels), "IM_0043.dump");

  std::cout << "Processing IM_0046..." << std::endl;
  image3d<int_u12> im46;
  io::dicom::load(im46, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0046.dcm");
  io::dump::save(igr(im46, c6(), nlabels), "IM_0046.dump");

  std::cout << "Processing IM_0049..." << std::endl;
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
  io::pbm::save(igr(im55, c6(), nlabels), "IM_0055.pbm");

  std::cout << "Processing IM_0058..." << std::endl;
  image2d<int_u12> im58;
  io::dicom::load(im58, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm");
  io::pbm::save(igr(im58, c6(), nlabels), "IM_0058.pbm");

  std::cout << "Processing IM_0061..." << std::endl;
  image3d<int_u12> im61;
  io::dicom::load(im61, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0061.dcm");
  io::dump::save(igr(im61, c6(), nlabels), "IM_0061.dump");

  std::cout << "Processing IM_0064..." << std::endl;
  image3d<int_u12> im64;
  io::dicom::load(im64, "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0064.dcm");
  io::dump::save(igr(im64, c6(), nlabels), "IM_0064.dump");

  return 0;
}
