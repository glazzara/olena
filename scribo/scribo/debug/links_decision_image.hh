// Copyright (C) 2009, 2010, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_DEBUG_LINKS_DECISION_IMAGE_HH
# define SCRIBO_DEBUG_LINKS_DECISION_IMAGE_HH

/// \file
///
/// Save a color image showing the difference between to object groups.

# include <mln/core/concept/image.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>
# include <mln/draw/box.hh>
# include <mln/draw/line.hh>

# include <scribo/core/object_links.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;

    /// FIXME: DOC!
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    links_decision_image(const Image<I>& input_,
			 const object_links<L>& links,
			 const object_links<L>& filtered_links);

# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    links_decision_image(const Image<I>& input_,
			 const object_links<L>& links,
			 const object_links<L>& filtered_links)
    {
      trace::entering("scribo::debug::links_decision_image");
      const I& input = exact(input_);

      const component_set<L>& comps = links.components();

      mln_precondition(input.is_valid());
      mln_precondition(comps.is_valid());
      mln_precondition(links.is_valid());
      mln_precondition(filtered_links.is_valid());
      mln_precondition(links.size() == filtered_links.size());
      /// FIXME: Check that objects has been computed from input.

      mln_ch_value(I,value::rgb8)
	links_decision_image = data::convert(value::rgb8(), input);

      for_all_comps(i, comps)
	mln::draw::box(links_decision_image, comps(i).bbox(), literal::blue);

      for_all_links(i, links)
	if (links(i) != i)
	{
	  value::rgb8 value = literal::green;
	  if (links(i) != filtered_links(i))
	    value = literal::red;

	  mln_site(I)
	    p1 = comps(i).mass_center(),
	    p2 = comps(links(i)).mass_center();

	  mln::draw::line(links_decision_image, p1, p2, value);
	}

      trace::exiting("scribo::debug::links_decision_image");
      return links_decision_image;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_LINKS_DECISION_IMAGE_HH
