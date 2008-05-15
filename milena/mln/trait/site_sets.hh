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

#ifndef MLN_TRAIT_SITE_SETS_HH
# define MLN_TRAIT_SITE_SETS_HH

/*! \file mln/trait/site_sets.hh
 *
 * \brief Some base trait types for site sets.
 */

# include <mln/trait/undef.hh>
# include <mln/trait/site_set/props.hh>


# define mln_trait_site_set_nsites(S)   typename mln::trait::site_set_< S >::nsites
# define mln_trait_site_set_bbox(S)     typename mln::trait::site_set_< S >::bbox
# define mln_trait_site_set_contents(S) typename mln::trait::site_set_< S >::contents
# define mln_trait_site_set_arity(S)    typename mln::trait::site_set_< S >::arity



namespace mln
{

  namespace trait
  {

    template <typename I>
    struct undefined_site_set_
    {
      typedef undef nsites;   // Known or unknown.
      typedef undef bbox;     // Unknown, lazy, or straight.
      typedef undef contents; // Fixed, growing, or free.
      typedef undef arity;    // Unique or multiple.
    };

    template <typename I>
    struct site_set_ : undefined_site_set_<I>
    {
    };

  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/site_set/print.hh>


#endif // ! MLN_TRAIT_SITE_SETS_HH
