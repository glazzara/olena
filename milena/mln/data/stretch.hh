// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_DATA_STRETCH_HH
# define MLN_DATA_STRETCH_HH

/// \file
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
# include <mln/data/transform.hh>

# include <mln/value/internal/encoding.hh>


namespace mln
{

  namespace data
  {

    /*! \brief Stretch the values of \p input so that they can be
     *  stored in \p output.
     *
     * \param[in] v       A value to set the output value type.
     * \param[in] input   The input image.
     *
     * \return A stretch image with values of the same type as \p v.
     *
     * \pre input.is_valid
     *
     * \ingroup mlndata
     */
    template <typename V, typename I>
    mln_ch_value(I, V)
    stretch(const V& v, const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      /// Generic implementation of data::stretch.
      ///
      /// \param[in] v       A value to set the output value type.
      /// \param[in] input   The input image.
      ///
      /// \return A stretch image with values of the same type as \p v.
      //
      template <typename V, typename I>
      inline
      mln_ch_value(I, V)
      stretch(const V&, const Image<I>& input)
      {
	mln_trace("data::impl::stretch");

	mlc_converts_to(float, V)::check();

	mln_ch_value(I, V) output;

	mln_value(I) min_, max_;
	estim::min_max(input, min_, max_);
	if (max_ != min_)
	{
	  //FIXME: we would like to use float instead of double but we
	  //can't for precision reasons. See ticket #179.
	  double
	    min = double(min_),
	    max = double(max_),
	    epsilon = mln_epsilon(float),
	    M = mln_max(V) + 0.5f - epsilon,
	    m = 0.0f - 0.5f + epsilon,
	    a = (M - m) / (max - min),
	    b = (m * max - M * min) / (max - min);
	  fun::v2v::linear_sat<mln_value(I), double, V> f(a, b);
	  output = data::transform(input, f);
	}
	else
	{
	  initialize(output, input);
	  mln_trace_warning("output has no significative data!");
	}

	return output;
      }

    } // end of namespace mln::data::impl



    // Facade

    template <typename V, typename I>
    inline
    mln_ch_value(I, V)
    stretch(const V&, const Image<I>& input)
    {
      mln_trace("data::stretch");

      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, V) output = impl::stretch(V(), input);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_STRETCH_HH
