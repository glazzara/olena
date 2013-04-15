// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_TRAIT_SITE_SETS_HH
# define MLN_TRAIT_SITE_SETS_HH

/*! \file
 *
 * \brief Some base trait types for site sets.
 */

# include <mln/trait/undef.hh>
# include <mln/trait/site_set/props.hh>


/// Shortcut to the site set property about the 'nsites' method
/// presence.
# define mln_trait_site_set_nsites(S)   typename mln::trait::site_set_< S >::nsites

/// Shortcut to the site set property about the 'bbox' method
/// presence.
# define mln_trait_site_set_bbox(S)     typename mln::trait::site_set_< S >::bbox

/// Shortcut to the site set property about how the contents can
/// evolve.
# define mln_trait_site_set_contents(S) typename mln::trait::site_set_< S >::contents

/// Shortcut to the site set property about the unicity or
/// multiplicity of its elements.
# define mln_trait_site_set_arity(S)    typename mln::trait::site_set_< S >::arity



namespace mln
{

  namespace trait
  {

    /*!
      \internal
      \brief Pack of 'undefined' type values for properties of site sets.
    */
    template <typename I>
    struct undefined_site_set_
    {
      typedef undef nsites;   // Known or unknown.
      typedef undef bbox;     // Unknown, lazy, or straight.
      typedef undef contents; // Fixed, growing, or free.
      typedef undef arity;    // Unique or multiple.
    };


    /*!
      \internal
      \brief The trait pack structure for properties of site sets.

      This structure is specialized for every concrete class of site
      set so that properties are properly defined.

      \see mln::doc::Site_Set for the documentation of the "site set"
      concept.
    */
    template <typename I>
    struct site_set_ : undefined_site_set_<I>
    {
    };

  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/site_set/print.hh>


#endif // ! MLN_TRAIT_SITE_SETS_HH
