// Copyright (C) 2011, 2012 EPITA Research and Development Laboratory
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

/// \file
///
/// Crop an image. Localization between input and output may not be
/// preserved.

#ifndef MLN_GEOM_CROP_WITHOUT_LOCALIZATION_HH
# define MLN_GEOM_CROP_WITHOUT_LOCALIZATION_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/box.hh>
# include <mln/data/paste_without_localization.hh>

namespace mln
{

  namespace geom
  {

    /*! \brief Crop an image. Localization between input and output
     *  may not be preserved.

      \ingroup mlngeom
     */
    template <typename I, typename B>
    mln_concrete(I) crop_without_localization(const Image<I>& input,
			 const Box<B>& b_in, const Box<B>& b_out);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename B>
    mln_concrete(I)
    crop_without_localization(const Image<I>& input_,
			      const Box<B>& b_in_, const Box<B>& b_out_)
    {
      trace::entering("mln::geom::crop_without_localization");

      const I& input = exact(input_);
      const B& b_in = exact(b_in_);
      const B& b_out = exact(b_out_);
      mln_precondition(input.is_valid());
      mln_precondition(b_in.is_valid());
      mln_precondition(b_out.is_valid());
      mln_precondition(b_in <= input.domain());
      mln_precondition(b_in.nsites() == b_out.nsites());

      mln_concrete(I) output(b_out);
      data::paste_without_localization(input | b_in, output);

      trace::exiting("mln::geom::crop_without_localization");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln

#endif // ! MLN_GEOM_CROP_WITHOUT_LOCALIZATION_HH
