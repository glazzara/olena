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

/*! \file tests/slow_seeds2tiling.cc
 *
 * \brief Test on mln::geom::seed2tiling*.hh.
 */


# include <mln/core/image/image2d.hh>
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
# include <mln/geom/seeds2tiling_with_chamfer.hh>
# include <mln/make/voronoi.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image_if_value.hh>


#include <mln/core/alias/w_window2d_int.hh>
#include <mln/make/win_chamfer.hh>


# include <mln/core/queue_p.hh>
# include <mln/core/routine/duplicate.hh>
# include <mln/accu/mean.hh>
# include <mln/estim/min_max.hh>
# include <mln/algebra/vec.hh>

# include <mln/norm/l2.hh>
# include <mln/norm/l1.hh>
# include <mln/norm/infty.hh>

namespace mln
{

  namespace geom
  {
    template <typename I>
    I
    slow_seeds2tiling (Image<I>& input_,
		       float max)
    {
      I& input = exact(input_);
      I output = duplicate(input_);
      
      mln_piter(I) p1(input.domain());
      mln_piter(I) p2(input.domain());
      
      for_all(p1) if (input(p1) == 0)
	{
	  float d = max;
	  for_all(p2) if (input(p2) != 0)
	    {
	      mln_dpoint(I) dp = p1 - p2;
	      float new_distance = norm::l2<2, unsigned> (dp.to_vec ());
	      if (new_distance < d)
		{
		  output(p1) = input(p2);
		  d = new_distance;
		}
	    }
	}
      return output;
    }
  }
}

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cerr << "missing argument : specify the location of the binary image (*.pbm)" << std::endl;
      return 1;
    }
  using namespace mln;
  using value::int_u8;
  float max = 2048;

  image2d<bool> input = io::pbm::load(argv[1]);

  {
    image2d<unsigned> lab(input.domain());
    image2d<unsigned> inte(input.domain());
    image2d<int_u8> inte2(input.domain());
    image2d<int_u8> out(input.domain());

    //    const w_window2d_int& w_win = win_chamfer::mk_chamfer_3x3_int<2,3> ();

    unsigned n;
    labeling::foreground(input, c4(), lab, n);
    std::cout << "number of labels = " << n << std::endl;


    inte = geom::slow_seeds2tiling(lab, max);
    border::fill (inte, 0);
    data::stretch (inte, inte2);
    io::pgm::save(inte2, "ima_slow.pgm");
    std::cout << "ima_slow generate with seeds2tiling"
	      << std::endl;

    inte = geom::seeds2tiling(lab, c4 ());
    border::fill (inte, 0);
    data::stretch (inte, inte2);
    io::pgm::save(inte2, "ima1.pgm");
    std::cout << "ima1 generate with seeds2tiling"
	      << std::endl;


//     std::cout << "ima1 generate with seeds2tiling"
// 	     << std::endl;
//     inte = geom::seeds2tiling_with_chamfer(lab, w_win, max, c4 ());
//     border::fill (inte, 0);
//     data::stretch (inte, inte2);

//     io::pgm::save(inte2, "ima2.pgm");
//     std::cout << "ima2 generate with seeds2tiling_with_chamfer"
// 	     << std::endl;
  }

}
