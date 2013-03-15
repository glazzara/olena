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

#ifndef SCRIBO_PRIMITIVE_LINK_WITH_SINGLE_RIGHT_LINK_DMAX_RATIO_HH
# define SCRIBO_PRIMITIVE_LINK_WITH_SINGLE_RIGHT_LINK_DMAX_RATIO_HH

/// \file
///
/// Link text objects with their right neighbor according to a maximum
/// distance.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/accu/center.hh>
# include <mln/labeling/compute.hh>
# include <mln/math/abs.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/object_links.hh>

# include <scribo/primitive/link/internal/find_link.hh>
# include <scribo/primitive/link/internal/link_single_dmax_ratio_base.hh>

# include <scribo/primitive/link/compute.hh>
# include <scribo/primitive/link/internal/dmax_default.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      /*! \brief Link objects with their right neighbor if exists.

	  \param[in] components A component set.
	  \param[in] anchor Starting point for the neighbor lookup.
	  \param[in] dmax_f DMax functor defining the maximum lookup
	                    distance.

	  \return Object links data.


	  Look for a neighbor until a maximum distance is reached. The
	  maximum distance is defined thanks to a functor \p dmax_f.
      */
      template <typename L, typename F>
      inline
      object_links<L>
      with_single_right_link_dmax_ratio(const component_set<L>& components,
					const DMax_Functor<F>& dmax_f,
					anchor::Type anchor);


      /// \overload
      /// \p dmax_f is set to internal::dmax_default.
      template <typename L>
      inline
      object_links<L>
      with_single_right_link_dmax_ratio(const component_set<L>& components,
					float dmax_ratio,
					anchor::Type anchor);

      /// \overload
      /// anchor is set to MassCenter.
      template <typename L>
      inline
      object_links<L>
      with_single_right_link_dmax_ratio(const component_set<L>& components,
					float dmax_ratio);


      /// \overload
      /// dmax_ratio is set to 3.
      /// anchor is set to MassCenter.
      template <typename L>
      inline
      object_links<L>
      with_single_right_link_dmax_ratio(const component_set<L>& components);



# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	// Functor

	template <typename L, typename F>
	class single_right_dmax_ratio_functor
	  : public link_single_dmax_ratio_base<L, F,
					       single_right_dmax_ratio_functor<L,F> >
	{
	  typedef single_right_dmax_ratio_functor<L, F> self_t;
	  typedef link_single_dmax_ratio_base<L, F, self_t> super_;

	public:
	  typedef mln_site(L) P;

	  single_right_dmax_ratio_functor(const component_set<L>& components,
					  const DMax_Functor<F>& dmax_f)
	    : super_(components, anchor::Horizontal, exact(dmax_f))
	  {
	  }

	  void compute_next_site_(P& p)
	  {
	    ++p.col();
	  }

	};

      } // end of namespace scribo::primitive::link::internal



      // Facades


      template <typename L, typename F>
      inline
      object_links<L>
      with_single_right_link_dmax_ratio(const component_set<L>& components,
					const DMax_Functor<F>& dmax_f,
					anchor::Type anchor)
      {
	mln_trace("scribo::primitive::link::with_single_right_link_dmax_ratio");

	mln_precondition(components.is_valid());

	internal::single_right_dmax_ratio_functor<L, F>
	  functor(components, dmax_f);

	object_links<L> output = compute(functor, anchor);

	return output;
      }

      template <typename L>
      inline
      object_links<L>
      with_single_right_link_dmax_ratio(const component_set<L>& components,
					float dmax_ratio,
					anchor::Type anchor)
      {
	return
	  with_single_right_link_dmax_ratio(components,
					    internal::dmax_default(dmax_ratio),
					    anchor);
      }



      template <typename L>
      inline
      object_links<L>
      with_single_right_link_dmax_ratio(const component_set<L>& components,
					float dmax_ratio)
      {
	return with_single_right_link_dmax_ratio(components,
						 dmax_ratio,
						 anchor::MassCenter);
      }


      template <typename L>
      inline
      object_links<L>
      with_single_right_link_dmax_ratio(const component_set<L>& components)
      {
	return with_single_right_link_dmax_ratio(components, 3);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_WITH_SINGLE_RIGHT_LINK_DMAX_RATIO_HH
