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


/*-----------------.
| Error messages.  |
`-----------------*/

namespace stc
{
  /// Error messages raised by static assertions.
  namespace ERROR
  {

    struct FIRST_PARAMETER_OF_rec_get_vtype_SHOULD_BE_A_TAG;
    struct PARAMETER_OF_get_super_types_helper_IS_NOT_A_VALID_VALIST;

  } // end of namespace stc::ERROR

} // end of namespace stc



/*------------.
| Equipment.  |
`------------*/

// Note: TypedefName *must* be of the form `typedef_::foo'.
# define stc_internal_get_typedef(Type, TypedefName) \
   typename TypedefName::template from_< Type >::ret


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
    struct maybe_super_type						      \
    {									      \
      typedef typename 							      \
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
    /* FIXME: Factor with a macro?  */					      \
    typedef typename internal::maybe_super_type<type, 1>::ret s1;	      \
    typedef typename internal::maybe_super_type<type, 2>::ret s2;	      \
    typedef typename internal::maybe_super_type<type, 3>::ret s3;	      \
    typedef typename internal::maybe_super_type<type, 4>::ret s4;	      \
    typedef typename internal::maybe_super_type<type, 5>::ret s5;	      \
    typedef typename internal::maybe_super_type<type, 6>::ret s6;	      \
    typedef typename internal::maybe_super_type<type, 7>::ret s7;	      \
    typedef typename internal::maybe_super_type<type, 8>::ret s8;	      \
    typedef typename internal::maybe_super_type<type, 9>::ret s9;	      \
									      \
    typedef mlc::valist_<s1, s2, s3, s4, s5, s6, s7, s8, s9> ret;	      \
  };									      \
									      \
									      \
  /* Get supers classes.  */						      \
									      \
  namespace internal 							      \
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
  /* FIXME: Add a multiple pseudoinheritance mechanism? */		      \
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
									      \
    /** Try to get \a typedef_type from \a from_type.  In case this */	      \
    /** typedef is mlc::not_found, don't perform a recursive        */	      \
    /** retrieval, simply return mlc::not_found.                    */	      \
    template <typename method, typename category,			      \
	      typename from_type, typename typedef_type>		      \
    struct get_vtype							      \
    {									      \
      /* Nothing (no method selected).  */				      \
    };									      \
									      \
    /** Specialization of get_vtypes for retrievals within */		      \
    /** internal vtypes.                                   */		      \
    template <typename category, typename from_type, typename typedef_type>   \
    struct get_vtype<tag::internal, category, from_type, typedef_type>	      \
    {									      \
      /** Set of vtypes associated with FROM_TYPE.  */			      \
      typedef vtypes<category, from_type> types;			      \
      /** Typedef in the current vtypes (may be mlc::not_found).  */	      \
      typedef stc_internal_get_typedef(types, typedef_type) ret;	      \
    };									      \
									      \
    /** Specialization of get_vtypes for retrievals within */		      \
    /** external vtypes.                                   */		      \
    template <typename category, typename from_type, typename typedef_type>   \
    struct get_vtype<tag::external, category, from_type, typedef_type>	      \
    {									      \
      /** Set of vtypes associated with FROM_TYPE.  */			      \
      typedef ext_vtype<category, from_type, typedef_type> ext_type;	      \
      /** Typedef in the current vtypes (may be mlc::not_found).  */	      \
      typedef mlc_ret(ext_type) ret;					      \
    };									      \
									      \
									      \
    /* FIXME: Do a basic scheme of the algorithm in pseudo-code.  */	      \
    /* FIXME: Check for mlc::undefined?  */				      \
									      \
    /** Recursive retrieval of a virtual type.  */			      \
    template <typename method, typename category,			      \
	      typename from_type, typename typedef_type>		      \
    struct rec_get_vtype :						      \
      private mlc::assert_< mlc_is_a(method, tag::method),		      \
        stc::ERROR::FIRST_PARAMETER_OF_rec_get_vtype_SHOULD_BE_A_TAG >	      \
    {									      \
      typedef typename							      \
      get_vtype<method, category, from_type, typedef_type>::ret type;	      \
									      \
      /** Implicit parent (i.e. super), if any.  */			      \
      typedef stc_get_super(from_type) super;				      \
      /** Pseudosuper class, if any.  */				      \
      typedef stc_get_pseudosuper(from_type) pseudosuper;		      \
									      \
      typedef typename							      \
      mlc::if_<								      \
	mlc::neq_< type, mlc::not_found >,				      \
	/* then	*/							      \
	/*   return it					*/		      \
	/*   (the typedef has been found in the vtypes	*/		      \
	/*   associated to FROM_TYPE)			*/		      \
	type,								      \
	/* else	*/							      \
	/*   check if the vtype of the `super' of FROM_TYPE */		      \
	/*   has the typedef				    */		      \
	typename							      \
	mlc::if_< mlc::neq_< typename rec_get_vtype< method,		      \
						     category,		      \
						     super,		      \
						     typedef_type >::ret,     \
			     mlc::not_found >,				      \
		  /* then */						      \
		  /*   return it */					      \
		  typename rec_get_vtype< method,			      \
					  category,			      \
					  super,			      \
					  typedef_type >::ret,		      \
		  /* else */						      \
		  /*   check if the FROM_TYPE has a decl_parent */	      \
		  /*   and try to retrieve the typedef from it. */	      \
		  typename rec_get_vtype< method,			      \
					  category,			      \
					  pseudosuper,			      \
					  typedef_type >::ret >::ret >::ret   \
      ret;								      \
    };									      \
									      \
    /** Ends of the recursive retrieval (mlc::none is at the end of the */    \
    /** transitive closure of every `super' relation).			*/    \
    /** \{ */								      \
    /** Case where \a from_type = mlc::none (end of a recursive	*/	      \
    /** retrieval following `super' types).			*/	      \
    template <typename method, typename category, typename typedef_type>      \
    struct rec_get_vtype<method, category, mlc::none, typedef_type>	      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
    /** Case where \a from_type = mlc::not_found (end of a recursive */	      \
    /** retrieval following `super' types).			     */	      \
    template <typename method, typename category, typename typedef_type>      \
    struct rec_get_vtype<method, category, mlc::not_found, typedef_type>      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
    /** \} */								      \
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
  /* FIXME: Don't query from_type directly, but exact_type(from_type) */      \
  /* instead.  We need mlc::any for this.  */				      \
  template <typename category, typename from_type, typename typedef_type>     \
  struct type_of_							      \
  {									      \
    /* Look for the typedef in internal types.  */			      \
    typedef typename							      \
    internal::rec_get_vtype<internal::tag::internal, category,		      \
                            from_type, typedef_type>::ret		      \
    internal_typedef;							      \
    /* Look for the typedef as an external type.  */			      \
    typedef typename							      \
    internal::rec_get_vtype<internal::tag::external, category,		      \
                            from_type, typedef_type>::ret		      \
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
