// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_WORLD_INTER_PIXEL_FULL2IMAGE_HH
# define MLN_WORLD_INTER_PIXEL_FULL2IMAGE_HH

/// \file
///
/// Convert an inter-pixel image to a classical image.

# include <mln/core/concept/image.hh>
# include <mln/data/paste_without_localization.hh>
# include <mln/geom/nsites.hh>
# include <mln/world/inter_pixel/is_pixel.hh>


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      /// Convert an inter-pixel image to a classical image.
      ///
      /// \param[in] input An inter-pixel image.
      ///
      /// \return A classical image without inter-pixel data.
      //
      template <typename I>
      mln_concrete(I)
      full2image(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      mln_concrete(I)
      full2image(const Image<I>& input_)
      {
	mln_trace("world::inter_pixel::full2image");

	mlc_is_a(mln_domain(I), Box)::check();

	const I& input = exact(input_);
	mln_precondition(input.is_valid());

	mln_domain(I) b(input.domain().pmin() / 2,
			input.domain().pmax() / 2);
	mln_concrete(I) output(b);
	mln_assertion(geom::nsites(output) == geom::nsites(input | is_pixel()));

	data::paste_without_localization(input | is_pixel(), output);

	return output;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_FULL2IMAGE_HH
