// Copyright (C) 2005, 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_STC_SCOOP_HH
# define OLN_STC_SCOOP_HH

# include <mlc/flags.hh>
# include <mlc/typedef.hh>
# include <mlc/ret.hh>
# include <mlc/assert.hh>
# include <mlc/abort.hh>
# include <mlc/bool.hh>
# include <mlc/pair.hh>
# include <mlc/basic.hh>
# include <mlc/cmp.hh>
# include <mlc/if.hh>
# include <mlc/is_a.hh>
# include <mlc/case.hh>




namespace stc
{

  struct not_delegated;

  struct abstract;

  template <typename T>
  struct final;

  template < template <class> class category >
  struct is;

  typedef mlc::true_  true_;
  typedef mlc::false_ false_;




  /// Equality test between a couple of types.
  /// \{
  template <typename T>
  struct is_found : public mlc::bexpr_<true>
  {};

  template <>
  struct is_found< mlc::not_found > : public mlc::bexpr_<false>
  {};

  template <typename T>
  struct is_not_found : public mlc::bexpr_<false>
  {};

  template <>
  struct is_not_found< mlc::not_found > : public mlc::bexpr_<true>
  {};
  /// \}
  


} // end of namespace stc


#define stc_decl_associated_type  mlc_decl_typedef


#define stc_super(T) typename super_trait_< T >::ret

#define stc_stm(From, Target) typename get_stm< From , Target >::ret



// Access to associated type
# define stc_type_(From, Type) vtype<From, typedef_::Type>::ret
# define stc_type(From, Type) typename stc_type_(From, Type)

# define stc_type_in_(Namespace, From, Type) Namespace::vtype<From, Namespace::typedef_::Type>::ret
# define stc_type_in(Namespace, From, Type) typename stc_type_in_(Namespace, From, Type)


# define stc_get_type_(Type) vtype<Exact, typedef_::Type>::ret
# define stc_get_type(Type) typename stc_get_type_(Type)

// FIXME: Swap name of stc_type and stc_get_type?


// below the more tolerant version is used, namely stc_deferred,
// yet it sometimes can be replaced by "stc_find_type(E, Name)"
# define stc_find_type_(From, Type) find_vtype<From, typedef_::Type>::ret
# define stc_find_type(From, Type) typename stc_find_type_(From, Type)

// Boolean expression counterpart of stc_find_type
# define stc_type_is_found(Type)      stc::is_found< stc_deferred(Type) >
# define stc_type_is_not_found(Type)  stc::is_not_found< stc_deferred(Type) >

# define stc_is_found_type(From, Type)      stc::is_found< stc_deferred_type(From, Type) >
# define stc_is_not_found_type(From, Type)  stc::is_not_found< stc_deferred_type(From, Type) >



# define stc_is_a(T, U)												\
mlc::wrap_<													\
  typename mlc::is_a_< sizeof(mlc::form::of< U >()) >::template ret< typename mlc::basic_< stc_deferred(T) >::ret, U >	\
>





// For concepts.
# define stc_typename(Type) typedef stc_type(Exact, Type) Type
# define stc_using(Type)    typedef typename super::Type Type
# define stc_using_from(Abstraction, Type)    typedef typename Abstraction<Exact>::Type Type
# define stc_deduce_typename(Src, Type) typedef stc_type(Src, Type) Type


// For impl classes.
# define stc_deferred(Type)  typename deferred_vtype<Exact, typedef_::Type >::ret
# define stc_lookup(Type) typedef typename vtype< stc_type(current, exact_type), typedef_::Type>::ret Type

// For set_impl classes.
# define stc_deferred_typename(Type)  typedef stc_deferred(Type) Type


# define stc_deferred_type(From, Type)  typename deferred_vtype< From, typedef_::Type >::ret




// the macro below was called stc_prop which was ambiguous
// (that lets us think that it is true_ or false_) but the
// result is a mlc::bexpr_!
// so it has been renamed as stc_is
# define stc_is(Type)    mlc::eq_< stc_find_type(E, Type), stc::true_ >

// likewise:

# define stc_is_not(Type)			\
   mlc::or_< mlc::eq_< stc_find_type(E, Type),	\
                       mlc::not_found >,	\
             mlc::eq_< stc_find_type(E, Type),	\
                       stc::false_ > >

# define stc_prop(From, Type)      typename mlc::eq_< stc_find_type(From, Type), stc::true_ >::eval



#endif // ! OLN_STC_SCOOP_HH
