// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_BINARIZATION_THRESHOLD_HH
# define MLN_BINARIZATION_THRESHOLD_HH

/// \file
///
/// \brief Threshold an image.

# include <mln/binarization/binarization.hh>
# include <mln/fun/v2b/threshold_ge.hh>


namespace mln
{

  namespace binarization
  {

    /*! \brief Gaussian subsampling
      Thresholds the values of \p input so that they can be stored in
      the \p output binary image.

      \param[in] input The input image.
      \param[in] threshold The threshold.

      If input(p) is greater or equal than the threshold, the
      value in the output image in the same point will be TRUE, else FALSE.

      \ingroup mlngeom
    */
    template <typename I>
    mln_ch_value(I, bool)
    threshold(const Image<I>& input, const mln_value(I) threshold);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_ch_value(I, bool)
    threshold(const Image<I>& input, const mln_value(I) threshold_value)
    {
      trace::entering("binarization::threshold");

      mln_precondition(exact(input).is_valid());
      mlc_is(mln_trait_value_nature(mln_value(I)),
	     trait::value::nature::scalar)::check();

      mln_ch_value(I, bool) output(exact(input).domain());

      // FIXME : threshold value should be a percentage.
      fun::v2b::threshold_ge< mln_value(I) > f(threshold_value);

      output = mln::binarization::binarization(exact(input), f);

      trace::exiting("binarization::threshold");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::binarization

} // end of namespace mln


#endif // ! MLN_BINARIZATION_THRESHOLD_HH
