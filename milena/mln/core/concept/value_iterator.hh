// Copyright (C) 2007, 2008, 2009, 2011, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_VALUE_ITERATOR_HH
# define MLN_CORE_CONCEPT_VALUE_ITERATOR_HH

/*! \file
 *
 * \brief Definition of the concept of mln::Value_Iterator.
 */

# include <mln/core/concept/iterator.hh>


namespace mln
{

  /*!
    \brief Base class for implementation of classes of iterator on
    values.

    An iterator on values is an iterator that browse over a set of
    values.

    \ingroup modconcepts
   */
  template <typename E>
  struct Value_Iterator : public Iterator<E>
  {
    /*
      typedef value;
      operator value() const;
     */

  protected:
    Value_Iterator();
  };


  /*! \brief Print an iterator \p v on value set into the output
   *  stream \p ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] v An iterator on value set.
   *
   * \pre \p v is a valid.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Value_Iterator
   */
  template <typename E>
  std::ostream& operator<<(std::ostream& ostr, const Value_Iterator<E>& v);



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Value_Iterator<E>::Value_Iterator()
  {
    typedef mln_value(E) value;
    value (E::*m)() const = & E::operator value;
    (void) m;
  }

  template <typename E>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Value_Iterator<E>& v)
  {
    mln_precondition(exact(v).is_valid());
    return ostr << mln_value(E)(exact(v));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_VALUE_ITERATOR_HH
