// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_EXACT_GCC_2_95_HH
# define MLN_CORE_INTERNAL_EXACT_GCC_2_95_HH

/// \file mln/core/internal/exact_gcc_2_95.hh
///
/// \brief Definition of the mln::exact downcast routines
/// for g++-2.95.

# include <mln/core/internal/exact.hh>



namespace mln
{

  namespace internal
  {

    // For exact(T&):

    template <typename T>
    struct exact_gcc_2_95
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef E& ret;
      static ret run(T& t)
      {
	return *internal::exact_<T>::run(&t);
      }
    };

    template <typename T>
    struct exact_gcc_2_95< const T >
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef const E& ret;
      static ret run(T& t)
      {
	return *internal::exact_<T>::run(const_cast<T*>(&t));
      }
    };

    template <typename T>
    struct exact_gcc_2_95< T* >
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef E* ret;
      static ret run(T* t)
      {
	return internal::exact_<T>::run(t);
      }
    };

    template <typename T>
    struct exact_gcc_2_95< const T* >
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef const E* ret;
      static ret run(const T* t)
      {
	return internal::exact_<T>::run(const_cast<T*>(t));
      }
    };



    // For exact(const T&):

    template <typename T>
    struct exact_const_gcc_2_95
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef const E& ret;
      static ret run(T& t)
      {
 	return *internal::exact_<T>::run(&t);
      }
    };

    template <typename T>
    struct exact_const_gcc_2_95< const T >
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef const E& ret;
      static ret run(T& t)
      {
 	return *internal::exact_<T>::run(const_cast<T*>(&t));
      }
    };

    template <typename T>
    struct exact_const_gcc_2_95< T* >
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef E* ret;
      static ret run(T* t)
      {
	return internal::exact_<T>::run(t);
      }
    };

    template <typename T>
    struct exact_const_gcc_2_95< const T*const >
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef const E* ret;
      static ret run(T* t)
      {
	return internal::exact_<T>::run(const_cast<T*>(t));
      }
    };

    template <typename T>
    struct exact_const_gcc_2_95< const T* >
    {
      typedef typename mln::internal::exact_<T>::ret E;
      typedef const E* ret;
      static ret run(const T* t)
      {
	return internal::exact_<T>::run(const_cast<T*>(t));
      }
    };


  } // end of namespace mln::internal



  template <typename T>
  typename internal::exact_gcc_2_95<T>::ret
  inline
  exact(T& t)
  {
    return internal::exact_gcc_2_95<T>::run(t);
  }

  template <typename T>
  typename internal::exact_const_gcc_2_95<T>::ret
  inline
  exact(const T& t)
  {
    return internal::exact_const_gcc_2_95<T>::run(const_cast<T&>(t));
  }



} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_EXACT_GCC_2_95_HH
