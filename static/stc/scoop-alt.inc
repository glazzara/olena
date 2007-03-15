								// -*- C++ -*-

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


#ifndef STATIC_SCOOP_ALT_HH_INCLUDED
# error "alt/scoop.inc included but alt/scoop.hh hasn't been included"
#endif


/* \file stc/scoop-alt.inc

   \brief Alternative equipment for SCOOP 2 (in particular, virtual
   types).

   To be included inside the namespace to equip.  \file
   stc/scoop-alt.hh must have been previously included before this
   file.  */


/* Install core virtual types in the equipped namespace.  */
/* \{ */
mlc_decl_typedef(exact_);

mlc_decl_typedef(category);
mlc_decl_typedef(behavior);
mlc_decl_typedef(delegatee);
/* \} */


/*--------.
| Super.  |
`--------*/

/// Default version.
template <typename class_name>
struct super_trait_ :
  mlc::abort_< class_name,
	       stc::ERROR::super_trait_not_defined_for_< class_name > >
{
  typedef mlc::none ret;
};

/// Version for abstractions (security cap).
template <template <class> class abstraction, typename Exact>
struct super_trait_< abstraction<Exact> >
{
  typedef mlc::none ret;
};

/// Helper for const versions.
template <typename class_name>
struct super_trait_ <const class_name> : public super_trait_<class_name>
{
};


/*---------.
| Vtypes.  |
`---------*/

template <typename class_name>
struct vtypes
{
};

/// Helper for const versions.
template <typename class_name>
struct vtypes <const class_name> : public vtypes <class_name>
{
};

template <typename class_name, typename type_name>
struct single_vtype
{
  typedef mlc::not_found ret;
};



/*------.
| Any.  |
`------*/

// FIXME: Get rid of stc/any.hh?

template <typename Exact>
struct Any;

template <typename Exact>
struct super_trait_< Any<Exact> >
{
  typedef mlc::none ret;
};

template <typename Exact>
struct vtypes< Any<Exact> >
{
  typedef stc::final<Exact> exact_;
};

template <typename Exact>
struct Any
{
  typedef Exact exact_;
protected:
  Any() {}
};


/*--------.
| Exact.  |
`--------*/

template <typename Exact>
Exact& exact(Any<Exact>& ref)
{
  return *(Exact*)(void*)(&ref);
}

template <typename Exact>
const Exact& exact(const Any<Exact>& cref)
{
  return *(const Exact*)(const void*)(&cref);
}


template <typename Exact>
Exact* exact(Any<Exact>* ptr)
{
  return (Exact*)(void*)(ptr);
}

template <typename Exact>
const Exact* exact(const Any<Exact>* cptr)
{
  return (const Exact*)(const void*)(cptr);
}

template <typename Exact>
const Exact* my_exact(const Any<Exact>* cptr)
{
  return (const Exact*)(const void*)(cptr);
}


/*----------------------.
| Virtual type lookup.  |
`----------------------*/

/* ----------------------------------------------------- find_local.  */

/* Highly inspired from Théo's get_stm!  */

namespace find_local_
{
  /* Forward declaration.  */
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
    mlc::abort_<T,
		  stc::ERROR::IN_find_local_VIRTUAL_TYPE_MULTIPLY_DEFINED>
  {
  };

} /* End of namespace find_local_.  */

template <typename source, typename target>
struct find_local
{
  typedef vtypes<source> decl1;
  typedef typename target::template from_<decl1>::ret res1;

  typedef single_vtype<source, target> decl2;
  typedef typename decl2::ret ret2;


  /* Result.  */
  typedef typename find_local_::match_with<res1, ret2>::ret ret;
};


/* --------------------------------------------------------- merge2.  */


namespace merge2_
{
  /* Forward declaration.  */
  template <typename T, typename U> struct match_with;


  /* ----------------------------- */
  /* local_res == stc::not_found.  */
  /* ----------------------------- */

  stc_internal_match_pair_0p_with(stc::not_found, stc::not_found,
				    stc::not_found);

  stc_internal_match_pair_0p_with(stc::not_found, stc::abstract,
				    stc::not_found);

  stc_internal_match_pair_1p_with(U,
				    stc::not_found, stc::final<U>,
				    stc::final<U>);

  stc_internal_match_pair_1p_with(U,
				    stc::not_found, U,
				    U);


  /* ---------------------------- */
  /* local_res == stc::abstract.  */
  /* ---------------------------- */

  stc_internal_match_pair_0p_with(stc::abstract, stc::not_found,
				    stc::abstract);

  stc_internal_match_pair_0p_with(stc::abstract, stc::abstract,
				    stc::abstract);

  stc_internal_match_pair_1p_with_error(U,
    stc::abstract, stc::final<U>,
    stc::ERROR::IN_merge2_FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT);

  stc_internal_match_pair_1p_with_error(U,
    stc::abstract, U,
    stc::ERROR::IN_merge2_VIRTUAL_TYPE_REDEFINED_ABSTRACT);


  /* ---------------------------- */
  /* local_res == stc::final<T>.  */
  /* ---------------------------- */

  stc_internal_match_pair_1p_with(T,
				    stc::final<T>, stc::not_found,
				    stc::final<T>);

  stc_internal_match_pair_1p_with(T,
				    stc::final<T>, stc::abstract,
				    stc::final<T>);

  stc_internal_match_pair_2p_with_error(T, U,
    stc::final<T>, stc::final<U>,
    stc::ERROR::IN_merge2_FINAL_VIRTUAL_TYPE_REDEFINED_FINAL);

  stc_internal_match_pair_2p_with(T, U,
				    stc::final<T>, U,
				    stc::final<T>);


  /* ---------------- */
  /* local_res == T.  */
  /* ---------------- */

  stc_internal_match_pair_1p_with(T,
				    T, stc::not_found,
				    T);

  stc_internal_match_pair_1p_with(T,
				    T, stc::abstract,
				    T);

  stc_internal_match_pair_2p_with_error(T, U,
    T, stc::final<U>,
    stc::ERROR::IN_merge2_FINAL_VIRTUAL_TYPE_REDEFINED);

  template <typename T, typename U>
  struct match_with
  {
    typedef T ret;
  };

} /* End of namespace merge2_.  */

template <typename local_res, typename super_res>
struct merge2
{
  /* Result.  */
  typedef typename merge2_::match_with<local_res, super_res>::ret ret;
};


/* --------------------------------------------------------- merge3.  */


namespace merge3_
{
  /* Forward declaration.  */
  template <typename T, typename U, typename V> struct match_with;


  /* ----------------------------- */
  /* local_res == stc::not_found.  */
  /* ----------------------------- */

  /* super_res == stc::not_found.  */
  stc_internal_match_triple_0p_with(
    stc::not_found, stc::not_found, stc::not_found,
    stc::not_found);

  stc_internal_match_triple_0p_with(
    stc::not_found, stc::not_found, stc::abstract,
    stc::not_found);

  stc_internal_match_triple_1p_with(V,
    stc::not_found, stc::not_found, stc::final<V>,
    stc::final<V>);

  stc_internal_match_triple_1p_with(V,
    stc::not_found, stc::not_found, V,
    V);

  /* super_res == stc::abstract.  */
  stc_internal_match_triple_0p_with(
    stc::not_found, stc::abstract, stc::not_found,
    stc::not_found);

  stc_internal_match_triple_0p_with(
    stc::not_found, stc::abstract, stc::abstract,
    stc::abstract);

  stc_internal_match_triple_1p_with(V,
    stc::not_found, stc::abstract, stc::final<V>,
    stc::final<V>);

  stc_internal_match_triple_1p_with(V,
    stc::not_found, stc::abstract, V,
    V);

  /* super_res is a concrete type.  */
  stc_internal_match_triple_2p_with(U, V,
    stc::not_found, stc::final<U>, V,
    stc::final<U>);

  stc_internal_match_triple_2p_with(U, V,
    stc::not_found, U, V,
    U);


  /* ---------------------------- */
  /* local_res == stc::abstract.  */
  /* ---------------------------- */

  /* super_res == stc::not_found.  */
  stc_internal_match_triple_0p_with(
    stc::abstract, stc::not_found, stc::not_found,
    stc::abstract);

  stc_internal_match_triple_0p_with(
    stc::abstract, stc::not_found, stc::abstract,
    stc::abstract);

  stc_internal_match_triple_1p_with(V,
    stc::abstract, stc::not_found, stc::final<V>,
    stc::final<V>);

  stc_internal_match_triple_1p_with(V,
    stc::abstract, stc::not_found, V,
    V);


  /* super_res == stc::abstract.  */
  stc_internal_match_triple_0p_with(
    stc::abstract, stc::abstract, stc::not_found,
    stc::abstract);

  stc_internal_match_triple_0p_with(
    stc::abstract, stc::abstract, stc::abstract,
    stc::abstract);

  stc_internal_match_triple_1p_with(V,
    stc::abstract, stc::abstract, stc::final<V>,
    stc::final<V>);

  stc_internal_match_triple_1p_with(V,
    stc::abstract, stc::abstract, V,
    V);


  /* super_res == stc::not_delegated_abstract.  */
  stc_internal_match_triple_1p_with(V,
    stc::abstract, stc::not_delegated_abstract, V,
    stc::not_delegated_abstract);

  stc_internal_match_triple_1p_with(V,
    stc::abstract, stc::not_delegated, V,
    stc::not_delegated_abstract);


  /* super_res is a concrete type.  */
  stc_internal_match_triple_2p_with_error(U, V,
    stc::abstract, stc::final<U>, V,
    stc::ERROR::IN_merge3_FINAL_VIRTUAL_TYPE_REDEFINED_ABSTRACT);

  stc_internal_match_triple_2p_with_error(U, V,
    stc::abstract, U, V,
    stc::ERROR::IN_merge3_VIRTUAL_TYPE_REDEFINED_ABSTRACT);


  /* --------------------------------- */
  /* local_res == stc::not_delegated.  */
  /* --------------------------------- */

  stc_internal_match_triple_1p_with(V,
			     stc::not_delegated, stc::not_found, V,
			     stc::not_delegated);

  stc_internal_match_triple_1p_with(V,
			     stc::not_delegated, stc::abstract, V,
			     stc::not_delegated_abstract);

  stc_internal_match_triple_1p_with(V,
    stc::not_delegated, stc::not_delegated_abstract, V,
    stc::not_delegated_abstract);


  /* ------------------------------------------ */
  /* local_res == stc::not_delegated_abstract.  */
  /* ------------------------------------------ */

  /* FIXME: Shouldn't we introduce a means to tag a vtype both as */
  /* abstract *and* not delegated?  (Currently, the rule below    */
  /* prevents this).                                              */
  stc_internal_match_triple_2p_with_error(U, V,
    stc::not_delegated_abstract, U, V,
    stc::ERROR::IN_merge3_LOCAL_DECLARATION_OF_NOT_DELEGATED_AND_ABSTRACT);


  /* ---------------------------- */
  /* local_res == stc::final<T>.  */
  /* ---------------------------- */

  stc_internal_match_triple_3p_with_error(T, U, V,
    stc::final<T>, stc::final<U>, V,
    stc::ERROR::IN_merge3_FINAL_VIRTUAL_TYPE_REDEFINED_FINAL);

  stc_internal_match_triple_3p_with(T, U, V,
			     stc::final<T>, U, V,
			     stc::final<T>);


  /* ---------------- */
  /* local_res == T.  */
  /* ---------------- */

  stc_internal_match_triple_3p_with_error(T, U, V,
    T, stc::final<U>, V,
    stc::ERROR::IN_merge3_FINAL_VIRTUAL_TYPE_REDEFINED);

  template <typename T, typename U, typename V>
  struct match_with
  {
    typedef T ret;
  };

} /* End of namespace merge3_.  */


template <typename local_res, typename super_res, typename delegatee_res>
struct merge3
{
  /* Result.  */
  typedef typename
  merge3_::match_with<local_res, super_res, delegatee_res>::ret ret;
};


/* ------------------------------------------------------  find_rec.  */

/* Forward declarations.  */
template <typename source, typename target> struct find_rec;
template <typename source, typename target> struct find_rec_in_supers;


namespace find_rec_
{

  namespace find_delegatee_res_
  {
    /* Forward declaration.  */
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

  } /* End of namespace find_delegatee_res_.  */


  /* Forward declaration.  */
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
    /* delegatee is the name of the (optional) virtual type  */
    /* containing the (type of the) delgatee.                */
    typedef typename
    find_rec_in_supers<source, typedef_::delegatee>::ret delegatee;

    typedef typename
    find_delegatee_res_::match_with<delegatee, target>::ret delegatee_res;
    typedef typename
    merge3<local_res, super_res, delegatee_res>::ret ret;
  };

} /* End of namespace find_rec_.  */

template <typename source, typename target>
struct find_rec
{
  /* Result.  */
  typedef typename find_rec_::match_with<source, target>::ret ret;
};


/* --------------------------------------------- find_rec_in_supers.  */

namespace find_rec_in_supers_
{
  /* Forward declaration.  */
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

} /* End of namespace find_rec_in_supers_.  */

template <typename source, typename target>
struct find_rec_in_supers
{
  /* Result.  */
  typedef typename
  find_rec_in_supers_::match_with<source, target>::ret ret;
};


/* ----------------------------------------------------------- find.  */

namespace find_
{
  /* Forward declaration.  */
  template <typename T> struct match_with;

  /* FIXME: We'd like to add a static abort here, but we can't, */
  /* since stc::abstract is not a free parameter (enabling this */
  /* abort statement would prevent any compilation).            */
  template <>
  struct match_with<stc::abstract>
  /* : mlc::abort_<stc::abstract,                                */
  /*               stc::ERROR::IN_find_VIRTUAL_TYPE_IS_ABSTRACT> */
  {
  };

  /* FIXME: We'd like to add a static abort here, but we can't, */
  /* since stc::not_delegated_abstract is not a free parameter  */
  /* (enabling this abort statement would prevent any           */
  /* compilation).                                              */
  template <>
  struct match_with<stc::not_delegated_abstract>
  /* : mlc::abort_<stc::not_delegated_abstract,                  */
  /*               stc::ERROR::IN_find_VIRTUAL_TYPE_IS_ABSTRACT> */
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

} /* End of namespace find_.  */


/** Find a virtual type.  */
template <typename source, typename target>
struct find_vtype
{
  typedef typename find_rec<source, target>::ret res;
  /* Result.  */
  typedef typename find_::match_with<res>::ret ret;
};


/** Find a virtual type, and ensure it is found.  */
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


/*------------.
| Selectors.  |
`------------*/

// FIXME: This part has been copied as-is from Théo's version
// (oln/stc/scoop.hxx).

mlc_case_equipment_for_namespace(internal);


namespace internal
{


  template < template <class> class abstraction,
	       unsigned num = 1 >
  struct selector
  {
  protected: selector() {}
  };


  /* fwd decl */
  template < template <class> class abstraction, typename E, unsigned num >
  struct plug_node;

  template < template <class> class abstraction,
	       typename E,
	       unsigned num,
	       typename another_selector = mlc::false_ >
  struct next_plug_node
  {
    /* here: no other selector */
  protected: next_plug_node() {}
  };

  template < template <class> class abstraction,
	       typename E >
  struct next_plug_node < abstraction, E, 1, mlc::false_ >
    : public abstraction<E>
  {
  protected: next_plug_node() {}
  };


  template < template <class> class abstraction,
	       typename E,
	       unsigned num >
  struct next_plug_node < abstraction,
			    E,
			    num,
			    mlc::true_>

  : /* plug to client selectors */
    public virtual switch_< selector<abstraction, num>, E >::ret,

    /* here: another selector (number is 'num + 1') */
    public plug_node<abstraction, E, num + 1>
  {
  protected: next_plug_node() {}
  };


  template < template <class> class abstraction,
	       typename E,
	       unsigned num >
  struct plug_node
    : public next_plug_node< abstraction,
			       E,
			       num,
			       typename mlc::is_defined_< case_< selector<abstraction, num>,
								 E, 1 > >::eval >
  {
  protected: plug_node() {}
  };


  template < template <class> class abstraction,
	       unsigned num,
	       typename E >
  struct default_case_ < selector<abstraction, num>,
			   E >
  {
    typedef abstraction<E> ret;
  };


  template < template <class> class abstraction,
	       typename E >
  struct plug : public plug_node<abstraction, E, 1>
  {
  protected: plug() {}
  };

  template <typename abstraction, typename E>
  struct top__;

  template <template<class> class abstraction, typename E>
  struct top__ < stc::is<abstraction>, E > : public plug< abstraction, E >
  {
  protected: top__() {}
  };

  template <typename E>
  struct top__ < mlc::none, E > : public Any<E>
  {
  protected: top__() {}
  };

  template <typename E>
  struct top__ < mlc::not_found, E >; /* FIXME: Error msg here */


} /* End of namespace internal.  */


/*--------------------------------------------.
| Top class of the implementation hierarchy.  |
`--------------------------------------------*/

// FIXME: This part has been copied as-is from Théo's version
// (oln/stc/scoop.hxx).

template <typename E> struct top;

template <typename E>
struct vtypes< top<E> >
{
  typedef stc::final<E> exact;
  /* default is "no category" */
};

template <typename E>
struct top : public internal::top__< stc_find_type(E, category), E >
{
protected:
  top() {}
};


/*----------------------------.
| Automatic implementations.  |
`----------------------------*/

// FIXME: This part has been copied (almost) as-is from Théo's version
// (oln/stc/scoop.hxx).

namespace automatic
{

  /// set_impl
  ///
  /// To be defined by the client.
  template < template <class> class abstraction, typename behavior,
	     typename E >
  struct set_impl;

  
  /// impl
  /// \{
  template < template <class> class abstraction, typename behavior,
	     typename E >
  struct impl : public set_impl< abstraction, behavior, E >
  { /* fetch */ };

  template < template <class> class abstraction, typename E >
  struct impl< abstraction, /* behavior is */ mlc::not_found, E >
  { /* nothing */ };

  template < template <class> class abstraction, typename E >
  struct impl< abstraction, mlc::none /* behavior */, E >
  { /* nothing */ };
  /// \}


  /// get_impl
  template < template <class> class abstraction, typename E >
  struct get_impl : impl< abstraction, stc_find_type(E, behavior), E >
  { /* depends upon behavior */ };

} // End of namespace automatic.
