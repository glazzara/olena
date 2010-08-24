// Copyright (C) 2010 EPITA Research and Development Laboratory
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
	trace::entering("scribo::filter::internal::alignment_angle_rad");

	mln_precondition(comps.is_valid());

	float dr, dc;

	if (nbh_object == current_object)
	  return 0;

	// Center
	if (anchor == anchor::Center)
	{
	  dr = math::abs(comps(current_object).bbox().pcenter().row()
			 - comps(nbh_object).bbox().pcenter().row());
	  dc = math::abs(comps(current_object).bbox().pcenter().col()
			 - comps(nbh_object).bbox().pcenter().col());

	  return std::atan(dr / dc);
	}

	// Top
	else if (anchor == anchor::Top)
	{
	  dr = math::abs(comps(current_object).bbox().pmin().row()
			 - comps(nbh_object).bbox().pmin().row());
	  dc = math::abs(comps(current_object).bbox().pcenter().col()
			 - comps(nbh_object).bbox().pcenter().col());

	  return std::atan(dr / dc);
	}

	// Bottom
	else if (anchor == anchor::Bottom)
	{
	  dr = math::abs(comps(current_object).bbox().pmax().row()
			 - comps(nbh_object).bbox().pmax().row());
	  dc = math::abs(comps(current_object).bbox().pcenter().col()
			 - comps(nbh_object).bbox().pcenter().col());

	  return std::atan(dr / dc);
	}

	// Left
	else if (anchor == anchor::Left)
	{
	  dr = math::abs(comps(current_object).bbox().pcenter().row()
			 - comps(nbh_object).bbox().pcenter().row());
	  dc = math::abs(comps(current_object).bbox().pmin().col()
			 - comps(nbh_object).bbox().pmin().col());

	  return std::atan(dc / dr);
	}

	// Right
	else if (anchor == anchor::Right)
	{
	  dr = math::abs(comps(current_object).bbox().pcenter().row()
			 - comps(nbh_object).bbox().pcenter().row());
	  dc = math::abs(comps(current_object).bbox().pmax().col()
			 - comps(nbh_object).bbox().pmax().col());

	  return std::atan(dc / dr);
	}

	else
	  trace::warning("Invalid anchor value... Aborting computation.");

	trace::exiting("scribo::filter::internal::alignment_angle_rad");
	return 0;

      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::filter::internal

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // SCRIBO_FILTER_INTERNAL_ALIGNMENT_ANGLE_HH
