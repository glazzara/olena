// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_FUN_V2V_INC_HH
# define MLN_FUN_V2V_INC_HH

/// \file
///
/// Incrementation function.

# include <mln/fun/internal/selector.hh>


namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      // FIXME: Doc!

      template <typename T>
      struct inc
	: fun::internal::selector_<T, T, inc<T> >::ret
      {
	typedef T result;
	T operator()(const T& t) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      inline
      T
      inc<T>::operator()(const T& t) const
      {
	return static_cast<T>(t + 1);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2V_INC_HH
