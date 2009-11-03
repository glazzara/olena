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

#ifndef SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_CLOSEST_LINKS_HH
# define SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_CLOSEST_LINKS_HH

/// \file
///
/// Link text bounding boxes with their right neighbor.
///
/// Merge code with primitive::link::with_single_right_link.hh
///
/// \todo Factor code with primitive::link::with_several_right_links

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/extension/fill.hh>

# include <mln/util/array.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/object_image.hh>
# include <scribo/core/macros.hh>
# include <scribo/primitive/internal/init_link_array.hh>
# include <scribo/primitive/internal/find_right_link.hh>
# include <scribo/util/text.hh>


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
      with_several_right_closest_links(const object_image(L)& objects,
				       unsigned neighb_max_distance);

      /// \overload
      template <typename L>
      inline
      object_links<L>
      with_several_right_closest_links(const object_image(L)& objects);


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
      with_several_right_closest_links(const object_image(L)& objects,
				       unsigned neighb_max_distance)
      {
	trace::entering("scribo::primitive::link::with_several_right_closest_links");

	mln_precondition(objects.is_valid());

	extension::fill(objects, 0);

	object_links<L>
	  link_1(objects, objects.nlabels().next()),
	  link_2(objects, objects.nlabels().next()),
	  link_3(objects, objects.nlabels().next()),
	  final_link(objects, objects.nlabels().next());

	primitive::internal::init_link_array(link_1);
	primitive::internal::init_link_array(link_2);
	primitive::internal::init_link_array(link_3);

	mln::util::array<mln_result(accu::center<mln_psite(L)>)>
	    mass_centers = labeling::compute(accu::meta::center(),
					     objects, objects.nlabels());


	for_all_ncomponents(i, objects.nlabels())
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

	  float midcol = (objects.bbox(i).pmax().col()
			  - objects.bbox(i).pmin().col()) / 2;
	  float dmax = midcol + neighb_max_distance;

	  mln_site(L) c = objects.bbox(i).center();

	  algebra::vec<3, mln::util::couple<bool, mln_site(L)> > res;

	  // Right link from the top anchor.
	  mln_site(L) a1 = c;
	  a1.row() = objects.bbox(i).pmin().row()
	    + (c.row() - objects.bbox(i).pmin().row()) / 4;
	  res[0] = primitive::internal::find_right_link(objects, link_1,
							i, dmax, a1);

	  // Right link from the central site
	  res[1] = primitive::internal::find_right_link(objects, link_2,
							i, dmax,
							mass_centers[i]);

	  // Right link from the bottom anchor.
	  mln_site(L) a2 = c;
	  a2.row() = objects.bbox(i).pmax().row()
	    - (c.row() - objects.bbox(i).pmin().row()) / 4;
	  res[2] = primitive::internal::find_right_link(objects, link_3,
							i, dmax, a2);

	  // Try to find the closest object.

	  unsigned closest_idx = internal::find_closest(res, c);


	  // If there exists a link and the site is not outside the
	  // image domain.
 	  if (res[closest_idx].first())
 	    final_link[i] = objects(res[closest_idx].second());
 	  else
	    final_link[i] = i;
	}

	trace::exiting("scribo::primitive::link::with_several_right_closest_links");
	return final_link;
      }


      template <typename L>
      inline
      object_links<L>
      with_several_right_closest_links(const object_image(L)& objects)
      {
	return with_several_right_closest_links(objects, mln_max(unsigned));
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_RIGHT_CLOSEST_LINKS_HH
