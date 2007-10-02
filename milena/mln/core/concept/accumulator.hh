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

#ifndef MLN_CORE_CONCEPT_ACCUMULATOR_HH
# define MLN_CORE_CONCEPT_ACCUMULATOR_HH

/*! \file mln/core/concept/accumulator.hh
 *
 * \brief Definition of the concept of mln::Accumulator.
 *
 * \todo Rename value as argument + Add a conversion op.
 */

# include <mln/core/concept/object.hh>


namespace mln
{


  /*! \brief Base class for implementation of accumulators.
   *
   * The parameter \a E is the exact type.
   *
   * \see mln::doc::Accumulator for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Accumulator : public Object<E>
  {
    typedef Accumulator<void> category;

    /*
      typedef value;
      typedef result;
      void init();
      void take(const value& v);
      void take(const E& other);
      result to_result() const;
     */

    // Default impl.
    template <typename T>
    void take_as_init(const T& t); // 't' is either value or E.

  protected:
    Accumulator();
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Accumulator<E>::Accumulator()
  {
    typedef mln_value(E)  value;
    typedef mln_result(E) result;

    void (E::*m1)() = & E::init;
    m1 = 0;
    void (E::*m2)(const value&) = & E::take;
    m2 = 0;
    void (E::*m3)(const E&) = & E::take;
    m3 = 0;
    result (E::*m4)() const = & E::to_result;
    m4 = 0;
  }

  template <typename E>
  template <typename T>
  void
  Accumulator<E>::take_as_init(const T& t) // either value or E
  {
    exact(this)->init();
    exact(this)->take(t);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_ACCUMULATOR_HH
