// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_NEIGHBORHOOD_HH
# define MLN_TRAIT_NEIGHBORHOOD_HH

/// \file mln/trait/neighborhood.hh
/// \brief Some base trait types for neighborhood types.

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

    struct undefined_neighborhood
    {
      typedef undef kind;
    };


    struct default_neighborhood : undefined_neighborhood
    {
      typedef trait::neighborhood::kind::generic kind;
    };


    template <typename V>
    struct neighborhood : default_neighborhood
    {
    };


    /*----------------.
    | Traits values.  |
    `----------------*/

    // FIXME: Might be moved to another file, as it's the case for
    // images and values.

    /// Traits related to neighborhoods.
    namespace neighborhood
    {

      /// Kind of neighborhood.
      struct kind
      {
	/// The base class of the hierarchy of neighborhood traits.
	struct any
	{
	  std::string name() const { return "kind::any"; }
	};

	/// A generic neighborhood, with no particular feature.
	struct generic : any
	{
	  std::string name() const { return "kind::generic"; }
	};

	/// A neighborhood on a regular grid, i.e.
	/// holding/convertible to a window.
	struct regular : any
	{
	  std::string name() const { return "kind::regular"; }
	};
      };
    }


  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_TRAIT_NEIGHBORHOOD_HH
