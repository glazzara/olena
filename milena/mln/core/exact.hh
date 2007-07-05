// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_EXACT_HH
# define MLN_CORE_EXACT_HH

/*! \file mln/core/exact.hh
 * \brief Definition of the mln::exact downcast routines.
 */

# include <mln/core/concept/object.hh>


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

  template <typename E>
  E* exact(Object<E>* ptr);

  template <typename E>
  const E* exact(const Object<E>* ptr);

  template <typename E>
  E& exact(Object<E>& ref);

  template <typename E>
  const E& exact(const Object<E>& ref);

  /// \}


  namespace internal
  {

    template <typename E, typename O>
    struct helper_force_exact_
    {
      typedef E ret;
    };

    template <typename E, typename O>
    struct helper_force_exact_< E, const O >
    {
      typedef const E ret;
    };

  } // end of namespace mln::internal


  /*! \brief Violent exact cast routine.
   *
   * This cast is an alternative to the mln::exact cast.  It is used
   * for objects than do not derive from mln::Object.
   * 
   * It preserves the nature (pointer or reference, const or mutable)
   * of its argument.  The parameter \a E should not have qualifiers.
   *
   * \warning Prefer not to use this cast!   
   *
   * \see mln::exact
   * \todo Move into mln::internal.
   */ 
  /// \{

  template <typename E, typename O>
  E* force_exact(O* ptr);

  template <typename E, typename O>
  const E* force_exact(const O* ptr);

  template <typename E, typename O>
  typename internal::helper_force_exact_<E, O>::ret & // help g++-3.3
  force_exact(O& ref);

  /// \}


//   template <typename E, typename O>
//   E& force_exact(O& ref);

//   template <typename E, typename O>
//   const E& force_exact(const O& ref);


# ifndef MLN_INCLUDE_ONLY

  // exact

  template <typename E>
  E* exact(Object<E>* ptr)
  {
    return (E*)(void*)ptr;
  }

  template <typename E>
  const E* exact(const Object<E>* ptr)
  {
    return (const E*)(const void*)ptr;
  }

  template <typename E>
  E& exact(Object<E>& ref)
  {
    return *(E*)(void*)(&ref);
  }

  template <typename E>
  const E& exact(const Object<E>& ref)
  {
    return *(const E*)(const void*)(&ref);
  }

  // force_exact

  template <typename E, typename O>
  E* force_exact(O* ptr)
  {
    return (E*)(void*)ptr;
  }

  template <typename E, typename O>
  const E* force_exact(const O* ptr)
  {
    return (const E*)(const void*)ptr;
  }

//   template <typename E, typename O>
//   E& force_exact(O& ref)
//   {
//     return *(E*)(void*)(&ref);
//   }

//   template <typename E, typename O>
//   const E& force_exact(const O& ref)
//   {
//     return *(const E*)(const void*)(&ref);
//   }

  template <typename E, typename O>
  typename internal::helper_force_exact_<E, O>::ret &
  force_exact(O& ref)
  {
    return *(E*)(void*)(&ref);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_EXACT_HH
