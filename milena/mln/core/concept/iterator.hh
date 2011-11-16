// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_CONCEPT_ITERATOR_HH
# define MLN_CORE_CONCEPT_ITERATOR_HH

/*! \file
 * \brief Definition of the concept of mln::Iterator.
 */

# include <mln/core/concept/object.hh>

/*! \brief Loop to browse all the elements targetted by the iterator
 * \p x.
 */
# define for_all(x) for(x.start(); x.is_valid(); x.next())


/*! \brief Loop to browse all the elements targetted by the couples of
 * iterators \p x1 and \p x2.
 */
# define for_all_2(x1, x2)			\
  for(x1.start(), x2.start();			\
      x1.is_valid();				\
      x1.next(), x2.next())


/*! \brief Loop to browse all the elements targetted by the triplet of
 * iterators \p x1, \p x2, and \p x3.
 */
# define for_all_3(x1, x2, x3)			\
  for(x1.start(), x2.start(), x3.start();	\
      x1.is_valid();				\
      x1.next(), x2.next(), x3.next())


/*! \brief Loop to browse all the remaining elements targetted by the
 * iterator \p x.
 */
# define for_all_remaining(x) \
  if (! x.is_valid()) {} else while (x.next(), x.is_valid())



namespace mln
{
  /*! \brief Base class for implementation classes that are iterators.
   *
   * \see mln::doc::Iterator for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Iterator : public Object<E>
  {
    /*
      bool is_valid() const;
      void invalidate();
      void start();
      void next_();
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
    Iterator();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  void Iterator<E>::next() // final
  {
    assert(exact(this)->is_valid());
    exact(this)->next_();
  }

  template <typename E>
  inline
  Iterator<E>::Iterator()
  {
    bool (E::*m1)() const = & E::is_valid;
    (void) m1;
    void (E::*m2)() = & E::invalidate;
    (void) m2;
    void (E::*m3)() = & E::start;
    (void) m3;
    void (E::*m4)() = & E::next_;
    (void) m4;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_ITERATOR_HH
