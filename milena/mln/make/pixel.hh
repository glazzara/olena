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

#ifndef MLN_MAKE_PIXEL_HH
# define MLN_MAKE_PIXEL_HH

/// \file mln/make/pixel.hh
/// \brief Routine to construct an mln::pixel.

# include <mln/core/concept/image.hh>
# include <mln/core/pixel.hh>


namespace mln
{

  namespace make
  {

    /// Create a mln::pixel from a constant image \p ima and a point \p p.
    template <typename I>
    mln::pixel<const I> pixel(const Image<I>& ima, const mln_point(I)& p);

    /// Create a mln::pixel from a mutable image \p ima and a point \p p.
    template <typename I>
    mln::pixel<I> pixel(Image<I>& ima, const mln_point(I)& p);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln::pixel<const I> pixel(const Image<I>& ima, const mln_point(I)& p)
    {
      mln::pixel<const I> tmp(exact(ima), p);
      return tmp;
    }

    template <typename I>
    inline
    mln::pixel<I> pixel(Image<I>& ima, const mln_point(I)& p)
    {
      mln::pixel<I> tmp(exact(ima), p);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_PIXEL_HH
