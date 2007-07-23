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

#ifndef MLN_CORE_CONCEPT_PIXEL_ITERATOR_HH
# define MLN_CORE_CONCEPT_PIXEL_ITERATOR_HH

/*! \file mln/core/concept/pixel_iterator.hh
 *
 * \brief Definition of the concept of mln::Pixel_Iterator.
 */

# include <mln/core/concept/iterator.hh>
# include <mln/core/concept/generalized_pixel.hh>


namespace mln
{

  /*! \brief Base class for the implementation of pixel iterator
   *  classes.
   *
   * An iterator on pixels is an iterator that is bound to a
   * particular image and that browses over a set of image pixels.
   *
   * \see mln::doc::Pixel_Iterator for a complete documentation of
   * this class contents.
   */
  template <typename E>
  struct Pixel_Iterator : public Iterator<E>,
			  public Generalized_Pixel<E>
  {
    /*
      typedef lvalue;

      lvalue operator*();
    */
  protected:
    Pixel_Iterator();
  };


#ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Pixel_Iterator<E>::Pixel_Iterator()
  {
    typedef mln_lvalue(E) lvalue;
    lvalue (E::*m)() = & E::operator*;
    m = 0;
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PIXEL_ITERATOR_HH
