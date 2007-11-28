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

#ifndef MLN_LITERAL_OPS_HH
# define MLN_LITERAL_OPS_HH

/*! \file mln/literal/ops.hh
 *
 * \brief Definitions of some operators where at least one literal is
 * involved.
 */

# include <mln/core/concept/literal.hh>
# include <mln/trait/all.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/converts_to.hh>



namespace mln
{


  namespace trait
  {

    template < template <class, class> class Name,
	       typename L, typename O >
    struct set_binary_< Name, mln::Literal, L, mln::Object, O >
    {
      typedef mln_trait_binary(Name, O, O) ret;
    };

    template < template <class, class> class Name,
	       typename O, typename L >
    struct set_binary_< Name, mln::Object, O, mln::Literal, L >
    {
      typedef mln_trait_binary(Name, O, O) ret;
    };

    template < typename L1, typename L2 >
    struct set_binary_< op::eq, mln::Literal, L1, mln::Literal, L2 >
    {
      typedef bool ret;
    };

    template < typename L1, typename L2 >
    struct set_binary_< op::neq, mln::Literal, L1, mln::Literal, L2 >
    {
      typedef bool ret;
    };

  } // end of namespace mln::trait


  // Arithmetical operators.

  template <typename O, typename L>
  mln_trait_op_plus(O, O)
  operator+(const Object<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  mln_trait_op_plus(O, O)
  operator+(const Literal<L>& lhs, const Object<O>& rhs);

  template <typename O, typename L>
  mln_trait_op_minus(O, O)
  operator-(const Object<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  mln_trait_op_minus(O, O)
  operator-(const Literal<L>& lhs, const Object<O>& rhs);

  template <typename O, typename L>
  mln_trait_op_times(O, O)
  operator*(const Object<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  mln_trait_op_times(O, O)
  operator*(const Literal<L>& lhs, const Object<O>& rhs);

  template <typename O, typename L>
  mln_trait_op_div(O, O)
  operator/(const Object<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  mln_trait_op_div(O, O)
  operator/(const Literal<L>& lhs, const Object<O>& rhs);

  template <typename O, typename L>
  mln_trait_op_mod(O, O)
  operator%(const Object<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  mln_trait_op_mod(O, O)
  operator%(const Literal<L>& lhs, const Object<O>& rhs);


  // Operator equal.

  template <typename O, typename L>
  mln_trait_op_eq(O, O)
  operator==(const Object<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  mln_trait_op_eq(O, O)
  operator==(const Literal<L>& lhs, const Object<O>& rhs);

  template <typename L1, typename L2>
  bool operator==(const Literal<L1>& lhs, const Literal<L2>& rhs);

  // FIXME: ... 



# ifndef MLN_INCLUDE_ONLY

  // FIXME: Static assertion code de-activated because when O's ctor
  // is explicit from literal the metal program does not compile...


  // Op +

  template <typename O, typename L>
  inline
  mln_trait_op_plus(O, O)
  operator+(const Object<O>& lhs, const Literal<L>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return exact(lhs) + O(exact(rhs));
  }

  template <typename L, typename O>
  inline
  mln_trait_op_plus(O, O)
  operator+(const Literal<L>& lhs, const Object<O>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return O(exact(lhs)) + exact(rhs);
  }

  // Op -

  template <typename O, typename L>
  inline
  mln_trait_op_minus(O, O)
  operator-(const Object<O>& lhs, const Literal<L>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return exact(lhs) - O(exact(rhs));
  }

  template <typename L, typename O>
  inline
  mln_trait_op_minus(O, O)
  operator-(const Literal<L>& lhs, const Object<O>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return O(exact(lhs)) - exact(rhs);
  }

  // Op *

  template <typename O, typename L>
  inline
  mln_trait_op_times(O, O)
  operator*(const Object<O>& lhs, const Literal<L>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return exact(lhs) * O(exact(rhs));
  }

  template <typename L, typename O>
  inline
  mln_trait_op_times(O, O)
  operator*(const Literal<L>& lhs, const Object<O>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return O(exact(lhs)) * exact(rhs);
  }

  // Op /

  template <typename O, typename L>
  inline
  mln_trait_op_div(O, O)
  operator/(const Object<O>& lhs, const Literal<L>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return exact(lhs) / O(exact(rhs));
  }

  template <typename L, typename O>
  inline
  mln_trait_op_div(O, O)
  operator/(const Literal<L>& lhs, const Object<O>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return O(exact(lhs)) / exact(rhs);
  }

  // Op %

  template <typename O, typename L>
  inline
  mln_trait_op_mod(O, O)
  operator%(const Object<O>& lhs, const Literal<L>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return exact(lhs) % O(exact(rhs));
  }

  template <typename L, typename O>
  inline
  mln_trait_op_mod(O, O)
  operator%(const Literal<L>& lhs, const Object<O>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return O(exact(lhs)) % exact(rhs);
  }


  // Op ==

  template <typename O, typename L>
  inline
  mln_trait_op_eq(O, O)
  operator==(const Object<O>& lhs, const Literal<L>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return exact(lhs) == O(exact(rhs));
  }

  template <typename L, typename O>
  inline
  mln_trait_op_eq(O, O)
  operator==(const Literal<L>& lhs, const Object<O>& rhs)
  {
    // mlc_converts_to(L, O)::check();
    return O(exact(lhs)) == exact(rhs);
  }

  template <typename L1, typename L2>
  inline
  bool
  operator==(const Literal<L1>&, const Literal<L2>&)
  {
    return mlc_equal(L1, L2)::value;
  }

  // FIXME: Add less, etc.

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_LITERAL_OPS_HH
