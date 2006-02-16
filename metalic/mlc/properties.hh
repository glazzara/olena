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
# define mlc_equip_namespace_with_properties()				    \
									    \
  /* ------------------------- */					    \
  /* Inheritance declaration.  */					    \
  /* ------------------------- */					    \
									    \
  template <typename type>						    \
  struct set_super_type							    \
  {									    \
    typedef mlc::none ret;						    \
  };									    \
									    \
									    \
  /* ---------------------------------------- */			    \
  /* ``Internal'' associated types facility.  */			    \
  /* ---------------------------------------- */			    \
									    \
  /** Fwd decl.  */							    \
  namespace internal {							    \
    template <typename category, typename from_type> struct get_types;	    \
  }									    \
									    \
  /** Specialize this class to set ``internal'' associated types.  */	    \
  template <typename category, typename from_type>			    \
  struct set_types							    \
  {									    \
  };									    \
									    \
  /** \brief Specialize this class to redefine ``internal''       */	    \
  /** associated types.                                           */	    \
  /**                                                             */	    \
  /** Notice the inheritance relation, which enable the automatic */	    \
  /** retrieval of the types associated to the super class of \a  */	    \
  /** from_type.                                                  */	    \
  template <typename category, typename from_type>			    \
  struct redefine_types : public mlc_super_types(category, from_type)	    \
  {      								    \
  };									    \
									    \
									    \
  /* ----------------------------------------- */			    \
  /* ``External'' associated types machinery.  */			    \
  /* ----------------------------------------- */			    \
									    \
  /** Fwd decl.  */							    \
  namespace internal {							    \
    template <typename category, typename from_type, typename typedef_type> \
    struct get_ext_type;						    \
  }									    \
									    \
  /** Specialize this class to set an ``external'' associated type.  */	    \
  template <typename category, typename from_type, typename typedef_type>   \
  struct set_ext_type							    \
  {									    \
  };									    \
									    \
  /** \brief Specialize this class to redefine an ``external''    */	    \
  /** associated type.                                            */	    \
  /**                                                             */	    \
  /** Notice the inheritance relation, which enable the automatic */	    \
  /** retrieval of the types associated to the super class of \a  */	    \
  /** from_type.                                                  */	    \
  template <typename category, typename from_type, typename typedef_type>   \
  struct redefine_ext_type :						    \
    public mlc_super_ext_type(category, from_type, typedef_type)	    \
  {      								    \
  };									    \
									    \
									    \
  /* -------------------- */						    \
  /* Internal machinery.  */						    \
  /* -------------------- */						    \
									    \
  /** The classes enclosed in this namespace must not be specialized */	    \
  /** by the user (they are part of the automatic associated types   */	    \
  /** retrieval mechanism).                                          */	    \
  namespace internal							    \
  {									    \
    template <typename category, typename from_type>			    \
    struct get_types :							    \
      public set_types<category, from_type>,				    \
      public redefine_types<category, from_type>			    \
    {      								    \
    };									    \
									    \
    /** End of the recursive construction of any get_types<> hierarchy.  */ \
    template <typename category>					    \
    struct get_types<category, mlc::none>				    \
    {									    \
    };									    \
									    \
    template <typename category, typename from_type, typename typedef_type> \
    struct get_ext_type :						    \
      public set_ext_type<category, from_type, typedef_type>,		    \
      public redefine_ext_type<category, from_type, typedef_type>	    \
    {									    \
    };									    \
									    \
    /** End of the recursive construction of any get_ext_type<> */	    \
    /** hierarchy.                                              */	    \
    template <typename category, typename typedef_type>			    \
    struct get_ext_type<category, mlc::none, typedef_type>		    \
    {									    \
    };									    \
									    \
    /** Typedef selector.  */						    \
    /** \{ */								    \
    /** Fwd decl.  */							    \
    template <bool external_typedef_p, bool internal_typedef_p,		    \
	      typename external_typedef, typename internal_typedef>	    \
    struct select_typedef;						    \
									    \
    /** The typedef is found in both an external and an internal */	    \
    /** type definitions: error.                                 */	    \
    template <typename external_typedef, typename internal_typedef>	    \
    struct select_typedef<true, true, external_typedef, internal_typedef>   \
    {									    \
      /* No ret member.  */						    \
    };									    \
									    \
    /** The typedef is found neither in an external nor in an */	    \
    /** internal type definition: error.                      */	    \
    template <typename external_typedef, typename internal_typedef>	    \
    struct select_typedef<false, false, external_typedef, internal_typedef> \
    {									    \
      /* No ret member.  */						    \
    };									    \
									    \
    /** The typedef is found in an extternal definition only: good.  */	    \
    template <typename external_typedef, typename internal_typedef>	    \
    struct select_typedef<true, false, external_typedef, internal_typedef>  \
    {									    \
      typedef external_typedef ret;					    \
    };									    \
									    \
    /** The typedef is found in an internal definition only: good.  */	    \
    template <typename external_typedef, typename internal_typedef>	    \
    struct select_typedef<false, true, external_typedef, internal_typedef>  \
    {									    \
      typedef internal_typedef ret;					    \
    };									    \
    /** \} */								    \
   									    \
  } /** End of namespace internal.  */					    \
									    \
									    \
  /** FIXME: Don't query from_type directly, but */			    \
  /** exact_type(from_type) instead              */			    \
  template <typename category, typename from_type, typename typedef_type>   \
  struct typeof_							    \
  {									    \
    typedef internal::get_types<category, from_type> types;		    \
    /* FIXME: Add a check in typeof_ to ensure that get_ext_type */	    \
    /* derives from get_ext_type<none>                           */	    \
    typedef								    \
    internal::get_ext_type<category, from_type, typedef_type> ext_type;	    \
    /* FIXME: Add a check in typeof_ to ensure that get_ext_type */	    \
    /* derives from get_ext_type<none>                           */	    \
									    \
    /** Look for the typedef as an external type.  */			    \
    typedef								    \
    mlc_internal_get_typedef(ext_type, typedef_::ret) external_typedef;	    \
    /** Look for the typedef in internal types.  */			    \
    typedef								    \
    mlc_internal_get_typedef(types, typedef_type) internal_typedef;	    \
									    \
    static const bool found_external_p =				    \
      mlc::is_found<external_typedef>::value;				    \
    static const bool found_internal_p =				    \
      mlc::is_found<internal_typedef>::value;				    \
									    \
    typedef typename							    \
    internal::select_typedef<found_external_p, found_internal_p,	    \
			     external_typedef, internal_typedef>::ret ret;  \
    };									    \
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

/// \def Get the immediate base class of T
# define mlc_super(T)				\
   set_super_type<T>::ret

// FIXME: Doc.
# define mlc_super_types(Category, FromType)			\
   internal::get_types<Category, typename mlc_super(FromType)>

// FIXME: Doc.
# define mlc_super_types_(Category, FromType)		\
   internal::get_types<Category, mlc_super(FromType)>

// FIXME: Doc.
# define mlc_super_ext_type(Category, FromType, Typedef)		   \
   internal::get_ext_type<Category, typename mlc_super(FromType), Typedef>

// FIXME: Doc.
# define mlc_super_ext_type_(Category, FromType, Typedef)		\
   internal::get_ext_type<Category, mlc_super(FromType), Typedef>

/// Get the property \a Typedef from \a FromType (version with typename).
#define mlc_typeof(Category, FromType, Typedef)				\
  typename typeof_<Category, FromType, typedef_:: Typedef##_type >::ret

/// Get the property \a Typedef from \a FromType (version without typename).
#define mlc_typeof_(Category, FromType, Typedef)		\
  typeof_<Category, FromType, typedef_:: Typedef##_type >::ret


#endif // ! METALIC_PROPERTIES_HH
