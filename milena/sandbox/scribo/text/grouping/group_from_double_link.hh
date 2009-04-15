// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


#ifndef SCRIBO_TEXT_GROUPING_GROUP_FROM_DOUBLE_LINK_HH
# define SCRIBO_TEXT_GROUPING_GROUP_FROM_DOUBLE_LINK_HH

/// \file scribo/text/grouping/group_from_double_link.hh
///
/// Link text bounding boxes with their left neighbor.

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>

# include <mln/accu/bbox.hh>

# include <mln/util/array.hh>

# include <scribo/core/macros.hh>

# include <scribo/util/text.hh>

# include <scribo/text/grouping/internal/find_root.hh>
# include <scribo/text/grouping/internal/is_link_valid.hh>


namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// Group text bounding boxes from left and right links and validate
      /// These links. A link must exist in both ways to be validated.
      /*!
      ** \param[in] text The   Lines of text.
      ** \param[in] left_link  The left neighbor of each line of text.
      ** \param[in] right_link The right neighbor of each line of text.
      **
      ** \return New lines of text. Some of the lines of \p text may have
      **	 been grouped.
      */
      template <typename I>
      scribo::util::text<I>
      group_from_double_link(const scribo::util::text<I>& text,
			     const mln::util::array<unsigned>& left_link,
			     const mln::util::array<unsigned>& right_link);

# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      scribo::util::text<I>
      group_from_double_link(const scribo::util::text<I>& text,
			     const mln::util::array<unsigned>& left_link,
			     const mln::util::array<unsigned>& right_link)
      {
	trace::entering("scribo::text::grouping::group_from_double_link");

	mln_precondition(left_link.nelements() == right_link.nelements());

	mln::util::array< accu::bbox<mln_site(I)> > tboxes;
	tboxes.resize(text.bboxes().nelements());

	mln::util::array<unsigned> parent(left_link.nelements());
	internal::init_link_array(parent);
	for_all_components(i, text.bboxes())
	{
	  mln::util::couple<bool, unsigned>
	    nbh = internal::is_link_valid(left_link, right_link, i);
	  if (nbh.first())
	  {
	    unsigned par = internal::find_root(parent, nbh.second());
	    if (par < i)
	      parent[par] = i;
	    else
	      parent[i] = par;
	  }
	}

	for_all_elements(i, parent)
	for (unsigned i = parent.nelements() - 1; i < parent.nelements(); --i)
	{
	  parent[i] = parent[parent[i]];
	  tboxes[parent[i]].take(text.bbox(i));
	}

	fun::i2v::array<unsigned> f;
	convert::from_to(parent, f);

	// Update bounding boxes information.
	mln::util::array< box<mln_site(I)> > bresult;
	// component 0, the background, has an invalid box.
	bresult.append(box<mln_site(I)>());
	for_all_components(i, tboxes)
	  if (tboxes[i].is_valid())
	    bresult.append(tboxes[i]);

	// Update label image.
	mln_value(I) new_nbboxes;
	I new_lbl = labeling::relabel(text.label_image(),
				      text.nbboxes(),
				      mln::make::relabelfun(f, text.nbboxes(),
							    new_nbboxes));

	mln_assertion(bresult.nelements() == new_nbboxes.next());

	/// FIXME: construct a new util::text from the old one.
	scribo::util::text<I> result(bresult, new_lbl, new_nbboxes);

	trace::exiting("scribo::text::grouping::group_from_double_link");
	return result;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_FROM_DOUBLE_LINK_HH
