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

#ifndef MLN_LEVEL_STRETCH_HH
# define MLN_LEVEL_STRETCH_HH

/*! \file mln/level/stretch.hh
 *
 * \brief Transform with fun::stretch the contents of an image into
 * another one.
 */

# include <mln/estim/min_max.hh>
# include <mln/value/int_u.hh>
# include <mln/fun/v2v/linear.hh>
# include <mln/level/transform.hh>


namespace mln
{

  namespace level
  {

    /*! Stretch the values of \p input so that they can be stored in \p output.
     *
     * \param[in] input The input image.
     * \param[out] output The result image.
     *
     * \pre \p output.domain == \p input.domain
     */
    template <typename I, typename O>
    void stretch(const Image<I>& input, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // FIXME: The first argument seems to have no effect.
      template <unsigned n, typename I, typename O>
      inline
      void stretch(value::int_u<n>,
		   const Image<I>& input, Image<O>& output)
      {
	trace::entering("level::impl::stretch");

	mln_value(I) min_, max_;
	estim::min_max(input, min_, max_);
	if (max_ != min_)
	  {
	    float min = float(min_), max = float(max_);
	    const float epsilon = mln_epsilon(float);
	    float m = 0.0f - 0.5f + epsilon;
	    float M = mln_max(value::int_u<n>) + 0.5f - epsilon;
	    float a = (M - m) / (max - min);
	    float b = (m * max - M * min) / (max - min);
	    fun::v2v::linear<float, float, int> f(a, b);
	    level::transform(input, f, output);
	  }
	trace::exiting("level::impl::stretch");
      }

    } // end of namespace mln::level::impl


    template <typename I, typename O>
    inline
    void stretch(const Image<I>& input, Image<O>& output)
    {
      trace::entering("level::stretch");

      initialize(output, input);

      mln_precondition(exact(output).domain() == exact(input).domain());
      impl::stretch(mln_value(O)(), input, output);

      trace::exiting("level::stretch");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_STRETCH_HH
