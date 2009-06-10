// Copyright (C) 2007, 2008, 2009, 20072008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_CONCEPT_SYMBOLIC_HH
# define MLN_VALUE_CONCEPT_SYMBOLIC_HH

/// \file
///
/// Define a generic class for symbolic values.

# include <mln/core/concept/value.hh>


namespace mln
{

  // Forward declaration.
  namespace value { template <typename E> struct Symbolic; }


  namespace trait
  {

    // FIXME...

  } // end of namespace mln::trait


  namespace value
  {

    // Category flag type.
    template <>
    struct Symbolic<void>
    {
      typedef Value<void> super;
    };

    template <typename E>
    struct Symbolic : public Value<E>
    {
      typedef Symbolic<void> category;
    };

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_CONCEPT_SYMBOLIC_HH
