// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_LEVEL_STRETCH_HH
# define MLN_LEVEL_STRETCH_HH

/// \file mln/level/stretch.hh
///
/// Transform linearly the contents of an image into another one in a
/// stretching way.
///
/// \todo Make it work with other types than scalars (e.g., vectors).
///
/// \todo Think about adding a stretch_inplace(?)

# include <mln/estim/min_max.hh>
# include <mln/value/int_u.hh>
# include <mln/fun/v2v/linear.hh>
# include <mln/level/transform.hh>


namespace mln
{

  namespace level
  {

    /*! Stretch the values of \p input so that they can be stored in
     *  \p output.
     *
     * \param[in] v       A value to set the output value type.
     * \param[in] input   The input image.
     * \param[out] output The result image.
     *
     * \pre input.has_data
     */
    template <typename V, typename I>
    mln_ch_value(I, V)
    stretch(V v, const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename V, typename I>
      inline
      mln_ch_value(I, V)
      stretch(V, const Image<I>& input)
      {
	trace::entering("level::impl::stretch");

	mlc_converts_to(float, V)::check();

	mln_ch_value(I, V) output;

	mln_value(I) min_, max_;
	estim::min_max(input, min_, max_);
	if (max_ != min_)
	  {
	    float
	      min = float(min_),
	      max = float(max_),
	      epsilon = mln_epsilon(float),
	      m = 0.0f - 0.5f + epsilon,
	      M = mln_max(V) + 0.5f - epsilon,
	      a = (M - m) / (max - min),
	      b = (m * max - M * min) / (max - min);
	    fun::v2v::linear<float, float, V> f(a, b);
	    output = level::transform(input, f);
	  }
	else
	  {
	    initialize(output, input);
	    // trace::warning("output has no significative data!");
	  }

	trace::exiting("level::impl::stretch");
	return output;
      }

    } // end of namespace mln::level::impl


    template <typename V, typename I>
    inline
    mln_ch_value(I, V)
    stretch(V, const Image<I>& input)
    {
      trace::entering("level::stretch");

      mln_precondition(exact(input).has_data());

      mln_ch_value(I, V) output = impl::stretch(V(), input);

      trace::exiting("level::stretch");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_STRETCH_HH
