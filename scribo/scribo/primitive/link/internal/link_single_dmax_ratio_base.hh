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
# include <scribo/core/tag/anchor.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/object_links.hh>

# include <scribo/core/concept/dmax_functor.hh>

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
	template <typename L, typename F, typename E>
	class link_single_dmax_ratio_base
	  : public link_functor_base<L,E>
	{
	  typedef link_functor_base<L,E> super_;
	  typedef mln_result(accu::center<mln_psite(L)>) ms_t;

	public:

	  typedef mln_site(L) P;

	  link_single_dmax_ratio_base(const component_set<L>& components,
				      anchor::Direction direction,
				      const DMax_Functor<F>& dmax_f);



	  bool verify_link_criterion_(unsigned current_object,
				      const P& start_point, const P& p,
				      anchor::Type anchor) const;

	  mln_site(L) start_point_(unsigned current_object,
				   anchor::Type anchor);

	  void start_processing_object_(unsigned current_object);

	protected:
	  const F dmax_f_;
	  float dmax_;
	  anchor::Direction direction_;
	};


# ifndef MLN_INCLUDE_ONLY


	template <typename L, typename F, typename E>
	inline
	link_single_dmax_ratio_base<L, F, E>::link_single_dmax_ratio_base(
	  const component_set<L>& components,
	  anchor::Direction direction,
	  const DMax_Functor<F>& dmax_f)

	: super_(components),
	    dmax_f_(exact(dmax_f)),
	    dmax_(0),
	    direction_(direction)
	{
	}

	template <typename L, typename F, typename E>
	inline
	bool
	link_single_dmax_ratio_base<L, F, E>::verify_link_criterion_(
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


	template <typename L, typename F, typename E>
	inline
	mln_site(L)
	link_single_dmax_ratio_base<L, F, E>::start_point_(unsigned current_object,
							   anchor::Type anchor)
	{
	  return internal::compute_anchor(this->components_,
					  current_object, anchor);
	}


	template <typename L, typename F, typename E>
	inline
	void
	link_single_dmax_ratio_base<L, F, E>::start_processing_object_(
	  unsigned current_object)
	{
	  dmax_ = dmax_f_(this->components_.info(current_object).bbox());


// 	  float
// 	    w = this->components_.info(current_object).bbox().width();
// //	    h = this->components_.info(current_object).bbox().height();

// 	  // FIXME: default case
// //	  dmax_ = (w / 2.0f) + (dmax_ratio_ * math::max(w, h));

// 	  // FIXME: to use for paragraph alignment
// 	  dmax_ = (w / 2.0f) + (dmax_ratio_ * w);

	  // FIXME: to use for newspapers like ICDAR documents!
//	  dmax_ = (w / 2.0f) + (h + w);

	  // FIXME: more general use case for newspapers
//	  dmax_ = (w / 2.0f) + dmax_ratio_ * (h + w);


	  // FIXME: WE WANT TO PROVIDE A DMAX RULE THROUGH A FUNCTOR.
	}


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SINGLE_DMAX_RATIO_BASE_HH_
