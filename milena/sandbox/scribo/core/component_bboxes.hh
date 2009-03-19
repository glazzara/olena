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


#ifndef SCRIBO_CORE_COMPONENT_BBOXES_HH
# define SCRIBO_CORE_COMPONENT_BBOXES_HH

/// \file scribo/core/component_bboxes.hh
///
/// Extract the bounding boxes of image components.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>

# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>

# include <mln/util/array.hh>

# include <mln/debug/println.hh>

namespace scribo
{

  using namespace mln;

  /// Extract the components bboxes.
  template <typename I, typename N, typename V>
  util::couple<util::array< box<mln_site(I)> >, mln_ch_value(I,V)>
  component_bboxes(const Image<I>& input,
		   const Neighborhood<N>& nbh,
		   V& nbboxes);

# ifndef MLN_INCLUDE_ONLY

  template <typename I, typename N, typename V>
  inline
  util::couple<util::array< box<mln_site(I)> >, mln_ch_value(I,V)>
  component_bboxes(const Image<I>& input,
		   const Neighborhood<N>& nbh,
		   V& nbboxes)
  {
    trace::entering("scribo::component_bboxes");

    mlc_equal(mln_value(I),bool)::check();
    mlc_is_a(V, mln::value::Symbolic)::check();
    mln_precondition(exact(input).is_valid());
    mln_precondition(exact(nbh).is_valid());

    mln_ch_value(I,V) lbl = labeling::blobs(input, nbh, nbboxes);
    mln_assertion(exact(lbl).is_valid());

    typedef util::array< box<mln_site(I)> > bboxes_t;
    bboxes_t bboxes = labeling::compute(accu::meta::bbox(), lbl, nbboxes);
    mln_postcondition(bboxes.nelements() == nbboxes.next());

    trace::exiting("scribo::component_bboxes");
    return mln::make::couple(bboxes, lbl);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo


#endif // ! SCRIBO_CORE_COMPONENT_BBOXES_HH
