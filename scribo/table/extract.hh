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

#ifndef SCRIBO_TABLE_EXTRACT_HH
# define SCRIBO_TABLE_EXTRACT_HH

/// \file
///
/// Extract tables from a binary image.
/// Use arbitrary criterions.

# include <mln/core/concept/image.hh>
# include <mln/util/couple.hh>
# include <mln/util/array.hh>

# include <mln/io/ppm/all.hh>
# include <mln/labeling/colorize.hh>

# include <scribo/core/object_image.hh>
# include <scribo/table/rebuild.hh>
# include <scribo/table/erase.hh>
# include <scribo/extract/primitive/lines_h_pattern.hh>
# include <scribo/extract/primitive/lines_v_pattern.hh>
# include <scribo/extract/primitive/objects.hh>

# include <scribo/debug/save_bboxes_image.hh>

namespace scribo
{

  namespace table
  {
    using namespace mln;

    /// Extract tables from a binary image.
    /// Use arbitrary criterions.
    /*
    ** \param[in] input_      A binary image.
    ** \param[in,out] ncells  The number of cells.
    **
    ** \return a couple. The first element is an image containing the
    ** cells components. The second element is a couple of vertical and
    ** horizontal table lines.
    */
    template <typename I, typename V>
    mln::util::couple<mln_ch_value(I,V),
		      mln::util::couple<mln::util::array<box<mln_site(I)> >,
				   mln::util::array<box<mln_site(I)> > > >
    extract(const Image<I>& input_, V& ncells);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename V>
    inline
    mln::util::couple<mln_ch_value(I,V),
		      mln::util::couple<mln::util::array<box<mln_site(I)> >,
				   mln::util::array<box<mln_site(I)> > > >
    extract(const Image<I>& input_, V& ncells)
    {
      trace::entering("scribo::table::extract");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mlc_equal(mln_value(I), bool)::check();

      image2d<bool>
	bhlines = scribo::extract::primitive::lines_h_pattern(input, 51),
	bvlines = scribo::extract::primitive::lines_v_pattern(input, 51);

      V nhlines, nvlines;
      object_image(mln_ch_value(I,V))
	hlines = scribo::extract::primitive::objects(bhlines, c8(), nhlines),
	vlines = scribo::extract::primitive::objects(bvlines, c8(), nvlines);

      typedef mln::util::couple<mln_ch_value(I,V),
				mln::util::couple<mln::util::array<box<mln_site(I)> >,
					     mln::util::array<box<mln_site(I)> > > >
	      tables_t;
      tables_t tables
	= scribo::table::rebuild(input,
				 mln::make::couple(vlines.bboxes(), hlines.bboxes()),
				 30, ncells);

      trace::exiting("scribo::table::extract");
      return tables;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_EXTRACT_HH
