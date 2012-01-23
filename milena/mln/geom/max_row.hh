// Copyright (C) 2007, 2008, 2009, 2010, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_GEOM_MAX_ROW_HH
# define MLN_GEOM_MAX_ROW_HH

/// \file
///
/// Give the maximum row of an image.

# include <mln/core/concept/image.hh>
# include <mln/geom/bbox.hh>


namespace mln
{

  namespace geom
  {

    /*! \brief Give the maximum row of an image.

      \ingroup mlngeom
     */
    template <typename I>
    mln_deduce(I, site, coord) max_row(const Image<I>& ima);


    /*! \brief Give the maximum row of an box 2d or 3d.

      \ingroup mlngeom
     */
    template <typename B>
    mln_deduce(B, site, coord) max_row(const Box<B>& b);

# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_deduce(I, site, coord) max_row(const Image<I>& ima)
    {
      mln_precondition(exact(ima).is_valid());
      mln_deduce(I, site, coord) maxrow = geom::bbox(ima).pmax().row();

      return maxrow;
    }


    template <typename B>
    inline
    mln_deduce(B, site, coord) max_row(const Box<B>& b)
    {
      metal::not_<metal::equal<metal::int_<B::dim>, metal::int_<1> > >::check();
      mln_deduce(B, site, coord) maxrow = exact(b).pmax().row();

      return maxrow;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_MAX_ROW_HH
