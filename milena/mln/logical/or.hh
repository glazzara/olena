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

#ifndef MLN_LOGICAL_OR_HH
# define MLN_LOGICAL_OR_HH

/*! \file mln/logical/or.hh
 *
 * \brief Point-wise "logical or" between binary images.
 *
 * \todo Add static assertion and save one iterator in in-place version.
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace logical
  {

    /*! Point-wise "logical or" between images \p lhs and \p rhs.
     *
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \param[out] output The result image.
     *
     * \pre \p output.domain == \p lhs.domain == \p rhs.domain
     */
    template <typename L, typename R, typename O>
    void or_(const Image<L>& lhs, const Image<R>& rhs, Image<O>& output);


    /*! Point-wise in-place "logical or" of image \p rhs in image \p lhs.
     *
     * \param[in] lhs First operand image.
     * \param[in,out] rhs Second operand image.
     *
     * It performs: \n
     *   for all p of rhs.domain \n
     *     lhs(p) = lhs(p) or rhs(p)
     *
     * \pre \p rhs.domain <= \p lhs.domain
     */
    template <typename L, typename R>
    void or_inplace(Image<L>& lhs, const Image<R>& rhs);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename L, typename R, typename O>
      void or__(const Image<L>& lhs_, const Image<R>& rhs_, Image<O>& output_)
      {
	const L& lhs = exact(lhs_);
	const R& rhs = exact(rhs_);
	O& output = exact(output_);
	mln_piter(L) p(lhs.domain());
	for_all(p)
	  output(p) = lhs(p) || rhs(p);
      }

      template <typename L, typename R, typename O>
      void or__(const Fast_Image<L>& lhs, const Fast_Image<R>& rhs, Fast_Image<O>& output)
      {
	mln_pixter(const L) lp(exact(lhs));
	mln_pixter(const R) rp(exact(rhs));
	mln_pixter(O)       op(exact(output));
	for_all_3(lp, rp, op)
	  op.val() = lp.val() || rp.val();
      }

    } // end of namespace mln::logical::impl


    // Facades.

    template <typename L, typename R, typename O>
    void or_(const Image<L>& lhs, const Image<R>& rhs, Image<O>& output)
    {
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());
      mln_precondition(exact(output).domain() == exact(lhs).domain());
      impl::or__(exact(lhs), exact(rhs), exact(output));
    }

    template <typename L, typename R>
    void or_inplace(Image<L>& lhs, const Image<R>& rhs)
    {
      mln_precondition(exact(rhs).domain() <= exact(lhs).domain());
      impl::or__(exact(lhs), exact(rhs), exact(lhs));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::logical

} // end of namespace mln


#endif // ! MLN_LOGICAL_OR_HH
