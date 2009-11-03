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

#ifndef SCRIBO_FILTER_OBJECT_LINKS_NON_ALIGNED_HH
# define SCRIBO_FILTER_OBJECT_LINKS_NON_ALIGNED_HH

/// \file
///
/// Invalidate links between two non aligned objects.


# include <mln/math/abs.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/object_image.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /*! \brief Invalidate links between two non aligned objects.
	Alignment is based on object bounding boxes.

	\param[in] objects   An object image.
	\param[in] links     Object links information.
	\param[in] dim       Choose the dimension on which applying the
	                     filter.
        \param[in] max_delta Maximum delta.


	Exemple with dim == 1 (horizontal filter):

	\verbatim
	                          v
                       ------ ~ ~ ~
                       |    |     |\
	  ------ ~ ~ ~ |~ ~ | ~ ~ ~ \
	  |    |       |    |     ^  \
	  | x------------x  |         => delta, must be < to max_delta
	  |    |       |    |     v  /
	  ------ ~ ~ ~ |~ ~ | ~ ~ ~ /
	  object1      |    |     |/
		       ------ ~ ~ ~
                      object2     ^

	\endverbatim

    */
    template <typename L>
    object_links<L>
    object_links_non_aligned(const object_image(L)& objects,
			     const object_links<L>& links,
			     unsigned dim,
			     unsigned max_delta);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    object_links<L>
    object_links_non_aligned(const object_image(L)& objects,
			     const object_links<L>& links,
			     unsigned dim,
			     unsigned max_delta)
    {
      trace::entering("scribo::filter::object_links_non_aligned");

      mln_precondition(objects.is_valid());


      typedef typename object_image(L)::bbox_t bbox_t;
      const mln::util::array<bbox_t>& bboxes = objects.bboxes();
      object_links<L> output(links);

      for_all_components(i, objects.bboxes())
	if (links[i] != i)
	{
	  unsigned
	    lnbh = bboxes(links[i]).pmax()[dim] - bboxes(links[i]).pmin()[dim],
	    lcurrent = bboxes(i).pmax()[dim] - bboxes(i).pmin()[dim],
	    hmin = i,
	    hmax = links[i];

	  if (lnbh < lcurrent)
	  {
	    hmin = links[i];
	    hmax = i;
	  }

	  if (static_cast<unsigned>(math::abs((bboxes[hmin].pmin()[dim] - bboxes[hmax].pmin()[dim]))) > max_delta
	      || static_cast<unsigned>(math::abs((bboxes[hmin].pmax()[dim] - bboxes[hmax].pmax()[dim]))) > max_delta)
	    output[i] = i;

	}

      trace::exiting("scribo::filter::object_links_non_aligned");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECT_LINKS_NON_ALIGNED_HH
