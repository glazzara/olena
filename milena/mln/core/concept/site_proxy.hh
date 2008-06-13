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



    // Meta-routine to get the site type from either a site or a site
    // proxy.

    template <typename P> struct site_from;

    template <typename P, bool is_proxy = true>
    struct helper_site_from
    {
      typedef typename P::subject P_;
      typedef typename site_from<P_>::ret ret;
    };

    template <typename P>
    struct helper_site_from<P, false>
    {
      typedef P ret;
    };

    template <typename P>
    struct site_from
    {
      enum { is_proxy = mlc_is_a(P, Site_Proxy)::value };
      typedef typename helper_site_from< P, is_proxy >::ret ret;
    };


    // Access to site reference.

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

    namespace deep
    {

      template <bool b> struct to_site_;

      template <>
      struct to_site_< /* is proxy = */ true >
      {
	template <typename P>
	const typename site_from<P>::ret&
	doit(const Site_Proxy<P>& p) const
	{
	  return exact(p).to_site();
	}
      };

      template <>
      struct to_site_< /* is proxy = */ false >
      {
	template <typename O>
	const O&
	doit(const O& obj) const
	{
	  return obj;
	}
      };

      // FIXME: was:

//       template <typename P>
//       const typename site_from<P>::ret&
//       to_site(const Site_Proxy<P>& p)
//       {
// 	return exact(p).to_site();
//       }

//       template <typename P>
//       const typename site_from<P>::ret&
//       to_site(const Object<P>& p)
//       {
// 	return exact(p);
//       }

    }  // end of namespace internal::deep

    template <typename O>
    const typename site_from<O>::ret&
    to_site(const O& obj)
    {
      enum { is_proxy = mlc_is_a(O, Site_Proxy)::value };
      return deep::to_site_< is_proxy >().doit(obj);
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_SITE_PROXY_HH
