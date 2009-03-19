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

#ifndef SCRIBO_TABLE_REBUILD_HH
# define SCRIBO_TABLE_REBUILD_HH

/// \file scribo/table/rebuild.hh
///
/// Rebuild a table from its line bounding boxes.

# include <mln/core/concept/image.hh>
# include <mln/labeling/background.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>
# include <mln/value/label_8.hh>

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

    template <typename I, typename V>
    mln_ch_value(I,V)
    rebuild(const Image<I>& in_,
	    const util::couple<util::array<box<mln_site(I)> >,
			       util::array<box<mln_site(I)> > >& lineboxes,
	    unsigned max_dist_lines,
	    V& ncells);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename V>
    mln_ch_value(I,V)
    rebuild(const Image<I>& in_,
	    const util::couple<util::array<box<mln_site(I)> >,
			       util::array<box<mln_site(I)> > >& lineboxes,
	    unsigned max_dist_lines,
	    V& ncells)
    {
      trace::entering("scribo::table::rebuild");
      const I& in = exact(in_);

      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(in.is_valid());

      util::couple<util::array<box<mln_site(I)> >,
		   util::array<box<mln_site(I)> > > tblboxes = lineboxes;

//# ifndef SCRIBO_NDEBUG
//      scribo::debug::save_table_image(in, tblboxes,
//				      literal::red, "table.ppm");
//# endif
      scribo::debug::save_table_image(in, tblboxes,
				      literal::red, "table-raw.ppm");

      util::array<int> rows = align_lines_horizontaly(in, tblboxes.second(), 5);
      util::array<int> cols = align_lines_verticaly(in, tblboxes.first(), 5);

# ifndef SCRIBO_NDEBUG
      scribo::debug::save_table_image(in, tblboxes,
				      literal::red, "table-aligned.ppm");
# endif

      repair_vertical_lines(in, tblboxes, 30);
      repair_horizontal_lines(in, tblboxes, 30);

# ifndef SCRIBO_NDEBUG
      scribo::debug::save_table_image(in, tblboxes,
				      literal::red, "table-repaired.ppm");
# endif

      // Connect vertical lines with horizontal lines.
      connect_vertical_lines(rows, tblboxes, in, max_dist_lines);
      connect_horizontal_lines(cols, tblboxes, in, max_dist_lines);

# ifndef SCRIBO_NDEBUG
      scribo::debug::save_table_image(in, tblboxes,
				      literal::red, "table-connected.ppm");
# endif


      mln_ch_value(I,bool) res;
      initialize(res, in);
      data::fill(res, false);
      for_all_components(i, tblboxes.first())
	mln::draw::box(res, tblboxes.first()[i], true);
      for_all_components(i, tblboxes.second())
	mln::draw::box(res, tblboxes.second()[i], true);

      mln_ch_value(I,V) lbl = labeling::background(res, c8(), ncells);

//# ifndef SCRIBO_NDEBUG
//      scribo::debug::save_table_image(in, tblboxes,
//				      literal::red, "table-connected.ppm");
//# endif

      trace::exiting("scribo::table::rebuild");
      return lbl;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_REBUILD_HH
