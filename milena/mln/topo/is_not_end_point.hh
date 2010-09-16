// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_IS_NOT_END_POINT_HH
# define MLN_TOPO_IS_NOT_END_POINT_HH

/// \file
/// \brief Non end point predicate in binary images.

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

namespace mln
{

  namespace topo
  {
    /// \brief Functor determining whether a point is not an end point
    /// in image of type \p I, based on neighborhood of type \p N.
    template <typename I, typename N>
    struct is_not_end_point : public Function_v2b< is_not_end_point<I, N> >
    {
      /// Build a functor, and assign an image to it.
      ///
      /// \param  nbh_fg  The foreground neighborhood.
      /// \apram  ima     The image.
      is_not_end_point(const Neighborhood<N>& nbh, const Image<I>& ima);

      // Is \a p not an end point?
      bool operator()(const mln_psite(I)& p) const;

    private:
      /// The foreground neighborhood.
      const N& nbh_;
      /// The image.
      const I& ima_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N>
    inline
    is_not_end_point<I, N>::is_not_end_point(const Neighborhood<N>& nbh,
					     const Image<I>& ima)
      : nbh_(exact(nbh)),
	ima_(exact(ima))
    {
    }

    template <typename I, typename N>
    inline
    bool 
    is_not_end_point<I, N>::operator()(const mln_psite(I)& p) const
    {
      // Number of foreground neighbors pixels.
      unsigned nneighbs = 0;
      mln_niter(N) n(nbh_, p);
      for_all(n)
	if (ima_.has(n) && ima_(n))
	  ++nneighbs;
      return nneighbs != 1;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_NOT_END_POINT_HH
