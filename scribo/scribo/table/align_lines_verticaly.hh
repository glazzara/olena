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

#ifndef SCRIBO_TABLE_ALIGN_LINES_VERTICALY_HH
# define SCRIBO_TABLE_ALIGN_LINES_VERTICALY_HH

/// \file
///
/// Align line bounding boxes verticaly.


# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/max_col.hh>

# include <mln/util/array.hh>

# include <scribo/table/internal/align_lines.hh>



namespace scribo
{

  namespace table
  {

    using namespace mln;

    /// Align line bounding boxes verticaly.
    ///
    /// \param[in]	input	     Image from which the line bboxes are
    ///				     extracted from.
    /// \param[in, out] line_bboxes vertical lines bounding boxes.
    /// \param[in]	max_alignment_diff max space between two lines to
    ///					   consider they are potentialy on the
    ///					   same line.
    ///
    /// \return A list of the resulting aligned cols. Each integer is actually
    ///		a col number.
    template <typename I>
    mln::util::array<int>
    align_lines_verticaly(const Image<I>& input,
			  mln::util::array<box<mln_site(I)> >& line_bboxes,
			  unsigned max_alignment_diff);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln::util::array<int>
    align_lines_verticaly(const Image<I>& input,
			  mln::util::array<box<mln_site(I)> >& line_bboxes,
			  unsigned max_alignment_diff)
    {
      mln_trace("scribo::table::align_lines_verticaly");

      mln_precondition(exact(input).is_valid());
      mln::util::array<int> res = internal::align_lines(geom::ncols(input),
						   geom::min_col(input),
						   geom::max_col(input),
						   line_bboxes, 1,
						   max_alignment_diff);

      return res;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_ALIGN_LINES_VERTICALY_HH
