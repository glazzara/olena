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

/*! \file tests/seeds2tiling.cc
 *
 * \brief Tests on mln::geom::seed2tiling.
 */


# include <mln/core/image2d.hh>
# include <mln/core/sub_image.hh>
# include <mln/core/neighb2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/level/fill.hh>
# include <mln/level/stretch.hh>
# include <mln/border/fill.hh>
# include <mln/io/pbm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/labeling/foreground.hh>
# include <mln/debug/println.hh>
# include <mln/draw/mesh.hh>
# include <mln/geom/seeds2tiling.hh>
# include <mln/geom/seeds2tiling_with_chamfer.hh>
# include <mln/make/voronoi.hh>

#include <mln/core/image2d.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>

#include <mln/core/w_window2d_int.hh>
#include <mln/make/win_chamfer.hh>

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cerr << "missing argument : specify the location of the binary image (*.pbm)" << std::endl;
      return 1;
    }
  using namespace mln;
  using value::int_u8;
  unsigned max = 2048;

  image2d<bool> input = io::pbm::load(argv[1]);

  {

    image2d<unsigned> lab(input.domain());
    image2d<unsigned> inte(input.domain());
    image2d<int_u8> inte2(input.domain());
    image2d<int_u8> out(input.domain());

    const w_window2d_int& w_win = win_chamfer::mk_chamfer_3x3_int<2,3> ();

    unsigned n;
    labeling::foreground(input, c4(), lab, n);
    std::cout << "number of labels = " << n << std::endl;


    inte = geom::seeds2tiling(lab, c4 ());
    border::fill (inte, 0);
    level::stretch (inte, inte2);
    io::pgm::save(inte2, "ima1.pgm");

    std::cout << "ima1 generate with seeds2tiling"
	     << std::endl;
    inte = geom::seeds2tiling_with_chamfer(lab, w_win, max, c4 ());
    border::fill (inte, 0);
    level::stretch (inte, inte2);

    io::pgm::save(inte2, "ima2.pgm");
    std::cout << "ima2 generate with seeds2tiling_with_chamfer"
	     << std::endl;
  }

}
