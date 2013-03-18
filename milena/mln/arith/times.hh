// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ARITH_TIMES_HH
# define MLN_ARITH_TIMES_HH

/// \file
///
/// Point-wise multiplication between images.
///
/// \todo Speedup; some versions are not optimal.

# include <mln/arith/includes.hh>


// Specializations are in:
# include <mln/arith/times.spe.hh>

namespace mln
{


  namespace trait
  {

    template <typename L, typename R>
    struct set_binary_< op::times, Image, L, Image, R >
    {
      typedef mln_trait_op_times(mln_value(L), mln_value(R)) value;
      typedef mln_ch_value(L, value) ret;
    };

    template <typename I, typename S>
    struct set_binary_< op::times, Image, I, mln::value::Scalar, S >
    {
      typedef mln_trait_op_times(mln_value(I), S) value;
      typedef mln_ch_value(I, value) ret;
    };

  } // end of namespace mln::trait



  template <typename L, typename R>
  mln_trait_op_times(L,R)
  operator*(const Image<L>& lhs, const Image<R>& rhs);

  template <typename L, typename R>
  L&
  operator*=(Image<L>& lhs, const Image<R>& rhs);


  template <typename I, typename S>
  mln_trait_op_times(I,S)
  operator*(const Image<I>& ima, const value::Scalar<S>& s);

  template <typename I, typename S>
  I&
  operator*=(Image<I>& ima, const value::Scalar<S>& s);



  namespace arith
  {

    /// Point-wise addition of images \p lhs and \p rhs.
    /*!
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \param[out] output The result image.
     *
     * \pre \p output.domain == \p lhs.domain == \p rhs.domain
     */
    template <typename L, typename R, typename O>
    void times(const Image<L>& lhs, const Image<R>& rhs, Image<O>& output);


    /// Point-wise addition of the value \p val to image \p input.
    /*!
     * \param[in] input The image.
     * \param[in] val The value.
     * \param[out] output The result image.
     *
     * \pre \p output.domain == \p input.domain
     */
    template <typename I, typename V, typename O>
    void times_cst(const Image<I>& input, const V& val, Image<O>& output);


    /// Point-wise addition of image \p rhs in image \p lhs.
    /*!
     * \param[in] lhs First operand image (subject to addition).
     * \param[in,out] rhs Second operand image (to be added to \p lhs).
     *
     * This addition performs: \n
     *   for all p of rhs.domain \n
     *     lhs(p) *= rhs(p)
     *
     * \pre \p rhs.domain <= \p lhs.domain
     */
    template <typename L, typename R>
    void times_inplace(Image<L>& lhs, const Image<R>& rhs);


  } // end of namespace mln::arith




# ifndef MLN_INCLUDE_ONLY


  template <typename L, typename R>
  inline
  mln_trait_op_times(L,R)
  operator*(const Image<L>& lhs, const Image<R>& rhs)
  {
    mln_precondition(exact(rhs).domain() == exact(lhs).domain());
    mln_trait_op_times(L,R) tmp;
    initialize(tmp, lhs);
    arith::times(lhs, rhs, tmp);
    return tmp;
  }

  template <typename L, typename R>
  inline
  L&
  operator*=(Image<L>& lhs, const Image<R>& rhs)
  {
    mln_precondition(exact(rhs).domain() == exact(lhs).domain());
    arith::times_inplace(lhs, rhs);
    return exact(lhs);
  }


  template <typename I, typename S>
  inline
  mln_trait_op_times(I,S)
  operator*(const Image<I>& ima, const value::Scalar<S>& s)
  {
    mln_precondition(exact(ima).is_valid());
    mln_trait_op_times(I,S) tmp;
    initialize(tmp, ima);
    arith::times_cst(ima, exact(s), tmp);
    return tmp;
  }

  template <typename I, typename S>
  inline
  I&
  operator*=(Image<I>& ima, const value::Scalar<S>& s)
  {
    mln_precondition(exact(ima).is_valid());
    arith::times_cst(ima, exact(s), ima);
    return exact(ima);
  }



  namespace arith
  {

    namespace impl
    {

      namespace generic
      {

	template <typename L, typename R, typename O>
	inline
	void times_(const L& lhs, const R& rhs, O& output)
	{
	  mln_trace("arith::impl::generic::times_");

	  mln_piter(L) p(lhs.domain());
	  for_all(p)
	    output(p) = lhs(p) * rhs(p);

	}

	template <typename L, typename R>
	inline
	void times_inplace_(L& lhs, const R& rhs)
	{
	  mln_trace("arith::impl::generic::times_inplace_");

	  mln_piter(R) p(rhs.domain());
	  for_all(p)
	  lhs(p) *= rhs(p);

	}

      } // end of namespace mln::arith::impl::generic

    } // end of namespace mln::arith::impl


    // Facades.

    template <typename L, typename R, typename O>
    inline
    void times(const Image<L>& lhs, const Image<R>& rhs, Image<O>& output)
    {
      mln_trace("arith::times");

      mln_precondition(exact(rhs).domain() == exact(lhs).domain());
      mln_precondition(exact(output).domain() == exact(lhs).domain());
      impl::times_(mln_trait_image_speed(L)(), exact(lhs),
		  mln_trait_image_speed(R)(), exact(rhs),
		  mln_trait_image_speed(O)(), exact(output));

    }

    template <typename I, typename V, typename O>
    inline
    void times_cst(const Image<I>& input, const V& val, Image<O>& output)
    {
      mln_trace("arith::times_cst");

      mln_precondition(exact(output).domain() == exact(input).domain());
      times(input, pw::cst(val) | exact(input).domain(), output);
      // Calls the previous version.

    }

    template <typename L, typename R>
    inline
    void times_inplace(Image<L>& lhs, const Image<R>& rhs)
    {
      mln_trace("arith::times_inplace");

      mln_precondition(exact(rhs).domain() <= exact(lhs).domain());
      impl::times_inplace_(mln_trait_image_speed(L)(), exact(lhs),
			   mln_trait_image_speed(R)(), exact(rhs));

    }

    template <typename I, typename V>
    inline
    void times_cst_inplace(Image<I>& input, const V& val)
    {
      mln_trace("arith::times_cst_inplace");

      mln_precondition(exact(input).is_valid());
      times_inplace(input, pw::cst(val) | exact(input).domain());
      // Calls the previous version.

    }

  } // end of namespace mln::arith

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_ARITH_TIMES_HH
