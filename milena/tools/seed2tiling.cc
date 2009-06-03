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

/// \file tools/seed2tiling.cc
///
/// Tool using mln::geom::seed2tiling.



# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/sub_image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/data/stretch.hh>
# include <mln/border/fill.hh>
# include <mln/io/pbm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/labeling/foreground.hh>
# include <mln/debug/println.hh>
# include <mln/geom/seeds2tiling.hh>
# include <mln/geom/seeds2tiling_roundness.hh>
# include <mln/make/voronoi.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/image_if.hh>


#include <mln/core/alias/w_window2d_int.hh>
#include <mln/make/win_chamfer.hh>

int main(int argc, char* argv[])
{
  if (argc < 2)
    {
      std::cerr << "missing argument: "
		<< "specify the location of the binary image (*.pbm)"
		<< std::endl;
      return 1;
    }
  using namespace mln;
  using value::int_u8;
  unsigned max = 2048;

  image2d<bool> input = io::pbm::load(argv[1]);

  {
    const w_window2d_int& w_win = make::mk_chamfer_3x3_int<2,3>();

    unsigned n;
    image2d<unsigned> lab = labeling::foreground(input, c4(), n);
    std::cout << "number of labels = " << n << std::endl;


    image2d<unsigned> inte = geom::seeds2tiling(lab, c4());
    border::fill(inte, 0);

    image2d<int_u8> inte2 = data::stretch(int_u8(), inte);
    io::pgm::save(inte2, "ima1.pgm");

    std::cout << "ima1 generate with seeds2tiling"
	     << std::endl;
    inte = geom::seeds2tiling_roundness(lab, w_win, max, c4());
    border::fill(inte, 0);

    inte2 = data::stretch(int_u8(), inte);

    io::pgm::save(inte2, "ima2.pgm");
    std::cout << "ima2 generate with seeds2tiling_roundness"
	      << std::endl;
  }

}
