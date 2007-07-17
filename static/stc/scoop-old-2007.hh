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
# include <mlc/pair.hh>
# include <mlc/cmp.hh>
# include <mlc/if.hh>
# include <mlc/is_a.hh>


namespace stc
{

  struct not_delegated;

  struct abstract;

  template <typename T>
  struct final;

} // end of namespace stc



#define stc_super(T) typename set_super_type< T >::ret
#define stc_stm(From, Target) typename get_stm< From , Target >::ret


# define stc_scoop_equipment_for_namespace(SCOOPED_NAMESPACE)					\
												\
												\
namespace SCOOPED_NAMESPACE									\
{												\
												\
  mlc_decl_typedef(delegatee_type);								\
												\
												\
  template <typename from_type>									\
  struct set_super_type										\
  {												\
    typedef mlc::none ret;									\
    /* FIXME: nothing here!									\
     *												\
     * mlc::none is not a default value								\
     * so that the client has to define stoppers						\
     */												\
  };												\
												\
  template <typename from_type>									\
  struct vtypes											\
  {												\
  };												\
												\
  template <typename from_type, typename type>							\
  struct single_vtype										\
  {												\
    typedef mlc::not_found ret;									\
  };												\
												\
												\
												\
  namespace ERROR										\
  {												\
												\
    template < typename first_definition,							\
	       typename re_definition,								\
	       typename target >								\
    struct final_vtype_redefined_;								\
												\
												\
    template < typename first_definition,							\
	       typename declaration,								\
	       typename target >								\
    struct vtype_declared_but_already_set_;							\
												\
												\
    template < typename from,									\
	       typename target >								\
    struct vtype_not_found;									\
												\
												\
    template < typename curr,									\
	       typename target,									\
	       typename stm >									\
    struct wrong_final_stm_;									\
												\
												\
    template < typename target,									\
	       typename declaration,								\
	       typename from >									\
    struct vtype_declared_but_not_defined;							\
												\
												\
    template <typename T>									\
    struct _declaration_is_in_;									\
												\
    template <typename T>									\
    struct _definition_is_looked_up_from_;							\
												\
												\
    template <typename T>									\
    struct _for_vtype_;										\
												\
												\
    struct no_delegatee_declared_;								\
												\
												\
  } /* end of namespace SCOOPED_NAMESPACE::ERROR */						\
												\
												\
												\
  namespace stc_vtype_internal									\
  {												\
												\
												\
    /*												\
     * Forward declarations.									\
     */												\
												\
												\
    template <typename from, typename target>							\
    struct get_stm;										\
												\
    template <typename res1, typename res2>							\
    struct helper_get_stm;									\
												\
    template <typename from, typename target>							\
    struct first_stm;										\
												\
    template <typename curr, typename target, typename stm>					\
    struct helper_first_stm;									\
												\
												\
    template <typename from, typename target, typename curr = from>				\
    struct superior_find;									\
												\
    template <typename from, typename target, typename curr, typename stm>			\
    struct helper_superior_find;								\
												\
    template <typename from, typename target>							\
    struct delegator_find;									\
												\
    template <typename from, typename target, typename delegatee>				\
    struct helper_delegator_find;								\
												\
    template <typename from, typename target>							\
    struct find;										\
												\
    template <typename from, typename target, typename location, typename res>			\
    struct helper_find;										\
												\
												\
												\
    template <typename from, typename target>							\
    struct check;										\
												\
    template <typename curr, typename target, typename stm>					\
    struct helper_check;									\
												\
												\
												\
    /* get_stm(from, target)									\
     *												\
     * Returns the statement for class 'from' corresponding to 'target'.			\
     * This statement is either defined in 'vtypes' or in 'single_vtype'.			\
     *												\
     * This routine is local; it does not recourse on superior classes.				\
     *												\
     */												\
												\
    template <typename res1>									\
    struct helper_get_stm <res1, mlc::not_found>						\
    {												\
      typedef res1 ret;										\
    };												\
												\
    template <typename res2>									\
    struct helper_get_stm <mlc::not_found, res2>						\
    {												\
      typedef res2 ret;										\
    };												\
												\
    template <>											\
    struct helper_get_stm <mlc::not_found, mlc::not_found>					\
    {												\
      typedef mlc::not_found ret;								\
    };												\
												\
    template <typename res1, typename res2>							\
    struct helper_get_stm /* FIXME: Insert mlc::abort_<> statement here?  */			\
    {												\
      /* error */										\
    };												\
												\
    template <typename from, typename target>							\
    struct get_stm										\
    {												\
      typedef SCOOPED_NAMESPACE::vtypes<from> decl1;						\
      typedef typename target::template from_<decl1>::ret res1;					\
												\
      typedef SCOOPED_NAMESPACE::single_vtype<from, target> decl2;				\
      typedef typename decl2::ret ret2;								\
												\
      typedef typename helper_get_stm<res1, ret2>::ret ret;					\
    };												\
												\
												\
												\
												\
    /* -------------------- */									\
    /* Checking algorithm.  */									\
    /* -------------------- */									\
												\
    /*												\
     * check_no_final_inherited									\
     *												\
     * FIXME: ...										\
     */												\
												\
    template < typename orig, typename curr, typename target,					\
	       typename stm = typename get_stm<curr, target>::ret >				\
    struct check_no_final_inherited								\
												\
      : mlc::assert_< mlc_is_not_a(stm, stc::final),						\
		      ERROR::final_vtype_redefined_< orig,					\
						     mlc::pair_<curr, stm>,			\
						     target >					\
                    >,										\
	/* rec */ check_no_final_inherited<orig, stc_super(curr), target>			\
    {												\
    };												\
												\
    template <typename orig, typename target, typename stm>					\
    struct check_no_final_inherited <orig, /* if curr == */ mlc::none, target, stm>		\
    {												\
    };												\
												\
												\
    /*												\
     * check_no_stm_inherited									\
     *												\
     * FIXME: ...										\
     */												\
												\
    template < typename orig, typename curr, typename target,					\
	       typename stm = typename get_stm<curr, target>::ret >				\
    struct check_no_stm_inherited								\
												\
      : mlc::assert_< mlc::is_not_found_<stm>,							\
		      ERROR::vtype_declared_but_already_set_< orig,				\
							      mlc::pair_<curr, stm>,		\
							      target >				\
		      >,									\
	/* rec */ check_no_stm_inherited<orig, stc_super(curr), target>				\
    {												\
    };												\
												\
    template <typename orig, typename target, typename stm>					\
    struct check_no_stm_inherited <orig, /* if curr == */ mlc::none, target, stm>		\
    {												\
    };												\
												\
												\
    /*												\
     * check_delegatee_inherited								\
     */												\
												\
    template <typename curr, typename target>							\
    struct check_delegatee_inherited								\
      : mlc::assert_< mlc::is_found_< typename first_stm<curr,					\
				      typedef_::delegatee_type>::ret::second_elt >,		\
		      ERROR::no_delegatee_declared_ >						\
    {												\
    };												\
												\
												\
    /*												\
     * check_final_stm										\
     */												\
    												\
    template <typename curr, typename target, typename stm>					\
    struct check_final_stm									\
    {};												\
												\
    template <typename curr, typename target>							\
    struct check_final_stm < curr, target, stc::abstract >					\
      : mlc::abort_< ERROR::wrong_final_stm_<curr, target, stc::final<stc::abstract> > >	\
    {};												\
												\
    template <typename curr, typename target, typename T>					\
    struct check_final_stm < curr, target, stc::final<T> >					\
      : mlc::abort_< ERROR::wrong_final_stm_<curr, target, stc::final<stc::final<T> > > >	\
    {};												\
												\
    template <typename curr, typename target>							\
    struct check_final_stm < curr, target, stc::not_delegated >					\
      : mlc::abort_< ERROR::wrong_final_stm_<curr, target, stc::final<stc::not_delegated> > >	\
    {};												\
												\
    template <typename curr, typename target>							\
    struct check_final_stm < curr, target, mlc::not_found >					\
      : mlc::abort_< ERROR::wrong_final_stm_<curr, target, stc::final<mlc::not_found> > >	\
    {};												\
												\
												\
												\
												\
    /*												\
     * check(from, target)									\
     *												\
     * pseudo-code:										\
     *												\
     * check(from, target)									\
     * {											\
     *   if (from == mlc::none) // stop because no more class					\
     *     return done;										\
     *   else											\
     *     return helper_check(from, target, get_stm(from, target));				\
     * }											\
     */												\
												\
    struct dummy {};										\
												\
    template <typename target>									\
    struct check < /* if from == */ mlc::none, target >						\
    {												\
      typedef dummy ret;									\
    };												\
												\
    template <typename from, typename target>							\
    struct check /* otherwise */								\
    {												\
      typedef typename helper_check< from, target,						\
				     stc_stm(from, target) >::ret ret;				\
    };												\
												\
												\
												\
												\
    /*												\
     * pseudo-code:										\
     *												\
     * helper_check(curr, target, stm)								\
     * {											\
     *   super = super(curr);									\
     * 												\
     *   if (stm == stc::abstract) {								\
     *     check_no_stm_inherited(curr, super, target);						\
     *     return check(super, target);								\
     *   }											\
     *   if (stm == stc::final<T>) {								\
     *     check_final_stm(T);									\
     *     check_no_final_inherited((curr, T), super, target);					\
     *     return check(super, target);								\
     *   }											\
     *   if (stm == stc::not_delegated) {							\
     *     check_delegatee_inherited(curr);							\
     *     check_no_final_inherited((curr, stc::not_delegated), super, target);			\
     *     return check(super, target);								\
     *   }											\
     *   if (stm == mlc::not_found) {								\
     *     return check(super, target);								\
     *   }											\
     *   // otherwise, non-final stm:								\
     *   if (stm == T) {									\
     *     check_no_final_inherited((curr, T), super, target);					\
     *     return check(super, target);								\
     *   }											\
     * }											\
     * 												\
     */												\
												\
    template <typename curr, typename target>							\
    struct helper_check < curr, target,								\
			  /* if stm == */ stc::abstract >					\
      : check_no_stm_inherited< mlc::pair_<curr, stc::abstract >,				\
				stc_super(curr),						\
				target >							\
    {												\
      typedef typename check<stc_super(curr), target>::ret ret;					\
    };												\
												\
    template <typename curr, typename target, typename T>					\
    struct helper_check < curr, target,								\
			  /* if stm == */ stc::final<T> >					\
      : check_no_final_inherited< mlc::pair_<curr, stc::final<T> >,				\
				  stc_super(curr),						\
				  target >							\
    {												\
      /* FIXME:											\
	 check_final_stm(T);									\
      */											\
      typedef typename check<stc_super(curr), target>::ret ret;					\
    };												\
												\
    template <typename curr, typename target>							\
    struct helper_check < curr, target,								\
			  /* if stm == */ stc::not_delegated >					\
      : check_no_final_inherited< mlc::pair_<curr, stc::not_delegated>,				\
				  stc_super(curr),						\
				  target >							\
    {												\
      /* FIXME:											\
	 check_delegatee_inherited(curr);							\
      */											\
      typedef typename check<stc_super(curr), target>::ret ret;					\
    };												\
												\
    template <typename curr, typename target>							\
    struct helper_check < curr, target,								\
			  /* if stm == */ mlc::not_found >					\
    {												\
      typedef typename check<stc_super(curr), target>::ret ret;					\
    };												\
												\
    template <typename curr, typename target, typename stm>					\
    struct helper_check /* otherwise, non-final stm */						\
      : check_no_final_inherited< mlc::pair_<curr, stm>,					\
				  stc_super(curr),						\
				  target >							\
    {												\
      typedef typename check<stc_super(curr), target>::ret ret;					\
    };												\
												\
												\
												\
    /* ------------------- */									\
    /* Look-up algorithm.  */									\
    /* ------------------- */									\
												\
    /*												\
     * first_stm(from, target)									\
     *												\
     * returns a pair (Location, Value) with Value being:					\
     * - stc::abstract										\
     * - stc::not_delegated									\
     * - mlc::not_found										\
     * - a type T										\
     * and Location being the class where the stm is found.					\
     *												\
     *												\
     * helper_first_stm(curr, target, stm)							\
     * {											\
     *   if (stm == mlc::not_found)								\
     *     return first_stm(super(curr), target);						\
     *   else if (stm == stc::final<T>)								\
     *     return pair(curr, T);								\
     *   else											\
     *     return pair(curr, stm);								\
     * }											\
     *												\
     * first_stm(from, target)									\
     * {											\
     *   if (from == mlc::none)									\
     *     return pair(mlc::none, mlc::not_found);						\
     *   else											\
     *     return helper_first_stm(from, target,						\
     *                             get_stm(from, target));					\
     * }											\
     *												\
     */												\
												\
    template <typename curr, typename target>							\
    struct helper_first_stm < curr, target,							\
			      /* if stm == */ mlc::not_found >					\
    {												\
      typedef typename first_stm<stc_super(curr), target>::ret ret;				\
    };												\
												\
    template <typename curr, typename target, typename T>					\
    struct helper_first_stm < curr, target,							\
			      /* if stm == */ stc::final<T> >					\
    {												\
      typedef mlc::pair_<curr, T> ret;								\
    };												\
												\
    template <typename curr, typename target, typename stm>					\
    struct helper_first_stm /* otherwise */							\
    {												\
      typedef mlc::pair_<curr, stm> ret;							\
    };												\
												\
    template <typename target>									\
    struct first_stm < /* if from == */ mlc::none, target >					\
    {												\
      typedef mlc::pair_<mlc::none, mlc::not_found> ret;					\
    };												\
												\
    template <typename from, typename target>							\
    struct first_stm /* otherwise */								\
    {												\
      typedef typename helper_first_stm<from, target,						\
					stc_stm(from, target)>::ret ret;			\
    };												\
												\
												\
												\
    /*												\
     * superior_find(from, target)								\
     *												\
     * returns:											\
     *   either mlc::not_found									\
     *   or a type T										\
     * behavior:										\
     * - the stm stc::not_delegated is ignored							\
     * - error when stc::abstract is reached							\
     *												\
     * helper_superior_find(from, target, curr, stm)						\
     * {											\
     *   if (stm == stc::abstract)								\
     *     error("<target> declared in <curr> but not defined at <from>");			\
     *   else if (stm == mlc::not_found or							\
     *            stm == stc::not_delegated)							\
     *     return superior_find(from, target, super(curr));					\
     *   else if (stm == stc::final<T>)								\
     *     return T;										\
     *   else											\
     *     return stm;										\
     * }											\
     *												\
     * superior_find(from, target, curr = from)							\
     * {											\
     *   if (curr == mlc::none)									\
     *     return mlc::not_found;								\
     *   else											\
     *     return helper_superior_find(from, target, curr,					\
     *                                 get_stm(curr, target));					\
     * }											\
     */												\
												\
    template <typename from, typename target, typename curr>					\
    struct helper_superior_find < from, target, curr,						\
				  /* if stm == */ stc::abstract >				\
    {												\
      /* FIXME: error("<target> declared in <curr> but not defined at <from>"); */		\
    };												\
												\
    template <typename from, typename target, typename curr>					\
    struct helper_superior_find < from, target, curr,						\
				  /* if stm == */ mlc::not_found >				\
    {												\
      typedef typename superior_find< from, target,						\
				      stc_super(curr) >::ret ret;				\
    };												\
												\
    template <typename from, typename target, typename curr>					\
    struct helper_superior_find < from, target, curr,						\
				  /* if stm == */ stc::not_delegated >				\
    {												\
      typedef typename superior_find< from, target,						\
				      stc_super(curr) >::ret ret;				\
    };												\
												\
    template <typename from, typename target, typename curr, typename T>			\
    struct helper_superior_find < from, target, curr,						\
				  /* if stm == */ stc::final<T> >				\
    {												\
      typedef T ret;										\
    };												\
												\
    template <typename from, typename target, typename curr, typename stm>			\
    struct helper_superior_find /* otherwise */							\
    {												\
      typedef stm ret;										\
    };												\
												\
    template <typename from, typename target>							\
    struct superior_find < from, target, /* if curr == */ mlc::none >				\
    {												\
      typedef mlc::not_found ret;								\
    };												\
												\
    template <typename from, typename target, typename curr>					\
    struct superior_find /* otherwise */							\
    {												\
      typedef typename helper_superior_find<from, target, curr,					\
					    stc_stm(curr, target)>::ret ret;			\
    };												\
												\
												\
												\
    /*												\
     * delegator_find(from, target)								\
     *												\
     *												\
     * delegator_find(from, target)								\
     * {											\
     *   precondition(target != delegatee_type);						\
     *   delegatee = superior_find(from, delegatee_type);					\
     *   if (delegatee == mlc::not_found)							\
     *     return mlc::not_found;								\
     *   else											\
     *     return find(delegatee, target);							\
     * }											\
     */												\
												\
    template <typename from, typename target>							\
    struct helper_delegator_find < from, target,						\
				   /* if delegatee == */ mlc::not_found >			\
    {												\
      typedef mlc::not_found ret;								\
    };												\
												\
    template <typename from, typename target, typename delegatee>				\
    struct helper_delegator_find /* otherwise */						\
    {												\
      typedef typename find<delegatee, target>::ret ret;					\
    };												\
												\
    template <typename from, typename target>							\
    struct delegator_find									\
    {												\
      typedef typename superior_find<from, typedef_::delegatee_type>::ret delegatee;		\
      typedef typename helper_delegator_find<from, target, delegatee>::ret ret;			\
    };												\
												\
												\
												\
    /*												\
     * helper_find(from, target, location, res)							\
     *												\
     *												\
     *												\
     */												\
												\
    template <typename from, typename target, typename location>				\
    struct helper_find < from, target, location,						\
			 /* if res == */ mlc::not_found >					\
    {												\
      typedef typename delegator_find<from, target>::ret ret;					\
    };												\
												\
    template <typename from, typename target, typename location>				\
    struct helper_find < from, target, location,						\
			 /* if res == */ stc::abstract >					\
    {												\
      typedef typename delegator_find<from, target>::ret res_d;					\
      struct check_										\
	: mlc::assert_< mlc::is_found_<res_d>,							\
			ERROR::vtype_declared_but_not_defined					\
			<  ERROR::_for_vtype_<target>,						\
			   ERROR::_declaration_is_in_<location>,				\
			   ERROR::_definition_is_looked_up_from_<from>  > >			\
      /* FIXME: error("<target> declared in <location> but not defined at <from>"); */		\
      {												\
	typedef res_d ret;									\
      };											\
      typedef typename check_::ret ret;								\
    };												\
												\
    template <typename from, typename target, typename location>				\
    struct helper_find < from, target, location,						\
			 /* if res == */ stc::not_delegated >					\
    {												\
      typedef typename superior_find<from, target>::ret ret;					\
    };												\
												\
    template <typename from, typename target, typename location, typename res>			\
    struct helper_find /* otherwise */								\
    {												\
      typedef res ret;										\
    };												\
												\
												\
												\
    /*												\
     * find(from, target)									\
     *												\
     * FIXME: ...										\
     *												\
     * find(from, target)									\
     * {											\
     *   if (target == delegatee_type)								\
     *     return superior_find(from, delegatee_type);						\
     *   else											\
     *     return helper_find(from, target);							\
     * }											\
     */												\
												\
    template <typename from>									\
    struct find <from, /* if target == */ typedef_::delegatee_type >				\
    {												\
      typedef typename superior_find<from, typedef_::delegatee_type>::ret ret;			\
    };												\
												\
    template <typename from, typename target>							\
    struct find /* otherwise */									\
    {												\
      typedef typename first_stm<from, target>::ret stm;					\
      typedef mlc_elt(stm, 1) location;								\
      typedef mlc_elt(stm, 2) res;								\
      typedef typename helper_find<from, target,						\
				   location, res>::ret ret;					\
    };												\
												\
												\
  } /* end of SCOOPED_NAMESPACE::stc_vtype_internal */						\
												\
												\
  /*												\
   * find_vtype(from, target)									\
   *												\
   * FIXME:...											\
   *												\
   */												\
												\
  template <typename from, typename target>							\
  struct deferred_vtype										\
  {												\
    typedef typename stc_vtype_internal::find<from, target>::ret ret;				\
  };												\
												\
  template <typename from, typename target>							\
  struct find_vtype										\
  {												\
    typedef typename stc_vtype_internal::find<from, target>::ret ret;				\
    typedef typename stc_vtype_internal::check<from, target>::ret chk;				\
  };												\
												\
  template <typename from, typename target>							\
  struct vtype											\
  {												\
    typedef typename find_vtype<from, target>::ret res;						\
    struct check_ : mlc::assert_< mlc::is_found_<res> >						\
    {												\
      typedef res ret;										\
    };												\
    typedef typename check_::ret ret;								\
  };												\
												\
												\
} /* end of SCOOPED_NAMESPACE */								\
												\
												\
struct e_n_d__w_i_t_h___s_e_m_i_c_o_l_o_n;



# define stc_find_vtype_(Namespace, From, Target) \
   Namespace::find_vtype<From, Namespace::typedef_::Target##_type>::ret

# define stc_find_vtype(Namespace, From, Target) \
   typename stc_find_vtype_(Namespace, From, Target)



# define stc_deferred_vtype(Namespace, From, Target) \
   typename Namespace::deferred_vtype<From, Namespace::typedef_::Target##_type>::ret



# define stc_vtype_(Namespace, From, Target) \
   Namespace::vtype<From, Namespace::typedef_::Target##_type>::ret

# define stc_vtype(Namespace, From, Target) \
   typename stc_vtype_(Namespace, From, Target)



# define stc_find_deduce_vtype_(Namespace, From, Target1, Target2)				\
   Namespace::find_vtype< Namespace::find_vtype<From,						\
						Namespace::typedef_::Target1##_type>::ret,	\
			  Namespace::typedef_::Target2##_type>::ret

# define stc_find_deduce_vtype(Namespace, From, Target1, Target2)				\
   typename											\
   Namespace::find_vtype< typename								\
                          Namespace::find_vtype<From,						\
						Namespace::typedef_::Target1##_type>::ret,	\
			  Namespace::typedef_::Target2##_type>::ret



# define stc_deduce_deferred_vtype(Namespace, From, Target1, Target2)			\
   typename										\
   Namespace::deferred_vtype< typename							\
                     Namespace::deferred_vtype<From,					\
				      Namespace::typedef_::Target1##_type>::ret,	\
		     Namespace::typedef_::Target2##_type>::ret				\



# define stc_deduce_vtype_(Namespace, From, Target1, Target2)				\
   Namespace::vtype< Namespace::vtype<From,						\
				      Namespace::typedef_::Target1##_type>::ret,	\
		     Namespace::typedef_::Target2##_type>::ret

# define stc_deduce_vtype(Namespace, From, Target1, Target2)				\
   typename										\
   Namespace::vtype< typename								\
                     Namespace::vtype<From,						\
				      Namespace::typedef_::Target1##_type>::ret,	\
		     Namespace::typedef_::Target2##_type>::ret





// # define stc_vtype_is_found_(Namespace, From, Target)
//    mlc::is_found_< Namespace::find_vtype<From, Namespace::typedef_::Target##_type>::ret >


#endif // ! STC_SCOOP_HH
