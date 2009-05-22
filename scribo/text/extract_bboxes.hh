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


#ifndef SCRIBO_TEXT_GROUPING_EXTRACT_BBOXES_HH
# define SCRIBO_TEXT_GROUPING_EXTRACT_BBOXES_HH

/// \file scribo/text/extract_bboxes.hh
///
/// Extract text bounding boxes from a binary image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>
# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>
# include <mln/util/array.hh>

# include <scribo/core/component_bboxes.hh>
# include <scribo/util/text.hh>
# include <scribo/make/text.hh>

namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// Extract text bounding boxes from a binary image.
    ///
    /// \param[in]     input_  A binary imag.
    /// \param[in]     nbh_    The neighborhood used for labeling.
    /// \param[in,out] nbboxes Will hold the number of bounding boxes at
    ///			       the end of the routine.
    ///
    /// \return an array of bounding boxes. The first bounding box is
    /// the background's.
    template <typename I, typename N, typename V>
    scribo::util::text<mln_ch_value(I,V)>
    extract_bboxes(const Image<I>& input_,
		   const Neighborhood<N>& nbh_, V& nbboxes);

# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename N, typename V>
    inline
    scribo::util::text<mln_ch_value(I,V)>
    extract_bboxes(const Image<I>& input_,
		   const Neighborhood<N>& nbh_, V& nbboxes)
    {
      trace::entering("scribo::text::extract_bboxes");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      typedef mln::util::array< box<mln_site(I)> > bboxes_t;
      typedef mln::util::couple<bboxes_t, mln_ch_value(I,V)> bboxes_and_lbl_t;
      bboxes_and_lbl_t bboxes_and_lbl = component_bboxes(input, nbh, nbboxes);

      trace::exiting("scribo::text::extract_bboxes");
      return scribo::make::text(bboxes_and_lbl.first(),
				bboxes_and_lbl.second(), nbboxes);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_EXTRACT_BBOXES_HH
