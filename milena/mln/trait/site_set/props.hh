// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

/*! \file
 *
 * \brief Properties of site set classes.
 *
 * \todo Precise the differences (?) between dynamic, growing, and
 * free...
 */

# include <string>
# include <mln/trait/undef.hh>



//  Properties of site sets.
//  ========================

//  nsites:   /any/
//              |
//              + -- unknown
//              |
//              + -- known

//  bbox:     /any/
//              |
//              + -- unknown
//              |
//              + -- /known/
//                     |
//                     + -- lazy
//                     |
//                     + -- straight

//  contents: /any/
//              |
//              + -- fixed
//              |
//              + -- /dynamic/
//                     |
//                     + -- growing
//                     |
//                     + -- free

//  arity:    /any/
//              |
//              + -- unique
//              |
//              + -- multiple


namespace mln
{

  namespace trait
  {

    namespace site_set
    {


      /// Site set property about the 'nsites' method presence.
      struct nsites
      {
	/// Base class for the site set 'nsites' property.
	struct any { protected: any() {} };

	/// Property that states that the number of sites cannot be
	/// retrieved from a site set in O(1) complexity so the site
	/// set does not feature the 'nsites' methods.
	struct unknown : any { std::string name() const { return "nsites::unknown"; } };

	/// Property that states that a site set features the method
	/// 'nsites' because the number of sites is known so its
	/// retrieval has O(1) complexity.
	struct known   : any { std::string name() const { return "nsites::known"; } };
      };


      /// Site set property about the 'bbox' method presence.
      struct bbox
      {
	/// Base class for the site set 'bbox' property.
	struct any { protected: any() {} };

	/// Property that states that the bounding box of a site set
	/// is not featured as a method.  This is either because the
	/// notion of bounding box is meaningless for the site set
	/// type, or because the bounding box cannot be retrieved in
	/// O(1) complexity.
	struct unknown : any { std::string name() const { return "bbox::unknown"; } };

	/// Property that states that the bounding box of a site set
	/// is featured by the 'bbox' method.  It means that the
	/// notion of bounding box makes sense and that such a piece
	/// of information can be retrieved in O(1) complexity.
	/// Warning: this property is pseudo-abstract.  The more
	/// precise properties are 'lazy' and 'straight'.
	struct known   : any { protected: known() {} };

	/// Property that states that the bounding box of a site set
	/// is computed by the site set in a lazy way.
	struct lazy     : known { std::string name() const { return "bbox::lazy"; } };

	/// Property that states that the bounding box of a site set
	/// is always kept up to date by the site set.
	struct straight : known { std::string name() const { return "bbox::straight"; } };
      };


      /// Site set property about how the contents can evolve.
      struct contents
      {
	/// Base class for the site set 'contents' property.
	struct any { protected: any() {} };
	struct fixed   : any     { std::string name() const { return "contents::fixed"; } };
	struct dynamic : any { protected: dynamic() {} };
	struct growing : dynamic { std::string name() const { return "contents::growing"; } };
	struct free    : dynamic { std::string name() const { return "contents::free"; } };
      };

      /// Site set property about the unicity or multiplicity of its
      /// elements.
      struct arity
      {
	/// Base class for the site set 'arity' property.
	struct any { protected: any() {} };
	struct unique   : any { std::string name() const { return "arity::unique"; } };
	struct multiple : any { std::string name() const { return "arity::multiple"; } };
      };


      // FIXME: support = psite::support


    } // end of namespace mln::trait::site_set

  } // end of namespace mln::trait


} // end of namespace mln


#endif // ! MLN_TRAIT_SITE_SET_PROPS_HH
