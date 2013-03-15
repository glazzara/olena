// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_CELLS_HH
# define SCRIBO_PRIMITIVE_EXTRACT_CELLS_HH

/// \file
///
/// Extract canvas cells from a binary image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/util/couple.hh>
# include <mln/util/array.hh>

# include <mln/labeling/compute.hh>

# include <scribo/table/rebuild.hh>
# include <scribo/table/erase.hh>

# include <scribo/primitive/extract/lines_discontinued.hh>

# include <scribo/make/debug_filename.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;

      /// \brief Extract canvas cells from a binary image.
      /// Use arbitrary criterions.
      ///
      /// \param[in] input      A binary image.
      /// \param[in] nbh      A neighborhood.
      /// \param[in,out] label_type  Type of the labeled image.
      ///
      /// \return A list of cell bounding boxes.
      //
      template <typename I, typename N, typename V>
      mln::util::couple<mln::util::array<box<mln_site(I)> >,
			mln::util::array<box<mln_site(I)> > >
      cells(const Image<I>& input,
	    const Neighborhood<N>& nbh, const V& label_type);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename N, typename V>
      inline
      mln::util::couple<mln::util::array<box<mln_site(I)> >,
			mln::util::array<box<mln_site(I)> > >
      cells(const Image<I>& input_,
	    const Neighborhood<N>& nbh_, const V& label_type)
      {
	mln_trace("scribo::primitive::cells");

	const I& input = exact(input_);
	const N& nbh = exact(nbh_);
	mln_precondition(input.is_valid());
	mln_precondition(nbh.is_valid());
	mlc_equal(mln_value(I), bool)::check();

	typedef mln::util::array< box<mln_site(I)> > boxarray_t;

	V ncells;
	win::line<mln_grid(I::site), 0, mln_coord(I::site)> vline(51);
	win::line<mln_grid(I::site), 1, mln_coord(I::site)> hline(51);
	boxarray_t
	  vlines = primitive::extract::lines_discontinued(input, nbh,
							  ncells, vline, 6),
	  hlines = primitive::extract::lines_discontinued(input, nbh,
							  ncells, hline, 6);

	typedef mln_ch_value(I,V) cells_ima_t;

	cells_ima_t
	  cells = scribo::table::rebuild(input, mln::make::couple(vlines,hlines),
					 30, ncells).first();
	mln::util::array<box<mln_site(I)> >
	  cellbboxes = labeling::compute(accu::meta::shape::bbox(), cells, ncells);

	return cellbboxes;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_CELLS_HH
