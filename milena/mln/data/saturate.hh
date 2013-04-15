// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_DATA_SATURATE_HH
# define MLN_DATA_SATURATE_HH

/// \file
///
/// Apply a saturation function to image pixel values.

# include <mln/fun/v2v/saturate.hh>
# include <mln/data/apply.hh>
# include <mln/data/transform.hh>


namespace mln
{

  namespace data
  {


    /*! \brief Apply the saturate function to image pixel values.
     *
     * \param[in] v     A value of the output type.
     * \param[in] input The input image.
     *
     * The saturation is based on the min and max values of the output
     * value type.  This assumes that the range of values in the input
     * image is larger than the one of the output image.
     *
     * \ingroup mlndata
     */
    template <typename V, typename I>
    mln_ch_value(I, V)
    saturate(V v, const Image<I>& input);


    /*! \brief Apply the saturate function to image pixel values.
     *
     * \param[in] input The input image.
     * \param[in] min The minimum output value.
     * \param[in] max The maximum output value.
     *
     * \ingroup mlndata
     */
    template <typename I, typename V>
    mln_ch_value(I, V)
    saturate(const Image<I>& input,
	     const V& min, const V& max);


    /*! \brief Apply the saturate function to image pixel values.
     *
     * \param[in,out] input The input image.
     * \param[in] min The minimum output value.
     * \param[in] max The maximum output value
     *
     * \ingroup mlndata
     */
    template <typename I>
    void saturate_inplace(Image<I>& input,
			  const mln_value(I)& min, const mln_value(I)& max);


# ifndef MLN_INCLUDE_ONLY

    template <typename V, typename I>
    inline
    mln_ch_value(I, V)
    saturate(V, const Image<I>& input)
    {
      mln_trace("data::saturate");

      mln_precondition(exact(input).is_valid());

      fun::v2v::saturate<V> f;
      mln_ch_value(I, V) output = data::transform(input, f);

      return output;
    }

    template <typename I, typename V>
    inline
    mln_ch_value(I, V)
    saturate(const Image<I>& input,
	     const V& min, const V& max)
    {
      mln_trace("data::saturate");

      mln_precondition(exact(input).is_valid());

      fun::v2v::saturate<V> f(min, max);
      mln_ch_value(I, V) output = data::transform(input, f);

      return output;
    }

    template <typename I>
    inline
    void saturate_inplace(Image<I>& input,
			  const mln_value(I)& min, const mln_value(I)& max)
    {
      mln_trace("data::saturate_inplace");

      mln_precondition(exact(input).is_valid());

      fun::v2v::saturate<mln_value(I)> f(min, max);
      data::apply(input, f);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_SATURATE_HH
