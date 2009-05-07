// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_WORLD_INTER_PIXEL_IMMERSE_HH
# define MLN_WORLD_INTER_PIXEL_IMMERSE_HH

/// \file mln/world/inter_pixel/immerse.hh
///
/// Convert a classical image to an inter-pixel image.

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

      /// Convert an image to an inter-pixel image.
      ///
      /// \param[in] input An image.
      /// \return An inter-pixel image.
      //
      template <typename I>
      image_if<mln_concrete(I), is_pixel>
      immerse(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      image_if<mln_concrete(I), is_pixel>
      immerse(const Image<I>& input_)
      {
	trace::entering("world::inter_pixel::immerse");

	mlc_is_a(mln_domain(I), Box)::check();

	const I& input = exact(input_);
	mln_precondition(input.is_valid());

	mln_domain(I) b(2 * input.domain().pmin(),
			2 * input.domain().pmax());
	mln_concrete(I) output(b);
	mln_assertion(geom::nsites(output | is_pixel()) == input.domain().nsites());

	data::paste_without_localization(input, (output | is_pixel()).rw());

	trace::exiting("world::inter_pixel::immerse");
	return output | is_pixel();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_IMMERSE_HH
