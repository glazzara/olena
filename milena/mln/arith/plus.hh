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

#ifndef MLN_ARITH_PLUS_HH
# define MLN_ARITH_PLUS_HH

/*! \file mln/arith/plus.hh
 *
 * \brief Point-wise addition between images.
 *
 * \todo Speedup versions with cst.
 */

# include <mln/core/concept/image.hh>

# include <mln/pw/cst.hh>
# include <mln/pw/image.hh>
# include <mln/trait/op_plus.hh>


namespace mln
{


//   namespace trait
//   {

//     template <typename L, typename R>
//     struct op_plus< Image,L, Image,R >
//     {
//       typedef mln_trait_op_plus(mln_value(L), mln_value(R)) value;
//       typedef mln_ch_value(L, value) ret;
//     };

//   } // end of namespace mln::trait


  namespace arith
  {

    /*! Point-wise addition of images \p lhs and \p rhs.
     *
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \param[out] output The result image.
     *
     * \pre \p output.domain == \p lhs.domain == \p rhs.domain
     */
    template <typename L, typename R, typename O>
    void plus(const Image<L>& lhs, const Image<R>& rhs, Image<O>& output);


    /*! Point-wise addition of the value \p val to image \p input.
     *
     * \param[in] input The image.
     * \param[in] val The value.
     * \param[out] output The result image.
     *
     * \pre \p output.domain == \p input.domain
     */
    template <typename I, typename V, typename O>
    void plus_cst(const Image<I>& input, const V& val, Image<O>& output);


    /*! Point-wise addition of image \p rhs in image \p lhs.
     *
     * \param[in] lhs First operand image (subject to addition).
     * \param[in,out] rhs Second operand image (to be added to \p lhs).
     *
     * This addition performs: \n
     *   for all p of rhs.domain \n
     *     lhs(p) += rhs(p)
     *
     * \pre \p rhs.domain <= \p lhs.domain
     */
    template <typename L, typename R>
    void plus_inplace(Image<L>& lhs, const Image<R>& rhs);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename L, typename R, typename O>
      void plus_(const Image<L>& lhs_, const Image<R>& rhs_, Image<O>& output_)
      {
	const L& lhs = exact(lhs_);
	const R& rhs = exact(rhs_);
	O& output = exact(output_);
	mln_piter(L) p(lhs.domain());
	for_all(p)
	  output(p) = lhs(p) + rhs(p);
      }

      template <typename L, typename R, typename O>
      void plus_(const Fastest_Image<L>& lhs, const Fastest_Image<R>& rhs, Fastest_Image<O>& output)
      {
	mln_pixter(const L) lp(exact(lhs));
	mln_pixter(const R) rp(exact(rhs));
	mln_pixter(O)       op(exact(output));
	for_all_3(lp, rp, op)
	  op.val() = lp.val() + rp.val();
      }

      template <typename L, typename R>
      void plus_inplace_(Image<L>& lhs_, const Image<R>& rhs_)
      {
	L& lhs = exact(lhs_);
	const R& rhs = exact(rhs_);
	mln_piter(R) p(rhs.domain());
	for_all(p)
	  lhs(p) += rhs(p);
      }

      template <typename L, typename R>
      void plus_inplace_(Fastest_Image<L>& lhs, const Fastest_Image<R>& rhs)
      {
	mln_pixter(L) lp(exact(lhs));
	mln_pixter(const R) rp(exact(rhs));
	for_all_2(rp, lp)
	  lp.val() += rp.val();
      }

    } // end of namespace mln::arith::impl


    // Facades.


    template <typename L, typename R, typename O>
    void plus(const Image<L>& lhs, const Image<R>& rhs, Image<O>& output)
    {
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());
      mln_precondition(exact(output).domain() == exact(lhs).domain());
      impl::plus_(exact(lhs), exact(rhs), exact(output));
    }


//     template <typename L, typename R>
//     mln_trait_op_plus(L, R)
//     plus(const Image<L>& lhs, const Image<R>& rhs)
//     {
//       mln_precondition(exact(rhs).domain() == exact(lhs).domain());
//       mln_precondition(exact(output).domain() == exact(lhs).domain());
//       impl::plus_(exact(lhs), exact(rhs), exact(output));
//     }


    template <typename I, typename V, typename O>
    void plus_cst(const Image<I>& input, const V& val, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      plus(input, pw::cst(val) | exact(input).domain(), output); // Calls the previous version.
    }

    template <typename L, typename R>
    void plus_inplace(Image<L>& lhs, const Image<R>& rhs)
    {
      mln_precondition(exact(rhs).domain() <= exact(lhs).domain());
      impl::plus_inplace_(exact(lhs), exact(rhs));
    }

    template <typename I, typename V>
    void plus_cst_inplace(Image<I>& input, const V& val)
    {
      mln_precondition(exact(input).has_data());
      plus_inplace(input, pw::cst(val) | exact(input).domain()); // Calls the previous version.
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::arith

} // end of namespace mln


#endif // ! MLN_ARITH_PLUS_HH
