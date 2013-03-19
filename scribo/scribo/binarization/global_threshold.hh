// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_GLOBAL_THRESHOLD_HH
# define SCRIBO_BINARIZATION_GLOBAL_THRESHOLD_HH

# include <mln/core/concept/image.hh>
# include <mln/value/concept/vectorial.hh>
# include <mln/fun/v2b/threshold_le.hh>
# include <mln/data/transform.hh>

/// \file
///
/// \brief Binarize an image using a global threshold value.


/// FIXME: result is not consistent with other binarization
/// methods. The result is inverted because of the threshold functor:
/// should we invert the test in this functor?

namespace scribo
{

  using namespace mln;


  namespace binarization
  {

    /// \brief Binarize an image using a global threshold value.
    ///
    /// For a site 'p' in \p input image:
    ///
    /// output(p) = input(p) >= threshold
    ///
    /// \param[in] input A grayscale image.
    /// \param[in] threshold A value.
    ///
    /// \return A boolean image.
    //
    template <typename I>
    mln_ch_value(I, bool)
    global_threshold(const Image<I>& input, const mln_value(I)& threshold);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_ch_value(I, bool)
    global_threshold(const Image<I>& input, const mln_value(I)& threshold)
    {
      mln_trace("scribo::binarization::global_threshold");

      mln_precondition(exact(input).is_valid());
      mlc_is_not_a(mln_value(I), value::Vectorial)::check();

      mln::fun::v2b::threshold_le<mln_value(I)> f(threshold);
      mln_ch_value(I, bool) output = data::transform(input, f);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_GLOBAL_THRESHOLD_HH
