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

#ifndef OLN_CORE_CONCEPT_OPERATORS_HH
# define OLN_CORE_CONCEPT_OPERATORS_HH

# include <oln/core/equipment.hh>
# include <oln/core/gen/traits.hh>
# include <oln/core/gen/value.hh>


namespace oln
{


  // Default versions for some operators.

  template <typename L, typename R>
  bool
  operator != (const Any<L>& lhs, const Any<R>& rhs); // Using "==".

  template <typename L, typename R>
  bool
  operator > (const Any<L>& lhs, const Any<R>& rhs); // Using "<".

  template <typename L, typename R>
  bool
  operator <= (const Any<L>& lhs, const Any<R>& rhs); // Using "<".

  template <typename L, typename R>
  bool
  operator >= (const Any<L>& lhs, const Any<R>& rhs); // Using "<=".

  template <typename T>
  T
  operator + (const Any<T>& lhs, const Any<T>& rhs); // Using "+=".

  template <typename T>
  T
  operator - (const Any<T>& lhs, const Any<T>& rhs); // Using "+=".

  template <typename T>
  T
  operator % (const Any<T>& lhs, const Any<T>& rhs); // Using "%=".




  namespace ERROR
  {

    template <typename Op>
    struct operator_
    {
      // unary
      template <typename T> struct _is_missing_for_;
      template <typename T> struct _is_invalid_for_;
      // binary
      template <typename L, typename R> struct _is_missing_for_types_;
      template <typename L, typename R> struct _is_invalid_for_types_;
    };

  } // end of namespace oln::ERROR



  //  Guards.
  // ----------------

  // Operator ==.
  template <typename L, typename R>
  void operator == (const Any<L>& lhs, const Any<R>& rhs);

  // Operator <.
  template <typename L, typename R>
  void operator < (const Any<L>& lhs, const Any<R>& rhs);

  // Operator -.
  template <typename T>
  void operator - (const Any<T>& rhs);

  // Operator +=.
  template <typename L, typename R>
  void operator += (const Any<L>& lhs, const Any<R>& rhs);

  // Operator -=.
  template <typename L, typename R>
  void operator -= (const Any<L>& lhs, const Any<R>& rhs);

  // Operator %=.
  template <typename L, typename R>
  void operator %= (const Any<L>& lhs, const Any<R>& rhs);

  // ----------------
  //  end of Guards.




# ifndef OLN_INCLUDE_ONLY
 
  template <typename L, typename R>
  bool operator != (const Any<L>& lhs, const Any<R>& rhs)
  {
    return not (exact(lhs) == exact(rhs));
  }

  template <typename L, typename R>
  bool operator > (const Any<L>& lhs, const Any<R>& rhs)
  {
    return exact(rhs) < exact(lhs); // use "operator <"
  }

  template <typename L, typename R>
  bool operator <= (const Any<L>& lhs, const Any<R>& rhs)
  {
    return not (exact(rhs) < exact(lhs)); // use "operator <" and "not"
  }

  template <typename L, typename R>
  bool operator >= (const Any<L>& lhs, const Any<R>& rhs)
  {
    return exact(rhs) <= exact(lhs); // use "operator <="
  }

  template <typename T>
  T operator + (const Any<T>& lhs, const Any<T>& rhs)
  {
    T tmp = exact(lhs);
    return tmp += exact(rhs);
  }

  template <typename T>
  T operator - (const Any<T>& lhs, const Any<T>& rhs)
  {
    T tmp = exact(lhs);
    return tmp -= exact(rhs);
  }

  template <typename T>
  T operator % (const Any<T>& lhs, const Any<T>& rhs)
  {
    T tmp = exact(lhs);
    return tmp %= exact(rhs);
  }



  //  Guards.
  // ----------------

  // Operator ==.
  template <typename L, typename R>
  void operator == (const Any<L>& lhs, const Any<R>& rhs)
  {
    mlc::abort_<L,
      typename ERROR::operator_< eq_id >::_is_missing_for_types_<L, R>
      >::check();
  }

  // Operator <.
  template <typename L, typename R>
  void operator < (const Any<L>& lhs, const Any<R>& rhs)
  {
    mlc::abort_<L,
      typename ERROR::operator_< less_id >::template _is_missing_for_types_<L, R>
      >::check();
  }

  // Operator +=.
  template <typename L, typename R>
  void operator += (const Any<L>& lhs, const Any<R>& rhs)
  {
    mlc::abort_<L,
      typename ERROR::operator_< plus_equal_id >::template _is_missing_for_types_<L, R>
      >::check();
  }

  // Operator -=.
  template <typename L, typename R>
  void operator -= (const Any<L>& lhs, const Any<R>& rhs)
  {
    mlc::abort_<L,
      typename ERROR::operator_< minus_equal_id >::template _is_missing_for_types_<L, R>
      >::check();
  }

  // Operator %=.
  template <typename L, typename R>
  void operator %= (const Any<L>& lhs, const Any<R>& rhs)
  {
    mlc::abort_<L,
      typename ERROR::operator_< modulus_equal_id >::template _is_missing_for_types_<L, R>
      >::check();
  }

  // Operator -.
  template <typename T>
  void operator - (const Any<T>& rhs)
  {
    mlc::abort_<T,
      typename ERROR::operator_< uminus_id >::template _is_missing_for_<T>
      >::check();
  }

  // ----------------
  //  end of Guards.


# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_OPERATORS_HH
