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

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/all.hh>
#include <mln/io/cloud/all.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/labeling/flat_zones.hh>
#include <mln/literal/colors.hh>
#include <mln/norm/l1.hh>

#include <mln/io/dump/all.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/level/compare.hh>
#include <mln/fun/v2b/threshold.hh>
#include <mln/level/transform.hh>
#include <mln/accu/count.hh>
#include <mln/accu/center.hh>
#include <mln/set/compute.hh>
#include <mln/value/label_16.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>
#include <mln/morpho/elementary/gradient_internal.hh>

//FIXME: remove
#include <mln/essential/2d.hh>
#include <iostream>
#include <mln/debug/println.hh>


template <typename I, typename N, typename L>
mln_ch_value(I, mln::value::rgb8)
igr(const mln::Image<I>& input_, const mln::Neighborhood<N>& nbh_, L& nlabels)
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  const I& input = exact(input_);
  const N& nbh = exact(nbh_);

  // Threshold.

  mln_ch_value(I, bool) threshold = level::transform(input, fun::v2b::threshold<int_u8>(25));

  // Labeling.

  mln_ch_value(I, L) labels = labeling::flat_zones(threshold, nbh, nlabels);
  accu::count<int_u8> a_;
  util::array<unsigned> a = labeling::compute(a_, threshold, labels, nlabels);

  // We keep the third and second biggest object.

  mln_ch_value(I, bool) big_second;
  initialize(big_second, input);
  data::fill(big_second, false);
  unsigned big_third_count = 0;
  unsigned big_third_lbl = 0;
  unsigned big_second_count = 0;
  unsigned big_second_lbl = 0;
  unsigned big_first_count = 0;
  unsigned big_first_lbl = 0;
  for (int i = 0; i < a.nelements(); ++i)
  {
    if (a[i] > big_third_count)
    {
      big_third_count = a[i];
      big_third_lbl = i;
    }
    if (big_third_count > big_second_count)
    {
      int swap = big_second_count;
      int swap_lbl = big_second_lbl;
      big_second_count = big_third_count;
      big_second_lbl = big_third_lbl;
      big_third_count = swap;
      big_third_lbl = swap_lbl;
    }
    if (big_second_count > big_first_count)
    {
      int swap = big_first_count;
      int swap_lbl = big_first_lbl;
      big_first_count = big_second_count;
      big_first_lbl = big_second_lbl;
      big_second_count = swap;
      big_second_lbl = swap_lbl;
    }
  }
  mln_VAR(big_third, threshold | pw::value(labels) == big_third_lbl);

  data::fill((big_second | pw::value(labels) == big_second_lbl).rw(), true);

  // Gradient.

  mln_ch_value(I, bool) gradient = morpho::elementary::gradient_internal(big_second, nbh);
  mln_VAR(gradient_map, gradient | pw::value(gradient) == true);

  mln_ch_value(I, rgb8) result = level::convert(rgb8(), input);
  data::fill((result | gradient_map.domain()).rw(), literal::red);

  // Center.

  accu::center<mln_site(I)> center_;
  mln_site(I) center = set::compute(center_, big_third.domain());
  result(center) = literal::red;

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

  std::cout << "Nbr sites = " << arr.nsites() << std::endl;
  io::cloud::save(arr, "cloud.txt");

  return result;
}

int main()
{
  using namespace mln;
  using value::int_u8;
  using value::label_16;

  trace::quiet = false;

//  image2d<int_u8> src;
//  io::pgm::load(src, "img/slice_7.pgm");
  image3d<int_u8> vol;
  io::dump::load(vol, "img/IRM.dump");

  label_16 nlabels;

//  io::ppm::save(igr(src, c4(), nlabels), "out.ppm");
  io::dump::save(igr(vol, c6(), nlabels), "out.raw");
}
