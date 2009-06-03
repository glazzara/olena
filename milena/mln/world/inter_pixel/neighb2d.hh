// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_WORLD_INTER_PIXEL_NEIGHB2D_HH
# define MLN_WORLD_INTER_PIXEL_NEIGHB2D_HH

/// \file mln/world/inter_pixel/neighb2d.hh
///
/// \brief Common neighborhood on inter-pixel images.

# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>
# include <mln/world/inter_pixel/dim2/is_row_odd.hh>


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      /// Double neighborhood used for inter-pixel images.
      typedef neighb< win::multiple<window2d, dim2::is_row_odd> > dbl_neighb2d;

      /// C4 neighborhood on pixels centered on an edge.
      const dbl_neighb2d& e2c();

      /// C8 neighborhood on edges centered on an edge.
      const dbl_neighb2d& e2e();


# ifndef MLN_INCLUDE_ONLY

      const dbl_neighb2d& e2c()
      {
	static const bool e2c_h[] = { 0, 1, 0,
				      0, 0, 0,
				      0, 1, 0 };

	static const bool e2c_v[] = { 0, 0, 0,
				      1, 0, 1,
				      0, 0, 0 };

	static dbl_neighb2d nbh = make::double_neighb2d(dim2::is_row_odd(), e2c_h, e2c_v);
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

	static dbl_neighb2d nbh = make::double_neighb2d(dim2::is_row_odd(), e2e_h, e2e_v);
	return nbh;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_NEIGHB2D_HH
