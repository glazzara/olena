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

#ifndef SCRIBO_TABLE_EXTRACT_HH
# define SCRIBO_TABLE_EXTRACT_HH

/// \file scribo/table/extract.hh
///
/// Extract tables from a binary image.
/// Use arbitrary criterions.

# include <mln/core/concept/image.hh>
# include <mln/util/couple.hh>
# include <mln/util/array.hh>

# include <scribo/table/rebuild.hh>
# include <scribo/table/erase.hh>
# include <scribo/table/extract_lines_with_rank.hh>

# include <scribo/make/debug_filename.hh>


namespace scribo
{

  namespace table
  {

    template <typename I, typename V>
    mln_ch_value(I,V)
    extract(const Image<I>& input_, V& ncells);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename V>
    inline
    mln_ch_value(I,V)
    extract(const Image<I>& input_, V& ncells)
    {
      trace::entering("scribo::table::extract");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mlc_equal(mln_value(I), bool)::check();

      typedef util::array< box<mln_site(I)> > boxarray_t;
      typedef util::couple<boxarray_t, boxarray_t> tblboxes_t;

      win::line<mln_grid(I::site), 0, mln_coord(I::site)> vline(51);
      win::line<mln_grid(I::site), 1, mln_coord(I::site)> hline(51);
      tblboxes_t lineboxes
	= table::extract_lines_with_rank(input, c8(), V(),
					 vline, hline, 6, 6);

      image2d<V> tables
	= scribo::table::rebuild(input, lineboxes, 30, ncells);

      trace::exiting("scribo::table::extract");
      return tables;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_EXTRACT_HH
