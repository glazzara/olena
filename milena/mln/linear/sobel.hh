// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LINEAR_SOBEL_HH
# define MLN_LINEAR_SOBEL_HH

/*! \file mln/linear/sobel.hh
 *
 *  \brief Sobel filter.
 *
 *  \todo Extends to the case of other dimensions (only the 2-d case
 *  is handled here).
 */

# include <mln/trait/ch_value.hh>
# include <mln/trait/value/nature.hh>

# include <mln/metal/int.hh>

# include <mln/arith/plus.hh>
# include <mln/level/abs.hh>
# include <mln/fun/v2v/norm.hh>
# include <mln/level/transform.hh>
# include <mln/linear/line_x2_convolve.hh>

# include <mln/value/stack.hh>

// FIXME: We might want to move these macros to another file, like
// mln/core/gradient.hh.


/// \brief Get the gradient component image type associated to image \a I.
#define mln_gradient_component(I)					\
  /* FIXME: Use mln_sum_x (or mln_sum_product) instead of mln_sum.  */	\
  mln_ch_value(I, mln_sum(mln_value(I)))

/// \brief Get the gradient image type associated to image \a I.
#define mln_gradient(I)						\
  mln::value::stack_image<I::point::dim, mln_gradient_component(I)>


namespace mln
{

  namespace linear
  {

    /// Sobel gradient components.
    /// \{
    /// Compute the vertical component of the Sobel gradient.
    template <typename I>
    mln_gradient_component(I) sobel_h(const Image<I>& input);

    /// Compute the horizontal component of the Sobel gradient.
    template <typename I>
    mln_gradient_component(I) sobel_v(const Image<I>& input);
    /// \}

    /// Compute the Sobel gradient of an image.
    ///
    /// \param[in] input A vectorial output image.
    /// \{
    template <typename I>
    mln_gradient(I) sobel(const Image<I>& input);
    /// \}

    /// Compute the norm of the Sobel gradient of an image.
    ///
    /// \param input  a vectorial input image
    /// \param norm   a norm functor (\see mln/fun/v2v/norm.hh),
    ///               defaulting to the L1-norm (mln::fun::v2v::l1_norm).
    /// \return the norm-image of the Sobel gradient of the input image
    /// FIXME: Add constraints on N (mln::Function_v2v<N>).
    /// \{
    template <typename I, typename N>
    mln_ch_value(I, mln_result(N)) sobel_norm(const Image<I>& input, N norm);
    /// \}


    /// \brief Compute the L-1 norm of the Sobel gradient of an image.
    ///
    /// This function is a shortcut for the general
    /// mln::linear::sobel_norm function.
    /// \{
    template <typename I>
    mln_ch_value(I, mln_sum(mln_value(I))) sobel_norm(const Image<I>& input);
    /// \}


# ifndef MLN_INCLUDE_ONLY

/// \brief Ensure type \p V is a scalar value.
///
/// For internal use only (#undef'd before the end of this file).  (We
/// might want to export this macro to the user interface; if so,
/// rename it and move it elsewhere.)
///
/// FIXME: At the moment (rev. 1645), we cannot use mlc_is_a to check
/// whether V derives from trait::value::nature::scalar like this
///
/// mlc_is_a(trait::value_<V>::nature, trait::value::nature::scalar)::check()
///
/// since `mlc_is_a' doesn't work on non template classes (yet).  Hence
/// the ugly workaround based on comprehensive checks.
/// \{
#  define mlc_internal_ensure_is_scalar(V)				\
    mlc_or(mlc_equal(typename mln::trait::value_<V>::nature,		\
                     mln::trait::value::nature::scalar),		\
    mlc_or(mlc_equal(typename mln::trait::value_<V>::nature,		\
		     mln::trait::value::nature::integer),		\
	   mlc_equal(typename mln::trait::value_<V>::nature,		\
		     mln::trait::value::nature::floating)))::check()
/// \}

    namespace impl
    {

      template <typename I>
      inline
      mln_gradient_component(I)
      sobel_h_(const Image<I>& input)
      {
	// We don't use const ints here, since it would confuse the
	// STL allocators used within the windows used in the
	// convolutions.
	int wrow[] = { -1, 0, 1 };
	int wcol[] = { 1,
		       2,
		       1 };
	// FIXME: It'd probably be nicer to have a "functional" version
	// of linear::line_x2_convolve.
	mln_gradient_component(I) output(exact(input).domain());
	linear::line_x2_convolve(input, wrow, wcol, output);
	return output;
      }

      template <typename I>
      inline
      mln_gradient_component(I)
      sobel_v_(const Image<I>& input)
      {
	// We don't use const ints here, since it would confuse the
	// STL allocators used within the windows used in the
	// convolutions.
	int wrow[] = { 1, 2, 1 };
	int wcol[] = { -1,
		       0,
		       +1 };
	// FIXME: It'd probably be nicer to have a "functional" version
	// of linear::line_x2_convolve.
	mln_gradient_component(I) output(exact(input).domain());
	linear::line_x2_convolve(input, wrow, wcol, output);
	return output;
      }

      template <typename I>
      inline
      mln_gradient(I)
      sobel_(const Image<I>& input)
      {
	/* FIXME: Not generic w.r.t. the dimension (only works on 2-D
	   images here).  Anyway, we'd need a n-dimensional definition
	   of the Sobel gradient to.

	   Stick with a very simple static assertion for the momemt,
	   and improve later.  */
	mlc_equal(metal::int_<I::point::dim>, metal::int_<2>)::check();

	mln_gradient_component(I) component_h = linear::sobel_h(input);
	mln_gradient_component(I) component_v = linear::sobel_v(input);
	return value::stack(component_h, component_v);
      }

      /* FIXME: Add constraints on N (mln::Function_v2v<N>).  */
      template <typename I, typename N>
      mln_ch_value(I, mln_result(N))
      sobel_norm_(const Image<I>& input, N norm)
      {
	mln_gradient(I) gradient = mln::linear::sobel (input);
	return level::transform (gradient, norm);
      }

    } // end of namespace mln::linear::impl


    // Facades.

    template <typename I>
    inline
    mln_gradient_component(I) sobel_h(const Image<I>& input)
    {
      mlc_internal_ensure_is_scalar(mln_value(I));
      return impl::sobel_h_(exact(input));
    }

    template <typename I>
    inline
    mln_gradient_component(I) sobel_v(const Image<I>& input)
    {
      mlc_internal_ensure_is_scalar(mln_value(I));
      return impl::sobel_v_(exact(input));
    }

    template <typename I>
    inline
    mln_gradient(I) sobel(const Image<I>& input)
    {
      mlc_internal_ensure_is_scalar(mln_value(I));
      return impl::sobel_(exact(input));
    }

    template <typename I, typename N>
    inline
    mln_ch_value(I, mln_result(N))
    sobel_norm(const Image<I>& input, N norm)
    {
      mlc_internal_ensure_is_scalar(mln_value(I));
      return impl::sobel_norm_(exact(input), norm);
    }

    template <typename I>
    inline
    mln_ch_value(I, mln_sum(mln_value(I)))
    sobel_norm(const Image<I>& input)
    {
      // The type of a component of a vector from the gradient.
      typedef mln_sum(mln_value(I)) gradient_val_t;
      // The type of a vector from the gradient.
      typedef mln::algebra::vec<I::point::dim, gradient_val_t> gradient_vec_t;
      return sobel_norm(input,
			fun::v2v::l1_norm<gradient_vec_t, gradient_val_t>());
    }

#  undef mlc_internal_ensure_is_scalar

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_SOBEL_HH
