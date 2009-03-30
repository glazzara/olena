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


#ifndef SCRIBO_TABLE_ALIGN_LINES_HORIZONTALY_HH
# define SCRIBO_TABLE_ALIGN_LINES_HORIZONTALY_HH

/// \file scribo/table/align_lines_horizontaly.hh
///
/// Align line bounding boxes horizontaly.


# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/geom/nrows.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/max_row.hh>

# include <mln/util/array.hh>

# include <scribo/table/internal/align_lines.hh>


namespace scribo
{

  namespace table
  {

    using namespace mln;

    /// Align line bounding boxes horizontaly.
    ///
    /// \param[in]	input	     Image from which the line bboxes are
    ///				     extracted from.
    /// \param[in, out] line_bboxes horizontal lines bounding boxes.
    /// \param[in]	max_alignment_diff max space between two lines to
    ///					   consider they are potentialy on the
    ///					   same line.
    ///
    /// \return A list of the resulting aligned rows. Each integer is actually
    ///		a row number.
    template <typename I>
    util::array<int>
    align_lines_horizontaly(const Image<I>& input,
			    util::array<box<mln_site(I)> >& line_bboxes,
			    unsigned max_alignment_diff);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    util::array<int>
    align_lines_horizontaly(const Image<I>& input,
			    util::array<box<mln_site(I)> >& line_bboxes,
			    unsigned max_alignment_diff)
    {
      trace::entering("scribo::table::align_lines_horizontaly");

      mln_precondition(exact(input).is_valid());
      util::array<int> res =  internal::align_lines(geom::nrows(input),
						    geom::min_row(input),
						    geom::max_row(input),
						    line_bboxes, 0,
						    max_alignment_diff);

      trace::exiting("scribo::table::align_lines_horizontaly");
      return res;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_ALIGN_LINES_HORIZONTALY_HH
