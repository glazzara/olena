// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_CONCEPT_PROXY_HH
# define MLN_CORE_CONCEPT_PROXY_HH

/*! \file
 *
 * \brief Definition of the concept of mln::Proxy.
 *
 * \todo preinc and predec are not tested; post-like ops are not handled.
 *
 * \todo add "op=(T)" when possible, so add a constness property.
 * \todo add "opT()const" when possible.
 */

# include <mln/core/concept/object.hh>
# include <mln/value/ops.hh> // So that we can handle builtins, scalars, and objects.

# include <mln/convert/from_to.hxx>
# include <mln/core/concept/proxy.hxx>


# define mln_decl_unop_proxy(Name, Symb)	\
						\
  template <typename P>				\
  mln_trait_op_##Name(P)			\
  operator Symb (const Proxy<P>& rhs);		\
						\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_def_unop_proxy(Name, Symb)		\
						\
  template <typename P>				\
  inline					\
  mln_trait_op_##Name(P)			\
  operator Symb (const mln::Proxy<P>& rhs)	\
  {						\
    return Symb exact(rhs).unproxy_();		\
  }						\
						\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n





# define mln_decl_binop_proxy(Name, Symb)			\
								\
  template <typename L, typename R>				\
  mln_trait_op_##Name(L, R)					\
  operator Symb (const Proxy<L>& lhs, const Proxy<R>& rhs);	\
								\
  template <typename P, typename O>				\
  mln_trait_op_##Name(P, O)					\
  operator Symb (const Proxy<P>& p, const Object<O>& o);	\
								\
  template <typename O, typename P>				\
  mln_trait_op_##Name(O, P)					\
  operator Symb (const Object<O>& o, const Proxy<P>& p);	\
								\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n



# define mln_def_binop_proxy(Name, Symb)					\
										\
  template <typename L, typename R>						\
  inline									\
  mln_trait_op_##Name(L, R)							\
  operator Symb (const mln::Proxy<L>& lhs, const mln::Proxy<R>& rhs)		\
  {										\
    typedef typename internal::helper_unprox_binop<L, R>::L_helper L_helper;	\
    typedef typename internal::helper_unprox_binop<L, R>::R_helper R_helper;	\
    return L_helper::on(lhs) Symb R_helper::on(rhs);				\
  }										\
										\
  template <typename P, typename O>						\
  inline									\
  mln_trait_op_##Name(P, O)							\
  operator Symb (const Proxy<P>& p, const Object<O>& o)				\
  {										\
    return exact(p).unproxy_() Symb exact(o);					\
  }										\
  										\
  template <typename O, typename P>						\
  inline									\
  mln_trait_op_##Name(O, P)							\
  operator Symb (const Object<O>& o, const Proxy<P>& p)				\
  {										\
    return exact(o) Symb exact(p).unproxy_();					\
  }										\
										\
  template <typename P, typename L>						\
  inline									\
  mln_trait_op_##Name(P, L)							\
  operator Symb (const Proxy<P>& p, const Literal<L>& l)			\
  {										\
    return exact(p).unproxy_() Symb exact(l);					\
  }										\
  										\
  template <typename L, typename P>						\
  inline									\
  mln_trait_op_##Name(L, P)							\
  operator Symb (const Literal<L>& l, const Proxy<P>& p)			\
  {										\
    return exact(l) Symb exact(p).unproxy_();					\
  }										\
										\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n




namespace mln
{

  // Forward declarations.
  template <typename E> struct Proxy;
  template <typename E> struct Literal;


  namespace trait
  {

    // Unary ops.

    template < template <class> class Op, typename P >
    struct set_unary_< Op, mln::Proxy, P >
    {
      typedef mlc_unqualif(mln_q_subject(P)) S;
      typedef mln_trait_unary(Op, S) ret;
    };

    // Binary ops.

    template < template <class, class> class Op,
	       typename L, typename R >
    struct set_binary_< Op, mln::Proxy, L, mln::Proxy, R >
    {
      typedef mln::internal::helper_unprox_binop<L, R> helper;
      typedef mln_trait_binary(Op,
			       typename helper::L_ret,
			       typename helper::R_ret) ret;
    };

    template < template <class, class> class Op,
	       typename P, typename O >
    struct set_binary_< Op, mln::Proxy, P, mln::Object, O >
    {
      typedef mlc_unqualif(mln_q_subject(P)) S;
      typedef mln_trait_binary(Op, S, O) ret;
    };

    template < template <class, class> class Op,
	       typename O, typename P >
    struct set_binary_< Op, mln::Object, O, mln::Proxy, P  >
    {
      typedef mlc_unqualif(mln_q_subject(P)) S;
      typedef mln_trait_binary(Op, O, S) ret;
    };


    // Disambiguate between (Proxy Op Object) and (Object Op Literal).

    template < template <class, class> class Op,
	       typename P, typename L >
    struct set_binary_< Op, mln::Proxy, P, mln::Literal, L >
    {
      typedef mlc_unqualif(mln_q_subject(P)) S;
      typedef mln_trait_binary(Op, S, L) ret;
    };

    template < template <class, class> class Op,
	       typename L, typename P >
    struct set_binary_< Op, mln::Literal, L, mln::Proxy, P  >
    {
      typedef mlc_unqualif(mln_q_subject(P)) S;
      typedef mln_trait_binary(Op, L, S) ret;
    };

  } // end of namespace mln::trait




  /// Proxy category flag type.
  template <>
  struct Proxy<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes of the notion of
   *  "proxy".
   */
  template <typename E>
  struct Proxy : Object<E>
  {
    typedef Proxy<void> category;

    /*
      enum { proxy_level };
      typedef q_subject;
      q_subject subj_();
    */

  protected:
    Proxy();
  };


  /// \internal Conversion: proxy -> T
  template <typename P, typename T>
  void
  from_to_(const Proxy<P>& from, T& to);



  // subject

  template <typename T>
  struct subject
  {
    typedef typename mln::internal::unproxy_rec_<T>::ret q_ret;
    typedef mlc_unqualif(q_ret) ret;
  };


  // unproxy_rec

  template <typename T>
  typename mln::internal::unproxy_rec_<T>::ret
  unproxy_rec(T& t);

  template <typename T>
  typename mln::internal::unproxy_rec_<const T>::ret
  unproxy_rec(const T& t);


  // operator <<

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Proxy<P>& p);


  // operators

  mln_decl_unop_proxy(uplus,  + );
  mln_decl_unop_proxy(uminus, - );
  mln_decl_unop_proxy(preinc, ++ );
  mln_decl_unop_proxy(predec, -- );
  mln_decl_unop_proxy(not,    ! );

  mln_decl_binop_proxy(plus,  + );
  mln_decl_binop_proxy(minus, - );
  mln_decl_binop_proxy(times, * );
  mln_decl_binop_proxy(div,   / );
  mln_decl_binop_proxy(mod,   % );

  mln_decl_binop_proxy(eq,  == );
  mln_decl_binop_proxy(neq, != );

  mln_decl_binop_proxy(less,    <  );
  mln_decl_binop_proxy(leq,     <= );
  mln_decl_binop_proxy(geq,     >= );
  mln_decl_binop_proxy(greater, >  );

  mln_decl_binop_proxy(and, && );
  mln_decl_binop_proxy(or,  || );
  mln_decl_binop_proxy(xor, ^  );



# ifndef MLN_INCLUDE_ONLY


  // Proxy

  template <typename E>
  inline
  Proxy<E>::Proxy()
  {
    enum { proxy_level = E::proxy_level }; // FIXME: Check that it is >= 0...

    typedef typename E::q_subject q_subject;

    q_subject (E::*m_)() = & E::subj_;
    (void) m_;
  }



  // Conversion

  template <typename P, typename T>
  void
  from_to_(const Proxy<P>& from, T& to)
  {
    convert::from_to(exact(from).unproxy_(), to);
  }


  // unproxy_rec

  template <typename T>
  inline
  typename mln::internal::unproxy_rec_<T>::ret
  unproxy_rec(T& t)
  {
    return mln::internal::unproxy_rec_<T>::on(t);
  }

  template <typename T>
  inline
  typename mln::internal::unproxy_rec_<const T>::ret
  unproxy_rec(const T& t)
  {
    return mln::internal::unproxy_rec_<const T>::on(t);
  }


  // operator <<

  template <typename P>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Proxy<P>& p)
  {
    return ostr << unproxy_rec(p);
  }


  // Unary operators.

  mln_def_unop_proxy(uplus,  + );
  mln_def_unop_proxy(uminus, - );
  mln_def_unop_proxy(preinc, ++ );
  mln_def_unop_proxy(predec, -- );
  mln_def_unop_proxy(not,    ! );


  // Binary operators.

  mln_def_binop_proxy(plus,  + );
  mln_def_binop_proxy(minus, - );
  mln_def_binop_proxy(times, * );
  mln_def_binop_proxy(div,   / );
  mln_def_binop_proxy(mod,   % );

  mln_def_binop_proxy(eq,  == );
  mln_def_binop_proxy(neq, != );

  mln_def_binop_proxy(less,    <  );
  mln_def_binop_proxy(leq,     <= );
  mln_def_binop_proxy(geq,     >= );
  mln_def_binop_proxy(greater, >  );

  mln_def_binop_proxy(and, && );
  mln_def_binop_proxy(or,  || );
  mln_def_binop_proxy(xor, ^  );


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PROXY_HH
