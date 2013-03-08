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

#ifndef MLN_TRAIT_NEIGHBORHOOD_HH
# define MLN_TRAIT_NEIGHBORHOOD_HH

/// \file
///
/// Some base trait types for neighborhood types.

# include <string>


/// Shortcuts.
/// \{
# define mln_trait_neighborhood_kind(N)			\
   typename mln::trait::neighborhood< N >::nature
# define mln_trait_neighborhood_kind_(N)		\
   mln::trait::neighborhood< N >::nature
/// \}


namespace mln
{

  namespace trait
  {

    // Forward declaration.
    struct undef;


    /*----------------.
    | Traits values.  |
    `----------------*/

    // FIXME: Might be moved to another file, as it's the case for
    // images and values.

    /*!
      \internal
      \brief Traits related to neighborhoods.
    */
    namespace neighborhood
    {

      /*!
	\internal
	\brief Kind of neighborhood.
      */
      struct kind
      {
	/*!
	  \internal
	  \brief The base class of the hierarchy of neighborhood traits.
	*/
	struct any
	{
	  std::string name() const { return "kind::any"; }
	};

	/*!
	  \internal
	  \brief A generic neighborhood, with no particular feature.
	*/
	struct generic : any
	{
	  std::string name() const { return "kind::generic"; }
	};

	/*!
	  \internal
	  \brief A neighborhood on a regular grid, i.e.
	  holding/convertible to a window.
	*/
	struct regular : any
	{
	  std::string name() const { return "kind::regular"; }
	};
      };
    }



    struct undefined_neighborhood_
    {
      typedef undef kind;
    };


    struct default_neighborhood_ : undefined_neighborhood_
    {
      typedef trait::neighborhood::kind::generic kind;
    };


    template <typename V>
    struct neighborhood_ : default_neighborhood_
    {
    };

  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_TRAIT_NEIGHBORHOOD_HH
