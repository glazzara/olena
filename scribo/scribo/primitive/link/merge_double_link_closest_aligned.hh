// Copyright (C) 2011, 2012 EPITA Research and Development Laboratory
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

/// \file
///
/// \brief Merge two object links data based on distance and angle.

#ifndef SCRIBO_PRIMITIVE_LINK_MERGE_DOUBLE_LINK_CLOSEST_ALIGNED_HH
# define SCRIBO_PRIMITIVE_LINK_MERGE_DOUBLE_LINK_CLOSEST_ALIGNED_HH


# include <mln/util/couple.hh>
# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/tag/anchor.hh>
# include <scribo/filter/internal/alignment_angle.hh>

namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      /*! \brief Merge two object links data based on distance and angle.

	Performs an 'OR' operation on the links according distance and
	angle criterion.

	If a component has several incoming links, only the link for
	which the the object is the closest or the angle performed
	between the two bboxes is the lower, will be preserved.
       */
      template <typename L>
      object_links<L>
      merge_double_link_closest_aligned(const object_links<L>& left,
					const object_links<L>& right,
					anchor::Type anchor_angle);

# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      object_links<L>
      merge_double_link_closest_aligned(const object_links<L>& left,
					const object_links<L>& right,
					anchor::Type anchor_angle)
      {
	mln_trace("scribo::primitive::link::merge_double_link_closest_aligned");
	mln_precondition(left.is_valid());
	mln_precondition(right.is_valid());


	const component_set<L>& components = left.components();

	mln::util::array<mln::util::couple<unsigned, float> > align_data;
	align_data.resize(value::next(left.nelements()));
	for (unsigned i = 0; i < align_data.nelements(); ++i)
	{
	  align_data(i).first() = mln_max(unsigned);
	  align_data(i).second() = 360;
	}

	object_links<L> output = right.duplicate();

	// Init alignment data.
	for_all_links(l, output)
	  if (right(l) != l)
	  {
	    //FIXME Handle the case when a component is included in another one ?

	    // Update distance
	    align_data(l).first()
	      = std::abs(components(l).bbox().pcenter().col()
			 - components(right(l)).bbox().pcenter().col());

	    // Update angle
	    align_data(l).second()
	      = filter::internal::alignment_angle(components,
						  l,
						  right(l),
						  anchor_angle);
	  }
	  else
	  {
	    // Update distance
	    align_data(l).first() = mln_max(unsigned);

	    // Update angle
	    align_data(l).second() = 360.f;
	  }



	// Trying to improve results with left links.
	for_all_links(l, left)
	  if (left(l) != l && left(l) != right(l))
	  {
	    unsigned dh = std::abs(components(l).bbox().pcenter().col()
				   - components(left(l)).bbox().pcenter().col());

	    float alpha
	      = filter::internal::alignment_angle(components,
						  l,
						  left(l),
						  anchor_angle);

	    if (right(left(l)) == left(l))
	    {
	      output.update(left(l), l);
	    }
	    else if ((dh < align_data(left(l)).first() * 0.66f)
		     || (alpha < align_data(left(l)).second()))
	    {
	      output.update(left(l), l);
	      align_data(left(l)).first() = dh;
	      align_data(left(l)).second() = alpha;
	    }
	  }

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_MERGE_DOUBLE_LINK_CLOSEST_ALIGNED_HH
