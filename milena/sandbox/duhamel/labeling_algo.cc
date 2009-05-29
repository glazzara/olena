// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file sandbox/duhamel/labeling_algo.cc
 *
 * \brief Test on mln::labeling::foreground.
 */

# include <mln/core/image2d_b.hh>
# include <mln/core/image/dmorph/sub_image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/data/fill.hh>
# include <mln/data/stretch.hh>
# include <mln/border/fill.hh>
# include <mln/io/pbm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/labeling/foreground.hh>
# include <mln/debug/println.hh>
# include <mln/draw/mesh.hh>
# include <mln/geom/seeds2tiling.hh>
# include <mln/make/voronoi.hh>

int main()
{
  using namespace mln;
  using value::int_u8;

  //  image2d_b<bool> in = io::pbm::load("../../img/toto.pbm");
  image2d_b<bool> in = io::pbm::load("toto.pbm");

  image2d_b<int_u8> lab(in.domain());
  image2d_b<int_u8> inte(in.domain());
  image2d_b<int_u8> out(in.domain());

  unsigned n;
  labeling::foreground(in, c8(), lab, n);
  std::cout << "number of labels = " << n << std::endl;
  std::vector<int_u8> vec;

  image2d_b<int> input(in.domain());
  data::fill(input, lab);
  lab(point2d (0,0)) = 0;

  inte = geom::seeds2tiling(lab, c4 ());
  border::fill (inte, 0);

  image2d_b<int_u8> inte2(inte.domain());

  data::stretch (inte, inte2);

  io::pgm::save(inte2, "inte.pgm");

  mesh_p<point2d> m = make::voronoi(inte, lab, c4());
  std::cout << "OK : generate inte.pgm and out.pgm" << std::endl;
  draw::mesh (out, m, 255, 128);

  io::pgm::save(out, "out.pgm");
}
