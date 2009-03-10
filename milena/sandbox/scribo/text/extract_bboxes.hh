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

/// \file scribo/text/grouping/group_with_single_link.hh
///
/// Extract text bounding boxes from a binary image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>
# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>
# include <mln/util/array.hh>

# include <scribo/core/component_bboxes.hh>

namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// Extract text bounding boxes from a binary image.
    ///
    /// \param[in] input_ A binary image.
    ///
    /// \return an array of bounding boxes. The first bounding box is
    /// the background's.
    template <typename I, typename N, typename V>
    inline
    util::array< box<mln_site(I)> >
    extract_bboxes(const Image<I>& input_,
		   const Neighborhood<N>& nbh, V& nbboxes);

# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename N, typename V>
    inline
    util::array< box<mln_site(I)> >
    extract_bboxes(const Image<I>& input_,
		   const Neighborhood<N>& nbh, V& nbboxes)
    {
      trace::entering("scribo::text::extract_bboxes");

      const I& input = exact(input_);

      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(input.is_valid());

      typedef util::array< box<mln_site(I)> > bboxes_t;
      bboxes_t bboxes = component_bboxes(input, nbh, nbboxes);

      trace::exiting("scribo::text::extract_bboxes");
      return bboxes;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_EXTRACT_BBOXES_HH
