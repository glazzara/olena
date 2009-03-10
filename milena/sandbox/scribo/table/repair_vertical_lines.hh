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

#ifndef SCRIBO_TABLE_REPAIR_VERTICAL_LINES_HH
# define SCRIBO_TABLE_REPAIR_VERTICAL_LINES_HH

/// \file scribo/table/repair_vertical_lines.hh
///
/// Repair vertical lines which have small discontinuities.

# include <mln/core/concept/image.hh>
# include <mln/util/couple.hh>
# include <mln/util/array.hh>

# include <scribo/table/internal/repair_lines.hh>

namespace scribo
{

  namespace table
  {


    /// Repair vertical lines which have small discontinuities.
    /*!
     *
     *	  |	  | \n
     *	  |	  | \n
     *	     ==>  | \n
     *	  |	  | \n
     *	  |	  | \n
     *
     *
     *	\param[in] input	      Image from which the table bounding
     *				      boxes are extracted.
     *	\param[in,out] tablesboxes    Table line bounding boxes.
     *	\param[in] max_discontinuity  Repair discontinuity which are smaller
     *				      than this value.
     */
    template <typename I>
    void
    repair_vertical_lines(const Image<I>& input,
			  util::couple<util::array<box<mln_site(I)> >,
				       util::array<box<mln_site(I)> > >& tblboxes,
			  unsigned max_discontinuity);

# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void
    repair_vertical_lines(const Image<I>& input,
			  util::couple<util::array<box<mln_site(I)> >,
				       util::array<box<mln_site(I)> > >& tblboxes,
			  unsigned max_discontinuity)
    {
      trace::entering("scribo::table::repair_vertical_lines");
      mln_precondition(exact(input).is_valid());

      internal::repair_lines<0>(input, tblboxes.first(), max_discontinuity);

      trace::exiting("scribo::table::repair_vertical_lines");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::table

} // end of namespace scribo


#endif // ! SCRIBO_TABLE_REPAIR_VERTICAL_LINES_HH
