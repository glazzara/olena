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

#ifndef MLN_VALUE_OPS_HH
# define MLN_VALUE_OPS_HH

/*! \file mln/value/ops.hh
 *
 * \brief Definitions of operators for value types.
 */

# include <mln/trait/op/all.hh>
# include <mln/value/builtin/all.hh>
# include <mln/value/concept/all.hh>
# include <mln/value/equiv.hh>
# include <mln/literal/zero.hh>
# include <mln/literal/one.hh>
# include <mln/metal/ret.hh>



// FIXME: In the definitions below, is that equiv or interop?



namespace mln
{

  namespace trait
  {

    // Unary traits for any Scalar type.

    template < template <class> class Name,
	       typename V >
    struct set_unary_< Name, mln::value::Scalar, V >
    {
      typedef mln_trait_unary(Name, mln_value_equiv(V)) ret;
    };

    // Binary traits for any Scalar type.

    template < template <class, class> class Name,
	       typename Vl, typename Vr >
    struct set_binary_< Name, mln::value::Scalar, Vl, mln::value::Scalar, Vr >
    {
      typedef mln_trait_binary(Name, mln_value_equiv(Vl), mln_value_equiv(Vr)) ret;
    };

  } // end of namespace mln::trait


  // Arithmetical binary operators.

  template <typename Vl, typename Vr>
  mln_trait_op_plus(Vl, Vr)
    operator + (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);

  template <typename Vl, typename Vr>
  mln_trait_op_minus(Vl, Vr)
    operator - (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);

  template <typename Vl, typename Vr>
  mln_trait_op_times(Vl, Vr)
    operator * (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);

  template <typename Vl, typename Vr>
  mln_trait_op_div(Vl, Vr)
    operator / (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);

  template <typename Vl, typename Vr>
  mln_trait_op_mod(Vl, Vr)
    operator % (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);


  // Arithmetical unary operators.

  template <typename S>
  mln_trait_op_uminus(S)
    operator - (const value::scalar_<S>& rhs); // Overload of op-(Object) in core/ops.
  // FIXME: It is dedicated to value::scalar_ so move elsewhere?



  // Logical operators.

  // FIXME: ...




# ifndef MLN_INCLUDE_ONLY

  template <typename Vl, typename Vr>
  mln_trait_op_plus(Vl, Vr)
    operator + (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    return value::equiv(lhs) + value::equiv(rhs);
  }

  template <typename Vl, typename Vr>
  mln_trait_op_minus(Vl, Vr)
    operator - (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    return value::equiv(lhs) - value::equiv(rhs);
  }

  template <typename Vl, typename Vr>
  mln_trait_op_times(Vl, Vr)
    operator * (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    return value::equiv(lhs) * value::equiv(rhs);
  }

  template <typename Vl, typename Vr>
  mln_trait_op_div(Vl, Vr)
    operator / (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    return value::equiv(lhs) / value::equiv(rhs);
  }

  template <typename Vl, typename Vr>
  mln_trait_op_mod(Vl, Vr)
    operator % (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    return value::equiv(lhs) % value::equiv(rhs);
  }

  template <typename S>
  mln_trait_op_uminus(S)
    operator - (const value::scalar_<S>& rhs)
  {
    return - rhs.to_equiv();
  }

  // ...

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_VALUE_OPS_HH
