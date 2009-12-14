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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_BASE_HH_
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_BASE_HH_

/// \file
///
/// \brief Base class for link functors using bounding box center and
/// a proportional max distance.


# include <mln/accu/center.hh>
# include <mln/labeling/compute.hh>
# include <mln/math/abs.hh>
# include <mln/math/max.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/anchors.hh>
# include <scribo/core/object_image.hh>
# include <scribo/core/object_links.hh>

# include <scribo/primitive/link/internal/compute_anchor.hh>
# include <scribo/primitive/link/internal/find_link.hh>
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

	/// \brief Base class for link functors using bounding box
	/// center and a proportional max distance.
	//
	template <typename L, typename E>
	class link_single_dmax_ratio_base
	  : public link_functor_base<L, E>
	{
	  typedef link_functor_base<L,E> super_;
	  typedef mln_result(accu::center<mln_psite(L)>) ms_t;

	public:

	  typedef mln_site(L) P;


	  link_single_dmax_ratio_base(const object_image(L)& objects,
				  float dmax_ratio,
				  unsigned center_type_);



	  bool verify_link_criterion_(unsigned current_object,
				      const P& start_point, const P& p) const;

	  mln_site(L) start_point_(unsigned current_object,
				   anchor::Type anchor);

	  void start_processing_object_(unsigned current_object);

	private:
	  float dmax_ratio_;
	  float dmax_;
	  mln::util::array<ms_t> mass_centers_;
	};


# ifndef MLN_INCLUDE_ONLY


	template <typename L, typename E>
	inline
	link_single_dmax_ratio_base<L, E>::link_single_dmax_ratio_base(
	  const object_image(L)& objects,
	  float dmax_ratio)

	  : super_(objects),
	    dmax_ratio_(dmax_ratio),
	    dmax_(0)
	{
	  mass_centers_ = labeling::compute(accu::meta::center(),
					    objects, objects.nlabels());
	}

	template <typename L, typename E>
	inline
	bool
	link_single_dmax_ratio_base<L, E>::verify_link_criterion_(
	  unsigned current_object,
	  const P& start_point,
	  const P& p) const
	{
	  (void) current_object;

	  float dist = math::abs(p.col() - start_point.col());
	  return dist <= dmax_; // Not too far
	}


	template <typename L, typename E>
	inline
	mln_site(L)
	link_single_dmax_ratio_base<L, E>::start_point_(unsigned current_object,
							anchor::Type anchor)
	{
	  (void) anchor;
	  return internal::compute_anchors(this->objects_, mass_centers_,
					   current_object, anchor);
	}


	template <typename L, typename E>
	inline
	void
	link_single_dmax_ratio_base<L, E>::start_processing_object_(
	  unsigned current_object)
	{
	  float
	    w = (this->objects_.bbox(current_object).pmax().col()
		 - this->objects_.bbox(current_object).pmin().col()),
	    h = (this->objects_.bbox(current_object).pmax().row()
		 - this->objects_.bbox(current_object).pmin().row());
	  dmax_ = (w / 2.0f) + (dmax_ratio_ * math::max(w, h));
	}


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_BASE_HH_
