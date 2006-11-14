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

   \brief Equipment for SCOOP: notably, Virtual types (also known as
   ``properties'') mechanism.

   Based on Theo's presentation from January 2006 (olena-06-jan.pdf).  */

#ifndef STC_SCOOP_HH
# define STC_SCOOP_HH

# include <mlc/flags.hh>
# include <mlc/typedef.hh>
# include <mlc/ret.hh>

# include <mlc/assert.hh>
# include <mlc/abort.hh>

# include <mlc/bool.hh>

# include <mlc/cmp.hh>
# include <mlc/if.hh>
# include <mlc/is_a.hh>


namespace stc
{

  /// Error messages raised by static assertions/abortions.
  namespace ERROR
  {

    /// Errors from check_type_of_ and check_exact_type_of_.
    /// \{
    struct VIRTUAL_TYPE_NOT_FOUND;
    /// \}

  } // end of namespace stc::ERROR

} // end of namespace stc



# define stc_internal_get_typedef(Type, TypedefName) \
   typename TypedefName::template from_< Type >::ret


# define stc_internal_get_super(Type) \
   typename typedef_::super_type::template from_< Type >::ret






# define stc_scoop_equipment_for_namespace(SCOOPED_NAMESPACE)				\
											\
namespace SCOOPED_NAMESPACE								\
{											\
											\
  template <typename from_type>								\
  struct set_super_type									\
  {											\
    typedef mlc::none ret;								\
  };											\
											\
  template <typename type>								\
  struct set_pseudosuper_type								\
  {											\
    typedef mlc::not_found ret;								\
  };											\
											\
  template <typename from_type>								\
  struct vtypes										\
  {											\
  };											\
											\
  template <typename from_type, typename type>						\
  struct single_vtype									\
  {											\
    typedef mlc::not_found ret;								\
  };											\
											\
											\
  namespace internal									\
  {											\
											\
    /* retrieve_from_single_vtype */							\
											\
    template <typename from_type, typename type>					\
    struct retrieve_from_single_vtype;							\
											\
    template <typename res, typename super, typename type>				\
    struct helper_retrieve_from_single_vtype						\
    {											\
      typedef res ret;									\
    };											\
											\
    template <typename super, typename type>						\
    struct helper_retrieve_from_single_vtype < mlc::not_found, super, type >		\
    {											\
      typedef typename retrieve_from_single_vtype<super, type>::ret ret;		\
    };											\
											\
    template <typename type>								\
    struct helper_retrieve_from_single_vtype < mlc::not_found, mlc::none, type >	\
    {											\
      typedef mlc::not_found ret;							\
    };											\
											\
    template <typename type>								\
    struct helper_retrieve_from_single_vtype < mlc::not_found, mlc::not_found, type >	\
    {											\
    };											\
											\
    template <typename from_type, typename type>					\
    struct retrieve_from_single_vtype							\
    {											\
      typedef SCOOPED_NAMESPACE::single_vtype<from_type, type> decl;			\
      typedef typename decl::ret res;							\
      typedef typename set_super_type<from_type>::ret super;				\
      typedef typename helper_retrieve_from_single_vtype< res, super, type >::ret ret;	\
    };											\
											\
    /* vtypes */									\
											\
    template <typename from_type, typename type>					\
    struct retrieve_from_vtypes;							\
											\
    template <typename res, typename super, typename type>				\
    struct helper_retrieve_from_vtypes							\
    {											\
      typedef res ret;									\
    };											\
											\
    template <typename super, typename type>						\
    struct helper_retrieve_from_vtypes < mlc::not_found, super, type >			\
    {											\
      typedef typename retrieve_from_vtypes<super, type>::ret ret;			\
    };											\
											\
    template <typename type>								\
    struct helper_retrieve_from_vtypes < mlc::not_found, mlc::none, type >		\
    {											\
      typedef mlc::not_found ret;							\
    };											\
											\
    template <typename type>								\
    struct helper_retrieve_from_vtypes < mlc::not_found, mlc::not_found, type >		\
    {											\
    };											\
											\
    template <typename from_type, typename type>					\
    struct retrieve_from_vtypes								\
    {											\
      typedef SCOOPED_NAMESPACE::vtypes<from_type> decl;				\
      typedef stc_internal_get_typedef(decl, type) res;					\
      typedef typename set_super_type<from_type>::ret super;				\
      typedef typename helper_retrieve_from_vtypes< res, super, type >::ret ret;	\
    };											\
											\
    /* pseudo super */									\
											\
    template <typename from_type>							\
    struct retrieve_pseudosuper;							\
											\
    template <typename res, typename super>						\
    struct helper_retrieve_pseudosuper							\
    {											\
      typedef res ret;									\
    };											\
											\
    template <typename super>								\
    struct helper_retrieve_pseudosuper < mlc::not_found, super >			\
    {											\
      typedef typename retrieve_pseudosuper<super>::ret ret;				\
    };											\
											\
    template <>										\
    struct helper_retrieve_pseudosuper < mlc::not_found, mlc::none >			\
    {											\
      typedef mlc::not_found ret;							\
    };											\
											\
    template <>										\
    struct helper_retrieve_pseudosuper < mlc::not_found, mlc::not_found >		\
    {											\
    };											\
											\
    template <typename from_type>							\
    struct retrieve_pseudosuper								\
    {											\
      typedef typename set_pseudosuper_type<from_type>::ret res;			\
      typedef typename set_super_type<from_type>::ret super;				\
      typedef typename helper_retrieve_pseudosuper< res, super >::ret ret;		\
    };											\
											\
    /* scoop_vtype_of */								\
											\
    template <typename from_type, typename type>					\
    struct step_from_vtypes; /* fwd decl of main entry */				\
											\
    /*  step 3  <=>  delegation  */							\
											\
    template <typename from_type, typename type>					\
    struct helper_step_from_delegation							\
    {											\
      /* REC */										\
      typedef typename step_from_vtypes<from_type, type>::ret ret;			\
    };											\
											\
    template <typename type>								\
    struct helper_step_from_delegation < mlc::not_found, type >				\
    {											\
      typedef mlc::not_found ret;							\
    };											\
											\
    template <typename from_type, typename type>					\
    struct step_from_delegation /* step 3 entry */					\
    {											\
      typedef typename retrieve_pseudosuper<from_type>::ret pseudo_type;		\
      typedef typename helper_step_from_delegation<pseudo_type, type>::ret ret;		\
    };											\
											\
    /*  step 2  <=>  single_vtype  */							\
											\
    template <typename res, typename from_type, typename type>				\
    struct helper_step_from_single_vtype						\
    {											\
      typedef res ret; /* found! */							\
    };											\
											\
    template <typename from_type, typename type>					\
    struct helper_step_from_single_vtype <mlc::not_found, /* => step 3 */		\
					  from_type, type>				\
    {											\
      typedef typename step_from_delegation<from_type, type>::ret ret;			\
    };											\
											\
    template <typename from_type, typename type>					\
    struct helper_step_from_single_vtype <mlc::undefined, /* => step 3 */		\
					  from_type, type>				\
    {											\
      typedef typename step_from_delegation<from_type, type>::ret ret;			\
    };											\
											\
    template <typename from_type, typename type>					\
    struct step_from_single_vtype /* step 2 entry */					\
    {											\
      typedef typename retrieve_from_single_vtype<from_type, type>::ret res;		\
      typedef typename helper_step_from_single_vtype< res, from_type, type>::ret ret;	\
    };											\
											\
    /*  step 1  <=>  vtypes  */								\
											\
    template <typename res, typename from_type, typename type>				\
    struct helper_step_from_vtypes							\
    {											\
      typedef res ret; /* found! */							\
    };											\
											\
    template <typename from_type, typename type>					\
    struct helper_step_from_vtypes <mlc::not_found, /* => step 2 */			\
				    from_type, type>					\
    {											\
      typedef typename step_from_single_vtype<from_type, type>::ret ret;		\
    };											\
											\
    template <typename from_type, typename type>					\
    struct helper_step_from_vtypes <mlc::undefined, /* => step 2 */			\
				    from_type, type>					\
    {											\
      typedef typename step_from_single_vtype<from_type, type>::ret ret;		\
    };											\
											\
    template <typename from_type, typename type>					\
    struct step_from_vtypes /* main entry (so step 1 entry) */				\
    {											\
      typedef typename retrieve_from_vtypes<from_type, type>::ret res;			\
      typedef typename helper_step_from_vtypes< res, from_type, type>::ret ret;		\
    };											\
											\
  } /* end of namespace SCOOPED_NAMESPACE ::internal */					\
											\
											\
  template <typename from_type, typename type>						\
  struct direct_type_of_								\
  {											\
    typedef typename internal::step_from_vtypes<from_type, type>::ret ret;		\
  };											\
											\
											\
} /* end of namespace SCOOPED_NAMESPACE */						\
											\
struct e_n_d__w_i_t_h___s_e_m_i_c_o_l_o_n



#endif // ! STC_SCOOP_HH
