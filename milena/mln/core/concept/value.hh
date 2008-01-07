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

#ifndef MLN_CORE_CONCEPT_VALUE_HH
# define MLN_CORE_CONCEPT_VALUE_HH

/*! \file mln/core/concept/value.hh
 * \brief Definition of the concept of mln::Value.
 */

# include <mln/core/concept/object.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Value;

  // Value category flag type.
  template <>
  struct Value<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes of values.
   *
   * \see mln::doc::Value for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Value : public Object<E>
  {
    typedef Value<void> category;

    /*
      typedef enc;   // encoding type
      typedef equiv; // equivalent type

      equiv to_equiv() const; // go to equivalent value
    */

  protected:
    Value();
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Value<E>::Value()
  {
    typedef mln_enc(E) enc;
    typedef mln_equiv(E) equiv;
    // FIXME HERE
//     equiv (E::*m)() const = & E::to_equiv;
//     m = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


//# include <mln/value/cast.hh>


#endif // ! MLN_CORE_CONCEPT_VALUE_HH
