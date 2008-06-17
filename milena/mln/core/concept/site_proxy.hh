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

#ifndef MLN_CORE_CONCEPT_SITE_PROXY_HH
# define MLN_CORE_CONCEPT_SITE_PROXY_HH

/*! \file mln/core/concept/site_proxy.hh
 *
 * \brief Definition of the concept of mln::Site_Proxy.
 */

# include <mln/core/concept/proxy.hh>
# include <mln/metal/is_a.hh>



namespace mln
{


  // Fwd decl.
  template <typename E> struct Site_Proxy;



  namespace internal
  {

    // Every "Site_Proxy" class should derive from site_impl.  The
    // couple of classes below are provided to be specialized so that
    // an effective implementation (with the interface of the
    // targetted site) can equip site proxy classes.

    template <typename Site, typename E>
    struct site_const_impl
    {
      // Default is none.
    };

    template <typename Site, typename E>
    struct site_mutable_impl
    {
      // Default is none.
    };

    template <bool is_mutable, typename Site, typename E>
    struct site_impl; // Is a selector w.r.t. to mutability.

    template <typename Site, typename E>
    struct site_impl</* is_mutable = */ true, Site, E> : site_mutable_impl<Site, E>
    {
    };

    template <typename Site, typename E>
    struct site_impl</* is_mutable = */ false, Site, E> : site_const_impl<Site, E>
    {
    };



    // Fwd decl.
    template <typename P> struct site_from;

    namespace deep
    {

      template <typename P, bool is_site_proxy = true>
      struct helper_site_from
      {
	typedef typename P::subject S;
	// Recursion.
	typedef typename mln::internal::site_from<S>::ret ret;
	// Routine.
	static const ret& on(const P& p)
	{
	  return p.to_site();
	}
      };

      template <typename O>
      struct helper_site_from<O, /* is_site_proxy = */ false>
      {
	// Stop Recursion.
	typedef O ret;
	// Routine.
	static const ret& on(const O& obj)
	{
	  return obj;
	}
      };

    } // end of namespace internal::deep


    /// Meta-routine to get the site type from a type \p P that can be
    /// either a site proxy or a site.
    template <typename P>
    struct site_from
    {
      typedef mln_exact(P) E;
      enum { is_site_proxy = mlc_is_a(E, Site_Proxy)::value };
      typedef typename deep::helper_site_from< E, is_site_proxy >::ret ret;
    };


    /// Access to site reference.
    template <typename O>
    const typename site_from<O>::ret&
    to_site(const O& obj);


  } // end of namespace internal




  /// Site_Proxy category flag type.
  template <>
  struct Site_Proxy<void>
  {
    typedef Proxy<void> super;
  };


  /*! \brief Base class for implementation classes of the notion of
   *  "site proxy".
   *
   * FIXME: Explain...
   */
  template <typename E>
  struct Site_Proxy : public Proxy<E>
  {
    typedef Site_Proxy<void> category;

    /*
      typedef site;

      const site& to_site() const;
      operator site() const;

      FIXME: Add if possible a mutable version of to_site().
      FIXME: Or (?) just remove this method (!)
    */

  protected:
    Site_Proxy();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Site_Proxy<E>::Site_Proxy()
  {
    typedef mln_site(E) site;

    const site& (E::*m1)() const = & E::to_site;
    m1 = 0;

    site (E::*m2)() const = & E::operator site;
    m2 = 0;
  }

  namespace internal
  {

    // Access to site reference.

    template <typename O>
    const typename site_from<O>::ret&
    to_site(const O& obj)
    {
      typedef mln_exact(O) E;
      enum { is_site_proxy = mlc_is_a(E, Site_Proxy)::value };
      return deep::helper_site_from<E, is_site_proxy>::on(exact(obj));
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_SITE_PROXY_HH
