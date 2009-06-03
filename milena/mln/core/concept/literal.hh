// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_CONCEPT_LITERAL_HH
# define MLN_CORE_CONCEPT_LITERAL_HH

/*! \file mln/core/concept/literal.hh
 * \brief Definition of the concept of mln::literal.
 */

# include <mln/core/concept/object.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Literal;

  // Literal category flag type.
  template <>
  struct Literal<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes of literals.
   *
   * \see mln::doc::Literal for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Literal : public Object<E>
  {
    typedef Literal<void> category;

    static const E& the();

  protected:
    Literal();
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Literal<E>::Literal()
  {
  }

  template <typename E>
  inline
  const E&
  Literal<E>::the()
  {
    static E the_;
    return the_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_LITERAL_HH
