// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef SCRIBO_BINARIZATION_SAUVOLA_HH
# define SCRIBO_BINARIZATION_SAUVOLA_HH

/// \file
///
///

# include <mln/core/concept/image.hh>
# include <scribo/binarization/internal/sauvola_functor.hh>
# include <scribo/binarization/internal/local_threshold_core.hh>

namespace scribo
{

  namespace binarization
  {

    using namespace mln;


    /*! \brief Convert an image into a binary image.

      \param[in]  input       A greyscale image.
      \param[in]  window_size The window size.
      \param[in]  K           Sauvola's formulae constant.

      \return A binary image.

      \ingroup grpalgobinsauvola
     */
    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input, unsigned window_size, double K);



    /*! \brief Convert an image into a binary image.

      \overload

      Sauvola's formulae constant K is set to 0.34.

      \param[in]  input       A grayscale image.
      \param[in]  window_size The window size.

      \return A binary image.

      \ingroup grpalgobinsauvola
     */
    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input, unsigned window_size);


    /*! \overload
      The window size is set to 11.

      \ingroup grpalgobinsauvola
    */
    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    // Facades

    template <typename I>
    mln_ch_value(I, bool)
      sauvola(const Image<I>& input, unsigned window_size, double K)
    {
      mln_trace("scribo::binarization::sauvola");

      mln_precondition(exact(input).is_valid());

      internal::sauvola_functor<I> f(input, K, SCRIBO_DEFAULT_SAUVOLA_R);
      internal::local_threshold_core(input, f, window_size);

      return f.output;
    }


    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input, unsigned window_size)
    {
      return sauvola(input, window_size, SCRIBO_DEFAULT_SAUVOLA_K);
    }


    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input)
    {
      return sauvola(input, 11);
    }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_SAUVOLA_HH
