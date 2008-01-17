// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#include <mln/core/image2d.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/geom/resize.hh>
#include <mln/display/all.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/closing.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/pw/all.hh>
#include <mln/core/inplace.hh>
#include <mln/level/stretch.hh>
#include <mln/labeling/level.hh>
#include <mln/literal/all.hh>

#include <mln/core/neighb2d.hh>

#include <mln/accu/all.hh>

#include <mln/draw/box.hh>

int main()
{
  using namespace mln;
  using namespace mln;

  typedef image2d<bool> ima2d_bool;
  typedef image2d<unsigned> ima2d_unsigned;

  image2d<value::int_u8> in;

  io::pgm::load(in , "facture.pgm");

  // Resize 50%.
  image2d<value::int_u8> small = in; //geom::resize(in, 0.5);

  // Binarisation.
  ima2d_bool bin(small.domain());
  level::paste(inplace(pw::value(small) > pw::cst(50) | small.domain()), bin);

  // Labeling.
  unsigned nlabels;
  image2d<unsigned> labels = labeling::level(bin, true, c4(), nlabels);

  // Get the caracteristics of the connected components.
  std::vector< accu::pair_< accu::bbox<point2d>, accu::count_<point2d> > > caracteristics(nlabels);
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
    is_array[i] = ratio < 0.14 || ratio > 7 || (float(n) / area) < 0.2;
  }

  // Clean the image.
  image2d<value::rgb8> output(small.domain());
  for_all(p)
    if (is_array[labels(p)])
      output(p) = literal::white;
    else
      output(p) = literal::black;

  // Draw the bounding boxes.
  for (int i = 0; i < nlabels; i++)
    if (is_array[i])
      draw::box(output, inplace(caracteristics[i].to_result().first), inplace(value::rgb8(literal::green)));

  io::ppm::save(output, "array.ppm");
}
