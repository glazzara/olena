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
# include <mln/value/props.hh>
# include <mln/literal/zero.hh>
# include <mln/literal/one.hh>
# include <mln/literal/ops.hh>
# include <mln/metal/ret.hh>


# define mln_sum_x(T, U) typename mln::value::props< mln_trait_op_times(T,U) >::sum



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


    // Binary traits for any Scalar type...

    template < template <class, class> class Name,
	       typename Vl, typename Vr >
    struct set_binary_< Name, mln::value::Scalar, Vl, mln::value::Scalar, Vr >
    {
      typedef mln_trait_binary(Name, mln_value_equiv(Vl), mln_value_equiv(Vr)) ret;
    };

    // ...and for the special case of a couple of value::scalar_.

    template < template <class, class> class Name,
	       typename Sl, typename Sr >
    struct set_binary_< Name,
			mln::value::Scalar, mln::value::scalar_<Sl>,
			mln::value::Scalar, mln::value::scalar_<Sr> >
    {
      typedef mln_trait_binary(Name, mln_value_equiv(Sl), mln_value_equiv(Sr)) ret;
    };


    // Some binary traits for "scalar(s) OP obj" when OP commutes => "obj OP scalar(s)".

    template < typename S, typename O >
    struct set_binary_< op::plus,
			mln::value::Scalar, mln::value::scalar_<S>,
			mln::Object,        O >
    {
      typedef mln_trait_op_plus(O, mln::value::scalar_<S>) ret;
    };

    template < typename S, typename O >
    struct set_binary_< op::times,
			mln::value::Scalar, mln::value::scalar_<S>,
			mln::Object,        O >
    {
      typedef mln_trait_op_times(O, mln::value::scalar_<S>) ret;
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



  template <typename S, typename O>
  mln_trait_op_plus(O, value::scalar_<S>)
    operator + (const value::scalar_<S>& lhs, const Object<O>& rhs);

  template <typename S, typename O>
  mln_trait_op_times(O, value::scalar_<S>)
    operator * (const value::scalar_<S>& lhs, const Object<O>& rhs);


  // Arithmetical unary operators.

  template <typename S>
  mln_trait_op_uminus(S)
    operator - (const value::scalar_<S>& rhs); // Overload of op-(Object) in core/ops.
  // FIXME: It is dedicated to value::scalar_ so move elsewhere?



  // Logical operators.

  // FIXME: ...



  // Case of value::scalar_ OP value::scalar_.

  template <typename Sl, typename Sr>
  mln_trait_op_plus(Sl, Sr)
    operator + (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename Sl, typename Sr>
  mln_trait_op_minus(Sl, Sr)
    operator - (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename Sl, typename Sr>
  mln_trait_op_times(Sl, Sr)
    operator * (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename Sl, typename Sr>
  mln_trait_op_div(Sl, Sr)
    operator / (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename Sl, typename Sr>
  mln_trait_op_mod(Sl, Sr)
    operator % (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);



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

  template <typename S, typename O>
  mln_trait_op_plus(O, value::scalar_<S>)
  operator + (const value::scalar_<S>& lhs, const Object<O>& rhs)
  {
    return exact(rhs) + lhs;
  }

  template <typename S, typename O>
  mln_trait_op_times(O, value::scalar_<S>)
  operator * (const value::scalar_<S>& lhs, const Object<O>& rhs)
  {
    return exact(rhs) * lhs;
  }

  // ...


  // With scalar_ OP scalar_.

  template <typename Sl, typename Sr>
  mln_trait_op_plus(Sl, Sr)
  operator + (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) + value::equiv(rhs);
  }

  template <typename Sl, typename Sr>
  mln_trait_op_minus(Sl, Sr)
  operator - (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) - value::equiv(rhs);
  }

  template <typename Sl, typename Sr>
  mln_trait_op_times(Sl, Sr)
  operator * (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) * value::equiv(rhs);
  }

  template <typename Sl, typename Sr>
  mln_trait_op_div(Sl, Sr)
  operator / (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) / value::equiv(rhs);
  }

  template <typename Sl, typename Sr>
  mln_trait_op_mod(Sl, Sr)
  operator % (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) % value::equiv(rhs);
  }

  // ...

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_VALUE_OPS_HH
