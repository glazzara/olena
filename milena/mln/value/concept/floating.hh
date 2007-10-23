// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_CONCEPT_FLOATING_HH
# define MLN_VALUE_CONCEPT_FLOATING_HH

/*! \file mln/value/concept/floating.hh
 *
 * \brief Define a generic class for floating values.
 */

# include <mln/value/concept/scalar.hh>


namespace mln
{

  // Fwd decl.
  namespace value { template <typename E> struct Floating; }


  namespace trait
  {
    // FIXME...

  } // end of namespace mln::trait


  namespace value
  {

    // Category flag type.
    template <>
    struct Floating<void>
    {
      typedef Scalar<void> super;
    };

    template <typename E>
    struct Floating : public Scalar<E>
    {
      typedef Floating<void> category;
    };

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_CONCEPT_FLOATING_HH
