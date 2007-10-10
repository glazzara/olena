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

#ifndef MLN_VALUE_CONCEPT_SCALAR_HH
# define MLN_VALUE_CONCEPT_SCALAR_HH

/*! \file mln/value/concept/scalar.hh
 *
 * \brief Define a generic class for scalar values.
 */

# include <mln/core/concept/value.hh>


namespace mln
{

  // Fwd decls.
  template <typename E> struct Value;
  namespace value
  {
    template <typename E> struct Scalar;
  }

  namespace trait
  {
    // FIXME
  } // end of namespace mln::trait

  namespace value
  {

    // Fwd decl.
    template <typename E> struct Scalar;

    // Category flag type.
    template <>
    struct Scalar<void>
    {
      typedef Value<void> super;
    };

    template <typename E>
    struct Scalar : public Value<E>
    {
    };

  } // end of namespace mln::value



  /// Pre-incrementation.
  template <typename S>
  S& operator++(value::Scalar<S>& rhs);


  /// Pre-decrementation.
  template <typename S>
  S& operator--(value::Scalar<S>& rhs);



  template <typename S>
  S& operator*=(value::Scalar<S>& lhs, typename S::interop i);

  template <typename S>
  S& operator/=(value::Scalar<S>& lhs, typename S::interop i);

  template <typename S>
  S& operator+=(value::Scalar<S>& lhs, typename S::interop i);

  template <typename S>
  S& operator-=(value::Scalar<S>& lhs, typename S::interop i);

  template <typename S>
  S& operator%=(value::Scalar<S>& lhs, typename S::interop i);



# ifndef MLN_INCLUDE_ONLY

  template <typename S>
  S& operator++(value::Scalar<S>& rhs)
  {
    exact(rhs) += 1; // FIXME: literal::one?
    return exact(rhs);
  }

  template <typename S>
  S& operator--(value::Scalar<S>& rhs)
  {
    exact(rhs) -= 1; // FIXME: literal::one?
    return exact(rhs);
  }

  template <typename S>
  S& operator*=(value::Scalar<S>& lhs_, typename S::interop i)
  {
    S& lhs = exact(lhs_);
    lhs = lhs * i;
    return lhs;
  }

  template <typename S>
  S& operator/=(value::Scalar<S>& lhs_, typename S::interop i)
  {
    S& lhs = exact(lhs_);
    lhs = lhs / i;
    return lhs;
  }

  template <typename S>
  S& operator+=(value::Scalar<S>& lhs_, typename S::interop i)
  {
    S& lhs = exact(lhs_);
    lhs = lhs + i;
    return lhs;
  }

  template <typename S>
  S& operator-=(value::Scalar<S>& lhs_, typename S::interop i)
  {
    S& lhs = exact(lhs_);
    lhs = lhs - i;
    return lhs;
  }


  template <typename S>
  S& operator%=(value::Scalar<S>& lhs_, typename S::interop i)
  {
    S& lhs = exact(lhs_);
    lhs = lhs % i;
    return lhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_VALUE_CONCEPT_SCALAR_HH
