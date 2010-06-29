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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SEVERAL_DMAX_HH_
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SEVERAL_DMAX_HH_

/// \file
///
/// Base class for link functors using mass centers and a given max
/// distance.


# include <mln/accu/center.hh>
# include <mln/labeling/compute.hh>
# include <mln/math/abs.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/tag/anchor.hh>
# include <scribo/core/object_image.hh>
# include <scribo/core/object_links.hh>

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

	/// \brief Base class for link functors using mass centers and
	/// a given max distance.
	//
	template <typename L, typename E>
	class link_several_dmax_base
	  : public link_functor_base<L, E>
	{
	  typedef mln_site(L) P;
	  typedef link_functor_base<L,E> super_;
	  typedef mln_result(accu::center<mln_psite(L)>) ms_t;
	  typedef mln::util::couple<anchor::Type, P> couple_t;

	public:

	  link_several_dmax_base(const object_image(L)& objects,
				 unsigned neighb_max_distance,
				 unsigned nanchors);



	  bool verify_link_criterion_(unsigned current_object,
				      const P& start_point, const P& p) const;

	  void start_processing_object_(unsigned current_object);

	  void validate_link_(unsigned current_object, const P& start_point,
			      const P& p, anchor::Type anchor);

	  void initialize_link_(unsigned current_object);
	  couple_t finalize_link_(unsigned current_object);



	protected:
	  mln::util::array<ms_t> mass_centers_;
	  mln::util::array<couple_t> potential_links_;
	  float dmax_;
	  float neighb_max_distance_;
	};


# ifndef MLN_INCLUDE_ONLY


	template <typename L, typename E>
	inline
	link_several_dmax_base<L, E>::link_several_dmax_base(
	  const object_image(L)& objects,
	  unsigned neighb_max_distance,
	  unsigned nanchors)

	  : super_(objects, nanchors),
	    dmax_(0),
	    neighb_max_distance_(neighb_max_distance)
	{

	  mass_centers_ = labeling::compute(accu::meta::center(),
					    objects, objects.nlabels());
	}

	template <typename L, typename E>
	inline
	bool
	link_several_dmax_base<L, E>::verify_link_criterion_(
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
	void
	link_several_dmax_base<L, E>::start_processing_object_(
	  unsigned current_object)
	{
	  float
	    midcol = (this->objects_.bbox(current_object).pmax().col()
		      - this->objects_.bbox(current_object).pmin().col()) / 2;
	  dmax_ = midcol + neighb_max_distance_;
	}


	template <typename L, typename E>
	inline
	void
	link_several_dmax_base<L, E>::validate_link_(unsigned current_object,
						     const P& start_point,
						     const P& p,
						     anchor::Type anchor)
	{
	  (void) current_object;
	  (void) start_point;
	  this->potential_links_.append(mln::make::couple(anchor, p));
	}


	template <typename L, typename E>
	inline
	void
	link_several_dmax_base<L, E>::initialize_link_(unsigned current_object)
	{
	  (void) current_object;
	  this->links_.clear();
	  this->potential_links_.clear();
	}


	template <typename L, typename E>
	inline
	mln::util::couple<anchor::Type, mln_site(L)>
	link_several_dmax_base<L, E>::finalize_link_(unsigned current_object)
	{
	  //TODO
	  mln::util::array<unsigned> dist;
	  unsigned
	    min = mln_max(unsigned),
	    tmp;

	  if (this->potential_links_.nelements() > 0)
	  {
	    for(unsigned i = 0; i < this->potential_links_.nelements(); ++i)
	    {
	      tmp = math::abs(this->objects_.bbox(current_object).pmax().col()
			      - this->potential_links_(i).second().col());
	      dist.append(tmp);
	      if (tmp < min)
		min = tmp;
	    }

	    mln_assertion(min != mln_max(unsigned));

	    // Keep closest links and compute vertical overlap.
	    unsigned
	      nratio = 0,
	      id_max_ratio = 0;
	    mln_value(L) nbh_id;
	    mln::util::array<float> overlap;
	    for(unsigned i = 0; i < this->potential_links_.nelements(); ++i)
	      if (dist[i] < (1.2 * min))
	      {
		unsigned
		  other_object = this->objects_(potential_links_(i).second());
		nbh_id = other_object;

		float
		  dr
		   = math::min(this->objects_.bbox(current_object).pmax().row(),
			       this->objects_.bbox(other_object).pmax().row())
 		   - math::min(this->objects_.bbox(current_object).pmin().row(),
			       this->objects_.bbox(other_object).pmin().row()),
		  dh = this->objects_.bbox(other_object).pmax().row()
		      - this->objects_.bbox(other_object).pmin().row(),
 		  ratio = dr / dh;

		overlap.append(ratio);
		if (ratio > overlap(id_max_ratio) || nratio == 0)
		{
		  id_max_ratio = i;
		  nratio = 1;
		  nbh_id = other_object;
		}
		else if (ratio == overlap(id_max_ratio)
			 && other_object != nbh_id)
		  ++nratio;
	      }
	      else
		overlap.append(0);

	    if (nratio == 1)
	    {
	      this->links_(current_object)
		= this->objects_(potential_links_(id_max_ratio).second());

	      return potential_links_(id_max_ratio);
	    }

	  }

	  this->links_(current_object) = current_object;
	  return mln::make::couple(anchor::Invalid, P());
	}



# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_SEVERAL_DMAX_HH_
