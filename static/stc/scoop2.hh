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

  struct none {};
  struct not_found;
  struct abstract;
  struct not_delegated;
  struct not_delegated_abstract;
  template <typename T> struct final;

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

// FIXME: Hack.
#define SCOOPED_NAMESPACE my


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

  namespace merge2_
  {
    // Forward declaration.
    template <typename T, typename U> struct match_with;


    // super_res == stc::not_found.
    template <>
    struct match_with<stc::abstract, stc::not_found>
    {
      typedef stc::abstract ret;
    };
    template <>
    struct match_with<stc::not_found, stc::not_found>
    {
      typedef stc::not_found ret;
    };
    template <typename T>
    struct match_with<stc::final<T>, stc::not_found>
    {
      typedef stc::final<T> ret;
    };

    template <typename T>
    struct match_with<T, stc::not_found>
    {
      typedef T ret;
    };


    // super_res == stc::abstract.
    template <>
    struct match_with<stc::not_found, stc::abstract>
    {
      typedef stc::not_found ret;
    };

    template <>
    struct match_with<stc::abstract, stc::abstract>
    {
      typedef stc::abstract ret;
    };

    template <typename T>
    struct match_with<stc::final<T>, stc::abstract>
    {
      typedef stc::final<T> ret;
    };

    template <typename T>
    struct match_with<T, stc::abstract>
    {
      typedef T ret;
    };


    // super_res == stc::final<U>.
    template <typename U>
    struct match_with<stc::abstract, stc::final<U> > :
      mlc::abort_<U, stc::ERROR::IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT>
    {
    };

    template <typename U>
    struct match_with<stc::not_found, stc::final<U> >
    {
      typedef stc::final<U> ret;
    };

    template <typename T, typename U>
    struct match_with<stc::final<T>, stc::final<U> > :
      mlc::abort_<U, stc::ERROR::IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED_FINAL>
    {
    };

    template <typename T, typename U>
    struct match_with<T, stc::final<U> > :
      mlc::abort_<U, stc::ERROR::IN_merge2__FINAL_VIRTUAL_TYPE_REDEFINED>
    {
    };


    // super_res == U.
    template <typename U>
    struct match_with<stc::abstract, U> :
      mlc::abort_<U, stc::ERROR::IN_merge2__VIRTUAL_TYPE_REDEFINED_ABSTRACT>
    {
    };

    template <typename U>
    struct match_with<stc::not_found, U>
    {
      typedef U ret;
    };

    template <typename T, typename U>
    struct match_with<stc::final<T>, U>
    {
      typedef stc::final<T> ret;
    };

    template <typename T, typename U>
    struct match_with
    {
      typedef T ret;
    };

  } // end of namespace merge2_

  template <typename local_res, typename super_res>
  struct merge2
  {
    // Result.
    typedef typename merge2_::match_with<local_res, super_res>::ret ret;
  };


  // -------------------------------------------------------------- merge3.

  // FIXME: Undef these macros after the definition of merge3.

#define stc_match_0p_with(Local, Super, Deleg, Res)	\
  template <>						\
  struct match_with< Local, Super, Deleg >		\
  {							\
    typedef Res ret;					\
  }

#define stc_match_1p_with(T1, Local, Super, Deleg, Res)	\
  template <typename T1>				\
  struct match_with< Local, Super, Deleg >		\
  {							\
    typedef Res ret;					\
  }

#define stc_match_2p_with(T1, T2, Local, Super, Deleg, Res)	\
  template <typename T1, typename T2>				\
  struct match_with< Local, Super, Deleg >			\
  {								\
    typedef Res ret;						\
  }

#define stc_match_3p_with(T1, T2, T3, Local, Super, Deleg, Res)	\
  template <typename T1, typename T2, typename T3>		\
  struct match_with< Local, Super, Deleg >			\
  {								\
    typedef Res ret;						\
  }

  // FIXME: Example of use.
  // stc_match_with(stc::not_found, stc::not_found, stc::not_found, stc::not_found);

  namespace merge3_
  {
    // Forward declaration.
    template <typename T, typename U, typename V> struct match_with;


    // local_res == stc::not_found.
    template <>
    struct match_with<stc::not_found, stc::not_found, stc::not_found>
    {
      typedef stc::not_found ret;
    };

    template <>
    struct match_with<stc::not_found, stc::not_found, stc::abstract>
    {
      typedef stc::not_found ret;
    };

    template <typename V>
    struct match_with<stc::not_found, stc::not_found, stc::final<V> >
    {
      typedef stc::final<V> ret;
    };

    template <typename V>
    struct match_with<stc::not_found, stc::not_found, V>
    {
      typedef V ret;
    };


    template <>
    struct match_with<stc::not_found, stc::abstract, stc::not_found>
    {
      typedef stc::not_found ret;
    };

    template <>
    struct match_with<stc::not_found, stc::abstract, stc::abstract>
    {
      typedef stc::abstract ret;
    };

    template <typename V>
    struct match_with<stc::not_found, stc::abstract, stc::final<V> >
    {
      typedef stc::final<V> ret;
    };

    template <typename V>
    struct match_with<stc::not_found, stc::abstract, V>
    {
      typedef V ret;
    };


    template <typename U, typename V>
    struct match_with<stc::not_found, stc::final<U>, V>
    {
      typedef stc::final<U> ret;
    };

    template <typename U, typename V>
    struct match_with<stc::not_found, U, V>
    {
      typedef U ret;
    };


    // local_res == stc::abstract.
    template <>
    struct match_with<stc::abstract, stc::not_found, stc::not_found>
    {
      typedef stc::abstract ret;
    };

    template <>
    struct match_with<stc::abstract, stc::not_found, stc::abstract>
    {
      typedef stc::abstract ret;
    };

    template <typename V>
    struct match_with<stc::abstract, stc::not_found, stc::final<V> >
    {
      typedef stc::final<V> ret;
    };

    template <typename V>
    struct match_with<stc::abstract, stc::not_found, V >
    {
      typedef V ret;
    };


    template <>
    struct match_with<stc::abstract, stc::abstract, stc::not_found>
    {
      typedef stc::abstract ret;
    };

    template <>
    struct match_with<stc::abstract, stc::abstract, stc::abstract>
    {
      typedef stc::abstract ret;
    };

    template <typename V>
    struct match_with<stc::abstract, stc::abstract, stc::final<V> >
    {
      typedef stc::final<V> ret;
    };

    template <typename V>
    struct match_with<stc::abstract, stc::abstract, V>
    {
      typedef V ret;
    };


    template <typename V>
    struct match_with<stc::abstract, stc::not_delegated_abstract, V>
    {
      typedef stc::not_delegated_abstract ret;
    };

    template <typename V>
    struct match_with<stc::abstract, stc::not_delegated, V>
    {
      typedef stc::not_delegated_abstract ret;
    };


    template <typename U, typename V>
    struct match_with<stc::abstract, stc::final<U>, V> :
      mlc::abort_<V, stc::ERROR::IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT>
    {
    };

    template <typename U, typename V>
    struct match_with<stc::abstract, U, V> :
      mlc::abort_<V, stc::ERROR::IN_merge3__VIRTUAL_TYPE_REDEFINED_ABSTRACT>
    {
    };


    // local_res == stc::not_delegated.
    template <typename V>
    struct match_with<stc::not_delegated, stc::not_found, V>
    {
      typedef stc::not_delegated ret;
    };

    template <typename V>
    struct match_with<stc::not_delegated, stc::abstract, V>
    {
      typedef stc::not_delegated_abstract ret;
    };

    template <typename V>
    struct match_with<stc::not_delegated, stc::not_delegated_abstract, V>
    {
      typedef stc::not_delegated_abstract ret;
    };


    // local_res == stc::not_delegated_abstract.  *)
    /* FIXME: Shouldn't we introduce a means to tag a vtype both
       as abstract *and* not delegated?  (Currently, the rule below
       prevents this).  */
    template <typename U, typename V>
    struct match_with<stc::not_delegated_abstract, U, V> :
      mlc::abort_<V, stc::ERROR::IN_merge3__LOCAL_DECLARATION_OF_NOT_DELEGATED_AND_ABSTRACT>
    {
    };


    // local_res == stc::final<T>.
    template <typename T, typename U, typename V>
    struct match_with<stc::final<T>, stc::final<U>, V> :
      mlc::abort_<V, stc::ERROR::IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED_FINAL>
    {
    };

    template <typename T, typename U, typename V>
    struct match_with<stc::final<T>, U, V>
    {
      typedef stc::final<T> ret;
    };


    // local_res == T.
    template <typename T, typename U, typename V>
    struct match_with<T, stc::final<U>, V> :
      mlc::abort_<V, stc::ERROR::IN_merge3__FINAL_VIRTUAL_TYPE_REDEFINED>
    {
    };

    template <typename T, typename U, typename V>
    struct match_with
    {
      typedef T ret;
    };

  } // end of namespace merge3_

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

  template <typename source, typename target>
  struct find
  {
    typedef typename find_rec<source, target>::ret res;
    // Result.
    typedef typename find_::match_with<res>::ret ret;
  };

} /* end of SCOOPED_NAMESPACE */


# define stc_find_vtype_(Namespace, Source, Target)			\
   Namespace::find<Source, Namespace::typedef_::Target##_type>::ret

# define stc_find_vtype(Namespace, Source, Target)	\
   typename stc_find_vtype_(Namespace, Source, Target)

#endif // ! STC_SCOOP2_HH
