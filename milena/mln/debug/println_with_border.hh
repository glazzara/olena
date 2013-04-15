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

#ifndef MLN_DEBUG_PRINTLN_WITH_BORDER_HH
# define MLN_DEBUG_PRINTLN_WITH_BORDER_HH

/// \file
///
/// Print an image on the standard output with its border.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/alias/box3d.hh>

# include <mln/geom/bbox.hh>

# include <mln/debug/format.hh>
// Specializations are in:
# include <mln/debug/println_with_border.spe.hh>
# include <mln/opt/element.hh>

namespace mln
{

  namespace debug
  {

    /*! \brief Print the image \p input on the standard output.
      \ingroup mlndebug
     */
    template <typename I>
    void println_with_border(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.
      template <typename S, typename I>
      inline
      void
      println_with_border(const S&, const I& input)
      {
	for (size_t i = 0; i < opt::nelements(input); i++)
	  std::cout << format(input.buffer()[i]) << ' ';
	std::cout << std::endl;
      }

    } // end of namespace mln::debug::impl


    // Facade.
    template <typename I>
    inline
    void
    println_with_border(const Image<I>& input)
    {
      mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest)::check();

      impl::println_with_border(geom::bbox(exact(input)), exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_WITH_BORDER_HH
