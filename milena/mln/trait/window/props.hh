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

#ifndef MLN_TRAIT_WINDOW_PROPS_HH
# define MLN_TRAIT_WINDOW_PROPS_HH

/*! \file
 *
 * \brief Properties of window classes.
 *
 * \todo Precise the differences (?) between dynamic, growing, and
 * free...
 */

# include <string>
# include <mln/trait/undef.hh>



//  Properties of windows.
//  ========================

//  size:       /any/
//                |
//                + -- fixed
//                |
//                + -- unknown

//  support:    /any/
//                |
//                + -- regular
//                |
//                + -- irregular

//  definition: /any/
//                |
//                + -- unique
//                |
//                + -- /multiple/
//                       |
//                       + -- n_ary
//                       |
//                       + -- varying


namespace mln
{

  namespace trait
  {

    namespace window
    {


      /*!
	\internal
	\brief Window property about the 'size' method presence.
      */
      struct size
      {
	/*!
	  \internal
	  \brief Base class for the window 'size' property.
	*/
	struct any { protected: any() {} };

	/*!
	  \internal
	  \brief Property that states that the size is fixed.
	*/
	struct fixed   : any { std::string name() const { return "size::fixed"; } };

	/*!
	  \internal
	  \brief Property that states that the size is not fixed so unknown.
	*/
	struct unknown : any { std::string name() const { return "size::unknown"; } };
      };


      /*!
	\internal
	\brief Window property about the 'support' it is designed for.
      */
      struct support
      {
	/*!
	  \internal
	  \brief Base class for the window 'support' property.
	*/
	struct any { protected: any() {} };

	/*!
	  \internal
	  \brief Property that states that the window is designed for a regular support.
	*/
	struct regular   : any { std::string name() const { return "support::regular"; } };

	/*!
	  \internal
	  \brief Property that states that the window is not designed for a regular support.
	*/
	struct irregular : any { std::string name() const { return "support::irregular"; } };
      };


      /*!
	\internal
	\brief Window property about how the window is defined.
      */
      struct definition
      {
	/*!
	  \internal
	  \brief Base class for the window 'definition' property.
	*/
	struct any { protected: any() {} };

	/*!
	  \internal
	  \brief Property that states that the definition is unique.
	*/
	struct unique : any { std::string name() const { return "definition::unique"; } };

	/*!
	  \internal
	  \brief Abstract property that states that the definition is multiple.
	*/
	struct multiple : any { protected: multiple() {} };

	/*!
	  \internal
	  \brief Property that states that this window has n definitions.
	*/
	struct n_ary : multiple { std::string name() const { return "definition::n_ary"; } };

	/*!
	  \internal
	  \brief Property that states that this window has a varying definition.
	*/
	struct varying : multiple { std::string name() const { return "definition::varying"; } };
      };


    } // end of namespace mln::trait::window

  } // end of namespace mln::trait


} // end of namespace mln


#endif // ! MLN_TRAIT_WINDOW_PROPS_HH
