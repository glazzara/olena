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

#ifndef MLN_CORE_CONCEPT_GENPIXEL_HH
# define MLN_CORE_CONCEPT_GENPIXEL_HH

/*! \file mln/core/concept/genpixel.hh
 *
 * \brief Definition of the concept of mln::Genpixel.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/internal/force_exact.hh>


namespace mln
{

  // FIXME: \class GenPixel GenPixel.hh "mln/core/concept/doc/GenPixel.hh"

  /*! \brief Base class for implementation classes that are pixels or that
   *  have the behavior of pixels.
   *
   * "GenPixel" is "Generalized Pixel" for short.
   *
   * \warning This class does \em not derive from mln::Object; it is
   * for use as a parallel hierarchy.
   *
   * \see mln::doc::GenPixel for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct GenPixel
  {
    /*
      typedef  value;
      typedef rvalue;

      rvalue operator*() const;
      value** address_() const;
    */
  protected:
    GenPixel();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  GenPixel<E>::GenPixel()
  {
    typedef mln_value(E) value;
    typedef mln_rvalue(E) rvalue;
    rvalue (E::*m1)() const = & E::operator*;
    m1 = 0;
    value** (E::*m2)() const = & E::address_;
    m2 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_GENPIXEL_HH
