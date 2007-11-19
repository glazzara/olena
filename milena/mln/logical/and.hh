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

#ifndef MLN_LOGICAL_AND_HH
# define MLN_LOGICAL_AND_HH

/*! \file mln/logical/and.hh
 *
 * \brief Point-wise "logical and" between binary images.
 *
 * \todo Add static assertion and save one iterator in in-place version.
 */

# include <mln/core/concept/image.hh>

// Specializations are in:
# include <mln/logical/and.spe.hh>


namespace mln
{

  namespace logical
  {

    /*! Point-wise "logical and" between images \p lhs and \p rhs.
     *
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \result The result image.
     *
     * \pre \p lhs.domain == \p rhs.domain
     */
    template <typename L, typename R>
    mln_concrete(L) and_(const Image<L>& lhs, const Image<R>& rhs);


    /*! Point-wise in-place "logical and" of image \p rhs in image \p lhs.
     *
     * \param[in,out] lhs First operand image.
     * \param[in] rhs Second operand image.
     *
     * It performs: \n
     *   for all p of rhs.domain \n
     *     lhs(p) = lhs(p) and rhs(p)
     *
     * \pre \p rhs.domain >= \p lhs.domain
     */
    template <typename L, typename R>
    void and_inplace(Image<L>& lhs, const Image<R>& rhs);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename L, typename R, typename O>
	void and__(const L& lhs, const R& rhs, O& output)
	{
	  trace::entering("logical::impl::generic::and__");

	  mln_piter(L) p(lhs.domain());
	  for_all(p)
	    output(p) = lhs(p) && rhs(p);

	  trace::exiting("logical::impl::generic::and__");
	}

      } // end of namespace mln::logical::impl::generic

    } // end of namespace mln::logical::impl


    // Facades.

    template <typename L, typename R>
    mln_concrete(L) and_(const Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("logical::and_");
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      mln_concrete(L) output;
      initialize(output, lhs);
      impl::and__(mln_trait_image_speed(L)(), exact(lhs),
		  mln_trait_image_speed(R)(), exact(rhs), output);

      trace::exiting("logical::and_");
      return output;
    }

    template <typename L, typename R>
    void and_inplace(Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("logical::and_inplace");
      mln_precondition(exact(rhs).domain() >= exact(lhs).domain());

      impl::and__(mln_trait_image_speed(L)(), exact(lhs),
		  mln_trait_image_speed(R)(), exact(rhs), exact(lhs));

      trace::exiting("logical::and_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::logical

} // end of namespace mln


#endif // ! MLN_LOGICAL_AND_HH
