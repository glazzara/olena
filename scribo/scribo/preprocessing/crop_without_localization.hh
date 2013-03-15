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

#ifndef SCRIBO_PREPROCESSING_CROP_WITHOUT_LOCALIZATION_HH
# define SCRIBO_PREPROCESSING_CROP_WITHOUT_LOCALIZATION_HH

# include <mln/core/concept/image.hh>
# include <mln/data/paste_without_localization.hh>

/// \file
///
/// \brief Crop an image without preserving the localization.


namespace scribo
{

  namespace preprocessing
  {

    using namespace mln;

    /*! \brief crop an image without preserving the localization.

      \param[in] input An image.
      \param[in] domain A region of interest.

      \return An image defined on a domain starting from the origin
      (literal::origin) and having the same size as \p domain. Its
      data corresponds to the data copied from \p input in the domain
      \p domain.

     */
    template <typename I>
    mln_concrete(I)
    crop_without_localization(const Image<I>& input,
			      const mln_box(I)& domain);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_concrete(I)
    crop_without_localization(const Image<I>& input,
			      const mln_box(I)& domain)
    {
      mln_trace("scribo::preprocessing::crop_without_localization");
      mln_assertion(exact(input).is_valid());


      typedef mln_site(I) P;
      P pmin, pmax;

      for (unsigned i = 0; i < P::dim; ++i)
      {
	pmin[i] = 0;
	pmax[i] = domain.pmax()[i] - domain.pmin()[i];
      }

      mln_box(I) b(pmin, pmax);
      mln_concrete(I) output(b);

      data::paste_without_localization(input | domain, output);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::preprocessing

} // end of namespace scribo

#endif // ! SCRIBO_PREPROCESSING_CROP_WITHOUT_LOCALIZATION_HH
