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


/* \file stc/scoop-alt.hh

   \brief Alternative equipment for SCOOP 2 (in particular, virtual
   types).  */


#ifndef STC_SCOOP_ALT_HH
# define STC_SCOOP_ALT_HH

/** Define a stick-macro so that we can check in
    \file stc/scoop-alt.inc that \file stc/scoop-alt.hh has been
    included (or not) when including the former from client code.  */
# define STATIC_SCOOP_ALT_HH_INCLUDED

# include <mlc/flags.hh>
# include <mlc/typedef.hh>
# include <mlc/abort.hh>
# include <mlc/case.hh>

# include <stc/internal/match-with.hh>


namespace stc
{
  /* FIXME: Define a real stc::none, instead of making it an alias of
     mlc::none.  For compatibility purpose with the current
     implementation of the SCOOP 2 paradigm (see stc/scoop.hh), we
     need to have stc::none be equal to mlc::none.  */
#if 0
  struct none {};
#endif
  using mlc::none;

  // FIXME: Likewise, define a true stc::not_found type.
#if 0
  struct not_found;
#endif
  typedef mlc::not_found not_found;
  struct abstract;
  struct not_delegated;
  struct not_delegated_abstract;
  template <typename T> struct final;

  template < template <class> class category >
  struct is;



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


  namespace ERROR
  {
    template <typename class_name>
    struct super_trait_not_defined_for_;

    struct IN_find_VIRTUAL_TYPE_IS_ABSTRACT;

    struct IN_find_local_VIRTUAL_TYPE_MULTIPLY_DEFINED;

    struct IN_merge2_FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT;
    struct IN_merge2_FINAL_VIRTUAL_TYPE_REDEFINED_FINAL;
    struct IN_merge2_FINAL_VIRTUAL_TYPE_REDEFINED;
    struct IN_merge2_VIRTUAL_TYPE_REDEFINED_ABSTRACT;

    struct IN_merge3_FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT;
    struct IN_merge3_VIRTUAL_TYPE_REDEFINED_ABSTRACT;
    struct IN_merge3_LOCAL_DECLARATION_OF_NOT_DELEGATED_AND_ABSTRACT;
    struct IN_merge3_FINAL_VIRTUAL_TYPE_REDEFINED_FINAL;
    struct IN_merge3_FINAL_VIRTUAL_TYPE_REDEFINED;

  } // end of namespace ERROR

} // end of namespace stc


/*------------------.
| Shortcuts macro.  |
`------------------*/

# define stc_decl_associated_type  mlc_decl_typedef


# define stc_super(T) typename super_trait_< T >::ret


// FIXME: Document all these macros.

/// Access to associated type.
/// \{
# define stc_type_(Source, Target) vtype<Source, typedef_::Target>::ret
# define stc_type(Source, Target) typename stc_type_(Source, Target)

# define stc_type_in_(Namespace, Source, Target)		\
   Namespace::vtype<Source, Namespace::typedef_::Target>::ret
# define stc_type_in(Namespace, Source, Target)		\
   typename stc_type_in_(Namespace, Source, Target)

///   Access from the interior of the class.
///   \{
# define stc_get_type_(Target) vtype<Exact, typedef_::Target>::ret
# define stc_get_type(Target) typename stc_get_type_(Target)
///   \}

/// \}


/// Likewise, but more tolerant.
/// \{
# define stc_find_type_(Source, Target)	\
   find_vtype<Source, typedef_::Target>::ret
# define stc_find_type(Source, Target)		\
   typename stc_find_type_(Source, Target)
/// \}

/// Boolean expression counterpart of stc_find_type
/// \{
# define stc_type_is_found(Target)		\
   stc::is_found< stc_deferred(Target) >
# define stc_type_is_not_found(Target)		\
   stc::is_not_found< stc_deferred(Target) >
/// \}


# define stc_is_a(T, U)							 \
   mlc::wrap_<								 \
     typename mlc::is_a_< sizeof(mlc::form::of< U >()) >		 \
       ::template ret< typename mlc::basic_< stc_deferred(T) >::ret, U > \
     >


/// For concepts.
/// \{
# define stc_typename(Target) typedef stc_type(Exact, Target) Target
# define stc_using(Target)    typedef typename super::Target Target
# define stc_using_from(Abstraction, Target)		\
   typedef typename Abstraction<Exact>::Target Target
# define stc_deduce_typename(Src, Target) typedef stc_type(Src, Target) Target
/// \}


/// For implementation classes.
/// \{
/// Dummy
# define stc_deferred(Target)			\
   stc_find_type(Exact, Target)
//   typename deferred_vtype<Exact, typedef_::Target >::ret
# define stc_lookup(Target)					\
   typedef typename vtype< stc_type(current, exact_type),	\
                           typedef_::Target>::ret Target
/// \}


/// For set_impl classes.
/// \{
# define stc_deferred_typename(Target)  typedef stc_deferred(Target) Target
/// \}

// Dummy alias, for compatibility purpose (deferred virtual types are
// not currently handled by this version of stc/scoop2.hh).
# define stc_deferred_type(Source, Target)	\
     stc_find_type(Source, Target)


// The macro below was called stc_prop which was ambiguous
// (that lets us think that it is true_ or false_) but the
// result is a mlc::bexpr_!  So it has been renamed as stc_is.
# define stc_is(Target)    mlc::eq_< stc_find_type(E, Target), stc::true_ >

// Likewise.
# define stc_is_not(Target)				\
   mlc::or_< mlc::eq_< stc_find_type(E, Target),	\
                       mlc::not_found >,		\
             mlc::eq_< stc_find_type(E, Target),	\
                       stc::false_ > >

# define stc_prop(Source, Target)					\
   typename mlc::eq_< stc_find_type(Source, Target), stc::true_ >::eval


#endif // ! STC_SCOOP_ALT_HH
