// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_FILTER_INTERNAL_COMPONENT_ALIGNED_HH
# define SCRIBO_FILTER_INTERNAL_COMPONENT_ALIGNED_HH

/// \file
///
/// Invalidate components between two non aligned components.


# include <mln/util/array.hh>
# include <mln/math/abs.hh>
# include <mln/math/pi.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/tag/anchor.hh>
# include <scribo/core/component_set.hh>
# include <scribo/filter/internal/alignment_angle.hh>

namespace scribo
{

  namespace filter
  {

    namespace internal
    {

      using namespace mln;


      template <typename L>
      bool
      component_aligned(const component_set<L>& comps,
		   unsigned current_object, unsigned nbh_object,
		   anchor::Type anchor, float max_alpha);


      template <typename L>
      bool
      component_aligned_rad(const component_set<L>& comps,
			    unsigned current_object, unsigned nbh_object,
			    anchor::Type anchor, float max_alpha_rad);


# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      bool
      component_aligned(const component_set<L>& comps,
			unsigned current_object, unsigned /* nbh_object */,
			anchor::Type anchor, float max_alpha)
      {
	float max_alpha_rad = (max_alpha / 180.0f) * math::pi;

	return component_aligned(comps, current_object, anchor, max_alpha_rad);
      }



      template <typename L>
      bool
      component_aligned_rad(const component_set<L>& comps,
			    unsigned current_object, unsigned nbh_object,
			    anchor::Type anchor, float max_alpha_rad)
      {
	mln_trace("scribo::filter::internal::component_aligned_rad");

	mln_precondition(comps.is_valid());

	float angle = alignment_angle(comps, current_object,
				      nbh_object, anchor);

	bool output = (angle <= max_alpha_rad);

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::filter::internal

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_INTERNAL_COMPONENT_ALIGNED_HH
