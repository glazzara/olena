// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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

#ifndef MLN_CORE_ROUTINE_MUTABLE_EXTEND_HH
# define MLN_CORE_ROUTINE_MUTABLE_EXTEND_HH

/// \file
///
/// Routine to extend a mutable image with a mutable extension.
///
/// \todo Use the 'instant' mechanism.
/// \todo Deal with ambiguities.
/// \todo Check that there is no extension yet (except "mutable_extendable").

# include <mln/core/image/dmorph/mutable_extension_ima.hh>



namespace mln
{

  /// Extend a mutable image with a mutable image.
  template <typename I, typename J>
  mutable_extension_ima<I, J>
  mutable_extend(Image<I>& ima, Image<J>& ext);


# ifndef MLN_INCLUDE_ONLY

  template <typename I, typename J>
  mutable_extension_ima<I, J>
  mutable_extend(Image<I>& ima, Image<J>& ext)
  {
    mlc_converts_to(mln_value(J), mln_value(I))::check();
    mutable_extension_ima<I, J> tmp(exact(ima), exact(ext));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_MUTABLE_EXTEND_HH
