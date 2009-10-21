// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_MAX_HH
# define MLN_MORPHO_MAX_HH

/*!
 * \file
 *
 * \brief Definition of a function that performs a point-wise operator
 * on a couple of images: a logical 'or' in the case of binary images
 * (sets), a 'max' otherwise (functions).
 */

# include <mln/data/compare.hh>
# include <mln/logical/or.hh>
# include <mln/arith/max.hh>


namespace mln
{

  namespace morpho
  {

    /*! Morphological max: either a logical "or" (if morpho on sets)
     *  or an arithmetical max (if morpho on functions).
     */
    template <typename I, typename J>
    mln_concrete(I)
      max(const Image<I>& lhs, const Image<J>& rhs);


    /*! Morphological max, inplace version: either a logical "or" (if
     *  morpho on sets) or an arithmetical max (if morpho on
     *  functions).
     */
    template <typename I, typename J>
    void max_inplace(Image<I>& lhs, const Image<J>& rhs);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Binary => morphology on sets.

      template <typename I, typename J, typename O>
      inline
      mln_concrete(I) max_(trait::image::kind::logic,
			   const I& lhs, const J& rhs)
      {
	return logical::or_(lhs, rhs);
      }

      template <typename I, typename J>
      inline
      void max_inplace_(trait::image::kind::logic,
			I& lhs, const J& rhs)
      {
	logical::or_inplace(lhs, rhs);
      }

      // Otherwise => morphology on functions.

      template <typename I, typename J>
      inline
      mln_concrete(I) max_(trait::image::kind::any,
			   const I& lhs, const J& rhs)
      {
	return arith::max(lhs, rhs);
      }

      template <typename I, typename J>
      inline
      void max_inplace_(trait::image::kind::any,
			I& lhs, const J& rhs)
      {
	arith::max_inplace(lhs, rhs);
      }

    } // end of namespace mln::morpho::impl


    // Facades.

    template <typename I, typename J>
    inline
    mln_concrete(I)
      max(const Image<I>& lhs, const Image<J>& rhs)
    {
      trace::entering("morpho::max");
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      mln_concrete(I) output = impl::max_(mln_trait_image_kind(I)(), exact(lhs), exact(rhs));

      trace::exiting("morpho::max");
      return output;
    }

    template <typename I, typename J>
    inline
    void max_inplace(Image<I>& lhs, const Image<J>& rhs)
    {
      trace::entering("morpho::max_inplace");
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      impl::max_inplace_(mln_trait_image_kind(I)(), exact(lhs), exact(rhs));

      trace::exiting("morpho::max_inplace_");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_MAX_HH
