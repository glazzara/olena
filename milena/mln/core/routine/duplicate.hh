// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ROUTINE_DUPLICATE_HH
# define MLN_CORE_ROUTINE_DUPLICATE_HH

/// \file
///
/// Clone an image, that is, get an effective copy.

# include <mln/core/concept/image.hh>
# include <mln/core/routine/init.hh>
# include <mln/data/fill.hh>


namespace mln
{

  /// Duplicate the image \p model with the values of the image \p
  /// data.
  ///
  /// \param[in] model The image to be duplicated.
  /// \result The duplicate.
  ///
  /// \pre model.is_valid
  template <typename I>
  mln_concrete(I) duplicate(const Image<I>& model);


# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  inline
  mln_concrete(I) duplicate(const Image<I>& model)
  {
    // FIXME: Add a static check that mln_concrete(I) actually *is* concrete...
    mln_trace("core::duplicate");

    mln_concrete(I) tmp;
    initialize(tmp, model);
    data::fill(tmp, model);

    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_DUPLICATE_HH
