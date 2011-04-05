// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_ALIGNED_DELTA_BASE_HH_
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_ALIGNED_DELTA_BASE_HH_

/// \file
///
/// \brief Base class for link functors using bounding box center,
/// a proportional max distance and alignment criterion.


# include <mln/accu/center.hh>
# include <mln/labeling/compute.hh>
# include <mln/math/abs.hh>
# include <mln/math/max.hh>
# include <mln/util/array.hh>
# include <mln/value/rgb8.hh>
# include <mln/draw/line.hh>
# include <mln/literal/colors.hh>
# include <mln/norm/l1.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/tag/anchor.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/concept/dmax_functor.hh>

# include <scribo/debug/logger.hh>

# include <scribo/primitive/link/internal/link_single_dmax_ratio_base.hh>
# include <scribo/filter/internal/component_aligned.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      namespace internal
      {
	using namespace scribo::debug;


	template <typename L, typename F, typename E>
	class link_single_dmax_ratio_aligned_delta_base
	  : public link_single_dmax_ratio_base<L, F, E>
	{
	  typedef link_single_dmax_ratio_base<L, F, E> super_;

	public:
	  typedef mln_site(L) P;

	  link_single_dmax_ratio_aligned_delta_base(
	    const component_set<L>& components,
	    const DMax_Functor<F>& dmax_f,
	    int delta_pixel,
	    anchor::Direction delta_direction)
	    : super_(components, anchor::Horizontal, exact(dmax_f)),
	      delta_pixel_(delta_pixel), delta_direction_(delta_direction)
	  {

	  }


	  mln_site(L)
	  start_point_(unsigned current_object, anchor::Type anchor)
	  {
	    return link::internal::compute_anchor(this->components_,
						  current_object, anchor);
	  }


	  inline
	  bool
	  valid_link_(unsigned current_object,
		      const P& start_point,
		      const P& p,
		      anchor::Type anchor)
	  {
	    if (!super_::valid_link_(current_object, start_point, p, anchor))
	      return false;

	    // We can assume that the current site 'p' points to an
	    // object different from the background.

	    mln_site(L)
	      p1 = compute_anchor(this->components_, current_object, anchor),
	      p2 = compute_anchor(this->components_, this->labeled_image_(p),
				  anchor);

	    return std::abs(p1[this->delta_direction_]
			    - p2[this->delta_direction_]) <= delta_pixel_;
	  }


	  void validate_link_(unsigned current_object,
	  		      const P& start_point,
	  		      const P& p,
	  		      anchor::Type anchor)
	  {
	    super_::validate_link_(current_object, start_point, p, anchor);
	  }

	  void invalidate_link_(unsigned current_object,
	  			const P& start_point,
	  			const P& p,
	  			anchor::Type anchor)
	  {
	    super_::invalidate_link_(current_object, start_point, p, anchor);
	  }

	  int delta_pixel_;
	  anchor::Direction delta_direction_;
	};


# ifndef MLN_INCLUDE_ONLY


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_ALIGNED_DELTA_BASE_HH_
