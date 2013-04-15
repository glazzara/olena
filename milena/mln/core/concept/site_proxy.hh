// Copyright (C) 2008, 2009, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_SITE_PROXY_HH
# define MLN_CORE_CONCEPT_SITE_PROXY_HH

/*! \file
 *
 * \brief Definition of the concept of mln::Site_Proxy.
 */

# include <mln/core/concept/proxy.hh>
# include <mln/metal/is_a.hh>



namespace mln
{


  // Fwd decl.
  template <typename E> struct Site_Proxy;

  /// \cond INTERNAL_API
  /// \brief Site_Proxy category flag type.
  template <>
  struct Site_Proxy<void>
  {
    typedef Proxy<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for implementation classes of the notion of
    "site proxy".

    FIXME: Explain...

    \ingroup modconcepts
   */
  template <typename E>
  struct Site_Proxy : public Proxy<E>
  {
    typedef Site_Proxy<void> category;

    /*
      typedef site;

      const site& to_site() const;
      operator site() const;
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
    (void) m1;

    // De-activate because sometimes it returns a "const site&"...
//     site (E::*m2)() const = & E::operator site;
//     (void) m2;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_SITE_PROXY_HH
