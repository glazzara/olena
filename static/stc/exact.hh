// Copyright (C) 2006  EPITA Research and Development Laboratory.
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

#ifndef STATIC_EXACT_HH
# define STATIC_EXACT_HH

# include <stc/any.hh>

# include <mlc/is_a.hh>
# include <mlc/case.hh>
# include <mlc/bexpr.hh>




# define stc_internal_is_any(T)						 \
  mlc::bexpr_< sizeof(internal::any_select(internal::makePtr< T >())) == \
    sizeof(internal::yes_) >



namespace stc
{

  namespace internal
  {
    typedef char yes_;
    struct no_ { char dummy[2]; };

    template <typename E, typename P>
    yes_ any_select(const stc::any<E,P>* arg);

    no_  any_select(...);

    template <typename T>
    T* makePtr();

  }

  template <typename T>
  struct is_any_
    : public stc_internal_is_any(T)
  {
  };

  template <typename T>
  struct is_any_ < T* >;

  template <typename T>
  struct is_any_ < const T >;

} // end of namespace stc


mlc_case_equipment_for_namespace(stc);


namespace stc
{


  namespace tag
  {
    struct exact;
  }


  template <class T>
  struct case_<tag::exact, T, 1> : public mlc::where_ < stc::is_any_<T> >
  {
    struct protected_
    {
      typedef typename T::exact_t ret;

      static inline ret& impl(T& t)
      {
	return t.exact();
      }
    };
  };

  template <typename T>
  struct default_case_<tag::exact, T>
  {
    struct protected_
    {
      typedef T ret;

      static inline ret& impl(T& t)
      {
	return t;
      }
    };

  };


  template <typename T>
  struct to_exact_
  {
    typedef typename switch_<tag::exact, T>::ret ret;

    static inline ret& impl(const T& t)
    {
      typedef typename case_<tag::exact, T>::ret case_t;
      return case_t::impl(const_cast<T&>(t));
    }
  };


  template <typename T>
  struct to_exact_ < const T >
  {
    typedef const typename to_exact_<T>::ret ret;

    static inline ret& impl(const T& t)
    {
      return to_exact_<T>::impl(const_cast<T&>(t));
    }
  };


  template <typename T>
  struct to_exact_ < T* >
  {
    typedef typename to_exact_<T>::ret * ret;

    static inline ret impl(T* t)
    {
      return & to_exact_<T>::impl(*t);
    }
  };



  template <typename T>
  const typename to_exact_<const T>::ret&
  exact(const T& t)
  {
    return to_exact_<const T>::impl(t);
  }

  template <typename T>
  typename to_exact_<T>::ret&
  exact(T& t)
  {
    return to_exact_<T>::impl(t);
  }

  template <typename T>
  typename to_exact_<const T*>::ret
  exact(const T* t)
  {
    return to_exact_<const T*>::impl(t);
  }

  template <typename T>
  typename to_exact_<T*>::ret
  exact(T* t)
  {
    return to_exact_<T*>::impl(t);
  }


  template <typename T>
  void exact(const T** t);

  template <typename T>
  void exact(T** t);


} // end of namespace stc

#endif // ! STATIC_EXACT_HH
