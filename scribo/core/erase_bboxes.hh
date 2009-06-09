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

#ifndef SCRIBO_CORE_ERASE_BBOXES_HH
# define SCRIBO_CORE_ERASE_BBOXES_HH

/// \file scribo/core/erase_bboxes.hh
///
/// Remove the content of bounding boxes from an image.

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/data/paste.hh>
# include <mln/pw/all.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>

namespace scribo
{

  using namespace mln;

  /// Remove the content of bounding boxes from an image.
  template <typename I>
  void
  erase_bboxes(Image<I>& input_,
	       const util::array< box<mln_site(I)> >& bboxes);


# ifndef MLN_INCLUDE_ONLY


  template <typename I>
  void
  erase_bboxes(Image<I>& input_,
	       const util::array< box<mln_site(I)> >& bboxes)
  {
    trace::entering("scribo::erase_bboxes");

    mlc_equal(mln_value(I),bool)::check();

    I& input = exact(input_);
    mln_precondition(input.is_valid());

    for_all_components(i, bboxes)
      data::paste((pw::cst(false) | bboxes[i] |
		  (pw::value(input) == true)), input);

    trace::exiting("scribo::erase_bboxes");
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_ERASE_BBOXES_HH
