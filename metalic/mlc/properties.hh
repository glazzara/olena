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

// \file mlc/properties.hh
// \brief Property mechanism.
//
// From Theo's presentation (olena-06-jan.pdf).

#ifndef METALIC_PROPERTIES_HH
# define METALIC_PROPERTIES_HH

# include <mlc/flags.hh>
# include <mlc/typedef.hh>
# include <mlc/bool.hh>
# include <mlc/cmp.hh>
# include <mlc/if.hh>
# include <mlc/is_a.hh>


/*------------.
| Equipment.  |
`------------*/

// Note: TypedefName *must* be of the form `typedef_::foo'.
# define mlc_internal_get_typedef(Type, TypedefName) \
   typename TypedefName::template from_< Type >::ret

// FIXME: Add support for hierarchies with several super classes.
# define mlc_equip_namespace_with_properties()				      \
									      \
  /* ----------------------- */						      \
  /* Typedefs declarations.  */						      \
  /* ----------------------- */						      \
									      \
  /* FIXME: Should we use the `ret' typdef defined in the global */	      \
  /* namespace instead ? (see at the end mlc/typedef.hh). */		      \
  mlc_decl_typedef(ret);						      \
									      \
  /* Declare the ``uplink'' typedef (access to a pseudosuper class).  */      \
  mlc_decl_typedef(pseudosuper_type);					      \
									      \
									      \
  /* ------------- */							      \
  /* Inheritance.  */							      \
  /* ------------- */							      \
									      \
  template <typename type>						      \
  struct set_super_type							      \
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
    /* FIXME: Do a basic scheme of the algorithm in pseudo-code.  */	      \
									      \
    /* FIXME: Check for mlc::undefined?  */				      \
									      \
    /* FIXME: The presence of `vtypes' is the only thing that makes */	      \
    /* this code different from the retrieval within an external    */	      \
    /* vtype.  How can we factor this?                              */	      \
    template <typename category, typename from_type, typename typedef_type>   \
    struct rec_get_vtype						      \
    {									      \
      /** Set of vtypes associated with FROM_TYPE.  */			      \
      typedef vtypes<category, from_type> types;			      \
      /** Typedef in the current vtypes (maybe mlc::not_found).  */	      \
      typedef mlc_internal_get_typedef(types, typedef_type) type;	      \
									      \
      /** Implicit parent (i.e. super), if any.  */			      \
      typedef mlc_super(from_type) super;				      \
      /** Pseudosuper class, if any.  */				      \
      typedef mlc_internal_get_typedef(types, typedef_::pseudosuper_type)     \
	pseudosuper;							      \
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
	mlc::if_< mlc::neq_< typename rec_get_vtype< category,		      \
						     super,		      \
						     typedef_type >::ret,     \
			     mlc::not_found >,				      \
		  /* then */						      \
		  /*   return it */					      \
		  typename rec_get_vtype< category,			      \
					  super,			      \
					  typedef_type >::ret,		      \
		  /* else */						      \
		  /*   check if the FROM_TYPE has a decl_parent */	      \
		  /*   and try to retrieve the typedef from it. */	      \
		  typename rec_get_vtype< category,			      \
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
    template <typename category, typename typedef_type>			      \
    struct rec_get_vtype<category, mlc::none, typedef_type>		      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
    /** Case where \a from_type = mlc::not_found (end of a recursive */	      \
    /** retrieval following `super' types).			     */	      \
    template <typename category, typename typedef_type>			      \
    struct rec_get_vtype<category, mlc::not_found, typedef_type>	      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
    /** \} */								      \
									      \
    /* ------------------------------------------ */			      \
    /* Recursive retrieval of an external vtype.  */			      \
    /* ------------------------------------------ */			      \
									      \
    /* FIXME: Merge this with rec_get_vtype.  */			      \
									      \
    template <typename category, typename from_type, typename typedef_type>   \
    struct rec_get_ext_vtype						      \
    {									      \
      /** Set of vtypes associated with FROM_TYPE.  */			      \
      typedef ext_vtype<category, from_type, typedef_type> ext_type;	      \
      /** Typedef in the current vtypes (maybe mlc::not_found).  */	      \
      typedef mlc_internal_get_typedef(ext_type, typedef_::ret) type;	      \
									      \
      /** Implicit parent (i.e. super), if any.  */			      \
      typedef mlc_super(from_type) super;				      \
      /** Pseudosuper class, if any.  */				      \
      /* FIXME: Looking for this information is not elegant. Have a  */	      \
      /* look at metalic/tests/properties.cc for a better suggestion.  */     \
      typedef vtypes<category, from_type> types;			      \
      typedef mlc_internal_get_typedef(types, typedef_::pseudosuper_type)     \
	pseudosuper;							      \
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
	mlc::if_< mlc::neq_< typename rec_get_ext_vtype< category,	      \
							 super,		      \
							 typedef_type >::ret, \
			     mlc::not_found >,				      \
		  /* then */						      \
		  /*   return it */					      \
		  typename rec_get_ext_vtype< category,			      \
					      super,			      \
					      typedef_type >::ret,	      \
		  /* else */						      \
		  /*   check if the FROM_TYPE has a decl_parent */	      \
		  /*   and try to retrieve the typedef from it. */	      \
		  typename rec_get_ext_vtype< category,			      \
					      pseudosuper,		      \
					      typedef_type >::ret>::ret>::ret \
      ret;								      \
    };									      \
									      \
    /** Ends of the recursive retrieval (mlc::none is at the end of the */    \
    /** transitive closure of every `super' relation).			*/    \
    /** \{ */								      \
    /** Case where \a from_type = mlc::none (end of a recursive	*/	      \
    /** retrieval following `super' types).			*/	      \
    template <typename category, typename typedef_type>			      \
    struct rec_get_ext_vtype<category, mlc::none, typedef_type>		      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
    /** Case where \a from_type = mlc::not_found (end of a recursive */	      \
    /** retrieval following `super' types).			     */	      \
    template <typename category, typename typedef_type>			      \
    struct rec_get_ext_vtype<category, mlc::not_found, typedef_type>	      \
    {									      \
      typedef mlc::not_found ret;					      \
    };									      \
    /** \} */								      \
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
  struct typeof_							      \
  {									      \
    /* Look for the typedef as an external type.  */			      \
    typedef typename							      \
    internal::rec_get_ext_vtype<category, from_type, typedef_type>::ret	      \
    external_typedef;							      \
    /* Look for the typedef in internal types.  */			      \
    typedef typename							      \
    internal::rec_get_vtype<category, from_type, typedef_type>::ret	      \
    internal_typedef;							      \
									      \
    /* Did we found the virtual type? */				      \
    static const bool found_external_p =				      \
      mlc::is_found<external_typedef>::value;				      \
    static const bool found_internal_p =				      \
      mlc::is_found<internal_typedef>::value;				      \
									      \
    typedef typename							      \
    internal::select_typedef<found_external_p, found_internal_p,	      \
			     external_typedef, internal_typedef>::ret ret;    \
    };									      \
									      \
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


/*---------.
| Macros.  |
`---------*/

/* FIXME: I don't know this macro will be really usable; what if T is
   a template class?  */
// mlc_set_super_type(T, S)  to declare the immediate base class S of T
# define mlc_set_super(Type, Super)		\
   template <>					\
   struct set_super_type<Type>			\
   {						\
     typedef Super ret;				\
   }

/// \def Get the immediate base class of T (version with typename).
# define mlc_super(T)				\
   typename set_super_type<T>::ret

/// \def Get the immediate base class of T (version without typename).
# define mlc_super_(T)				\
   set_super_type<T>::ret

/// Get the property \a Typedef from \a FromType (version with typename).
#define mlc_typeof(Category, FromType, Typedef)				\
  typename typeof_<Category, FromType, typedef_:: Typedef##_type >::ret

/// Get the property \a Typedef from \a FromType (version without typename).
#define mlc_typeof_(Category, FromType, Typedef)		\
  typeof_<Category, FromType, typedef_:: Typedef##_type >::ret


#endif // ! METALIC_PROPERTIES_HH
