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
   \brief Virtual types (also known as ``properties'') mechanism.

   Based on Theo's presentation from January 2006 (olena-06-jan.pdf).  */

#ifndef STATIC_VTYPES_HH
# define STATIC_VTYPES_HH

# include <mlc/flags.hh>
# include <mlc/typedef.hh>
# include <mlc/ret.hh>

# include <mlc/assert.hh>
# include <mlc/abort.hh>

# include <mlc/bool.hh>
# include <mlc/int.hh>

# include <mlc/cmp.hh>
# include <mlc/if.hh>
# include <mlc/is_a.hh>

# include <mlc/comma.hh>
# include <mlc/valist.hh>
# include <stc/valist_aggregator.hh>

# include <stc/exact.hh>
# include <stc/internal/extract_vtype_from_list.hh>


/*-----------------.
| Error messages.  |
`-----------------*/

namespace stc
{
  /// Error messages raised by static assertions.
  namespace ERROR
  {

    struct PARAMETER_OF_get_super_types_helper_IS_NOT_A_VALID_VALIST;

    struct FIRST_PARAMETER_OF_rec_get_vtype_IS_NOT_A_TAG;

    struct FIRST_PARAMETER_OF_rec_get_vtype_from_list_IS_NOT_A_TAG;
    struct THIRD_PARAMETER_OF_rec_get_vtype_from_list_IS_NOT_A_LIST;

  } // end of namespace stc::ERROR

} // end of namespace stc



/*------------.
| Equipment.  |
`------------*/

/// Internal macros, not to be used by the client.
/// \{

// Note: TypedefName *must* be of the form `typedef_::foo'.
# define stc_internal_get_typedef(Type, TypedefName) \
   typename TypedefName::template from_< Type >::ret

# define stc_internal_maybe_get_nth_super_type(Type, Nth)	\
   typename internal::maybe_get_super_type< Type, Nth >::ret

# define stc_internal_rec_get_vtype(Type)				\
   typename rec_get_vtype< method, category, Type, typedef_type >::ret

/// \}


// FIXME: Complete support for hierarchies with several super classes.

// FIXME: Could we extract some classes from this gigantic macro?

# define stc_equip_namespace_with_vtypes()				      \
									      \
  /* ----------------------- */						      \
  /* Typedefs declarations.  */						      \
  /* ----------------------- */						      \
									      \
  /* Declare the ``uplink'' typedef (access to a pseudosuper class).  */      \
  mlc_decl_typedef(pseudosuper_type);					      \
									      \
									      \
  /* ------------- */							      \
  /* Inheritance.  */							      \
  /* ------------- */							      \
									      \
  /* Set super classes.  */						      \
									      \
  template<typename type, unsigned N = 1>				      \
  struct set_super_type : public mlc::undefined				      \
  {									      \
    typedef mlc::none ret;						      \
  };									      \
									      \
									      \
  /* Get supers list.  */						      \
									      \
  namespace internal							      \
  {									      \
    template <typename type, unsigned N>				      \
    struct maybe_get_super_type						      \
    {									      \
      typedef typename							      \
      mlc::if_< mlc_is_a( mlc_comma_1( set_super_type<type, N> ),	      \
			  mlc::undefined ),				      \
		mlc::internal::valist_none,				      \
		mlc_ret(mlc_comma_1(set_super_type< type, N >)) >::ret	      \
      ret;								      \
    };									      \
  } /* end of namespace internal */					      \
									      \
  template <typename type>						      \
  struct get_supers_list						      \
  {									      \
    /* FIXME: Factor with a loop macro?  */				      \
    typedef stc_internal_maybe_get_nth_super_type(type, 1) s1;		      \
    typedef stc_internal_maybe_get_nth_super_type(type, 2) s2;		      \
    typedef stc_internal_maybe_get_nth_super_type(type, 3) s3;		      \
    typedef stc_internal_maybe_get_nth_super_type(type, 4) s4;		      \
    typedef stc_internal_maybe_get_nth_super_type(type, 5) s5;		      \
    typedef stc_internal_maybe_get_nth_super_type(type, 6) s6;		      \
    typedef stc_internal_maybe_get_nth_super_type(type, 7) s7;		      \
    typedef stc_internal_maybe_get_nth_super_type(type, 8) s8;		      \
    typedef stc_internal_maybe_get_nth_super_type(type, 9) s9;		      \
									      \
    typedef mlc::valist_<s1, s2, s3, s4, s5, s6, s7, s8, s9> ret;	      \
  };									      \
									      \
									      \
  /* Get super classes.  */						      \
									      \
  namespace internal							      \
  {									      \
									      \
    /** Accessor helper.  */						      \
    template<typename type>						      \
    struct get_super_types_helper;					      \
									      \
    /** Accessor helper: specialization for the case of a single */	      \
    /** super class.                                             */	      \
    template<typename super>						      \
    struct get_super_types_helper< mlc::valist_ <super> >		      \
    {									      \
      /** Return the super class directly.  */				      \
      typedef super ret;						      \
    };									      \
									      \
    /** Accessor helper: specializations for the case of two super */	      \
    /** classes or more.                                           */	      \
    template <typename super1, typename super2, typename super3>	      \
    struct get_super_types_helper< mlc::valist_ <super1, super2, super3> >    \
    {									      \
      /** Return an aggregate of the super classes.  */			      \
      typedef stc::valist_aggregator< mlc::valist_ <super1, super2, super3> > \
      ret;								      \
    };									      \
									      \
    /** Accessor helper: default case (abort).  */			      \
    template<typename type>						      \
    struct get_super_types_helper :					      \
      public mlc::abort_<						      \
        type,								      \
        stc::ERROR::PARAMETER_OF_get_super_types_helper_IS_NOT_A_VALID_VALIST \
      >									      \
    {									      \
    };									      \
									      \
  } /* end of namespace internal */					      \
									      \
									      \
  template<typename type>						      \
  struct get_super_types						      \
  {									      \
    typedef typename get_supers_list<type>::ret supers_list;		      \
    typedef typename internal::get_super_types_helper<supers_list>::ret ret;  \
  };									      \
									      \
									      \
  /* ------------------------ */					      \
  /* ``Pseudo'' inheritance.  */					      \
  /* ------------------------ */					      \
									      \
  template <typename type>						      \
  struct set_pseudosuper_type						      \
  {									      \
    typedef mlc::none ret;						      \
  };									      \
									      \
									      \
  /* --------------- */							      \
  /* Virtual types.  */							      \
  /* --------------- */							      \
									      \
  /** \brief Internal virtual types associated to \a from_type. */	      \
  /**								*/	      \
  /** Specialize this class for the desired \a from_type.	*/	      \
  template <typename category, typename from_type>			      \
  struct vtypes								      \
  {									      \
  };									      \
									      \
  /** End of the recursive construction of any vtypes hierarchy.  */	      \
  template <typename category>						      \
  struct vtypes<category, mlc::none>					      \
  {									      \
  };									      \
									      \
  /** \brief An external virtual type associated to \a from_type. */	      \
  /**								  */	      \
  /** Specialize this class for the desired \a from_type.	  */	      \
  template <typename category, typename from_type, typename typedef_type>     \
  struct ext_vtype							      \
  {									      \
  };									      \
									      \
  /** End of the recursive construction of any ext_vtype<> */		      \
  /** hierarchy.                                           */		      \
  template <typename category, typename typedef_type>			      \
  struct ext_vtype<category, mlc::none, typedef_type>			      \
  {									      \
  };									      \
									      \
  /** Optional packing structure, to be specialized by the user.  */	      \
  /** See tests/vtypes.hh for an example of use.              */	      \
  template <typename category, typename from_type>			      \
  struct packed_vtypes							      \
  {									      \
  };									      \
									      \
									      \
  /* -------------------- */						      \
  /* Internal machinery.  */						      \
  /* -------------------- */						      \
									      \
  /** The classes enclosed in this namespace must not be specialized */	      \
  /** by the user (they are part of the automatic associated types   */	      \
  /** retrieval mechanism).                                          */	      \
  namespace internal							      \
  {									      \
    /* ------------------------------------------ */			      \
    /* Recursive retrieval of an internal vtype.  */			      \
    /* ------------------------------------------ */			      \
									      \
    /** Tags for retrieval methods.  */					      \
    namespace tag							      \
    {									      \
									      \
      /** Abstraction for method tags.  */				      \
      struct method {};							      \
									      \
      /** Tag for retrieval within internal vtypes.  */			      \
      struct internal : public method {};				      \
      /** Tag for retrieval within external vtypes.  */			      \
      struct external : public method {};				      \
									      \
    } /** end of stc::internal::tag */					      \
									      \
    /** Try to get \a typedef_type from \a from_type.  In case this */	      \
    /** typedef is mlc::not_found, don't perform a recursive        */	      \
    /** retrieval, simply return mlc::not_found.                    */	      \
    template <typename method, typename category,			      \
	      typename from_type, typename typedef_type>		      \
    struct get_vtype_helper						      \
    {									      \
      /* Nothing (no method selected).  */				      \
    };									      \
									      \
    /** Specialization of get_vtypes for retrievals within */		      \
    /** internal vtypes.                                   */		      \
    template <typename category,					      \
	      typename from_type, typename typedef_type>		      \
    struct get_vtype_helper<tag::internal, category,			      \
			    from_type, typedef_type>			      \
    {									      \
      /** Set of vtypes associated with FROM_TYPE.  */			      \
      typedef vtypes<category, from_type> types;			      \
      /** Typedef in the current vtypes (may be mlc::not_found).  */	      \
      typedef stc_internal_get_typedef(types, typedef_type) ret;	      \
    };									      \
									      \
    /** Specialization of get_vtypes for retrievals within */		      \
    /** external vtypes.                                   */		      \
    template <typename category,					      \
	      typename from_type, typename typedef_type>		      \
    struct get_vtype_helper<tag::external, category,			      \
			    from_type, typedef_type>			      \
    {									      \
      /** Set of vtypes associated with FROM_TYPE.  */			      \
      typedef ext_vtype<category, from_type, typedef_type> ext_type;	      \
      /** Typedef in the current vtypes (may be mlc::not_found).  */	      \
      typedef mlc_ret(ext_type) ret;					      \
    };									      \
									      \
									      \
    /* FIXME: Do a basic scheme of the algorithm in pseudo-code.  */	      \
									      \
    /* FIXME: Check for mlc::undefined?  */				      \
									      \
    /* Forward declaration.  */						      \
    template <typename method, typename category,			      \
	      typename from_list, typename typedef_type>		      \
    struct rec_get_vtype_from_list;					      \
									      \
    /** Recursive retrieval of the virtual type \a typedef_type inside */     \
    /** the class \a from_type.                                        */     \
    template <typename method, typename category,			      \
	      typename from_type, typename typedef_type>		      \
    struct rec_get_vtype :						      \
      private mlc::assert_< mlc_is_a(method, tag::method),		      \
        stc::ERROR::FIRST_PARAMETER_OF_rec_get_vtype_IS_NOT_A_TAG >	      \
    {									      \
      /** Get potential vtype \a typedef_type from the current class */	      \
      /** (\a from_type).                                            */	      \
      typedef typename							      \
	get_vtype_helper< method, category, from_type, typedef_type>::ret     \
	type;								      \
									      \
      /** Implicit parents (i.e. supers), if any.  */			      \
      typedef typename get_supers_list<from_type>::ret supers_list;	      \
      /** Vtype deduced from the vtypes of the base class(es) of */	      \
      /** \a from_type, if any.                                  */	      \
      typedef typename							      \
	rec_get_vtype_from_list< method, category,			      \
				 supers_list, typedef_type >::ret	      \
	vtype_from_supers;						      \
									      \
      /** Pseudosuper class, if any.  */				      \
      typedef stc_get_pseudosuper(from_type) pseudosuper;		      \
      /** Vtype deduced from the vtype of the pseudo super class of */	      \
      /** \a from_type, if any.                                     */	      \
      typedef typename							      \
	rec_get_vtype< method, category, pseudosuper, typedef_type >::ret     \
	vtype_from_pseudo_super;					      \
									      \
      /* Core of the search algorithm.  */				      \
      typedef typename							      \
      mlc::if_<								      \
	mlc::neq_< type, mlc::not_found >,				      \
	/* then	*/							      \
	/*   return it					*/		      \
	/*   (the typedef has been found in the vtypes	*/		      \
	/*   associated to FROM_TYPE)			*/		      \
	type,								      \
	/* else	*/							      \
	/*   check if the vtypes of the `supers' of FROM_TYPE */	      \
	/*   has the typedef				      */	      \
	typename							      \
	mlc::if_< mlc::neq_< vtype_from_supers, mlc::not_found >,	      \
		  /* then */						      \
		  /*   return it */					      \
		  vtype_from_supers,					      \
		  /* else */						      \
		  /*   check if the FROM_TYPE has a pseudo super */	      \
		  /*   and try to retrieve the typedef from it.  */	      \
		  vtype_from_pseudo_super >::ret >::ret			      \
	ret;								      \
    };									      \
									      \
    /** Ends of the recursive retrieval (mlc::none is at the end of the */    \
    /** transitive closure of every `super' relation).			*/    \
    /** \{ */								      \
    /** Case where \a from_type = mlc::none (end of a recursive	*/	      \
    /** retrieval following `super' types).			*/	      \
    template <typename method, typename category, typename typedef_type>      \
    struct rec_get_vtype<method, category, mlc::none, typedef_type> :	      \
      private mlc::assert_< mlc_is_a(method, tag::method),		      \
        stc::ERROR::FIRST_PARAMETER_OF_rec_get_vtype_IS_NOT_A_TAG >	      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
    /** Case where \a from_type = mlc::not_found (end of a recursive */	      \
    /** retrieval following `super' types).			     */	      \
    template <typename method, typename category, typename typedef_type>      \
    struct rec_get_vtype<method, category, mlc::not_found, typedef_type> :    \
      private mlc::assert_< mlc_is_a(method, tag::method),		      \
        stc::ERROR::FIRST_PARAMETER_OF_rec_get_vtype_IS_NOT_A_TAG >	      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
    /** \} */								      \
									      \
									      \
    /** Recursive retrieval of vtype \a typedef_type inside */		      \
    /** \a from_list.                                       */		      \
    /** \{ */								      \
									      \
    /* Default case: \a from_type is not a mlc::valist_, abort.  */	      \
    template <typename method, typename category,			      \
	      typename from_list, typename typedef_type>		      \
    struct rec_get_vtype_from_list :					      \
      private mlc::abort_< from_list,					      \
       stc::ERROR::THIRD_PARAMETER_OF_rec_get_vtype_from_list_IS_NOT_A_LIST > \
    {									      \
    };									      \
									      \
    /** Case where the list is empty.  Suprisingly, the general    */	      \
    /**	specialization for mlc::valist<e1, ..., e9> is not enough: */	      \
    /**	this specialization is needed too.                         */	      \
    template <typename method, typename category, typename typedef_type>      \
    struct rec_get_vtype_from_list<					      \
      method, category, mlc::valist_<>, typedef_type>			      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
									      \
    /* Case where \a from_list is a genuine list.  */			      \
    template <typename method, typename category,			      \
	      typename e1, typename e2, typename e3,			      \
	      typename e4, typename e5, typename e6,			      \
	      typename e7, typename e8, typename e9,			      \
	      typename typedef_type>					      \
    struct rec_get_vtype_from_list<					      \
      method, category,							      \
      mlc::valist_<e1, e2, e3, e4, e5, e6, e7, e8, e9>, typedef_type	      \
    > :									      \
      private mlc::assert_< mlc_is_a(method, tag::method),		      \
        stc::ERROR::FIRST_PARAMETER_OF_rec_get_vtype_from_list_IS_NOT_A_TAG > \
    {									      \
      /* For each item of the list \a from_list, perform a */		      \
      /* rec_get_vtype search.                             */		      \
      /* FIXME: Factor with a loop macro?  */				      \
      typedef stc_internal_rec_get_vtype(e1) res1;			      \
      typedef stc_internal_rec_get_vtype(e2) res2;			      \
      typedef stc_internal_rec_get_vtype(e3) res3;			      \
      typedef stc_internal_rec_get_vtype(e4) res4;			      \
      typedef stc_internal_rec_get_vtype(e5) res5;			      \
      typedef stc_internal_rec_get_vtype(e6) res6;			      \
      typedef stc_internal_rec_get_vtype(e7) res7;			      \
      typedef stc_internal_rec_get_vtype(e8) res8;			      \
      typedef stc_internal_rec_get_vtype(e9) res9;			      \
									      \
      /* Then, create a list from the results.  */			      \
      typedef								      \
      mlc::valist_<res1, res2, res3, res4, res5, res6, res7, res8, res9>      \
      res_list;								      \
									      \
      /* Finally, match this list against a set of valid patterns. */	      \
      /* If the match fails, return mlc::not_found.             */	      \
      typedef typename							      \
	stc::internal::extract_vtype_from_list<res_list>::ret ret;	      \
    };									      \
									      \
									      \
    /* ------------------------------------- */				      \
    /* External/internal typedef selection.  */				      \
    /* ------------------------------------- */				      \
									      \
    /** \brief Typedef selector.                                        */    \
    /**                                                                 */    \
    /** A virtual type is considered valid if and only if it has been   */    \
    /** found as an internal vtype or (exclusive) as an external vtype. */    \
    /** Other cases (no definition or a double definition) are invalid. */    \
    /**                                                                 */    \
    /** \{                                                              */    \
    /** Fwd decl.  */							      \
    template <bool external_typedef_p, bool internal_typedef_p,		      \
	      typename external_typedef, typename internal_typedef>	      \
    struct select_typedef;						      \
									      \
    /** The typedef is found in both an external and an internal */	      \
    /** type definitions: error.                                 */	      \
    template <typename external_typedef, typename internal_typedef>	      \
    struct select_typedef<true, true, external_typedef, internal_typedef>     \
    {									      \
      /* No ret member.  */						      \
    };									      \
									      \
    /** The typedef is found neither in an external nor in an */	      \
    /** internal type definition: error.                      */	      \
    template <typename external_typedef, typename internal_typedef>	      \
    struct select_typedef<false, false, external_typedef, internal_typedef>   \
    {									      \
      /* No ret member.  */						      \
    };									      \
									      \
    /** The typedef is found in an extternal definition only: good.  */	      \
    template <typename external_typedef, typename internal_typedef>	      \
    struct select_typedef<true, false, external_typedef, internal_typedef>    \
    {									      \
      typedef external_typedef ret;					      \
    };									      \
									      \
    /** The typedef is found in an internal definition only: good.  */	      \
    template <typename external_typedef, typename internal_typedef>	      \
    struct select_typedef<false, true, external_typedef, internal_typedef>    \
    {									      \
      typedef internal_typedef ret;					      \
    };									      \
    /** \} */								      \
									      \
  } /** End of namespace internal.  */					      \
									      \
									      \
  /** Entry point of the vtype retrieval algorithm.  */			      \
  template <typename category, typename from_type, typename typedef_type>     \
  struct type_of_							      \
  {									      \
    /* Get the exact type of \a from_type.  */				      \
    typedef typename stc::to_exact_<from_type>::ret from_exact_type;	      \
									      \
    /* Look for the typedef in internal types.  */			      \
    typedef typename							      \
    internal::rec_get_vtype<internal::tag::internal, category,		      \
                            from_exact_type, typedef_type>::ret		      \
    internal_typedef;							      \
    /* Look for the typedef as an external type.  */			      \
    typedef typename							      \
    internal::rec_get_vtype<internal::tag::external, category,		      \
                            from_exact_type, typedef_type>::ret		      \
    external_typedef;							      \
									      \
    /* Did we found the virtual type?  */				      \
    static const bool found_internal_p =				      \
      mlc_bool(mlc::is_found_<internal_typedef>);			      \
    static const bool found_external_p =				      \
      mlc_bool(mlc::is_found_<external_typedef>);			      \
									      \
    typedef typename							      \
    internal::select_typedef<found_external_p, found_internal_p,	      \
			     external_typedef, internal_typedef>::ret ret;    \
    };									      \
									      \
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


/*---------------------.
| Abstraction helper.  |
`---------------------*/

namespace stc
{
  /// Allow the manipulation of an abstraction (i.e., a template type)
  /// as a plain type.
  template <template <typename> class abstraction>
  struct abstraction_as_type
  {
    template <typename E>
    struct instantiated_with
    {
      typedef abstraction<E> ret;
    };
  };
} // end of namespace stc


/*---------.
| Macros.  |
`---------*/

// ------------- //
// Inheritance.  //
// ------------- //

// Set.

/* FIXME: I don't know whether these macros will be really usable; what
   if Type is a template class?  We would have to provide additional
   versions of these macros, with support for one parameter, two
   parameters, etc.  */
/// \def Declare the immediate base class \a Super of \a Type.
#define stc_set_nth_super(Type, N, Super)	\
  template<>					\
  struct set_super_type< Type, N >		\
  {						\
    typedef Super ret;				\
  };

// Shortcut.
#define stc_set_super(Type, Super)		\
  stc_set_nth_super(Type, 1, Super)


// Get.

// FIXME: The error message given by the compiler is not explicit
// when trying to use stc_get_supers on a class which has no
// (declared) super class.  Improve this.

/** \a stc_get_supers(Type) returns a class which inerits from all the
    classes declared as super types of \a Type.
  
    However, there is an exception when Type as single super:
    stc_get_supers returns this super class directly, not a class
    ineriting from it, to avoid a useless indirection.  */
#define stc_get_supers(Type)			\
  get_super_types< Type >::ret

/// \def Get the Nth immediate base class(es) of Type (version to be
/// used inside a template).
# define stc_get_nth_super(Type, Nth)		\
   typename stc_get_nth_super_(Type, Nth)

/// \def Get the Nth immediate base class(es) of Type (version to be
/// used outside a template).
# define stc_get_nth_super_(Type, Nth)		\
   set_super_type< Type, Nth >::ret

/// \def Get the fisrt immediate base class of Type (version to be
/// used inside a template).
# define stc_get_super(Type)			\
   typename stc_get_super_(Type)

/// \def Get the first immediate base class of Type (version to be
/// used outside a template).
# define stc_get_super_(Type)			\
   stc_get_nth_super_(Type, 1)


// -------------------- //
// Pseudo inheritance.  //
// -------------------- //

/* FIXME: I don't know whether this macro will be really usable; what
   if Type is a template class?  We would have to provide additional
   versions of this macro, with support for one parameter, two
   parameters, etc.  */
/// \def Declare the pseudosuper class \a PseudoSuper of \a Type.
# define stc_set_pseudosuper(Type, PseudoSuper)	\
   template<>					\
   struct set_pseudosuper_type<Type>		\
   {						\
     typedef PseudoSuper ret;			\
   }

/// \def Get the pseudosuper class of Type (version to be used inside a
/// template).
# define stc_get_pseudosuper(Type)		\
   typename stc_get_pseudosuper_(Type)

/// \def Get the pseudosuper class of Type (version to be used outside a
/// template).
# define stc_get_pseudosuper_(Type)		\
   set_pseudosuper_type< Type >::ret


// ---------------------- //
// Virtual types access.  //
// ---------------------- //

// FIXME: Perhaps only ``external'' (i.e., non local) versions of
// stc_type_of are really useful (since they are more precise), and we
// could get rid of local versions (stc_local_type_of and
// stc_local_type_of_).

/// Get the vtype \a Typedef, declared in the current namespace,
/// from \a FromType (version to be used inside a template).
#define stc_local_type_of(Category, FromType, Typedef)	\
  typename stc_type_of_(Category, FromType, Typedef)

/// Get the vtype \a Typedef, declared in the current namespace,
/// from \a FromType (version to be used outside a template).
#define stc_local_type_of_(Category, FromType, Typedef)		\
  type_of_<Category, FromType, typedef_:: Typedef##_type >::ret

/// Get the vtype \a Typedef, declared in \a Namespace, from \a
/// FromType (version to be used inside a template).
#define stc_type_of(Namespace, Category, FromType, Typedef)	\
  typename stc_type_of_(Namespace, Category, FromType, Typedef)

/// Get the vtype \a Typedef, declared in \a Namespace, from \a
/// FromType (version to be used outside a template).
#define stc_type_of_(Namespace, Category, FromType, Typedef)		\
  Namespace::type_of_<Category, FromType,				\
                      Namespace::typedef_:: Typedef##_type >::ret

#endif // ! STATIC_VTYPES_HH
