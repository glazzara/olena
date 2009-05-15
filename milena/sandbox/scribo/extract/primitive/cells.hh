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

#ifndef SCRIBO_EXTRACT_PRIMITIVE_CELLS_HH
# define SCRIBO_EXTRACT_PRIMITIVE_CELLS_HH

/// \file scribo/primitive/cells.hh
///
/// Extract canvas cells from a binary image.

# include <mln/core/concept/image.hh>

# include <mln/accu/bbox.hh>

# include <mln/util/couple.hh>
# include <mln/util/array.hh>

# include <mln/labeling/compute.hh>

# include <scribo/table/rebuild.hh>
# include <scribo/table/erase.hh>

# include <scribo/primitive/discontinued_lines.hh>

# include <scribo/make/debug_filename.hh>


namespace scribo
{

  namespace extract
  {

    namespace primitive
    {

      /// Extract canvas cells from a binary image.
      /// Use arbitrary criterions.
      /*
      ** \param[in] input_      A binary image.
      ** \param[in,out] ncells  Will store the number of cells found.
      **
      ** \return A list of cell bounding boxes.
      */
      template <typename I, typename V>
      util::couple<util::array<box<mln_site(I)> >
      cells(const Image<I>& input_,
	    const Neighborhood<N>& nbh_, const V& label_type);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename N, typename V>
      inline
      util::couple<util::array<box<mln_site(I)> >
      cells(const Image<I>& input_,
	    const Neighborhood<N>& nbh_, const V& label_type)
      {
	trace::entering("scribo::primitive::cells");

	const I& input = exact(input_);
	const N& nbh = exact(nbh_);
	mln_precondition(input.is_valid());
	mln_precondition(nbh.is_valid());
	mlc_equal(mln_value(I), bool)::check();

	typedef util::array< box<mln_site(I)> > boxarray_t;

	V ncells;
	win::line<mln_grid(I::site), 0, mln_coord(I::site)> vline(51);
	win::line<mln_grid(I::site), 1, mln_coord(I::site)> hline(51);
	boxarray_t
	  vlines = primitive::discontinued_lines(input, nbh, ncells, vline, 6),
	  hlines = primitive::discontinued_lines(input, nbh, ncells, hline, 6);

	typedef mln_ch_value(I,V) cells_ima_t;

	cells_ima_t
	  cells = scribo::table::rebuild(input, make::couple(vlines,hlines),
					 30, ncells).first();
	util::array<box<mln_site(I)> >
	  cellbboxes = labeling::compute(accu::meta::bbox(), cells, ncells);

	trace::exiting("scribo::primitive::cells");
	return cellbboxes;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::extract::primitive

  } // end of namespace scribo::extract

} // end of namespace scribo

#endif // ! SCRIBO_EXTRACT_PRIMITIVE_CELLS_HH
