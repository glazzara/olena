// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_PRIMITIVE_LINK_WITH_SINGLE_UP_LINK_HH
# define SCRIBO_PRIMITIVE_LINK_WITH_SINGLE_UP_LINK_HH

/// \file
///
/// Link components with their up neighbor.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/accu/center.hh>

# include <mln/labeling/compute.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>


# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>

# include <scribo/primitive/link/internal/find_link.hh>
# include <scribo/primitive/link/internal/link_single_dmax_base.hh>

# include <scribo/primitive/link/compute.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      /// \brief Link components with their up neighbor if exists.
      ///
      /// \param[in] comps A component set.
      /// \param[in] neighb_max_distance The maximum distance allowed
      ///                                to seach a neighbor object.
      /// \param[in] anchor The neighborhod lookup start point.
      ///
      /// \return Object links data.
      //
      template <typename L>
      inline
      object_links<L>
      with_single_up_link(const component_set<L>& comps,
			  unsigned neighb_max_distance,
			  anchor::Type anchor);

      /// \overload
      /// Anchor type is set to anchor::Center
      //
      template <typename L>
      inline
      object_links<L>
      with_single_up_link(const component_set<L>& comps,
			  unsigned neighb_max_distance);


      /// \overload
      /// Max distance is set to mln_max(unsigned).
      //
      template <typename L>
      inline
      object_links<L>
      with_single_up_link(const component_set<L>& comps);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	// Functor

	template <typename L>
	class single_up_functor
	  : public internal::link_single_dmax_base<L, single_up_functor<L> >
	{
	  typedef
	    internal::link_single_dmax_base<L, single_up_functor<L> > super_;

	public:
	  typedef mln_site(L) P;

	  single_up_functor(const component_set<L>& comps, unsigned dmax)
	    : super_(comps, dmax, anchor::Vertical)
	  {
	  }

	  void compute_next_site_(P& p)
	  {
	    --p.row();
	  }

	};

      } // end of namespace scribo::primitive::link::internal



      // Facades

      template <typename L>
      inline
      object_links<L>
      with_single_up_link(const component_set<L>& comps,
			  unsigned neighb_max_distance,
			  anchor::Type anchor)
      {
	mln_trace("scribo::primitive::link::with_single_up_link");

	internal::single_up_functor<L>
	  functor(comps, neighb_max_distance);

	object_links<L> output = compute(functor, anchor);

	return output;
      }

      template <typename L>
      inline
      object_links<L>
      with_single_up_link(const component_set<L>& comps,
			  unsigned neighb_max_distance)
      {
	return with_single_up_link(comps, neighb_max_distance,
				   anchor::MassCenter);
      }


      template <typename L>
      inline
      object_links<L>
      with_single_up_link(const component_set<L>& comps)
      {
	return with_single_up_link(comps, mln_max(unsigned));
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_WITH_SINGLE_UP_LINK_HH
