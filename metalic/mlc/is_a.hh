// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef METALIC_IS_A_HH
# define METALIC_IS_A_HH

# include <mlc/bool.hh>



// internal macro so do not use it
# define mlc_internal_is_a__check_result_ \
sizeof(helper<T ,U >::check( (T *) helper<T,U >::makeT() )) == sizeof(mlc::internal::yes_)


namespace mlc
{

  namespace internal
  {

    //
    //  wrap is a workaround to fix g++-2.95 problem with implicit:
    //  instead of typename in <foo<T>::ret::dim>
    //  write <wrap<typename foo<T>::ret>::dim
    template <class T> struct wrap : public T {};


    /// form

    namespace form
    {
      enum {
	class_                          = 1,
	template_l_class_g_class_       = 2,
	template_l_class_class_g_class_ = 3,
	template_l_template_l_class_g_class_g_class_       = 4,
	template_l_template_l_class_class_g_class_g_class_ = 5
	// ...
      };

      template<unsigned id>
      struct desc
      { char dummy[id]; };
      
      template   < class T>
      static desc< class_ > get();
      
      template   < template < class > class T>
      static desc< template_l_class_g_class_ > get();
      
      template<    template < class,class > class T>
      static desc< template_l_class_class_g_class_ > get();
      
      template<    template < template < class > class > class T>
      static desc< template_l_template_l_class_g_class_g_class_ > get();
      
      template<    template < template < class,class > class > class T>
      static desc< template_l_template_l_class_class_g_class_g_class_ > get();
      
      // ...

    } // end of namespace mlc::internal::form


    typedef char yes_;
    struct no_ { char dummy[2]; };


    // dev note : below, is_a_<T,id> is a better factorization
    //            but g++ 2.95.4 has some trouble with it

    template<unsigned id>
    struct is_a_;

    // class_

    template<>
    struct is_a_< form::class_ >
    {
      typedef is_a_< form::class_ > self;

      template<class T,    class U>
      struct helper
      {
	static yes_ check(U*);
	static no_  check(...);
	static T* makeT();
      };

      template<class T,    class U>
      struct check
	: public bool_<( mlc_internal_is_a__check_result_ )>
      {
      };
    };

    // template_l_class_g_class_

    template<>
    struct is_a_< form::template_l_class_g_class_ >
    {
      typedef is_a_< form::template_l_class_g_class_ > self;

      template<class T,    template < class > class U>
      struct helper
      {
	template<class V>
	static yes_ check(U<V>*);
	static no_  check(...);
	static T* makeT();
      };

      template<class T,    template < class > class U>
      struct check
	: public bool_<( mlc_internal_is_a__check_result_ )>
      {
      };
    };

    // template_l_class_class_g_class_

    template<>
    struct is_a_< form::template_l_class_class_g_class_ >
    {
      typedef is_a_< form::template_l_class_class_g_class_ > self;

      template<class T,    template < class,class > class U>
      struct helper
      {
	template<class V, class W>
	static yes_ check(U<V,W>*);
	static no_  check(...);
	static T* makeT();
      };

      template<class T,    template < class,class > class U>
      struct check
	: public bool_<( mlc_internal_is_a__check_result_ )>
      {};
    };

    // template_l_template_l_class_g_class_g_class_

    template<>
    struct is_a_< form::template_l_template_l_class_g_class_g_class_ >
    {
      typedef is_a_< form::template_l_template_l_class_g_class_g_class_ > self;

      template<class T,    template < template < class > class > class U>
      struct helper
      {
	template<template<class> class V>
	static yes_ check(U<V>*);
	static no_  check(...);
	static T* makeT();
      };

      template<class T,    template < template < class > class > class U>
      struct check
	: public bool_<( mlc_internal_is_a__check_result_ )>
      {};
    };

    // template_l_template_l_class_class_g_class_g_class_

    template<>
    struct is_a_< form::template_l_template_l_class_class_g_class_g_class_ >
    {
      typedef is_a_< form::template_l_template_l_class_class_g_class_g_class_ > self;

      template<class T,    template < template < class,class > class > class U>
      struct helper
      {
	template<template<class,class> class V>
	static yes_ check(U<V>*);
	static no_  check(...);
	static T* makeT();
      };

      template<class T,    template < template < class,class > class > class U>
      struct check
	: public bool_<( mlc_internal_is_a__check_result_ )>
      {};
    };


  } // end of namespace mlc::internal

} // end of namespace mlc



/// Client macro mlc_is_a(T, U)

# define mlc_is_a(T, U)  \
mlc::internal::wrap<typename mlc::internal::is_a_< sizeof(mlc::internal::form::get<U >()) >::check<T, U > >

# define mlc_is_a_(T, U) \
mlc::internal::wrap<         mlc::internal::is_a_< sizeof(mlc::internal::form::get<U >()) >::check<T, U > >


#endif // ! METALIC_IS_A_HH
