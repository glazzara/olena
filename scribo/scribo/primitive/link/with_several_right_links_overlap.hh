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

#ifndef SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_LINKS_OVERLAP_HH
# define SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_LINKS_OVERLAP_HH

/// \file
///
/// Link text bounding boxes with their right neighbor.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/util/array.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/object_image.hh>
# include <scribo/core/macros.hh>
# include <scribo/primitive/internal/init_link_array.hh>
# include <scribo/primitive/link/internal/find_several_links.hh>
# include <scribo/primitive/link/internal/link_several_dmax_base.hh>
# include <scribo/util/text.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      using namespace mln;

      /// Map each character bounding box to its right bounding box neighbor
      /// if possible.
      /// Iterate to the right but link boxes to the right.
      ///
      /// \return an mln::util::array. Map a bounding box to its right neighbor.
      template <typename L>
      inline
      object_links<L>
      with_several_right_links_overlap(const object_image(L)& objects,
				       unsigned neighb_max_distance);

      /// \overload
      template <typename L>
      inline
      object_links<L>
      with_several_right_links_overlap(const object_image(L)& objects);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename L>
	class several_right_overlap_functor
	  : public link_several_dmax_base<L, several_right_overlap_functor<L> >
	{
	  typedef
	    link_several_dmax_base<L, several_right_overlap_functor<L> >
	  super_;

	public:
	  typedef mln_site(L) P;

	  several_right_overlap_functor(const object_image(L)& objects,
					unsigned dmax)
	    : super_(objects, dmax, 3)
	  {
	  }

	  mln_site(L)
	  start_point_(unsigned current_object, unsigned anchor)
	  {
	    return anchors_3(this->objects_, this->mass_centers_,
			     current_object, anchor);
	  }


	  void compute_next_site_(P& p)
	  {
	    ++p.col();
	  }

	};

      } // end of namespace scribo::primitive::link::internal


      template <typename L>
      inline
      object_links<L>
      with_several_right_links_overlap(const object_image(L)& objects,
				       unsigned neighb_max_distance)
      {
	mln_trace("scribo::primitive::link::with_several_right_links_overlap");

	mln_precondition(objects.is_valid());

	internal::several_right_overlap_functor<L>
	  functor(objects, neighb_max_distance);

	for_all_ncomponents(current_object, objects.nlabels())
	  internal::find_several_links(functor, current_object);

	return functor.links();
      }


      template <typename L>
      inline
      object_links<L>
      with_several_right_links_overlap(const object_image(L)& objects)
      {
	return with_several_right_links_overlap(objects, mln_max(unsigned));
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_LINKS_OVERLAP_HH
