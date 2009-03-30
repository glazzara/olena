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


#ifndef SCRIBO_TABLE_EXTRACT_LINES_WITH_OPENING_HH
# define SCRIBO_TABLE_EXTRACT_LINES_WITH_OPENING_HH

/// \file scribo/table/extract_lines_with_opening.hh
///
/// Extract table lines using a morphological opening filter.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>

# include <mln/morpho/erosion.hh>

# include <mln/accu/bbox.hh>

# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_bboxes.hh>

namespace scribo
{

  namespace table
  {

    using namespace mln;

    /// Find table bboxes thanks to a opening filter.
    /*!
     *
     * \param[in] input_ A binary image.
     * \param[in] nbh_ The neighborhood used for labeling image components.
     * \param[in] label_type Type used for labeling.
     * \param[in] vwin_ Window used to extract the vertical lines in a morphological
     *		       opening
     * \param[in] hwin_ Window used to extract the horizontal lines in a morphological
     *		       opening
     *
     * \return pair of array of bounding boxes. The first array holds the
     *		vertical lines bounding boxes and the second one the
     *		horizontal lines bounding boxes.
     */
    template <typename I, typename N, typename V, typename VW, typename HW>
    util::couple<util::array<box<mln_site(I)> >,
		 util::array<box<mln_site(I)> > >
    extract_lines_with_opening(const Image<I>& input_,
			       const Neighborhood<N>& nbh_, const V& label_type,
			       const Window<VW>& vwin_, const Window<HW>& hwin_);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename V, typename VW, typename HW>
    inline
    util::couple<util::array<box<mln_site(I)> >,
		 util::array<box<mln_site(I)> > >
    extract_lines_with_opening(const Image<I>& input_,
			    const Neighborhood<N>& nbh_, const V& label_type,
			    const Window<VW>& vwin_, const Window<HW>& hwin_)
    {
      trace::entering("scribo::table::extract_lines_with_opening");

      mlc_equal(mln_value(I),bool)::check();
      mlc_is_a(V, mln::value::Symbolic)::check();

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      const VW& vwin = exact(vwin_);
      const HW& hwin = exact(hwin_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());
      mln_precondition(vwin.is_valid());
      mln_precondition(hwin.is_valid());

      typedef util::array<box<mln_site(I)> > boxes_t;

      V ncomps;

      // Vertical lines
      mln_ch_value(I,bool) vfilter = morpho::erosion(input, vwin);
      boxes_t vboxes = component_bboxes(vfilter, nbh, ncomps).first();
      for_all_components(i, vboxes)
      {
	vboxes[i].enlarge(0, vwin.length() / 2);
	vboxes[i].crop_wrt(input.domain());
      }

      // Horizontal lines.
      mln_ch_value(I,bool) hfilter = morpho::erosion(input, hwin);
      boxes_t hboxes = component_bboxes(hfilter, nbh, ncomps).first();
      for_all_components(i, hboxes)
      {
	hboxes[i].enlarge(1, hwin.length() / 2);
	hboxes[i].crop_wrt(input.domain());
      }

      trace::exiting("scribo::table::extract_lines_with_opening");
      return mln::make::couple(vboxes, hboxes);
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_EXTRACT_LINES_WITH_OPENING_HH
