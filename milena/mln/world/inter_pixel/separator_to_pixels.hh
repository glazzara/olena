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

#ifndef MLN_WORLD_INTER_PIXEL_SEPARATOR_TO_PIXELS_HH
# define MLN_WORLD_INTER_PIXEL_SEPARATOR_TO_PIXELS_HH

/// \file
///
/// FIXME: doc.
///
/// \todo Generalize to n-D.

# include <mln/core/concept/gpoint.hh>
# include <mln/core/concept/site_proxy.hh>
# include <mln/world/inter_pixel/is_pixel.hh>
# include <mln/world/inter_pixel/is_separator.hh>


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {


      template <typename P>
      void
      separator_to_pixels(const Gpoint<P>& s,
			  Gpoint<P>& p1, Gpoint<P>& p2);

      template <typename Ps, typename P>
      void
      separator_to_pixels(const Site_Proxy<Ps>& s,
			  Gpoint<P>& p1, Gpoint<P>& p2);


# ifndef MLN_INCLUDE_ONLY


      template <typename P>
      inline
      void
      separator_to_pixels(const Gpoint<P>& s_,
			  Gpoint<P>& p1_, Gpoint<P>& p2_)
      {
	const P& s = exact(s_);
	P& p1 = exact(p1_);
	P& p2 = exact(p2_);

	{
	  // Pre-condition.
	  is_separator is_separator_;
	  mln_precondition(is_separator_(s));
	  (void) is_separator_;
	}

	// FIXME: 2D only.
	if (s.row() % 2)
	  {
	    // Horizontal edge.
	    p1 = point2d(s.row() - 1, s.col());
	    p2 = point2d(s.row() + 1, s.col());
	  }
	else
	  {
	    // Vertical edge.
	    p1 = point2d(s.row(), s.col() - 1);
	    p2 = point2d(s.row(), s.col() + 1);
	  }

	{
	  // Post-conditions.
	  is_pixel is_pixel_;
	  mln_postcondition(is_pixel_(p1));
	  mln_postcondition(is_pixel_(p2));
	  (void) is_pixel_;
	}
      }

      template <typename Ps, typename P>
      inline
      void
      separator_to_pixels(const Site_Proxy<Ps>& s_,
			  Gpoint<P>& p1_, Gpoint<P>& p2_)
      {
	const Ps& s = exact(s_);
	P& p1 = exact(p1_);
	P& p2 = exact(p2_);

	{
	  // Pre-condition.
	  is_separator is_separator_;
	  mln_precondition(is_separator_(s));
	  (void) is_separator_;
	}

	separator_to_pixels(s.to_site(), p1, p2);

	{
	  // Post-conditions.
	  is_pixel is_pixel_;
	  mln_postcondition(is_pixel_(p1));
	  mln_postcondition(is_pixel_(p2));
	  (void) is_pixel_;
	}
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_SEPARATOR_TO_PIXELS_HH
