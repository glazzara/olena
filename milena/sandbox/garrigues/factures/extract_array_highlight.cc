// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/geom/resize.hh>
#include <mln/display/all.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/opening.hh>
#include <mln/win/hline2d.hh>
#include <mln/win/vline2d.hh>
#include <mln/win/disk2d.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/inplace.hh>
#include <mln/level/stretch.hh>
#include <mln/level/median.hh>
#include <mln/morpho/gradient.hh>

#include <mln/labeling/level.hh>
#include <mln/literal/all.hh>

#include <mln/core/alias/neighb2d.hh>

#include <mln/accu/all.hh>

#include <mln/draw/box.hh>

using namespace mln;
typedef image2d<bool> ima2d_bool;
typedef image2d<unsigned> ima2d_unsigned;

image2d<bool>
filter_arrays(image2d<bool> in)
{
  // Labeling.
  unsigned nlabels;
  image2d<unsigned> labels = labeling::level(in, true, c4(), nlabels);

  // Get the caracteristics of the connected components.
  std::vector< accu::pair_< accu::bbox<point2d>, accu::count_<point2d> > > caracteristics(nlabels + 1);
  mln_fwd_piter_(ima2d_unsigned) p(labels.domain());
  for_all(p)
      caracteristics[labels(p)].take(p);

  // Filter each connected components.
  std::vector<bool> is_array(nlabels);
  for (int i = 0; i < nlabels; i++)
  {
    box_<point2d> b = caracteristics[i].to_result().first;
    unsigned n = caracteristics[i].to_result().second;
    float ratio;
    if (b.pmax() == b.pmin() || n < 10)
    {
      is_array[i] = false;
      continue;
    }

    if (0 != (b.pmax()[1] - b.pmin()[1]))
      ratio = float(b.pmax()[0] - b.pmin()[0]) / (b.pmax()[1] - b.pmin()[1]);
    else
      ratio = float(b.pmax()[1] - b.pmin()[1]) / (b.pmax()[0] - b.pmin()[0]);

    int area = (b.pmax()[0] - b.pmin()[0]) * (b.pmax()[1] - b.pmin()[1]);
    is_array[i] = ratio < 0.1 || ratio > 10 || (float(n) / area) < 0.2;
  }

  // Clean the image.
  image2d<bool> output(in.domain());
  for_all(p)
    output(p) = is_array[labels(p)];

  return output;
}

image2d<value::rgb8>
highlight_vlines(image2d<bool> in)
{

  image2d<bool> tmp;
  tmp = filter_arrays(in);

  // Opening.
  tmp = morpho::opening(tmp, win::vline2d(5));

  image2d<value::rgb8> output(tmp.domain());
  mln_fwd_piter_(ima2d_unsigned) p(tmp.domain());
  for_all(p)
    if (tmp(p))
      output(p) = literal::red;
    else
      output(p) = literal::black;

  return output;
}


image2d<value::rgb8>
highlight_hlines(image2d<bool> in)
{

  image2d<bool> tmp;
  tmp = filter_arrays(in);
  // Opening.
  tmp = morpho::opening(tmp, win::hline2d(5));

  image2d<value::rgb8> output(tmp.domain());
  mln_fwd_piter_(ima2d_unsigned) p(tmp.domain());
  for_all(p)
    if (tmp(p))
      output(p) = literal::green;
    else
      output(p) = literal::black;

  return output;
}


int main()
{

  image2d<value::int_u8> in;

  io::pgm::load(in , "facture.pgm");

  // Binarisation.
  ima2d_bool bin(in.domain());
  level::paste(inplace(pw::value(in) > pw::cst(50) | in.domain()), bin);

  image2d<value::rgb8> output_h = highlight_hlines(bin);
  image2d<value::rgb8> output_v = highlight_vlines(bin);

  image2d<value::rgb8> sum = output_v + output_h;
  io::ppm::save(sum, "array.ppm");
}
