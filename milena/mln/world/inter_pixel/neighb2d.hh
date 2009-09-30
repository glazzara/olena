// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_WORLD_INTER_PIXEL_NEIGHB2D_HH
# define MLN_WORLD_INTER_PIXEL_NEIGHB2D_HH

/// \file
/// \brief Common neighborhoods on inter-pixel images.

# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>
# include <mln/world/inter_pixel/dim2/is_row_odd.hh>


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      /// Double window.
      typedef win::multiple<window2d, dim2::is_row_odd> dbl_window2d;

      /// Double neighborhood, used for inter-pixel images.
      typedef neighb< dbl_window2d > dbl_neighb2d;


      /// Vertex (pixel) to neighboring vertices (pixels).
      const neighb2d& v2v();

      /// Vertex (pixel) to neighboring edges (inter-pixels).
      const neighb2d& v2e();

      /// Edge (inter-pixel) to neighborhood vertices (pixels).
      const dbl_neighb2d& e2v();

      /// Edge (inter-pixel) to neighboring edges (inter-pixels).
      const dbl_neighb2d& e2e();



# ifndef MLN_INCLUDE_ONLY

      const neighb2d& v2v()
      {
	static neighb2d it;
	if (it.size() == 0)
	  {
	    static const bool vals[] = { 0, 0, 1, 0, 0,
					 0, 0, 0, 0, 0,
					 1, 0, 0, 0, 1,
					 0, 0, 0, 0, 0,
					 0, 0, 1, 0, 0 };
	    convert::from_to(vals, it);
	  }
	return it;
      }

      const neighb2d& v2e()
      {
	return c4();
      }

      const dbl_neighb2d& e2v()
      {
	static const bool e2v_h[] = { 0, 1, 0,
				      0, 0, 0,
				      0, 1, 0 };

	static const bool e2v_v[] = { 0, 0, 0,
				      1, 0, 1,
				      0, 0, 0 };

	static dbl_neighb2d nbh =
	  make::double_neighb2d(dim2::is_row_odd(), e2v_h, e2v_v);
	return nbh;
      }



      const dbl_neighb2d& e2e()
      {
	static const bool e2e_h[] = { 0, 0, 1, 0, 0,
				      0, 1, 0, 1, 0,
				      0, 0, 0, 0, 0,
				      0, 1, 0, 1, 0,
				      0, 0, 1, 0, 0 };

	static const bool e2e_v[] = { 0, 0, 0, 0, 0,
				      0, 1, 0, 1, 0,
				      1, 0, 0, 0, 1,
				      0, 1, 0, 1, 0,
				      0, 0, 0, 0, 0 };

	static dbl_neighb2d nbh =
	  make::double_neighb2d(dim2::is_row_odd(), e2e_h, e2e_v);
	return nbh;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_NEIGHB2D_HH
