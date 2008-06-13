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


  /*
    FIXME: Re-activate.

  namespace trait
  {

    template < typename Op, typename P1, typename P2 >
    struct set_binary_< Op, mln::Proxy, P1, mln::Proxy, P2 >
    {
      typedef mln_trait_binary(Op, mln_subject(P1), mln_subject(P2)) ret;
    };

    template < typename Op, typename P, typename O >
    struct set_binary_< Op, mln::Proxy, P, mln::Object, O >
    {
      typedef mln_trait_binary(Op, mln_subject(P), O) ret;
    };

    template < typename Op, typename E, typename P >
    struct set_binary_< Op, mln::Object, E, mln::Proxy, P  >
    {
      typedef mln_trait_binary(Op, O, mln_subject(P)) ret;
    };

    template < typename Op, typename P >
    struct set_binary_< Op, mln::Proxy, P >
    {
      typedef mln_trait_unary(Op, mln_subject(P)) ret;
    };

  } // end of namespace mln::trait

  */



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
    {};

    template <typename Subject, typename E>
    struct helper_proxy_impl< Subject, E, false > // Stop rec.
    {};

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
