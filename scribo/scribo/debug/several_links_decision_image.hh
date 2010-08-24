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

#ifndef SCRIBO_DEBUG_SEVERAL_LINKS_DECISION_IMAGE_HH
# define SCRIBO_DEBUG_SEVERAL_LINKS_DECISION_IMAGE_HH

/// \file
///
/// Save a color image showing the difference between to object groups.

# include <mln/core/concept/image.hh>
# include <mln/accu/center.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>
# include <mln/util/array.hh>

# include <scribo/core/object_groups.hh>
# include <scribo/draw/bounding_boxes.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;

    /// FIXME: DOC!
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    several_links_decision_image(const Image<I>& input_,
				 const object_links<L>& links,
				 const object_links<L>& filtered_links);


# ifndef MLN_INCLUDE_ONLY



    namespace internal
    {

      template <typename L, typename I>
      bool
      draw_line(const L& objects, I& output,
		unsigned link,
		const mln_site(L)& p1, const mln_site(L)& p2,
		const mln_value(I)& value)
      {

	if (objects.domain().has(p2))
	  if (objects(p2) == link)
	  {
	    mln::draw::line(output, p1, p2, value);
	    return true;
	  }
	return false;
      }

    }



    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    several_links_decision_image(const Image<I>& input_,
				 const object_links<L>& links,
				 const object_links<L>& filtered_links)
    {
      trace::entering("scribo::debug::several_links_decision_image");
      const I& input = exact(input_);

      const component_set<L>& comps = links.components();

      mln_precondition(input.is_valid());
      mln_precondition(links.is_valid());
      mln_precondition(filtered_links.is_valid());

      /// Fixme: check that objects has been computed from input.

      image2d<value::rgb8>
	links_decision_image = data::convert(value::rgb8(), input);

      for_all_comps(c, comps)
	mln::draw::box(links_decision_image, comps(c).bbox(), literal::blue);

      for_all_links(l, links)
      {
	if (links(l) != l)
	{
	  value::rgb8 value = literal::green;
	  if (links(l) != filtered_links(l))
	    value = literal::red;

	  mln_site(L) c = comps(l).bbox().pcenter();

	  // Right link from the top anchor.
	  mln_site(L) a1 = c;
	  a1.row() = comps(l).bbox().pmin().row()
	    + (c.row() - comps(l).bbox().pmin().row()) / 4;

	  // Right link from the central site
	  mln_site(I) p1 = comps(l).mass_center();

	  // Right link from the bottom anchor.
	  mln_site(L) a2 = c;
	  a2.row() = comps(l).bbox().pmax().row()
	    - (c.row() - comps(l).bbox().pmin().row()) / 4;

	  mln_site(L)
	    a1_bak = a1,
	    a2_bak = a2;

	  const L& lbl_ima = comps.labeled_image();

	  mln_site(L) tmp;
	  while(lbl_ima.domain().has(a1)
		|| lbl_ima.domain().has(a2)
		|| lbl_ima.domain().has(p1))
	  {
	    if (internal::draw_line(lbl_ima, links_decision_image, links(l),
				    a1_bak, a1, value))
	      break;
	    else
	      ++a1.col();

	    if (internal::draw_line(lbl_ima, links_decision_image, links(l),
				    comps(l).mass_center(), p1, value))
	      break;
	    else
	      ++p1.col();

	    if (internal::draw_line(lbl_ima, links_decision_image, links(l),
				    a2_bak, a2, value))
	      break;
	    else
	      ++a2.col();

	  }
	}
      }

      trace::exiting("scribo::debug::several_links_decision_image");
      return links_decision_image;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_SEVERAL_LINKS_DECISION_IMAGE_HH
