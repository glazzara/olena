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

#ifndef SCRIBO_FILTER_INTERNAL_ALIGNMENT_ANGLE_HH
# define SCRIBO_FILTER_INTERNAL_ALIGNMENT_ANGLE_HH

/// \file
///
/// Return the alignment angle between two components according to an
/// anchor.


# include <mln/util/array.hh>
# include <mln/math/abs.hh>
# include <mln/math/pi.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/tag/anchor.hh>
# include <scribo/core/component_set.hh>

namespace scribo
{

  namespace filter
  {

    namespace internal
    {

      using namespace mln;


      /// Return the alignment angle between two components according
      /// to anchor.
      ///
      /// \return An angle in Radians.
      template <typename L>
      float
      alignment_angle(const component_set<L>& comps,
		      unsigned current_object, unsigned nbh_object,
		      anchor::Type anchor);


# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      float
      alignment_angle(const component_set<L>& comps,
		      unsigned current_object, unsigned nbh_object,
		      anchor::Type anchor)
      {
	mln_trace("scribo::filter::internal::alignment_angle");

	mln_precondition(comps.is_valid());

	float dr, dc, result = 0;

	if (nbh_object == current_object)
	  return 0;

	switch(anchor)
	{
	  // Center
	  case anchor::Center:
	  {
	    dr = math::abs(comps(current_object).bbox().pcenter().row()
			   - comps(nbh_object).bbox().pcenter().row());
	    dc = math::abs(comps(current_object).bbox().pcenter().col()
			   - comps(nbh_object).bbox().pcenter().col());

	    result = std::atan(dr / dc);
	  }
	  break;

	  // Mass Center
	  case anchor::MassCenter:
	  {
	    dr = math::abs(comps(current_object).mass_center().row()
			   - comps(nbh_object).mass_center().row());
	    dc = math::abs(comps(current_object).mass_center().col()
			   - comps(nbh_object).mass_center().col());

	    result = std::atan(dr / dc);
	  }
	  break;

	  // Top
	  case anchor::StrictTopCenter:
	  case anchor::TopStrictLeft:
	  case anchor::Top:
	  {
	    dr = math::abs(comps(current_object).bbox().pmin().row()
			   - comps(nbh_object).bbox().pmin().row());
	    dc = math::abs(comps(current_object).bbox().pcenter().col()
			   - comps(nbh_object).bbox().pcenter().col());

	    result = std::atan(dr / dc);
	  }
	  break;

	  // Bottom
	  case anchor::StrictBottomCenter:
	  case anchor::BottomStrictRight:
	  case anchor::Bottom:
	  {
	    dr = math::abs(comps(current_object).bbox().pmax().row()
			   - comps(nbh_object).bbox().pmax().row());
	    dc = math::abs(comps(current_object).bbox().pcenter().col()
			   - comps(nbh_object).bbox().pcenter().col());

	    result = std::atan(dr / dc);
	  }
	  break;

	  // Left
	  case anchor::Left:
	  {
	    dr = math::abs(comps(current_object).bbox().pcenter().row()
			   - comps(nbh_object).bbox().pcenter().row());
	    dc = math::abs(comps(current_object).bbox().pmin().col()
			   - comps(nbh_object).bbox().pmin().col());

	    result = std::atan(dc / dr);
	  }
	  break;

	  // Right
	  case anchor::Right:
	  {
	    dr = math::abs(comps(current_object).bbox().pcenter().row()
			   - comps(nbh_object).bbox().pcenter().row());
	    dc = math::abs(comps(current_object).bbox().pmax().col()
			   - comps(nbh_object).bbox().pmax().col());

	    result = std::atan(dc / dr);
	  }
	  break;

	  default:
	    trace::warning("scribo::filter::internal::alignment_angle,"
			   " Invalid anchor value... Aborting computation.");
	}

	return result;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::filter::internal

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // SCRIBO_FILTER_INTERNAL_ALIGNMENT_ANGLE_HH
