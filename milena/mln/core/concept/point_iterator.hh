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

#ifndef MLN_CORE_CONCEPT_PITER_HH
# define MLN_CORE_CONCEPT_PITER_HH

/*! \file mln/core/concept/point_iterator.hh
 *
 * \brief Definition of the concept of mln::Point_Iterator.
 *
 * \todo Fix mln/core/concept/doc/point_iterator.hh
 */

# include <mln/core/concept/point_site.hh>
# include <mln/core/concept/iterator.hh> // To fetch the macros.


namespace mln
{

  /*! \brief Base class for implementation of classes of iterator on
   *  points.
   *
   * An iterator on points is an iterator that browse over a set of
   * points.
   *
   * \see mln::doc::Point_Iterator for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Point_Iterator : public Point_Site<E>
  {
    /*
      typedef psite;
      bool is_valid() const;
      void invalidate();
      void start();
      void next_();
      operator psite() const;
    */

    /*! \brief Go to the next element.
     *
     * \warning This is a final method; iterator classes should not
     * re-defined this method.  The actual "next" operation has to be
     * defined through the \em next_ method.
     *
     * \pre The iterator is valid.
     */ 
    void next(); // final

  protected:
    Point_Iterator();
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  void
  Point_Iterator<E>::next() // final
  {
    assert(exact(this)->is_valid());
    exact(this)->next_();
  }

  template <typename E>
  Point_Iterator<E>::Point_Iterator()
  {
    typedef mln_psite(E) psite;
    mln::metal::is_a<psite, Point_Site>::check();

    psite (E::*m)() const = & E::operator psite;
    m = 0;

    bool (E::*m1)() const = & E::is_valid;
    m1 = 0;
    void (E::*m2)() = & E::invalidate;
    m2 = 0;
    void (E::*m3)() = & E::start;
    m3 = 0;
    void (E::*m4)() = & E::next_;
    m4 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PITER_HH
