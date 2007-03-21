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

#ifndef OLN_BORDER_FILL_HH
# define OLN_BORDER_FILL_HH

# include <oln/core/concept/image.hh>


namespace oln
{

  namespace border
  {

    // Fwd decl.

    template <typename I>
    void fill(const Image<I>& input, const oln_value(I)& value);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

#  ifdef OLN_ENV_2D

      template <typename I>
      void fill_2D_(I& input, const oln_value(I)& value)
      {
	oln_point(I)
	  pmin(input.min_row() - input.border(),
	       input.min_col() - input.border()),
	  pmax(input.max_row() + input.border(),
	       input.max_col() + input.border());
	if (not input.owns_(pmin) or not input.owns_(pmax))
	  return;
	oln_coord(I) row, col;
	for (col = pmin.col(); col <= pmax.col(); ++col)
	  {
	    // top
	    for (row = input.min_row() - input.border();
		 row <= input.min_row() - 1;
		 ++row)
	      input.at(row, col) = value;
	    // bot
	    for (row = input.max_row() + 1;
		 row <= input.max_row() + int(input.border());
		 ++row)
	      input.at(row, col) = value;
	  }
	for (row = input.min_row();
	     row <= input.max_row();
	     ++row)
	  {
	    // left
	    for (col = input.min_col() - input.border();
		 col <= input.min_col() - 1;
		 ++col)
	      input.at(row, col) = value;
	    // right
	    for (col = input.max_col() + 1;
		 col <= input.max_col() + int(input.border());
		 ++col)
	      input.at(row, col) = value;
	  }
      }

      template <typename I>
      void fill_nD_(Image_2D<I>& input, const oln_value(I)& value)
      {
	fill_2D_(exact(input), value);
      }

#  endif // OLN_ENV_2D


      template <typename I>
      void fill_(const Image_with_Border<I>& input, const oln_value(I)& value)
      {
	// FIXME: Requires also Point_Wise_Accessible_Image, Mutable_Image...
	I& input_ = const_cast<I&>(exact(input));
	fill_nD_(input_, value);
      }

      template <typename I>
      void fill_(const Image<I>&, const oln_value(I)&)
      {
	// no-op
      }

    } // end of namespace oln::border::impl


    // Facade.

    template <typename I>
    void fill(const Image<I>& input, const oln_value(I)& value)
    {
      impl::fill_(exact(input), value);
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::border

} // end of namespace oln


#endif // ! OLN_BORDER_FILL_HH
