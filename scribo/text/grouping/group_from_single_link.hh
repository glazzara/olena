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

#ifndef SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH
# define SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH

/// \file scribo/text/grouping/group_from_single_link.hh
///
/// Link lines of text with their neighbor line if they have one.

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/fun/i2v/array.hh>

# include <mln/accu/bbox.hh>

# include <mln/util/array.hh>

# include <scribo/text/grouping/internal/find_root.hh>

# include <scribo/core/macros.hh>
# include <scribo/util/text.hh>


namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// Link lines of text with their neighbor line if they have one.
      /*!
      ** \param[in] text       The lines of text.
      ** \param[in] link_array The neighbor line of each line.
      **
      ** \return The grouped and non-grouped lines of text.
      */
      template <typename I>
      scribo::util::text<I>
      group_from_single_link(const scribo::util::text<I>& text,
			     const mln::util::array<unsigned>& link_array);

# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      scribo::util::text<I>
      group_from_single_link(const scribo::util::text<I>& text,
			     const mln::util::array<unsigned>& link_array)
      {
	trace::entering("scribo::text::grouping::group_from_single_link");

	mln_precondition(text.is_valid());
	mln_precondition(link_array.nelements() == text.nbboxes().next());
	mln_precondition(link_array.nelements() == text.bboxes().nelements());

	mln::util::array<unsigned> parent_array = link_array;
	for_all_components(i, parent_array)
	  internal::find_root(parent_array, i);

	mln::util::array< accu::bbox<mln_site(I)> > tboxes;
	tboxes.resize(text.nbboxes().next());
	for_all_components(i, text.bboxes())
	  tboxes[parent_array[i]].take(text.bbox(i));

	mln::util::array< box<mln_site(I)> > result;
	// component 0, the background, has an invalid box.
	result.append(box<mln_site(I)>());
	for_all_components(i, tboxes)
	  if (tboxes[i].is_valid())
	    result.append(tboxes[i]);

	I lbl = labeling::relabel(text.label_image(), text.nbboxes(),
				  convert::to<fun::i2v::array<mln_value(I)> >(parent_array));
	mln_value(I) new_nbboxes = result.nelements() - 1;

	trace::exiting("scribo::text::grouping::group_from_single_link");
	/// FIXME: construct a new util::text from the old one.
	return scribo::make::text(result, lbl, new_nbboxes);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH
