// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_VALUE_CONCEPT_INTEGER_HH
# define MLN_VALUE_CONCEPT_INTEGER_HH

/// \file mln/value/concept/integer.hh
///
/// Define a generic class for integer values.
///
/// \todo Similar files needs to be cleaned-up.

# include <mln/value/concept/scalar.hh>


namespace mln
{

  namespace value
  {

    // Forward declaration.
    template <typename E> struct Integer;


    /// Category flag type.
    template <>
    struct Integer<void>
    {
      typedef Scalar<void> super;
    };


    /// Concept of integer.
    template <typename E>
    struct Integer : public Scalar<E>
    {
      typedef Integer<void> category;
    };

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_CONCEPT_INTEGER_HH
