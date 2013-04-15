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

#ifndef SCRIBO_TABLE_CONNECT_HORIZONTAL_LINES_HH
# define SCRIBO_TABLE_CONNECT_HORIZONTAL_LINES_HH

/// \file
///
/// Connect horizontal lines with the new aligned columns.
///
/// \todo do not modify arguments but return a new value.

# include <mln/core/concept/image.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <scribo/table/internal/connect_lines.hh>

#include <mln/value/int_u16.hh>
#include <mln/draw/line.hh>
#include <mln/io/pgm/all.hh>
namespace scribo
{

  namespace table
  {

    using namespace mln;

    /// Connect horizontal lines with the new aligned columns.
    ///
    /// \param[in]	aligned_cols  a list of new aligned cols.
    /// \param[in,out]	tablebboxes   the vertical and horizontal lines
    ///				      bounding boxes.
    /// \param[in]	input	      The image from where the lines are
    ///				      extracted.
    /// \param[in]	max_distance  max distance allowed between a vertical
    ///				      and horizontal lines.
    template <typename I>
    void
    connect_horizontal_lines(const mln::util::array<int>& aligned_cols,
			     mln::util::couple<mln::util::array<box<mln_site(I)> >,
					  mln::util::array<box<mln_site(I)> > >& tablebboxes,
			     const Image<I>& input,
			     unsigned max_distance);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    void
    connect_horizontal_lines(const mln::util::array<int>& aligned_cols,
			     mln::util::couple<mln::util::array<box<mln_site(I)> >,
					  mln::util::array<box<mln_site(I)> > >& tablebboxes,
			     const Image<I>& input,
			     unsigned max_distance)
    {
      mln_trace("scribo::table::connect_horizontal_lines");
      mln_precondition(exact(input).is_valid());

      internal::connect_lines(aligned_cols, tablebboxes.second(),
			      1, exact(input).ncols(), max_distance);

    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::table

} // end of namespace scribo


#endif // ! SCRIBO_TABLE_CONNECT_HORIZONTAL_LINES_HH
