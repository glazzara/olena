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

#ifndef SCRIBO_FILTER_OBJECT_LINKS_NON_ALIGNED_SIMPLE_HH
# define SCRIBO_FILTER_OBJECT_LINKS_NON_ALIGNED_SIMPLE_HH

/// \file
///
/// Invalidate links between two non aligned objects.


# include <mln/util/array.hh>
# include <mln/math/abs.hh>
# include <mln/math/pi.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/object_image.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /*! \brief Invalidate links between two non aligned objects.
	Alignment is based on a given edge of object bounding boxes.

	\param[in] objects   An object image.
	\param[in] links     Object links information.
        \param[in] max_alpha Maximum angle value (degrees).


	Exemple with dim == 1 and edge == 1 (bottom
	horizontal filter):

	\verbatim

                       ------
                       |    |
	  ------       |    |
	  |    |       |    |
	  | x------------x  |
	  |    |       |    |
	  ------~ ~ ~ ~| ~ ~| ~ ~
	 object1  ~    |    |   ^
	            ~  |    |   |
		      ~------   |
                      object2   | Alpha
                        ~       |
			  ~     |
			    ~   |
			      ~ v
			        ~
	\endverbatim

	The angle between the two bottoms must be lower than \p alpha.

	edge values :
	 0 = center
	 1 = top
	 2 = bottom

    */
    template <typename L>
    object_links<L>
    object_links_non_aligned_simple(const object_image(L)& objects,
				    const object_links<L>& links,
				    unsigned edge,
				    float max_alpha);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    object_links<L>
    object_links_non_aligned_simple(const object_image(L)& objects,
				    const object_links<L>& links,
				    unsigned edge,
				    float max_alpha)
    {
      trace::entering("scribo::filter::object_links_non_aligned_simple");

      mln_precondition(objects.is_valid());
      mln_precondition(links.is_valid());

      typedef typename object_image(L)::bbox_t bbox_t;
      const mln::util::array<bbox_t>& bboxes = objects.bboxes();
      object_links<L> output(links);
      float dr, dc;


      float max_alpha_rad = (max_alpha / 180.0f) * math::pi;

      // Center
      if (edge == 0)
      {
	for_all_components(i, objects.bboxes())
	{
	  if (links[i] != i)
	  {
	    dr = math::abs(bboxes[i].center().row()
			   - bboxes[links[i]].center().row());
	    dc = math::abs(bboxes[i].center().col()
			   - bboxes[links[i]].center().col());

	    if (std::atan(dr / dc) > max_alpha_rad)
	      output[i] = i;
	  }
	}
      }
      // Top
      else if (edge == 1)
      {
	for_all_components(i, objects.bboxes())
	  if (links[i] != i)
	  {
	    dr = math::abs(bboxes[i].pmin().row()
			   - bboxes[links[i]].pmin().row());
	    dc = math::abs(bboxes[i].center().col()
			   - bboxes[links[i]].center().col());

	    if (std::atan(dr / dc) > max_alpha_rad)
	      output[i] = i;
	  }
      }
      // Bottom
      else if (edge == 2)
      {
	for_all_components(i, objects.bboxes())
	{
	  if (links[i] != i)
	  {
	    dr = math::abs(bboxes[i].pmax().row()
			   - bboxes[links[i]].pmax().row());
	    dc = math::abs(bboxes[i].center().col()
			   - bboxes[links[i]].center().col());

	    if (std::atan(dr / dc) > max_alpha_rad)
	      output[i] = i;
	  }
	}
      }
      else
      {
	trace::warning("Invalid edge value... Aborting computation.");
	trace::exiting("scribo::filter::object_links_non_aligned_simple");
	return output;
      }


      trace::exiting("scribo::filter::object_links_non_aligned_simple");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECT_LINKS_NON_ALIGNED_SIMPLE_HH
