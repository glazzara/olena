// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_OTSU_HH
# define SCRIBO_BINARIZATION_OTSU_HH

# include <mln/core/concept/image.hh>

# include <scribo/binarization/otsu_threshold.hh>
# include <scribo/binarization/global_threshold.hh>

namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    /*! An implementation of Otsu's binarization algorithm.

      It is based on Ocropus's implementation.
      http://code.google.com/p/ocropus/

      \param[in] input A gray-scale image.

      \return A binary image. True for foreground, False for
      background.
     */
    template <typename I>
    mln_ch_value(I,bool)
    otsu(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_ch_value(I,bool)
    otsu(const Image<I>& input_)
    {
      trace::entering("scribo::binarization::otsu");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mlc_is_a(mln_value(I), value::Scalar)::check();
      // FIXME: Check that input value is gray level.

      mln_value(I) threshold = otsu_threshold(input);

      // Computing final result.
      mln_ch_value(I,bool) output = global_threshold(input, threshold);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // ! SCRIBO_BINARIZATION_OTSU_HH
