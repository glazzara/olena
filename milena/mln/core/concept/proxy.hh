// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_CONCEPT_PROXY_HH
# define MLN_CORE_CONCEPT_PROXY_HH

/*! \file mln/core/concept/proxy.hh
 *
 * \brief Definition of the concept of mln::Proxy.
 *
 * \todo preinc and predec are not tested; post-like ops are not handled.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/internal/force_exact.hh>
# include <mln/value/ops.hh> // So that we can handle builtins, scalars, and objects.



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
    return Symb exact(rhs).unproxy();		\
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
    typedef typename internal::unproxy_couple<L, R>::L_helper L_unproxy;	\
    typedef typename internal::unproxy_couple<L, R>::R_helper R_unproxy;	\
    return L_unproxy::on(lhs) Symb R_unproxy::on(rhs);				\
  }										\
										\
  template <typename P, typename O>						\
  inline									\
  mln_trait_op_##Name(P, O)							\
  operator Symb (const Proxy<P>& p, const Object<O>& o)				\
  {										\
    return exact(p).unproxy() Symb exact(o);					\
  }										\
  										\
  template <typename O, typename P>						\
  inline									\
  mln_trait_op_##Name(O, P)							\
  operator Symb (const Object<O>& o, const Proxy<P>& p)				\
  {										\
    return exact(o) Symb exact(p).unproxy();					\
  }										\
										\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n




namespace mln
{

  // Fwd decls.
  template <typename E> struct Proxy;


  namespace internal
  {

    template <bool b, typename P> struct unproxy_if;

    template <typename P>
    struct unproxy_if< true, P >
    {
      typedef mln_subject(P) ret;
      static typename P::q_subject on(const Proxy<P>& p) { return exact(p).unproxy(); }
    };

    template <typename P>
    struct unproxy_if< false, P >
    {
      typedef P ret;
      static const P& on(const Proxy<P>& p) { return exact(p); }
    };

    template <typename L, typename R>
    struct unproxy_couple
    {
      enum { L_level = L::proxy_level,
	     R_level = R::proxy_level }; // To prevent a weird g++ warning.
      typedef internal::unproxy_if<(L_level >= R_level), L> L_helper;
      typedef internal::unproxy_if<(R_level >= L_level), R> R_helper;
      typedef typename L_helper::ret L_ret;
      typedef typename R_helper::ret R_ret;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    // Unary ops.

    template < template <class> class Op, typename P >
    struct set_unary_< Op, mln::Proxy, P >
    {
      typedef mln_trait_unary(Op, mln_subject(P)) ret;
    };

    // Binary ops.

    template < template <class, class> class Op,
	       typename L, typename R >
    struct set_binary_< Op, mln::Proxy, L, mln::Proxy, R >
    {
      typedef mln::internal::unproxy_couple<L, R> helper;
      typedef mln_trait_binary(Op,
			       typename helper::L_ret,
			       typename helper::R_ret) ret;
    };

    template < template <class, class> class Op,
	       typename P, typename O >
    struct set_binary_< Op, mln::Proxy, P, mln::Object, O >
    {
      typedef mln_trait_binary(Op, mln_subject(P), O) ret;
    };

    template < template <class, class> class Op,
	       typename O, typename P >
    struct set_binary_< Op, mln::Object, O, mln::Proxy, P  >
    {
      typedef mln_trait_binary(Op, O, mln_subject(P)) ret;
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
  struct Proxy : public Object<E>
  {
    typedef Proxy<void> category;

    /*
      enum { proxy_level };
      typedef subject;
      typedef q_subject;
      q_subject unproxy() const;

      // FIXME:
      // return "const subject&"?
      // overload with not-const method?
      // add op subject() const?
    */
  protected:
    Proxy();
  };



  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Proxy<P>& p);


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



  namespace internal
  {

    // External way of getting an address of an object from/through a
    // proxy.  This is a recursive implementation since we can have a
    // proxy of proxy, etc.

    // Case 1: Not found so unproxy.

    template <typename T, typename P>
    void get_adr(const T *& ptr, const Proxy<P>& obj)
    {
      get_adr(ptr, exact(obj).unproxy());
    }

    template <typename T, typename P>
    void get_adr(      T *& ptr,       Proxy<P>& obj)
    {
      get_adr(ptr, exact(obj).unproxy());
    }

    // Case 2: Found.  (Note that T can also be a Proxy.)

    template <typename T>
    void get_adr(const T *& ptr, const Object<T>& obj)
    {
      ptr = & exact(obj);
    }

    template <typename T>
    void get_adr(      T *& ptr,       Object<T>& obj)
    {
      ptr = & exact(obj);
    }

    template <typename T>
    void get_adr(      T *& ptr,       T& obj)
    {
      ptr = & obj;
    }

    // Case 3: Fail to found!

    template <typename T, typename O>
    void get_adr(const T *& ptr, const Object<O>& obj)
    {
      ptr = 0;
    }

    template <typename T, typename O>
    void get_adr(      T *& ptr,       Object<O>& obj)
    {
      ptr = 0;
    }


    // A proxy should convert towards its subject.  And, if we have a
    // proxy of proxy, it should also convert towards its subject of
    // subject, and so on.  It leads to a recursive implementation
    // where conversions are automatically obtained through
    // inheritance.
    //
    // E is a Proxy type; Subject is its subject type.

    template <typename Subject, typename E> struct proxy_impl;

    template <typename Subject, typename E, bool rec = true>
    struct helper_proxy_impl : proxy_impl< mln_subject(Subject), E > // Rec.
    {
      enum { proxy_level = Subject::proxy_level + 1};
    };

    template <typename Subject, typename E>
    struct helper_proxy_impl< Subject, E, false > // Stop rec.
    {
      enum { proxy_level = 1 };
    };

    template <typename Subject, typename E>
    struct proxy_impl : helper_proxy_impl< Subject, E,
					   mlc_is_a(Subject, Proxy)::value >
    {
      operator Subject() const
      {
 	return mln::internal::force_exact<const E>(*this).unproxy();

	// Technical note:
	//
	// The code above seems more effective than the one below:
	// 	const Subject* adr;
	// 	get_adr(adr, mln::internal::force_exact<const E>(*this));
	// 	mln_postcondition(adr != 0);
	// 	return *adr;
      }
    };


  } // end of namespace mln::internal




# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Proxy<E>::Proxy()
  {
    enum { proxy_level = E::proxy_level }; // FIXME: Check that it is >= 0...

    typedef mln_subject(E) subject;
    typedef typename E::q_subject q_subject;

    q_subject (E::*m)() const = & E::unproxy;
    m = 0;
  }

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Proxy<P>& p)
  {
    return ostr << exact(p).unproxy();
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
