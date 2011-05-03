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

#ifndef SCRIBO_DEBUG_DECISION_IMAGE_HH
# define SCRIBO_DEBUG_DECISION_IMAGE_HH

/// \file
///
/// Save a color image showing the difference between to object groups.

# include <mln/core/concept/image.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>
# include <mln/util/array.hh>
# include <mln/draw/box.hh>
# include <mln/draw/line.hh>

# include <scribo/core/tag/anchor.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/primitive/link/internal/compute_anchor.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /*! \brief Save a color image showing the difference between to
        object groups.

	\input[in] input An image. It's value type must be convertible
                         towards rgb8.
	\input[in] groups          Object groups information.
	\input[in] filtered_groups A copy of \p groups which have been
	                           filtered.

	\return A color image. Components part of a validated group are
	        drawn in green with their bounding box. Otherwise,
	        they are drawn in red.
     */
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    decision_image(const Image<I>& input_,
		   const object_groups<L>& groups,
		   const object_groups<L>& filtered_groups,
		   anchor::Type anchor);

    /*! \brief Save a color image showing the difference between to
        object links.

	\input[in] input An image. It's value type must be convertible
                         towards rgb8.
	\input[in] links          Object links information.
	\input[in] filtered_links A copy of \p links which have been
	                           filtered.

	\return A color image. Non filtered links are drawn in
	        green. Others are drawn in red.
     */
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    decision_image(const Image<I>& input_,
		   const object_links<L>& links,
		   const object_links<L>& filtered_links,
		   anchor::Type anchor);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    decision_image(const Image<I>& input_,
		   const object_groups<L>& groups,
		   const object_groups<L>& filtered_groups)
    {
      trace::entering("scribo::debug::decision_image");
      const I& input = exact(input_);

      const component_set<L>& components = groups.components();

      mln_precondition(input.is_valid());
      mln_precondition(groups.is_valid());
      mln_precondition(filtered_groups.is_valid());
      /// Fixme: check that components has been computed from input.

      image2d<value::rgb8>
	decision_image = data::convert(value::rgb8(), input);

      for (unsigned i = 1; i < groups.size(); ++i)
	if (groups(i) != filtered_groups(i))
	  mln::draw::box(decision_image, components(i).bbox(),
			 literal::red);
	else
	  mln::draw::box(decision_image, components(i).bbox(),
			 literal::green);

      trace::exiting("scribo::debug::decision_image");
      return decision_image;
    }


    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    decision_image(const Image<I>& input_,
		   const object_links<L>& links,
		   const object_links<L>& filtered_links,
		   anchor::Type anchor)
    {
      trace::entering("scribo::debug::decision_image");
      const I& input = exact(input_);

      const component_set<L>& comps = links.components();

      mln_precondition(input.is_valid());
      mln_precondition(links.is_valid());
      mln_precondition(filtered_links.is_valid());
      /// Fixme: check that components has been computed from input.

      image2d<value::rgb8>
	decision_image = data::convert(value::rgb8(), input);

      for_all_comps(i, comps)
	mln::draw::box(decision_image, comps(i).bbox(), literal::blue);

      for_all_links(i, links)
      {

	if (links.is_linked(i))
	{
	  mln_site(L)
	    p1 = primitive::link::internal::compute_anchor(comps, i, anchor),
	    p2 = primitive::link::internal::compute_anchor(comps, links(i),
							   anchor);

	  value::rgb8 value = literal::green;
	  if (links(i) != filtered_links(i))
	    value = literal::red;
	  mln::draw::line(decision_image, p1, p2, value);
	}
      }

      trace::exiting("scribo::debug::decision_image");
      return decision_image;
    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_DECISION_IMAGE_HH
