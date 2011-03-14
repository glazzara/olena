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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_ALIGNED_BASE_HH_
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_ALIGNED_BASE_HH_

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
	class link_single_dmax_ratio_aligned_base
	  : public link_single_dmax_ratio_base<L, F, E>
	{
	  typedef link_single_dmax_ratio_base<L, F, E> super_;

	public:
	  typedef mln_site(L) P;

	  link_single_dmax_ratio_aligned_base(
	    const component_set<L>& components,
	    const DMax_Functor<F>& dmax_f,
	    float min_angle,
	    float max_angle,
	    anchor::Type anchor)
	    : super_(components, anchor::Horizontal, exact(dmax_f)),
	      debug_anchor_(anchor)
	  {
	    min_alpha_rad = (min_angle / 180.0f) * math::pi;
	    max_alpha_rad = (max_angle / 180.0f) * math::pi;

#  ifndef SCRIBO_NDEBUG
	    if (scribo::debug::logger().is_at_level(debug::AuxiliaryResults))
	    {
	      initialize(debug_, components.labeled_image());
	      data::fill(debug_, literal::black);
	      data::fill((debug_
			  | (pw::value(components.labeled_image()) != 0u)).rw(),
			 literal::white);
	      debug_angle_ = duplicate(debug_);
	    }
#  endif // ! SCRIBO_NDEBUG
	  }

	  void compute_next_site_(P& p)
	  {
	    ++p.col();
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

	    // Distance between the two components.
	    float dist;// = math::abs(p[this->direction_] - start_point[this->direction_]);

	    box<P> b = this->components_(current_object).bbox();
	    if (p[this->direction_] > b.pmax()[this->direction_])
	      dist = math::abs(p[this->direction_] - b.pmax()[this->direction_]);
	    // current object is on the right.
	    else
	      dist = math::abs(p[this->direction_] - b.pmin()[this->direction_]);


	    unsigned ldist = this->components_(current_object).bbox().width() / 2;


	    // Components are really close, so the angle is more permissive.
	    if (dist < ldist * 3)
	    {
	      return
		filter::internal::component_aligned_rad(this->components_,
							current_object,
							this->labeled_image_(p),
							anchor,
							max_alpha_rad);
	    }


	    // Components are really far, so the angle is less permissive.
	    return
	      filter::internal::component_aligned_rad(this->components_,
						      current_object,
						      this->labeled_image_(p),
						      anchor,
						      min_alpha_rad);
	  }


	  void validate_link_(unsigned current_object,
			      const P& start_point,
			      const P& p,
			      anchor::Type anchor)
	  {
	    super_::validate_link_(current_object, start_point, p, anchor);

#  ifndef SCRIBO_NDEBUG
	    if (logger().is_at_level(debug::AuxiliaryResults))
	    {
	      mln_site(L)
	    	p1 = link::internal::compute_anchor(this->components_,
	    					    current_object, debug_anchor_),
	    	p2 = link::internal::compute_anchor(this->components_,
	    					    this->labeled_image_(p),
						    debug_anchor_);
	      mln::draw::line(debug_, p1, p2, literal::green);


	      float
	    	angle = filter::internal::alignment_angle(this->components_,
	    						  current_object,
	    						  this->labeled_image_(p),
	    						  anchor);
	      angle = (angle * 180.0f) / math::pi;
	      angle = angle * 20.0f + 1.0f;
	      mln::draw::line(debug_angle_, p1, p2,
	    		      value::rgb8(unsigned(angle),
	    				  unsigned(angle),
	    				  unsigned(angle)));
	    }
#  endif // ! SCRIBO_NDEBUG
	  }

	  void invalidate_link_(unsigned current_object,
				const P& start_point,
				const P& p,
				anchor::Type anchor)
	  {
	    super_::invalidate_link_(current_object, start_point, p, anchor);

#  ifndef SCRIBO_NDEBUG
	    if (logger().is_at_level(debug::AuxiliaryResults))
	    {
	      if (this->labeled_image_.domain().has(p) && this->labeled_image_(p) != 0)
	      {
	    	mln_site(L)
	    	  p1 = link::internal::compute_anchor(this->components_,
	    					      current_object, debug_anchor_),
	    	  p2 = link::internal::compute_anchor(this->components_,
	    					      this->labeled_image_(p),
	    			       debug_anchor_);

		// Arbitrary check to avoid large lines drawn over the whole page.
	    	if (this->labeled_image_.domain().has(p2)
	    	    && norm::l1_distance(p1.to_vec(), p2.to_vec()) < 300)
	    	{
	    	  mln::draw::line(debug_, p1, p2, literal::red);
	    	}


	    	float
	    	  angle = filter::internal::alignment_angle(this->components_,
	    						    current_object,
	    						    this->labeled_image_(p),
	    						    anchor);
	    	angle = (angle * 180.0f) / math::pi;
	    	angle = angle * 20.0f + 1.0f;
	    	mln::draw::line(debug_angle_, p1, p2,
	    			value::rgb8(unsigned(angle),
	    				    unsigned(angle),
	    				    unsigned(angle)));
	      }
	    }
#  endif // ! SCRIBO_NDEBUG
	  }

	  float min_alpha_rad;
	  float max_alpha_rad;

	  // Anchor used for debug output!
	  anchor::Type debug_anchor_;

	  mln_ch_value(L, value::rgb8) debug_;
	  mln_ch_value(L, value::rgb8) debug_angle_;
	};


# ifndef MLN_INCLUDE_ONLY


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_ALIGNED_BASE_HH_
