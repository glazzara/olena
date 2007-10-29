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

#ifndef MLN_MORPHO_MINUS_HH
# define MLN_MORPHO_MINUS_HH

/*!
 * \file  mln/morpho/minus.hh
 *
 * \brief Definition of a function that make a minus (logical "and
 * not" or arithmetical min) of an image.
 */

# include <mln/level/compare.hh>
# include <mln/logical/and_not.hh>
# include <mln/arith/minus.hh>


namespace mln
{

  namespace morpho
  {

    /*! Morphological minus: either a logical "and not" (if morpho on
     *  sets) or an arithmetical minus (if morpho on functions).
     */
    template <typename I, typename J>
    mln_concrete(I) minus(const Image<I>& lhs, const Image<J>& rhs);


//     /*! Morphological minus, inplace version: either a logical "and
//      *  not" (if morpho on sets) or an arithmetical minus (if morpho
//      *  on functions).
//      */
//     template <typename I, typename J>
//     void minus_inplace(Image<I>& lhs, const Image<J>& rhs);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Binary => morphology on sets.

      template <typename I, typename J>
      mln_concrete(I) minus_(trait::image::kind::logic,
			     const I& lhs, const J& rhs)
      {
	return logical::and_not(lhs, rhs);
      }

      // Otherwise => morphology on functions.

      template <typename I, typename J>
      mln_concrete(I) minus_(trait::image::kind::any,
			     const I& lhs, const J& rhs)
      {
	return arith::minus<mln_value(I)>(lhs, rhs);
      }

    } // end of namespace mln::morpho::impl


    // Facades.

    template <typename I, typename J>
    mln_concrete(I)
      minus(const Image<I>& lhs, const Image<J>& rhs)
    {
      trace::entering("morpho::minus");
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      mln_concrete(I) output = impl::minus_(mln_trait_image_kind(I)(),
					    exact(lhs), exact(rhs));

      trace::exiting("morpho::minus");
      return output;
    }

//     template <typename I, typename J>
//     void minus_inplace(Image<I>& lhs, const Image<J>& rhs)
//     {
//       mln_precondition(exact(rhs).domain() == exact(lhs).domain());
//       morpho::minus(lhs, rhs, lhs); // Calls the previous version.
//     }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_MINUS_HH
