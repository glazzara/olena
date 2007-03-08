
/* \file oln/stc/scoop.hxx
   \brief To be included within a namespace to import SCOOP 2 equipment. */


mlc_decl_typedef(exact_);

mlc_decl_typedef(category);
mlc_decl_typedef(behavior);
mlc_decl_typedef(delegatee);



namespace ERROR
{

  template < typename first_definition,
	       typename re_definition,
	       typename target >
  struct final_vtype_redefined_;


  template < typename first_definition,
	       typename declaration,
	       typename target >
  struct vtype_declared_but_already_set_;


  template < typename from,
	       typename target >
  struct vtype_not_defined;


  template < typename curr,
	       typename target,
	       typename stm >
  struct wrong_final_stm_;


  template < typename target,
	       typename declaration,
	       typename from >
  struct vtype_declared_but_not_defined;


  template <typename T>
  struct _declaration_is_in_;

  template <typename T>
  struct _definition_is_looked_up_from_;


  template <typename T>
  struct _for_vtype_;


  struct no_delegatee_declared_;

  template <typename T>
  struct super_trait_not_defined_for_;


} /* end of namespace ERROR */




/* vtypes */

template <typename from_type>
struct vtypes
{
};

template <typename from_type>
struct vtypes <const from_type> : public vtypes <from_type>
{
};

template <typename from_type, typename type>
struct single_vtype
{
  typedef mlc::not_found ret;
};



/* super_trait_ */

template <typename from_type>
struct super_trait_;


template <typename from_type>
struct super_trait_ <const from_type> : super_trait_<from_type>
{
};

template <template <class> class abstraction, typename Exact>
struct super_trait_< abstraction<Exact> >
{
  typedef mlc::none ret;
};

template <typename from_type>
struct super_trait_ :
  mlc::abort_< ERROR::super_trait_not_defined_for_<from_type> >
{
  typedef mlc::none ret;
};





/* Any */

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






namespace internal
{


  /*
   * Forward declarations.
   */


  template <typename from, typename target>
  struct get_stm;

  template <typename res1, typename res2>
  struct helper_get_stm;

  template <typename from, typename target>
  struct first_stm;

  template <typename curr, typename target, typename stm>
  struct helper_first_stm;


  template <typename from, typename target, typename curr = from>
  struct superior_find;

  template <typename from, typename target, typename curr, typename stm>
  struct helper_superior_find;

  template <typename from, typename target>
  struct delegator_find;

  template <typename from, typename target, typename delegatee>
  struct helper_delegator_find;

  template <typename from, typename target>
  struct find;

  template <typename from, typename target, typename where, typename res>
  struct helper_find;



  template <typename from, typename target>
  struct check;

  template <typename curr, typename target, typename stm>
  struct helper_check;



  /* get_stm(from, target)
   *
   * Returns the statement for class 'from' corresponding to 'target'.
   * This statement is either defined in 'vtypes' or in 'single_vtype'.
   *
   * This routine is local; it does not recourse on superior classes.
   *
   */

  template <typename res1>
  struct helper_get_stm <res1, mlc::not_found>
  {
    typedef res1 ret;
  };

  template <typename res2>
  struct helper_get_stm <mlc::not_found, res2>
  {
    typedef res2 ret;
  };

  template <>
  struct helper_get_stm <mlc::not_found, mlc::not_found>
  {
    typedef mlc::not_found ret;
  };

  template <typename res1, typename res2>
  struct helper_get_stm /* FIXME: Insert mlc::abort_<> statement here?  */
  {
    /* error */
  };

  template <typename from, typename target>
  struct get_stm
  {
    typedef vtypes<from> decl1;
    typedef typename target::template from_<decl1>::ret res1;

    typedef single_vtype<from, target> decl2;
    typedef typename decl2::ret ret2;

    typedef typename helper_get_stm<res1, ret2>::ret ret;
  };




  /* -------------------- */
  /* Checking algorithm.  */
  /* -------------------- */

  /*
   * check_no_final_inherited
   *
   * FIXME: ...
   */

  template < typename orig, typename curr, typename target,
	       typename stm = typename get_stm<curr, target>::ret >
  struct check_no_final_inherited

    : mlc::assert_< mlc_is_not_a(stm, stc::final),
		      ERROR::final_vtype_redefined_< orig,
						     mlc::pair_<curr, stm>,
						     target >
                  >,
	/* rec */ check_no_final_inherited<orig, stc_super(curr), target>
  {
  };

  template <typename orig, typename target, typename stm>
  struct check_no_final_inherited <orig, /* if curr == */ mlc::none, target, stm>
  {
  };


  /*
   * check_no_stm_inherited
   *
   * FIXME: ...
   */

  template < typename orig, typename curr, typename target,
	       typename stm = typename get_stm<curr, target>::ret >
  struct check_no_stm_inherited

    : mlc::assert_< stc::is_not_found<stm>,
		      ERROR::vtype_declared_but_already_set_< orig,
							      mlc::pair_<curr, stm>,
							      target >
		      >,
	/* rec */ check_no_stm_inherited<orig, stc_super(curr), target>
  {
  };

  template <typename orig, typename target, typename stm>
  struct check_no_stm_inherited <orig, /* if curr == */ mlc::none, target, stm>
  {
  };


  /*
   * check_delegatee_inherited
   */

  template <typename curr, typename target>
  struct check_delegatee_inherited
    : mlc::assert_< stc::is_found< typename first_stm<curr,
				     typedef_::delegatee>::ret::second_elt >,
		      ERROR::no_delegatee_declared_ >
  {
  };


  /*
   * check_final_stm
   */

  template <typename curr, typename target, typename stm>
  struct check_final_stm
  {};

  template <typename curr, typename target>
  struct check_final_stm < curr, target, stc::abstract >
    : mlc::abort_< ERROR::wrong_final_stm_<curr, target, stc::final<stc::abstract> > >
  {};

  template <typename curr, typename target, typename T>
  struct check_final_stm < curr, target, stc::final<T> >
    : mlc::abort_< ERROR::wrong_final_stm_<curr, target, stc::final<stc::final<T> > > >
  {};

  template <typename curr, typename target>
  struct check_final_stm < curr, target, stc::not_delegated >
    : mlc::abort_< ERROR::wrong_final_stm_<curr, target, stc::final<stc::not_delegated> > >
  {};

  template <typename curr, typename target>
  struct check_final_stm < curr, target, mlc::not_found >
    : mlc::abort_< ERROR::wrong_final_stm_<curr, target, stc::final<mlc::not_found> > >
  {};




  /*
   * check(from, target)
   *
   * pseudo-code:
   *
   * check(from, target)
   * {
   *   if (from == mlc::none) // stop because no more class
   *     return done;
   *   else
   *     return helper_check(from, target, get_stm(from, target));
   * }
   */

  struct dummy {};

  template <typename target>
  struct check < /* if from == */ mlc::none, target >
  {
    typedef dummy ret;
  };

  template <typename from, typename target>
  struct check /* otherwise */
  {
    typedef typename helper_check< from, target,
				     stc_stm(from, target) >::ret ret;
  };




  /*
   * pseudo-code:
   *
   * helper_check(curr, target, stm)
   * {
   *   super = super(curr);
   *
   *   if (stm == stc::abstract) {
   *     check_no_stm_inherited(curr, super, target);
   *     return check(super, target);
   *   }
   *   if (stm == stc::final<T>) {
   *     check_final_stm(T);
   *     check_no_final_inherited((curr, T), super, target);
   *     return check(super, target);
   *   }
   *   if (stm == stc::not_delegated) {
   *     check_delegatee_inherited(curr);
   *     check_no_final_inherited((curr, stc::not_delegated), super, target);
   *     return check(super, target);
   *   }
   *   if (stm == mlc::not_found) {
   *     return check(super, target);
   *   }
   *   // otherwise, non-final stm:
   *   if (stm == T) {
   *     check_no_final_inherited((curr, T), super, target);
   *     return check(super, target);
   *   }
   * }
   *
   */

  template <typename curr, typename target>
  struct helper_check < curr, target,
			  /* if stm == */ stc::abstract >
    : check_no_stm_inherited< mlc::pair_<curr, stc::abstract >,
				stc_super(curr),
				target >
  {
    typedef typename check<stc_super(curr), target>::ret ret;
  };

  template <typename curr, typename target, typename T>
  struct helper_check < curr, target,
			  /* if stm == */ stc::final<T> >
    : check_no_final_inherited< mlc::pair_<curr, stc::final<T> >,
				  stc_super(curr),
				  target >
  {
    /* FIXME:
	 check_final_stm(T);
    */
    typedef typename check<stc_super(curr), target>::ret ret;
  };

  template <typename curr, typename target>
  struct helper_check < curr, target,
			  /* if stm == */ stc::not_delegated >
    : check_no_final_inherited< mlc::pair_<curr, stc::not_delegated>,
				  stc_super(curr),
				  target >
  {
    /* FIXME:
	 check_delegatee_inherited(curr);
    */
    typedef typename check<stc_super(curr), target>::ret ret;
  };

  template <typename curr, typename target>
  struct helper_check < curr, target,
			  /* if stm == */ mlc::not_found >
  {
    typedef typename check<stc_super(curr), target>::ret ret;
  };

  template <typename curr, typename target, typename stm>
  struct helper_check /* otherwise, non-final stm */
    : check_no_final_inherited< mlc::pair_<curr, stm>,
				  stc_super(curr),
				  target >
  {
    typedef typename check<stc_super(curr), target>::ret ret;
  };



  /* ------------------- */
  /* Look-up algorithm.  */
  /* ------------------- */

  /*
   * first_stm(from, target)
   *
   * returns a pair (Where, Value) with Value being:
   * - stc::abstract
   * - stc::not_delegated
   * - mlc::not_found
   * - a type T
   * and Where being the class where the stm is found.
   *
   *
   * helper_first_stm(curr, target, stm)
   * {
   *   if (stm == mlc::not_found)
   *     return first_stm(super(curr), target);
   *   else if (stm == stc::final<T>)
   *     return pair(curr, T);
   *   else
   *     return pair(curr, stm);
   * }
   *
   * first_stm(from, target)
   * {
   *   if (from == mlc::none)
   *     return pair(mlc::none, mlc::not_found);
   *   else
   *     return helper_first_stm(from, target,
   *                             get_stm(from, target));
   * }
   *
   */

  template <typename curr, typename target>
  struct helper_first_stm < curr, target,
			      /* if stm == */ mlc::not_found >
  {
    typedef typename first_stm<stc_super(curr), target>::ret ret;
  };

  template <typename curr, typename target, typename T>
  struct helper_first_stm < curr, target,
			      /* if stm == */ stc::final<T> >
  {
    typedef mlc::pair_<curr, T> ret;
  };

  template <typename curr, typename target, typename stm>
  struct helper_first_stm /* otherwise */
  {
    typedef mlc::pair_<curr, stm> ret;
  };

  template <typename target>
  struct first_stm < /* if from == */ mlc::none, target >
  {
    typedef mlc::pair_<mlc::none, mlc::not_found> ret;
  };

  template <typename from, typename target>
  struct first_stm /* otherwise */
  {
    typedef typename helper_first_stm<from, target,
					stc_stm(from, target)>::ret ret;
  };



  /*
   * superior_find(from, target)
   *
   * returns:
   *   either mlc::not_found
   *   or a type T
   * behavior:
   * - the stm stc::not_delegated is ignored
   * - error when stc::abstract is reached
   *
   * helper_superior_find(from, target, curr, stm)
   * {
   *   if (stm == stc::abstract)
   *     error("<target> declared in <curr> but not defined at <from>");
   *   else if (stm == mlc::not_found or
   *            stm == stc::not_delegated)
   *     return superior_find(from, target, super(curr));
   *   else if (stm == stc::final<T>)
   *     return T;
   *   else
   *     return stm;
   * }
   *
   * superior_find(from, target, curr = from)
   * {
   *   if (curr == mlc::none)
   *     return mlc::not_found;
   *   else
   *     return helper_superior_find(from, target, curr,
   *                                 get_stm(curr, target));
   * }
   */

  template <typename from, typename target, typename curr>
  struct helper_superior_find < from, target, curr,
				  /* if stm == */ stc::abstract >
  {
    /* FIXME: error("<target> declared in <curr> but not defined at <from>"); */
  };

  template <typename from, typename target, typename curr>
  struct helper_superior_find < from, target, curr,
				  /* if stm == */ mlc::not_found >
  {
    typedef typename superior_find< from, target,
				      stc_super(curr) >::ret ret;
  };

  template <typename from, typename target, typename curr>
  struct helper_superior_find < from, target, curr,
				  /* if stm == */ stc::not_delegated >
  {
    typedef typename superior_find< from, target,
				      stc_super(curr) >::ret ret;
  };

  template <typename from, typename target, typename curr, typename T>
  struct helper_superior_find < from, target, curr,
				  /* if stm == */ stc::final<T> >
  {
    typedef T ret;
  };

  template <typename from, typename target, typename curr, typename stm>
  struct helper_superior_find /* otherwise */
  {
    typedef stm ret;
  };

  template <typename from, typename target>
  struct superior_find < from, target, /* if curr == */ mlc::none >
  {
    typedef mlc::not_found ret;
  };

  template <typename from, typename target, typename curr>
  struct superior_find /* otherwise */
  {
    typedef typename helper_superior_find<from, target, curr,
					    stc_stm(curr, target)>::ret ret;
  };



  /*
   * delegator_find(from, target)
   *
   *
   * delegator_find(from, target)
   * {
   *   precondition(target != delegatee);
   *   delegatee = superior_find(from, delegatee);
   *   if (delegatee == mlc::not_found)
   *     return mlc::not_found;
   *   else
   *     return find(delegatee, target);
   * }
   */

  template <typename from, typename target>
  struct helper_delegator_find < from, target,
				   /* if delegatee == */ mlc::not_found >
  {
    typedef mlc::not_found ret;
  };

  template <typename from, typename target, typename delegatee>
  struct helper_delegator_find /* otherwise */
  {
    typedef typename find<delegatee, target>::ret ret;
  };

  template <typename from, typename target>
  struct delegator_find
  {
    typedef typename superior_find<from, typedef_::delegatee>::ret delegatee;
    typedef typename helper_delegator_find<from, target, delegatee>::ret ret;
  };



  /*
   * helper_find(from, target, where, res)
   *
   *
   *
   */

  template <typename from, typename target, typename where>
  struct helper_find < from, target, where,
			 /* if res == */ mlc::not_found >
  {
    typedef typename delegator_find<from, target>::ret ret;
  };

  template <typename from, typename target, typename where>
  struct helper_find < from, target, where,
			 /* if res == */ stc::abstract >
  {
    typedef typename delegator_find<from, target>::ret res_d;
    struct check_
	: mlc::assert_< stc::is_found<res_d>,
			ERROR::vtype_declared_but_not_defined
			<  ERROR::_for_vtype_<target>,
			   ERROR::_declaration_is_in_<where>,
			   ERROR::_definition_is_looked_up_from_<from>  > >
    /* FIXME: error("<target> declared in <where> but not defined at <from>"); */
    {
	typedef res_d ret;
    };
    typedef typename check_::ret ret;
  };

  template <typename from, typename target, typename where>
  struct helper_find < from, target, where,
			 /* if res == */ stc::not_delegated >
  {
    typedef typename superior_find<from, target>::ret ret;
  };

  template <typename from, typename target, typename where, typename res>
  struct helper_find /* otherwise */
  {
    typedef res ret;
  };



  /*
   * find(from, target)
   *
   * FIXME: ...
   *
   * find(from, target)
   * {
   *   if (target == delegatee)
   *     return superior_find(from, delegatee);
   *   else
   *     return helper_find(from, target);
   * }
   */

  template <typename from>
  struct find <from, /* if target == */ typedef_::delegatee >
  {
    typedef typename superior_find<from, typedef_::delegatee>::ret ret;
  };

  template <typename from, typename target>
  struct find /* otherwise */
  {
    typedef typename first_stm<from, target>::ret stm;
    typedef mlc_elt(stm, 1) where;
    typedef mlc_elt(stm, 2) res;
    typedef typename helper_find<from, target,
				   where, res>::ret ret;
  };


} /* end of internal */


/*
 * find_vtype(from, target)
 *
 * FIXME:...
 *
 */

template <typename from, typename target>
struct deferred_vtype
{
  typedef typename internal::find<from, target>::ret ret;
};

template <typename from, typename target>
struct find_vtype
{
  typedef typename internal::find<from, target>::ret ret;
  typedef typename internal::check<from, target>::ret chk;
};

template <typename from, typename target>
struct vtype
{
  typedef typename find_vtype<from, target>::ret res;
  struct check_ : mlc::assert_< stc::is_found<res>,
                                ERROR::vtype_not_defined<from, target> >
  {
    typedef res ret;
  };
  typedef typename check_::ret ret;
};



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


} /* end of namespace internal */


/* top class */

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



namespace automatic
{

  /*
   *  set_impl
   */

  template < template <class> class abstraction, typename behavior, typename E >
  struct set_impl
  /* to be defined by the client */ ;



  /*
   *  impl
   */

  template < template <class> class abstraction, typename behavior, typename E >
  struct impl : public set_impl< abstraction, behavior, E >
  { /* fetch */ };

  template < template <class> class abstraction, typename E >
  struct impl< abstraction, /* behavior is */ mlc::not_found, E >
  { /* nothing */ };

  template < template <class> class abstraction, typename E >
  struct impl< abstraction, mlc::none /* behavior */, E >
  { /* nothing */ };



  /*
   *  get_impl
   */

  template < template <class> class abstraction, typename E >
  struct get_impl : impl< abstraction, stc_find_type(E, behavior), E >
  { /* depends upon behavior */ };


} /* end of namespace automatic */
