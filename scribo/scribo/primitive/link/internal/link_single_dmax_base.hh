// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_BASE_HH_
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_BASE_HH_

/// \file
///
/// Base class for link functors using center sites and a given max
/// distance.


# include <mln/accu/center.hh>
# include <mln/labeling/compute.hh>
# include <mln/math/abs.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/tag/anchor.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/object_links.hh>

# include <scribo/primitive/link/internal/find_link.hh>
# include <scribo/primitive/link/internal/compute_anchor.hh>
# include <scribo/primitive/link/internal/link_functor_base.hh>

# include <scribo/primitive/link/compute.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      namespace internal
      {

	/// \brief Base class for link functors using mass centers and
	/// a given max distance.
	//
	template <typename L, typename E>
	class link_single_dmax_base
	  : public link_functor_base<L, E>
	{
	  typedef link_functor_base<L,E> super_;
	  typedef mln_result(accu::center<mln_psite(L)>) ms_t;

	public:

	  typedef mln_site(L) P;

	  link_single_dmax_base(const component_set<L>& components,
				unsigned neighb_max_distance,
				anchor::Direction direction);


	  bool verify_link_criterion_(unsigned current_object,
				      const P& start_point, const P& p,
				      anchor::Type anchor) const;

	  void start_processing_object_(unsigned current_object);

	private:
	  float dmax_;
	  float neighb_max_distance_;
	  anchor::Direction direction_;
	};


# ifndef MLN_INCLUDE_ONLY


	template <typename L, typename E>
	inline
	link_single_dmax_base<L, E>::link_single_dmax_base(
	  const component_set<L>& components,
	  unsigned neighb_max_distance,
	  anchor::Direction direction)

	  : super_(components),
	    dmax_(0),
	    neighb_max_distance_(neighb_max_distance),
	    direction_(direction)
	{
	}


	template <typename L, typename E>
	inline
	bool
	link_single_dmax_base<L, E>::verify_link_criterion_(
	  unsigned current_object,
	  const P& start_point,
	  const P& p,
	  anchor::Type anchor) const
	{
	  (void) current_object;
	  (void) anchor;

	  float dist = math::abs(p[direction_] - start_point[direction_]);
	  return dist <= dmax_; // Not too far
	}


	template <typename L, typename E>
	inline
	void
	link_single_dmax_base<L, E>::start_processing_object_(
	  unsigned current_object)
	{
	  float
	    midcol = (this->components_(current_object).bbox().pmax()[direction_]
		      - this->components_(current_object).bbox().pmin()[direction_]) / 2;
	  dmax_ = midcol + neighb_max_distance_;
	}


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_BASE_HH_
