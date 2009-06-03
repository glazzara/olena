// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LINEAR_CH_CONVOLVE_HH
# define MLN_LINEAR_CH_CONVOLVE_HH

/// \file mln/linear/ch_convolve.hh
///
/// Define convolution return type.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/trait/ch_value.hh>
# include <mln/value/ops.hh>

# include <mln/algebra/vec.hh>


/// Define the result of the convolution of an image with type \p I
/// with a weighted window of type \p W or weights of type \p W.
# define mln_ch_convolve(I, W) \
  typename mln::linear::ch_convolve<I, W>::ret

# define mln_ch_convolve_grad(I, W) \
  typename mln::trait::ch_value< I, algebra::vec< I::site::dim, typename mln::linear::ch_convolve<I,W>::ret::value > >::ret



namespace mln
{

  namespace linear
  {

    namespace internal
    {

      template <bool b, /* = true, i.e., W is a Weighted_Window */
		typename I, typename W>
      struct ch_convolve_helper
      {
	typedef mln_sum_product(mln_value(I), mln_weight(W)) V;
	typedef mln_ch_value(I, V) ret;
      };

      template <typename I, typename W>
      struct ch_convolve_helper<false, I, W>
      {
	typedef mln_sum_product(mln_value(I), W) V;
	typedef mln_ch_value(I, V) ret;
      };

    } // end of namespace mln::linear::internal


    template <typename I, typename W>
    struct ch_convolve
      : private mlc_and( mlc_is_a(I, Image),
			 mlc_is_not_a(W, Window) )::check_t
    {
    protected:
      enum { is_w_win = mlc_is_a(W, Weighted_Window)::value };
      typedef internal::ch_convolve_helper<is_w_win, I, W> helper;
    public:
      typedef mlc_ret(helper) ret;
    };

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_CH_CONVOLVE_HH
