// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef SCRIBO_FILTER_OBJECT_LINKS_BBOX_OVERLAP_HH
# define SCRIBO_FILTER_OBJECT_LINKS_BBOX_OVERLAP_HH

/// \file
///
/// Invalidate links between two components having their bounding box
/// overlapping too much.


# include <mln/math/min.hh>
# include <mln/math/max.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>
# include <scribo/filter/object_links_bbox_ratio.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /*! \brief Invalidate links between two components having their bounding box
        overlapping too much.

	\param[in] links             Link components information.
	\param[in] max_overlap_ratio The maximum ratio of the overlapping
	                             areas.

	\result A filtered object link information.
    */
    template <typename L>
    object_links<L>
    object_links_bbox_overlap(const object_links<L>& links,
			      float max_overlap_ratio);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    object_links<L>
    object_links_bbox_overlap(const object_links<L>& links,
			      float max_overlap_ratio)
    {
      trace::entering("scribo::filter::object_links_bbox_overlap");

      mln_precondition(links.is_valid());

      const component_set<L>& components = links.components();
      object_links<L> output = links.duplicate();

      bool has_intersection;
      mln_site(L) pmin, pmax;
      float ratio_i, ratio_link_i;

      for_all_links(i, links)
	if (links.is_linked(i))
	{
	  has_intersection = true;
	  for (unsigned dim = 0; dim < mln_site_(L)::dim; ++dim)
	  {
	    pmin[dim] = math::max(components(i).bbox().pmin()[dim],
				  components(links(i)).bbox().pmin()[dim]);
	    pmax[dim] = math::min(components(i).bbox().pmax()[dim],
				  components(links(i)).bbox().pmax()[dim]);

	    if (pmin[dim] > pmax[dim])
	    {
	      has_intersection = false;
	      break;
	    }
	  }

	  if (!has_intersection)
	    continue;

	  mln_box(L) interbbox(pmin, pmax);

	  ratio_i = interbbox.nsites() /(float)components(i).bbox().nsites();
	  ratio_link_i = interbbox.nsites() /(float)components(links(i)).bbox().nsites();

	  if (ratio_i >= max_overlap_ratio
	      || ratio_link_i >= max_overlap_ratio)
	    output.clear(i);
	}

      trace::exiting("scribo::filter::object_links_bbox_overlap");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECT_LINKS_BBOX_OVERLAP_HH
