// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef SCRIBO_CORE_ERASE_OBJECTS_HH
# define SCRIBO_CORE_ERASE_OBJECTS_HH

/// \file
///
/// Erase components in an image.

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/data/fill.hh>
# include <mln/pw/all.hh>
# include <mln/util/array.hh>

# include <scribo/core/component_set.hh>
# include <scribo/core/macros.hh>

namespace scribo
{

  using namespace mln;

  /// Remove labeled components from a binary image.
  ///
  /// \param[in,out] input   A binary image.
  /// \param[in]     comps    A set of components to be erased.
  ///
  //
  template <typename I, typename L>
  void
  erase_objects(Image<I>& input, const component_set<L>& comps);


# ifndef MLN_INCLUDE_ONLY


  template <typename I, typename L>
  void
  erase_objects(Image<I>& input_,
		const component_set<L>& comps)
  {
    mln_trace("scribo::erase_objects");

    mlc_equal(mln_value(I),bool)::check();

    I& input = exact(input_);
    mln_precondition(input.is_valid());

    data::fill((input | (pw::value(comps.labeled_image())
			 != pw::cst(literal::zero))).rw(),
	       false);

  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_ERASE_OBJECTS_HH
