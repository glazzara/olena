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

#ifndef SCRIBO_FILTER_OBJECT_LINKS_NON_ALIGNED_SIMPLE_HH
# define SCRIBO_FILTER_OBJECT_LINKS_NON_ALIGNED_SIMPLE_HH

/// \file
///
/// Invalidate links between two non aligned components.


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

    /*! \brief Invalidate links between two non aligned components.
	Alignment is based on a given edge of object bounding boxes.

	\param[in] components   A component set.
	\param[in] links        Object links information.
        \param[in] max_alpha    Maximum angle value (degrees).


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
	 3 = left
	 4 = right

    */
    template <typename L>
    object_links<L>
    object_links_non_aligned_simple(const component_set<L>& components,
				    const object_links<L>& links,
				    unsigned edge,
				    float max_alpha);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    object_links<L>
    object_links_non_aligned_simple(const component_set<L>& comps,
				    const object_links<L>& links,
				    unsigned edge,
				    float max_alpha)
    {
      trace::entering("scribo::filter::object_links_non_aligned_simple");

      mln_precondition(comps.is_valid());
      mln_precondition(links.is_valid());

      object_links<L> output(links);
      float dr, dc;


      float max_alpha_rad = (max_alpha / 180.0f) * math::pi;

      // Center
      if (edge == 0)
      {
	for_all_comps(i, comps)
	{
	  if (links[i] != i)
	  {
	    dr = math::abs(comps(i).bbox().pcenter().row()
			   - comps(links[i]).bbox().pcenter().row());
	    dc = math::abs(comps(i).bbox().pcenter().col()
			   - comps(links[i]).bbox().pcenter().col());

	    if (std::atan(dr / dc) > max_alpha_rad)
	      output[i] = i;
	  }
	}
      }
      // Top
      else if (edge == 1)
      {
	for_all_comps(i, comps.bboxes())
	  if (links[i] != i)
	  {
	    dr = math::abs(comps(i).bbox().pmin().row()
			   - comps(links[i]).bbox().pmin().row());
	    dc = math::abs(comps(i).bbox().pcenter().col()
			   - comps(links[i]).bbox().pcenter().col());

	    if (std::atan(dr / dc) > max_alpha_rad)
	      output[i] = i;
	  }
      }
      // Bottom
      else if (edge == 2)
      {
	for_all_comps(i, comps.bboxes())
	{
	  if (links[i] != i)
	  {
	    dr = math::abs(comps(i).bbox().pmax().row()
			   - comps(links[i]).bbox().pmax().row());
	    dc = math::abs(comps(i).bbox().pcenter().col()
			   - comps(links[i]).bbox().pcenter().col());

	    if (std::atan(dr / dc) > max_alpha_rad)
	      output[i] = i;
	  }
	}
      }
      // Left
      else if (edge == 3)
      {
	for_all_comps(i, comps.bboxes())
	{
	  if (links[i] != i)
	  {
	    dr = math::abs(comps(i).bbox().pcenter().row()
			   - comps(links[i]).bbox().pcenter().row());
	    dc = math::abs(comps(i).bbox().pmin().col()
			   - comps(links[i]).bbox().pmin().col());

	    if (std::atan(dc / dr) > max_alpha_rad)
	      output[i] = i;
	  }
	}
      }
      // Right
      else if (edge == 4)
      {
	for_all_comps(i, comps.bboxes())
	{
	  if (links[i] != i)
	  {
	    dr = math::abs(comps(i).bbox().pcenter().row()
			   - comps(links[i]).bbox().pcenter().row());
	    dc = math::abs(comps(i).bbox().pmax().col()
			   - comps(links[i]).bbox().pmax().col());

	    if (std::atan(dc / dr) > max_alpha_rad)
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
