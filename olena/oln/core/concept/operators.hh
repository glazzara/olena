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


namespace oln
{

  /// \{
  /// Operator == (default version).

  template <typename L, typename R>
  bool operator==(const Any<L>& lhs, const Any<R>& rhs)
  {
    return exact(lhs).op_equal_(exact(rhs));
  }

  /// \}


  /// \{
  /// Operator != (default version).
 
  template <typename L, typename R>
  bool operator!=(const Any<L>& lhs, const Any<R>& rhs)
  {
    return not (lhs == rhs);
  }

  /// \}


  /// \{
  /// Operator <  (default version).

  template <typename L, typename R>
  bool operator< (const Any<L>& lhs, const Any<R>& rhs)
  {
    return exact(lhs).op_less_(exact(rhs));
  }

  /// \}


  /// \{
  /// Operator >  (default version).

  template <typename L, typename R>
  bool operator> (const Any<L>& lhs, const Any<R>& rhs)
  {
    return rhs < lhs;
  }

  /// \}


  /// \{
  /// Operator >= (default version).

  template <typename L, typename R>
  bool operator>=(const Any<L>& lhs, const Any<R>& rhs)
  {
    return not (lhs < rhs);
  }

  /// \}


  /// \{
  /// Operator <= (default version).

  template <typename L, typename R>
  bool operator<=(const Any<L>& lhs, const Any<R>& rhs)
  {
    return not (rhs < lhs);
  }

  /// \}


  /// \{
  /// Operator += (default version).

  template <typename L, typename R>
  L& operator+=(Any<L>& lhs, const Any<R>& rhs)
  {
    return exact(lhs).op_plus_equal_(exact(rhs));
  }

  /// \}


  /// \{
  /// Operators + (default versions).

  template <typename T>
  T operator+ (const Any<T>& lhs, const Any<T>& rhs)
  {
    T tmp = exact(lhs);
    return tmp += rhs;
  }

//   template <typename L, typename R>
//   xtd_op_plus_trait(L, R) operator+ (const Any<L>& lhs, const Any<R>& rhs)
//   {
//     return exact(lhs).op_plus_(exact(rhs));
//   }

  /// \}


  /// \{
  /// Operator -= (default version).

  template <typename L, typename R>
  L& operator-=(Any<L>& lhs, const Any<R>& rhs)
  {
    return exact(lhs).op_minus_equal_(exact(rhs));
  }

  /// \}

  
  /// \{
  /// Operators - (default versions).

  template <typename T>
  T operator- (const Any<T>& rhs)
  {
    return exact(rhs).op_unary_minus_();
  }

  template <typename T>
  T operator- (const Any<T>& lhs, const Any<T>& rhs)
  {
    T tmp = exact(lhs);
    return tmp -= rhs;
  }

//   template <typename L, typename R>
//   xtd_op_minus_trait(L, R) operator- (const Any<L>& lhs, const Any<R>& rhs)
//   {
//     return exact(lhs).op_minus_(exact(rhs));
//   }

  /// \}

  

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_OPERATORS_HH
