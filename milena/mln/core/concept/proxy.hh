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
 */

# include <mln/core/concept/object.hh>
# include <mln/core/internal/force_exact.hh>
# include <mln/trait/all.hh>



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

    template < template <class> class Op, typename P >
    struct set_unary_< Op, mln::Proxy, P >
    {
      typedef mln_trait_unary(Op, mln_subject(P)) ret;
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



  template <typename L, typename R>
  inline
  mln_trait_op_minus(L, R)
  operator-(const mln::Proxy<L>& lhs, const mln::Proxy<R>& rhs)
  {
    typedef typename internal::unproxy_couple<L, R>::L_helper L_unproxy;
    typedef typename internal::unproxy_couple<L, R>::R_helper R_unproxy;
    return L_unproxy::on(lhs) - R_unproxy::on(rhs);
  }

  template <typename P, typename O>
  inline
  mln_trait_op_minus(P, O)
  operator-(const Proxy<P>& p, const Object<O>& o)
  {
    return exact(p).unproxy() - exact(o);
  }
  
  template <typename O, typename P>
  inline
  mln_trait_op_minus(O, P)
  operator-(const Object<O>& o, const Proxy<P>& p)
  {
    return exact(o) - exact(p).unproxy();
  }



  template <typename L, typename R>
  inline
  mln_trait_op_eq(L, R)
  operator==(const mln::Proxy<L>& lhs, const mln::Proxy<R>& rhs)
  {
    typedef typename internal::unproxy_couple<L, R>::L_helper L_unproxy;
    typedef typename internal::unproxy_couple<L, R>::R_helper R_unproxy;
    return L_unproxy::on(lhs) == R_unproxy::on(rhs);
  }

  template <typename P, typename O>
  inline
  mln_trait_op_eq(P, O)
  operator==(const Proxy<P>& p, const Object<O>& o)
  {
    return exact(p).unproxy() == exact(o);
  }
  
  template <typename O, typename P>
  inline
  mln_trait_op_eq(O, P)
  operator==(const Object<O>& o, const Proxy<P>& p)
  {
    return exact(o) == exact(p).unproxy();
  }



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
	// The code above seems more effective than the one below:
	//
	// 	const Subject* adr;
	// 	get_adr(adr, mln::internal::force_exact<const E>(*this));
	// 	mln_postcondition(adr != 0);
	// 	return *adr;
      }
    };


  } // end of namespace mln::internal


  // FIXME:...

//   template <typename L, typename R>
//   bool operator==(const Proxy<L>& lhs, const Proxy<R>& rhs);

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Proxy<P>& p);




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

  // FIXME: Code operators...

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PROXY_HH
