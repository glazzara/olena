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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_IS_INVALID_LINK_HH
# define SCRIBO_PRIMITIVE_INTERNAL_IS_INVALID_LINK_HH

/// \file
///
/// Check whether an objects link is invalid or not.


# include <mln/math/abs.hh>
# include <mln/literal/zero.hh>

# include <scribo/core/object_image.hh>

namespace scribo
{

  namespace primitive
  {

    namespace internal
    {

      using namespace mln;


      /// Check whether an objects link is invalid or not.
      ///
      /// \param objects      An image of objects.
      /// \param left_link    The left neighbors.
      /// \param p            The current site.
      /// \param current_comp The current object id.
      /// \param c            The left link start point.
      /// \param dmax         The maximum lookup distance.
      //
      template <typename L>
      bool
      is_invalid_link(const object_image(L)& objects_,
		      mln::util::array<unsigned>& link_array,
		      const mln_site(L)& p,
		      unsigned current_comp,
		      const mln_site(L)& c,
		      int dmax);

# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      bool
      is_invalid_link(const object_image(L)& objects,
		      mln::util::array<unsigned>& link_array,
		      const mln_site(L)& p,
		      unsigned current_comp,
		      const mln_site(L)& c,
		      int dmax)
      {
	return (objects.domain().has(p)           // Not outside image domain
		&& (objects(p) == literal::zero   // Is the background
		    || objects(p) == current_comp // Is the current component
		    || link_array[objects(p)] == current_comp) // Creates a loop
		&& math::abs(p.col() - c.col()) < dmax); // Not too far
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::internal

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_INTERNAL_IS_INVALID_LINK_HH


