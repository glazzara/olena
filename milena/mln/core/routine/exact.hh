// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_ROUTINE_EXACT_HH
# define MLN_CORE_ROUTINE_EXACT_HH

/*! \file mln/core/routine/exact.hh
 * \brief Definition of the mln::exact downcast routines.
 */

#include <mln/core/internal/exact.hh>


/// FIXME: Doc!
#define mln_exact(T)  typename mln::internal::exact_<T>::ret
#define mln_exact_(T)          mln::internal::exact_<T>::ret



namespace mln
{

  /*! \brief Exact cast routine for mln objects.
   *
   * This set of routines can be used to downcast an object towards
   * its exact type.  The only argument, respectively \p ptr or \p
   * ref, should be an mln::Object.
   *
   * \c The parameter E is the exact type of the object.
   *
   * \return The return follows the nature of the argument (either a
   * pointer or a reference, const or not).
   */
  /// \{

  template <typename T>
  mln_exact(T)* exact(T* ptr);

  template <typename T>
  mln_exact(T)& exact(T& ref);

  /// \}


# ifndef MLN_INCLUDE_ONLY

  // exact

  template <typename T>
  inline
  mln_exact(T)* exact(T* ptr)
  {
    return internal::exact_<T>::run(ptr);
  }

  template <typename T>
  inline
  mln_exact(T)& exact(T& ref)
  {
    return *exact(&ref);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_EXACT_HH
