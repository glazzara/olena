// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CONVERT_IMPL_FROM_SITE_SET_TO_IMAGE_HH
# define MLN_CONVERT_IMPL_FROM_SITE_SET_TO_IMAGE_HH

/// \file
///
/// General conversion procedure from a site_set to an image.

# include <mln/core/image/dmorph/sub_image.hh>
# include <mln/geom/bbox.hh>
# include <mln/trait/image_from_grid.hh>
# include <mln/data/fill.hh>


namespace mln
{

  // Forward declarations.
  template <typename E> struct Site_Set;
  template <typename E> struct Image;


  namespace convert
  {

    namespace impl
    {

      /// Conversion of an image \p from towards a site set \p to.
      template <typename S, typename I>
      void
      from_site_set_to_image(const Site_Set<S>& from, Image<I>& to);



# ifndef MLN_INCLUDE_ONLY

      template <typename S, typename I>
      inline
      void
      from_site_set_to_image(const Site_Set<S>& from_, Image<I>& to_)
      {
	const S& from = exact(from_);
	I& to = exact(to_);
	
	box<mln_site(S)> b = geom::bbox(from);

	typedef mln_deduce(I, site, grid) G;
	typedef mln_value(I) V;
	mln_image_from_grid(G, V) ima(b);
	data::fill(ima, false);
	data::fill((ima | from).rw(), true);

	to = ima;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::convert::impl

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_IMPL_FROM_SITE_SET_TO_IMAGE_HH
