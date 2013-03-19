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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_CANVAS_HH
# define SCRIBO_PRIMITIVE_EXTRACT_CANVAS_HH

/// \file
///
/// Rebuild a table from its line bounding boxes.

# include <mln/core/concept/image.hh>
# include <mln/labeling/background.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>
# include <mln/value/label_8.hh>
# include <mln/util/couple.hh>

# include <scribo/table/align_lines_verticaly.hh>
# include <scribo/table/align_lines_horizontaly.hh>
# include <scribo/table/connect_vertical_lines.hh>
# include <scribo/table/connect_horizontal_lines.hh>
# include <scribo/table/repair_horizontal_lines.hh>
# include <scribo/table/repair_vertical_lines.hh>

# include <scribo/debug/save_table_image.hh>



namespace scribo
{

  namespace primitive
  {

    namespace extract
    {
      using namespace mln;

      /// Rebuild a table from its line bounding boxes.
      /*!
      ** \param[in] input_	  A binary image.
      ** \param[in] hlines_	  Vorizontal line bounding boxes.
      ** \param[in] vlines_	  vertical line bounding boxes.
      ** \param[in] max_dist_lines  The maximum distance allowed between
      **				  vertical and horizontal lines to connect
      **				  them eachother.
      **
      ** \return The canvas as a binary image. canvas lines are set to true.
      */
      template <typename I, typename V>
      mln_ch_value(I,bool)
      canvas(const Image<I>& input_,
	     const mln::util::array<box<mln_site(I)> >& hlines_,
	     const mln::util::array<box<mln_site(I)> >& vlines_,
	     unsigned max_dist_lines);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename V>
      mln_ch_value(I,bool)
      canvas(const Image<I>& input_,
	     const mln::util::array<box<mln_site(I)> >& hlines,
	     const mln::util::array<box<mln_site(I)> >& vlines,
	     unsigned max_dist_lines)
      {
	mln_trace("scribo::primitive::canvas");
	const I& input = exact(input_);

	mlc_equal(mln_value(I), bool)::check();
	mln_precondition(input.is_valid());

	typedef mln::util::array<box<mln_site(I)> > lines_t;
	mln::util::couple<lines_t, lines_t>
	  lines = mln::make::couple(hlines, vlines);

	mln::util::array<int> rows = align_lines_horizontaly(input, hlines, 5);
	mln::util::array<int> cols = align_lines_verticaly(input, vlines, 5);

	repair_vertical_lines(input, lines, 30);
	repair_horizontal_lines(input, lines, 30);

	// Connect vertical lines with horizontal lines.
	connect_vertical_lines(rows, lines, input, max_dist_lines);
	connect_horizontal_lines(cols, lines, input, max_dist_lines);

	mln_ch_value(I,bool) res;
	initialize(res, input);
	data::fill(res, false);
	for_all_elements(i, lines.first())
	  mln::draw::box(res, lines.first()[i], true);
	for_all_elements(i, lines.second())
	  mln::draw::box(res, lines.second()[i], true);

	return res;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_CANVAS_HH
