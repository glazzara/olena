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

#ifndef MLN_TRAIT_SITE_SET_PROPS_HH
# define MLN_TRAIT_SITE_SET_PROPS_HH

/*! \file mln/trait/site_set/props.hh
 *
 * \brief Properties of site set classes.
 */

# include <string>
# include <mln/trait/undef.hh>


namespace mln
{

  namespace trait
  {

    namespace site_set
    {

      struct nsites
      {
	struct any {};
	struct unknown : any { std::string name() const { return "nsites::unknown"; } };
	struct known   : any { std::string name() const { return "nsites::known"; } };
      };

      struct bbox
      {
	struct any {};
	struct unknown : any { std::string name() const { return "bbox::unknown"; } };
	struct known   : any {};
	struct lazy     : known { std::string name() const { return "bbox::lazy"; } };
	struct straight : known { std::string name() const { return "bbox::straight"; } };
      };

      struct contents
      {
	struct any {};
	struct fixed   : any { std::string name() const { return "contents::fixed"; } };
	struct dynamic : any {};
	struct growing : dynamic { std::string name() const { return "contents::growing"; } };
	struct free    : dynamic { std::string name() const { return "contents::dynamic"; } };
      };

      struct arity
      {
	struct any {};
	struct unique   : any { std::string name() const { return "arity::unique"; } };
	struct multiple : any { std::string name() const { return "arity::multiple"; } };
      };


      // FIXME: support = psite::support


    } // end of namespace mln::trait::site_set

  } // end of namespace mln::trait


} // end of namespace mln


#endif // ! MLN_TRAIT_SITE_SET_PROPS_HH
