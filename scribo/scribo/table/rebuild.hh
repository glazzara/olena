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

#ifndef SCRIBO_TABLE_REBUILD_HH
# define SCRIBO_TABLE_REBUILD_HH

/// \file
///
/// \brief Rebuild a table from its line bounding boxes.

# include <mln/core/concept/image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/labeling/background.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>
# include <mln/value/label_8.hh>
# include <mln/literal/colors.hh>

# include <scribo/table/align_lines_verticaly.hh>
# include <scribo/table/align_lines_horizontaly.hh>
# include <scribo/table/connect_vertical_lines.hh>
# include <scribo/table/connect_horizontal_lines.hh>
# include <scribo/table/repair_horizontal_lines.hh>
# include <scribo/table/repair_vertical_lines.hh>

# include <scribo/debug/save_table_image.hh>



namespace scribo
{

  namespace table
  {


    /*! \brief Rebuild a table from its line bounding boxes.
    **
    ** \param[in] input	          A binary image.
    ** \param[in] vlines Component set corresponding to vertical
    **                   lines.
    ** \param[in] hlines Component set corresponding to horizontal
    **                   lines.
    ** \param[in] max_dist_lines  The maximum distance allowed between
    **				  vertical and horizontal lines to connect
    **				  them eachother.
    ** \param[out] ncells	  Store the number of cells found in the
    **				  rebuilt tables.
    **
    ** \return A couple. The first argument is a label image in which each table
    ** cell is labeled. The second argument are the aligned and connected
    ** table line bounding boxes.
    */
    template <typename I, typename L>
    mln::util::couple<L,
		      mln::util::couple<component_set<L>,
					component_set<L> > >
    rebuild(const Image<I>& input,
	    const component_set<L>& vlines,
	    const component_set<L>& hlines,
	    unsigned max_dist_lines,
	    mln_value(L)& ncells);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    mln::util::couple<L,
		      mln::util::couple<component_set<L>,
					component_set<L> > >
    rebuild(const Image<I>& input_,
	    const component_set<L>& vlines,
	    const component_set<L>& hlines,
	    unsigned max_dist_lines,
	    mln_value(L)& ncells)
    {
      trace::entering("scribo::table::rebuild");
      const I& input = exact(input_);

      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());

# ifndef SCRIBO_DEBUG
      scribo::debug::save_table_image(input, vlines, hlines,
				      literal::red, "table-raw.ppm");
# endif

      mln::util::array<int> rows = align_lines_horizontaly(input, vlines, 5);
      mln::util::array<int> cols = align_lines_verticaly(input, hlines, 5);

# ifndef SCRIBO_DEBUG
      scribo::debug::save_table_image(input, vlines, hlines,
				      literal::red, "table-aligned.ppm");
# endif

      repair_vertical_lines(input, vlines, 10);
      repair_horizontal_lines(input, hlines, 10);

# ifndef SCRIBO_DEBUG
      scribo::debug::save_table_image(input, vlines, hlines,
				      literal::red, "table-repaired.ppm");
# endif

      // Connect vertical lines with horizontal lines.
      connect_vertical_lines(rows, vlines, hlines, input, max_dist_lines);
      connect_horizontal_lines(cols, vlines, hlines, input, max_dist_lines);

# ifndef SCRIBO_DEBUG
      scribo::debug::save_table_image(input, vlines, hlines,
				      literal::red, "table-connected.ppm");
# endif


      mln_ch_value(I,bool) res;
      initialize(res, input);
      data::fill(res, false);
      for_all_comps(i, vlines)
	mln::draw::box(res, vlines(i).bbox(), true);
      for_all_comps(i, hlines)
	mln::draw::box(res, hlines(i).bbox(), true);

      L lbl = labeling::background(res, c8(), ncells);

      trace::exiting("scribo::table::rebuild");
      return mln::make::couple(lbl,
			       mln::make::couple(vlines, hlines));
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_REBUILD_HH
