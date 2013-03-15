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

#ifndef SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_CLOSEST_LINKS_HH
# define SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_CLOSEST_LINKS_HH

/// \file
///
/// Link text bounding boxes with their right neighbor.
///
/// Merge code with primitive::link::with_single_right_link.hh
///
/// \todo Factor code with primitive::link::with_several_right_links
///
/// FIXME: rewrite the code with functors -> See with_several_right_links

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/math/abs.hh>

# include <mln/extension/fill.hh>

# include <mln/util/array.hh>


# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      using namespace mln;


      /// Map each character bounding box to its right bounding box
      /// neighbor if possible. If there are several right neighbor,
      /// the closest one is chosen.
      ///
      /// \return an mln::util::array. Map a bounding box to its right
      /// neighbor.
      //
      template <typename L>
      inline
      object_links<L>
      with_several_right_closest_links(const component_set<L>& comps,
				       unsigned neighb_max_distance);

      /// \overload
      template <typename L>
      inline
      object_links<L>
      with_several_right_closest_links(const component_set<L>& comps);


# ifndef MLN_INCLUDE_ONLY



      namespace internal
      {

	template <unsigned n, typename P>
	inline
	unsigned
	find_closest(const algebra::vec<n,
		                        mln::util::couple<bool, P> >& res,
		     const P& c)
	{
	  algebra::vec<3, unsigned> dist;
	  for (unsigned j = 0; j < 3; ++j)
	    dist[j] = math::abs(res[j].second().col() - c.col());

	  unsigned idx = 0;
	  for (unsigned i = 1; i < n; ++i)
	    if (dist[i] < dist[idx])
	      idx = i;

	  return idx;
	}

      } // end of namespace scribo::primitive::link::internal



      template <typename L>
      inline
      object_links<L>
      with_several_right_closest_links(const component_set<L>& comps,
				       unsigned neighb_max_distance)
      {
	mln_trace("scribo::primitive::link::with_several_right_closest_links");

	extension::fill(comps.labeled_image_(), 0);

	object_links<L>
	  link_1(comps), link_2(comps), link_3(comps), final_link(comps);

	link_1.init();
	link_2.init();
	link_3.init();

	for_all_comps(i, comps)
	{
	  //  -------
	  //  |	 a1------->
	  //  |	    |
	  //  |	    |
	  //  |	 mc------->
	  //  |	    |
	  //  |	    |
	  //  |	 a2------->
	  //  -------

	  float midcol = (comps(i).bbox().pmax().col()
			  - comps(i).bbox().pmin().col()) / 2;
	  float dmax = midcol + neighb_max_distance;

	  mln_site(L) c = comps(i).bbox().center();

	  algebra::vec<3, mln::util::couple<bool, mln_site(L)> > res;

	  // Right link from the top anchor.
	  mln_site(L) a1 = c;
	  a1.row() = comps(i).bbox().pmin().row()
	    + (c.row() - comps(i).bbox().pmin().row()) / 4;
	  res[0] = primitive::internal::find_right_link(comps, link_1,
							i, dmax, a1);

	  // Right link from the central site
	  res[1] = primitive::internal::find_right_link(comps, link_2,
							i, dmax,
							comps(i).mass_center());

	  // Right link from the bottom anchor.
	  mln_site(L) a2 = c;
	  a2.row() = comps(i).bbox().pmax().row()
	    - (c.row() - comps(i).bbox().pmin().row()) / 4;
	  res[2] = primitive::internal::find_right_link(comps, link_3,
							i, dmax, a2);

	  // Try to find the closest object.

	  unsigned closest_idx = internal::find_closest(res, c);


	  // If there exists a link and the site is not outside the
	  // image domain.
 	  if (res[closest_idx].first())
 	    final_link[i] = comps(res[closest_idx].second());
 	  else
	    final_link[i] = i;
	}

	return final_link;
      }


      template <typename L>
      inline
      object_links<L>
      with_several_right_closest_links(const component_set<L>& comps)
      {
	return with_several_right_closest_links(comps, mln_max(unsigned));
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_CLOSEST_LINKS_HH
