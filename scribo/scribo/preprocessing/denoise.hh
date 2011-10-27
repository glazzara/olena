// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PREPROCESSING_DENOISE_HH
# define SCRIBO_PREPROCESSING_DENOISE_HH

/// \file
///
/// Denoise an image.

# include <scribo/preprocessing/denoise_fg.hh>
# include <scribo/preprocessing/denoise_bg.hh>


namespace scribo
{

  namespace preprocessing
  {

    using namespace mln;

    /// \brief Denoise an image.
    /// Denoising is performed both on the foreground and the background.
    ///
    /// \param[in] input    A binary image. True for objects, False for
    ///                     background.
    /// \param[in] nbh      Neighborhood to use for denoising.
    /// \param[in] fg_min_card Minimum component cardinality to not be
    ///                        considered as noise in the foreground.
    /// \param[in] bg_min_card Minimum component cardinality to not be
    ///                        considered as noise in the foreground.
    ///
    /// \return A binary image with the same domain as \p input. All
    /// small components have been removed.
    //
    template <typename I, typename N>
    mln_concrete(I)
    denoise(const Image<I>& input, const Neighborhood<N>& nbh,
	    unsigned fg_min_card, unsigned bg_min_card);


  } // end of namespace scribo::preprocessing

} // end of namespace mln


# ifndef MLN_INCLUDE_ONLY


namespace scribo
{

  namespace preprocessing
  {

    template <typename I, typename N>
    mln_concrete(I)
    denoise(const Image<I>& input_, const Neighborhood<N>& nbh_,
	    unsigned fg_min_card, unsigned bg_min_card)
    {
      trace::entering("scribo::preprocessing::denoise");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());

      mln_concrete(I) output;

      output = denoise_fg(input, nbh, fg_min_card);
      output = denoise_bg(output, nbh, bg_min_card);

      trace::exiting("scribo::preprocessing::denoise");
      return output;
    }


  } // end of namespace scribo::preprocessing

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY

# endif // SCRIBO_PREPROCESSING_DENOISE_HH
