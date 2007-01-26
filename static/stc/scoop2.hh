// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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

/* \file stc/vtypes.hh

   \brief Equipment for SCOOP 2 (in particular, virtual types).  */

#ifndef STC_SCOOP2_HH
# define STC_SCOOP2_HH

# include <mlc/flags.hh>
# include <mlc/typedef.hh>
# include <mlc/abort.hh>


namespace stc
{
  /* FIXME: Define a real stc::none, instead of making it an alias of
     mlc::none.  For compatibility purpose with the current
     implementation of the SCOOP 2 paradigme (see stc/scoop.hh), we
     need to have stc::none be equal to mlc::none.  */
#if 0
  struct none {};
#endif
  using mlc::none;

  struct not_found;
  struct abstract;
  struct not_delegated;
  struct not_delegated_abstract;
  template <typename T> struct final;


  /// \brief Shortcuts for comparison with stc::not_found.
  ///
  /// Duplicate with their Metalic's homonyms, but still useful, since
  /// they deal with std::not_found (not mlc::not_found).
  /// \{
  template <typename T>
  struct is_not_found_ : public mlc_is_a(T, stc::not_found)::bexpr
  {
  };

  template <typename T>
  struct is_found_ : public mlc_is_not_a(T, stc::not_found)::bexpr
  {
  };
  /// \}

  namespace ERROR
  {
    struct IN_find__VIRTUAL_TYPE_IS_ABSTRACT;

    struct IN_find_local__VIRTUAL_TYPE_MULTIPLY_DEFINED;

    struct IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT;
    struct IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED_FINAL;
    struct IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED;
    struct IN_merge2__VIRTUAL_TYPE_REDEFINED_ABSTRACT;

    struct IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT;
    struct IN_merge3__VIRTUAL_TYPE_REDEFINED_ABSTRACT;
    struct IN_merge3__LOCAL_DECLARATION_OF_NOT_DELEGATED_AND_ABSTRACT;
    struct IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED_FINAL;
    struct IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED;

  } // end of namespace ERROR

} // end of namespace stc


#define stc_super(T) typename set_super_type< T >::ret

// Based on doc/algorithm.ml.

// FIXME: Temp. hack.  The contents of SCOOPED_NAMESPACE will be
// available later as a macro, as in stc/scoop.hh.  Currently, it's
// easier to work without this macro equipment.
#define SCOOPED_NAMESPACE oln


// ------------------------------------------------------------ find_local.

// Highly inspired from Théo's get_stm!

namespace SCOOPED_NAMESPACE
{

  // Declare delegatee_type.
  mlc_decl_typedef(delegatee_type);


  template <typename class_name>
  struct set_super_type
  {
  };

  template <typename class_name>
  struct vtypes
  {
  };

  template <typename class_name, typename type_name>
  struct single_vtype
  {
    typedef mlc::not_found ret;
  };

  namespace find_local_
  {
    // Forward declaration.
    template <typename T, typename U> struct match_with;

    template <typename T>
    struct match_with <T, mlc::not_found>
    {
      typedef T ret;
    };

    template <typename U>
    struct match_with <mlc::not_found, U>
    {
      typedef U ret;
    };

    template <>
    struct match_with <mlc::not_found, mlc::not_found>
    {
      typedef stc::not_found ret;
    };

    template <typename T, typename U>
    struct match_with :
      mlc::abort_<T, stc::ERROR::IN_find_local__VIRTUAL_TYPE_MULTIPLY_DEFINED>
    {
    };

  } // end of namespace find_local_

  template <typename source, typename target>
  struct find_local
  {
    typedef SCOOPED_NAMESPACE::vtypes<source> decl1;
    typedef typename target::template from_<decl1>::ret res1;

    typedef SCOOPED_NAMESPACE::single_vtype<source, target> decl2;
    typedef typename decl2::ret ret2;


    // Result.
    typedef typename find_local_::match_with<res1, ret2>::ret ret;
  };


  // -------------------------------------------------------------- merge2.

// Shortcuts macros.
#define stc_match_pair_0p_with(Local, Super, Res)	\
  template <>						\
  struct match_with< Local, Super >			\
  {							\
    typedef Res ret;					\
  }

#define stc_match_pair_1p_with(T1, Local, Super, Res)	\
  template < typename T1 >				\
  struct match_with< Local, Super >			\
  {							\
    typedef Res ret;					\
  }

#define stc_match_pair_2p_with(T1, T2, Local, Super, Res)	\
  template < typename T1, typename T2 >				\
  struct match_with< Local, Super >				\
  {								\
    typedef Res ret;						\
  }

// Erroneous cases.
#define stc_match_pair_1p_with_error(T1, Local, Super, Msg)	\
  template < typename T1 >					\
  struct match_with< Local, Super > :				\
    mlc::abort_< T1, Msg >					\
  {								\
  }

#define stc_match_pair_2p_with_error(T1, T2, Local, Super, Msg)	\
  template < typename T1, typename T2 >				\
  struct match_with< Local, Super > :				\
    mlc::abort_< T2, Msg >					\
  {								\
  }


  namespace merge2_
  {
    // Forward declaration.
    template <typename T, typename U> struct match_with;


    /*------------------------------.
    | local_res == stc::not_found.  |
    `------------------------------*/

    stc_match_pair_0p_with(stc::not_found, stc::not_found,
			   stc::not_found);

    stc_match_pair_0p_with(stc::not_found, stc::abstract,
			   stc::not_found);

    stc_match_pair_1p_with(U,
			   stc::not_found, stc::final<U>,
			   stc::final<U>);

    stc_match_pair_1p_with(U,
			   stc::not_found, U,
			   U);


    /*-----------------------------.
    | local_res == stc::abstract.  |
    `-----------------------------*/

    stc_match_pair_0p_with(stc::abstract, stc::not_found,
			   stc::abstract);

    stc_match_pair_0p_with(stc::abstract, stc::abstract,
			   stc::abstract);

    stc_match_pair_1p_with_error(U,
      stc::abstract, stc::final<U>,
      stc::ERROR::IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT);

    stc_match_pair_1p_with_error(U,
      stc::abstract, U,
      stc::ERROR::IN_merge2__VIRTUAL_TYPE_REDEFINED_ABSTRACT);


    /*-----------------------------.
    | local_res == stc::final<T>.  |
    `-----------------------------*/

    stc_match_pair_1p_with(T,
			   stc::final<T>, stc::not_found,
			   stc::final<T>);

    stc_match_pair_1p_with(T,
			   stc::final<T>, stc::abstract,
			   stc::final<T>);

    stc_match_pair_2p_with_error(T, U,
      stc::final<T>, stc::final<U>,
      stc::ERROR::IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED_FINAL);

    stc_match_pair_2p_with(T, U,
			   stc::final<T>, U,
			   stc::final<T>);


    /*-----------------.
    | local_res == T.  |
    `-----------------*/

    stc_match_pair_1p_with(T,
			   T, stc::not_found,
			   T);

    stc_match_pair_1p_with(T,
			   T, stc::abstract,
			   T);

    stc_match_pair_2p_with_error(T, U,
      T, stc::final<U>,
      stc::ERROR::IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED);

    template <typename T, typename U>
    struct match_with
    {
      typedef T ret;
    };

  } // end of namespace merge2_

#undef stc_match_pair_2p_with_error
#undef stc_match_pair_1p_with_error
#undef stc_match_pair_2p_with
#undef stc_match_pair_1p_with
#undef stc_match_pair_0p_with

  template <typename local_res, typename super_res>
  struct merge2
  {
    // Result.
    typedef typename merge2_::match_with<local_res, super_res>::ret ret;
  };


  // -------------------------------------------------------------- merge3.

// Shortcuts macros.
#define stc_match_triple_0p_with(Local, Super, Deleg, Res)	\
  template <>							\
  struct match_with< Local, Super, Deleg >			\
  {								\
    typedef Res ret;						\
  }

#define stc_match_triple_1p_with(T1, Local, Super, Deleg, Res)	\
  template < typename T1 >					\
  struct match_with< Local, Super, Deleg >			\
  {								\
    typedef Res ret;						\
  }

#define stc_match_triple_2p_with(T1, T2, Local, Super, Deleg, Res)	\
  template < typename T1, typename T2 >					\
  struct match_with< Local, Super, Deleg >				\
  {									\
    typedef Res ret;							\
  }

#define stc_match_triple_3p_with(T1, T2, T3, Local, Super, Deleg, Res)	\
  template < typename T1, typename T2, typename T3 >			\
  struct match_with< Local, Super, Deleg >				\
  {									\
    typedef Res ret;							\
  }

// Erroneous cases.
#define stc_match_triple_2p_with_error(T1, T2, Local, Super, Deleg, Msg) \
  template < typename T1, typename T2 >					 \
  struct match_with< Local, Super, Deleg > :				 \
      mlc::abort_< T2, Msg >						 \
  {									 \
  }

#define stc_match_triple_3p_with_error(T1, T2, T3, Local, Super, Deleg, Msg) \
  template < typename T1, typename T2, typename T3 >			     \
  struct match_with< Local, Super, Deleg > :				     \
      mlc::abort_< T3, Msg >						     \
  {									     \
  }


  namespace merge3_
  {
    // Forward declaration.
    template <typename T, typename U, typename V> struct match_with;


    /*------------------------------.
    | local_res == stc::not_found.  |
    `------------------------------*/

    // super_res == stc::not_found.
    stc_match_triple_0p_with(stc::not_found, stc::not_found, stc::not_found,
			     stc::not_found);

    stc_match_triple_0p_with(stc::not_found, stc::not_found, stc::abstract,
			     stc::not_found);

    stc_match_triple_1p_with(V,
			     stc::not_found, stc::not_found, stc::final<V>,
			     stc::final<V>);

    stc_match_triple_1p_with(V,
			     stc::not_found, stc::not_found, V,
			     V);

    // super_res == stc::abstract.
    stc_match_triple_0p_with(stc::not_found, stc::abstract, stc::not_found,
			     stc::not_found);

    stc_match_triple_0p_with(stc::not_found, stc::abstract, stc::abstract,
			     stc::abstract);

    stc_match_triple_1p_with(V,
			     stc::not_found, stc::abstract, stc::final<V>,
			     stc::final<V>);

    stc_match_triple_1p_with(V,
			     stc::not_found, stc::abstract, V,
			     V);

    // super_res is a concrete type.
    stc_match_triple_2p_with(U, V,
			     stc::not_found, stc::final<U>, V,
			     stc::final<U>);

    stc_match_triple_2p_with(U, V,
			     stc::not_found, U, V,
			     U);


    /*-----------------------------.
    | local_res == stc::abstract.  |
    `-----------------------------*/

    // super_res == stc::not_found.
    stc_match_triple_0p_with(stc::abstract, stc::not_found, stc::not_found,
			     stc::abstract);

    stc_match_triple_0p_with(stc::abstract, stc::not_found, stc::abstract,
			     stc::abstract);

    stc_match_triple_1p_with(V,
			     stc::abstract, stc::not_found, stc::final<V>,
			     stc::final<V>);

    stc_match_triple_1p_with(V,
			     stc::abstract, stc::not_found, V,
			     V);


    // super_res == stc::abstract.
    stc_match_triple_0p_with(stc::abstract, stc::abstract, stc::not_found,
			     stc::abstract);

    stc_match_triple_0p_with(stc::abstract, stc::abstract, stc::abstract,
			     stc::abstract);

    stc_match_triple_1p_with(V,
			     stc::abstract, stc::abstract, stc::final<V>,
			     stc::final<V>);

    stc_match_triple_1p_with(V,
			     stc::abstract, stc::abstract, V,
			     V);


    // super_res == stc::not_delegated_abstract.
    stc_match_triple_1p_with(V,
			     stc::abstract, stc::not_delegated_abstract, V,
			     stc::not_delegated_abstract);

    stc_match_triple_1p_with(V,
			     stc::abstract, stc::not_delegated, V,
			     stc::not_delegated_abstract);


    // super_res is a concrete type.
    stc_match_triple_2p_with_error(U, V,
      stc::abstract, stc::final<U>, V,
      stc::ERROR::IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT);

    stc_match_triple_2p_with_error(U, V,
      stc::abstract, U, V,
      stc::ERROR::IN_merge3__VIRTUAL_TYPE_REDEFINED_ABSTRACT);


    /*----------------------------------.
    | local_res == stc::not_delegated.  |
    `----------------------------------*/

    stc_match_triple_1p_with(V,
			     stc::not_delegated, stc::not_found, V,
			     stc::not_delegated);

    stc_match_triple_1p_with(V,
			     stc::not_delegated, stc::abstract, V,
			     stc::not_delegated_abstract);

    stc_match_triple_1p_with(V,
      stc::not_delegated, stc::not_delegated_abstract, V,
      stc::not_delegated_abstract);


    /*----------------------------------------------.
    | local_res == stc::not_delegated_abstract.  *) |
    `----------------------------------------------*/

    /* FIXME: Shouldn't we introduce a means to tag a vtype both
       as abstract *and* not delegated?  (Currently, the rule below
       prevents this).  */
    stc_match_triple_2p_with_error(U, V,
      stc::not_delegated_abstract, U, V,
      stc::ERROR::IN_merge3__LOCAL_DECLARATION_OF_NOT_DELEGATED_AND_ABSTRACT);


    /*-----------------------------.
    | local_res == stc::final<T>.  |
    `-----------------------------*/

    stc_match_triple_3p_with_error(T, U, V,
      stc::final<T>, stc::final<U>, V,
      stc::ERROR::IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED_FINAL);

    stc_match_triple_3p_with(T, U, V,
			     stc::final<T>, U, V,
			     stc::final<T>);


    /*-----------------.
    | local_res == T.  |
    `-----------------*/

    stc_match_triple_3p_with_error(T, U, V,
      T, stc::final<U>, V,
      stc::ERROR::IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED);

    template <typename T, typename U, typename V>
    struct match_with
    {
      typedef T ret;
    };

  } // end of namespace merge3_


#undef stc_match_triple_3p_with_error
#undef stc_match_triple_2p_with_error
#undef stc_match_triple_3p_with
#undef stc_match_triple_2p_with
#undef stc_match_triple_1p_with
#undef stc_match_triple_0p_with


  template <typename local_res, typename super_res, typename delegatee_res>
  struct merge3
  {
    // Result.
    typedef typename merge3_::match_with<local_res, super_res, delegatee_res>::ret ret;
  };


  // -----------------------------------------------------------  find_rec.

  // Forward declarations.
  template <typename source, typename target> struct find_rec;
  template <typename source, typename target> struct find_rec_in_supers;


  namespace find_rec_
  {

    namespace find_delegatee_res_
    {
      // Forward declaration.
      template <typename delegatee, typename target> struct match_with;

      template <typename target>
      struct match_with<stc::not_found, target>
      {
	typedef stc::not_found ret;
      };

      template <typename target>
      struct match_with<stc::abstract, target>
      {
	typedef stc::not_found ret;
      };

      template <typename delegatee, typename target>
      struct match_with
      {
	typedef typename find_rec<delegatee, target>::ret ret;
      };

    } // end of namespace find_delegatee_res_


    // Forward declaration.
    template <typename source, typename target> struct match_with;

    template <typename target>
    struct match_with<stc::none, target>
    {
      typedef stc::not_found ret;
    };

    template <typename source, typename target>
    struct match_with
    {
      typedef typename find_local<source, target>::ret local_res;
      typedef typename find_rec<stc_super(source), target>::ret super_res;
      // delegatee_type is the name of the (optional) virtual type
      // containing the (type of the) delgatee.
      typedef typename find_rec_in_supers<source, typedef_::delegatee_type>::ret delegatee;

      typedef typename find_delegatee_res_::match_with<delegatee, target>::ret delegatee_res;
      typedef typename merge3<local_res, super_res, delegatee_res>::ret ret;
    };

  } // end of namespace find_rec_

  template <typename source, typename target>
  struct find_rec
  {
    // Result.
    typedef typename find_rec_::match_with<source, target>::ret ret;
  };


  // -------------------------------------------------- find_rec_in_supers.

  namespace find_rec_in_supers_
  {
    // Forward declaration.
    template <typename source, typename target> struct match_with;

    template <typename target>
    struct match_with<stc::none, target>
    {
      typedef stc::none ret;
    };

    template <typename source, typename target>
    struct match_with
    {
      typedef typename find_local<source, target>::ret local_res;
      typedef typename find_rec<stc_super(source), target>::ret super_res;
      typedef typename merge2<local_res, super_res>::ret ret;
    };

  } // end of namespace find_rec_in_supers_

  template <typename source, typename target>
  struct find_rec_in_supers
  {
    // Result.
    typedef typename find_rec_in_supers_::match_with<source, target>::ret ret;
  };


  // ---------------------------------------------------------------- find.

  namespace find_
  {
    // Forward declaration.
    template <typename T> struct match_with;

    template <>
    struct match_with<stc::abstract>
/* FIXME: We'd like to add a static abort here, but we can't, since
   stc::abstract is not a free parameter (enabling this abort statement
   would prevent any compilation).  */
#if 0
      : mlc::abort_<stc::abstract,
		     stc::ERROR::IN_find__VIRTUAL_TYPE_IS_ABSTRACT>
#endif
    {
    };

    template <>
    struct match_with<stc::not_delegated_abstract>
/* FIXME: We'd like to add a static abort here, but we can't, since
   stc::not_delegated_abstract is not a free parameter (enabling this
   abort statement would prevent any compilation).  */
#if 0
      : mlc::abort_<stc::not_delegated_abstract,
		    stc::ERROR::IN_find__VIRTUAL_TYPE_IS_ABSTRACT>
#endif
    {
    };

    template <>
    struct match_with<stc::not_delegated>
    {
      typedef stc::not_found ret;
    };

    template <typename T>
    struct match_with< stc::final<T> >
    {
      typedef T ret;
    };

    template <typename T>
    struct match_with
    {
      typedef T ret;
    };

  } // end of namespace find_


  /// Find a virtual type.
  template <typename source, typename target>
  struct find_vtype
  {
    typedef typename find_rec<source, target>::ret res;
    // Result.
    typedef typename find_::match_with<res>::ret ret;
  };


  /// Find a virtual type, and ensure it is found.
  template <typename from, typename target>
  struct vtype
  {
    typedef typename find_vtype<from, target>::ret res;
    struct check_ : mlc::assert_< mlc::is_found_<res> >
    {
      typedef res ret;
    };
    typedef typename check_::ret ret;
  };

} /* end of SCOOPED_NAMESPACE */


// FIXME: Document all these macros.

# define stc_find_vtype_(Namespace, Source, Target)			\
   Namespace::find_vtype<Source, Namespace::typedef_::Target##_type>::ret

# define stc_find_vtype(Namespace, Source, Target)	\
   typename stc_find_vtype_(Namespace, Source, Target)

// Dummy alias, for compatibility purpose (deferred virtual types are
// not currently handled by this version of stc/scoop2.hh).
# define stc_deferred_vtype(Namespace, From, Target)	\
     stc_find_vtype(Namespace, From, Target)

# define stc_vtype_(Namespace, From, Target)				\
   Namespace::vtype<From, Namespace::typedef_::Target##_type>::ret

# define stc_vtype(Namespace, From, Target)	\
   typename stc_vtype_(Namespace, From, Target)

# define stc_find_deduce_vtype_(Namespace, From, Target1, Target2)	\
   Namespace::find_vtype<						\
     Namespace::find_vtype<						\
       From,								\
       Namespace::typedef_::Target1##_type				\
     >::ret,								\
     Namespace::typedef_::Target2##_type				\
   >::ret

# define stc_find_deduce_vtype(Namespace, From, Target1, Target2)	\
   typename Namespace::find_vtype<					\
     typename Namespace::find_vtype<					\
       From,								\
       Namespace::typedef_::Target1##_type				\
     >::ret,								\
     Namespace::typedef_::Target2##_type				\
   >::ret

# define stc_deduce_deferred_vtype(Namespace, From, Target1, Target2)	\
  stc_find_deduce_vtype(Namespace, From, Target1, Target2)

# define stc_deduce_vtype_(Namespace, From, Target1, Target2)	\
   Namespace::vtype<						\
     Namespace::vtype<						\
       From,							\
       Namespace::typedef_::Target1##_type			\
     >::ret,							\
     Namespace::typedef_::Target2##_type			\
   >::ret

# define stc_deduce_vtype(Namespace, From, Target1, Target2)	\
   typename Namespace::vtype<					\
     typename Namespace::vtype<					\
       From,							\
       Namespace::typedef_::Target1##_type			\
     >::ret,							\
     Namespace::typedef_::Target2##_type			\
   >::ret


#endif // ! STC_SCOOP2_HH
