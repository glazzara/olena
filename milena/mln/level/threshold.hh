// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_THRESHOLD_HH
# define MLN_LEVEL_THRESHOLD_HH

/*! \file mln/level/threshold.hh
 *
 * \brief Threshold the contents of an image into another binary one.
 */

# include <mln/estim/min_max.hh>
# include <mln/value/int_u.hh>
# include <mln/fun/v2v/threshold.hh>
# include <mln/level/transform.hh>


namespace mln
{

  namespace level
  {

    /*! Threshold the values of \p input so that they can be stored in
     *  the \p output binary image.
     *
     * \param[in] input The input image.
     * \param[in] threshold The threshold.
     *
     * If the input value is greater or equal than the threshold, the
     * result we be TRUE, else FALSE.
     *
     */
    template <typename I>
    mln_concrete_ch_value(I, bool)
    threshold(const Image<I>& input, const mln_value(I) threshold);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      mln_concrete_ch_value(I, bool)
      threshold(const I& input, const mln_value(I) threshold)
      {
	trace::entering("level::impl::threshold");
	mln_concrete_ch_value(I, bool) output(input.domain());

	fun::v2v::threshold< mln_value(I) > f(threshold);
	level::transform(input, f, output);

	trace::exiting("level::impl::threshold");
	return output;
      }

    } // end of namespace mln::level::impl


    template <typename I>
    inline
    mln_concrete_ch_value(I, bool)
    threshold(const Image<I>& input, const mln_value(I) threshold)
    {
      trace::entering("level::threshold");
      mln_precondition(exact(input).has_data());
      mlc_is(mln_trait_value_nature(mln_value(I)),
	     trait::value::nature::scalar)::check();

      mln_concrete_ch_value(I, bool) output(exact(input).domain());
      output = impl::threshold(exact(input), threshold);

      trace::exiting("level::threshold");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_THRESHOLD_HH
