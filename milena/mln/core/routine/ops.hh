// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_ROUTINE_OPS_HH
# define MLN_CORE_ROUTINE_OPS_HH

/// \file
///
/// \brief Definitions of some default implementations for operators.

/*! The set of operators defined in this file is:
 *
 * \code

   l += r  :  l = l + r, -> l&
   l -= r  :  l = l - r, -> l&
   l *= r  :  l = l * r, -> l&
   l /= r  :  l = l / r, -> l&
   l %= r  :  l = l % r, -> l&

   + r     :  -> r
   - r     :  -> (0 - r)

   l ++    :  t = l, ++l, -> t
   l --    :  t = l, --l, -> t

   ++ r    :  r += 1, -> r&
   -- r    :  r -= 1, -> r&

   l != r  :  -> ! (l == r)

   l >  r  :  -> (r < l)
   l >= r  :  -> (r <= l)
   l <= r  :  -> ! (r < l)   warning: re-define when partial ordering

 * \endcode
 *
 *
 * As a consequence, the set of operators to be defined along with a
 * client class is:
 *
 * \code

   l + r
   l - r
   l * r
   l / r

   l == r

   l < r
   l <= r  in case of partial ordering

 * \endcode
 *
 * \todo Complete those definitions (...) + Overload for const (?)
 *
 * \todo Optimize deduced ops to eliminate object creation.  E.g.
 * in the unary minus op:
 *    literal::zero_t* p_zero = 0;
 *    return O(*p_zero) - exact(rhs);
 * can become:
 *    static O zero =...
 *    return zero - exact(rhs);
 *
 * \todo Address the remark in op/=.
 */

# include <mln/trait/op/all.hh>
# include <mln/core/concept/object.hh>
# include <mln/metal/converts_to.hh>


namespace mln
{

  // Fwd decls.
  namespace literal { struct zero_t; struct one_t; }


  namespace trait
  {

    // For unary operators.


    /*!
      \internal
      \brief Default definition of op::uplus trait.
    */
    template <typename O>
    struct set_unary_< op::uplus, Object,O > { typedef O ret; };

    /*!
      \internal
      \brief Default definition of op::uminus trait.
    */
    template <typename O>
    struct set_unary_< op::uminus, Object,O > { typedef mln_trait_op_minus(O, O) ret; };

    /*!
      \internal
      \brief Default definition of op::preinc trait.
    */
    template <typename O>
    struct set_unary_< op::preinc, Object,O > { typedef O& ret; };

    /*!
      \internal
      \brief Default definition of op::preinc trait.
    */
    template <typename O>
    struct set_unary_< op::predec, Object,O > { typedef O& ret; };

    /*!
      \internal
      \brief Default definition of op::postinc trait.
    */
    template <typename O>
    struct set_unary_< op::postinc, Object,O > { typedef O ret; };

    /*!
      \internal
      \brief Default definition of op::postinc trait.
    */
    template <typename O>
    struct set_unary_< op::postdec, Object,O > { typedef O ret; };



    // For binary operators.

    template <typename O1, typename O2>
    struct set_binary_< op::eq, Object,O1, Object,O2 >  { typedef bool ret; };

    template <typename O1, typename O2>
    struct set_binary_< op::neq, Object,O1, Object,O2 >
    {
      // O1 != O2  -->  ! (O1 == O2)
      typedef mln_trait_op_eq(O1, O2) B_;
      typedef mln_trait_op_not(B_) ret;
    };

    template <typename O1, typename O2>
    struct set_binary_< op::less, Object,O1, Object,O2 >  { typedef bool ret; };

    template <typename O1, typename O2>
    struct set_binary_< op::leq, Object,O1, Object,O2 >
    {
      // O1 <= O2  -->  ! (O2 < 01)
      typedef mln_trait_op_less(O2, O1) B_;
      typedef mln_trait_op_not(B_) ret;
    };

    template <typename O1, typename O2>
    struct set_binary_< op::geq, Object,O1, Object,O2 >
    {
      // O1 >= O2  -->  O2 <= O1
      typedef mln_trait_op_leq(O2, O1) ret;
    };

    template <typename O1, typename O2>
    struct set_binary_< op::greater, Object,O1, Object,O2 >
    {
      // O1 > O2  -->  O2 < O1
      typedef mln_trait_op_less(O2, O1) ret;
    };

    // FIXME: Same for the other definitions below...



    // Case of "const" in types.

    template< template <class> class Name,
	      typename O >
    struct set_precise_unary_< Name, const O >
    {
      typedef mln_trait_unary(Name, O) ret;
    };

    template< template <class,class> class Name,
	      typename O1, typename O2 >
    struct set_precise_binary_< Name, O1, const O2 >
    {
      typedef mln_trait_binary(Name, O1, O2) ret;
    };

    template< template <class,class> class Name,
	      typename O1, typename O2 >
    struct set_precise_binary_< Name, const O1, O2 >
    {
      typedef mln_trait_binary(Name, O1, O2) ret;
    };

    template< template <class,class> class Name,
	      typename O1, typename O2 >
    struct set_precise_binary_< Name, const O1, const O2 >
    {
      typedef mln_trait_binary(Name, O1, O2) ret;
    };


  } // end of mln::trait



  /*! \brief General definition of the "not equal to" operator.
   *
   * The "not equal to" operator is here defined for every milena
   * objects.  It relies on the definition of the "equal to" operator.
   * It returns "not (lhs == rhs)".
   *
   * \warning There shall not be any other definition of this operator
   * in Milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  mln_trait_op_neq(O1, O2)
    operator!=(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief General definition of the "greater than" operator.
   *
   * The "greater than" operator is here defined for every milena
   * objects.  It relies on the definition of the "less than"
   * operator.  It returns "rhs < lhs".
   *
   * \warning There shall not be any other definition of this operator
   * in Milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  mln_trait_op_greater(O1, O2)
    operator>(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief General definition of the "greater than or equal to"
   *  operator.
   *
   * The "greater than or equal to" operator is here defined for
   * every Milena objects.  It relies on the definition of the "less
   * than or equal to" operator.  It returns "rhs <= lhs".
   *
   * \warning There shall not be any other definition of this operator
   * in Milena when applying on a couple of mln::Object.
   */
  template <typename O1, typename O2>
  mln_trait_op_geq(O1, O2)
    operator>=(const Object<O1>& lhs, const Object<O2>& rhs);


  /*! \brief Default definition of the "less than or equal to"
   *  operator.
   *
   * A default version of the "less than or equal to" operator is
   * defined for every Milena objects.  It relies on the definition of
   * the "less than" operator.  It returns "not (rhs < lhs)".
   *
   * \warning In the case of partial ordering between objects, this
   * operator has to be re-defined.
   */
  template <typename O1, typename O2>
  mln_trait_op_leq(O1, O2)
    operator<=(const Object<O1>& lhs, const Object<O2>& rhs);


  /* \brief Default definition of the post-incrementation operator.
   *
   * It relies on the definition of the pre-incrementation operator.
   */
  template <typename O>
  O operator++(Object<O>& lhs, int);


  /* \brief Default definition of the post-decrementation operator.
   *
   * It relies on the definition of the pre-decrementation operator.
   */
  template <typename O>
  O operator--(Object<O>& lhs, int);


  /* \brief Default definition of the pre-incrementation operator.
   *
   * It relies on "+ literal::one".
   */
  template <typename O>
  O& operator++(Object<O>& rhs);


  /* \brief Default definition of the pre-decrementation operator.
   *
   * It relies on "- literal::one".
   */
  template <typename O>
  O& operator--(Object<O>& rhs);


  /* \brief Default definitions of the "unary plus" operator.
   *
   * \param[in] rhs An object
   * \return A copy of \p rhs.
   */
  template <typename O>
  O operator+(const Object<O>& rhs);


  /* \brief Default definition of the "unary minus" operator.
   *
   * \param[in] rhs An object
   * \return 0 - \p rhs.
   *
   * It relies on "O(literal::zero) - rhs".
   */
  template <typename O>
  mln_trait_op_minus(O, O)
  operator-(const Object<O>& rhs);


  /* \brief Default definition of the "plus equal" operator.
   *
   * \param[in,out] lhs The target object.
   * \param[in] rhs The auxiliary object.
   * \return The target object \p lhs once modified.
   *
   * It relies on "lhs = L(lhs) + rhs".
   */
  template <typename L, typename R>
  L&
  operator+=(Object<L>& lhs, const Object<R>& rhs);


  /* \brief Default definition of the "minus equal" operator.
   *
   * \param[in,out] lhs The target object.
   * \param[in] rhs The auxiliary object.
   * \return The target object \p lhs once modified.
   *
   * It relies on "lhs = L(lhs) - rhs".
   */
  template <typename L, typename R>
  L&
  operator-=(Object<L>& lhs, const Object<R>& rhs);


  /* \brief Default definition of the "times equal" operator.
   *
   * \param[in,out] lhs The target object.
   * \param[in] rhs The auxiliary object.
   * \return The target object \p lhs once modified.
   *
   * It relies on "lhs = L(lhs) * rhs".
   */
  template <typename L, typename R>
  L&
  operator*=(Object<L>& lhs, const Object<R>& rhs);


  /* \brief Default definition of the "div equal" operator.
   *
   * \param[in,out] lhs The target object.
   * \param[in] rhs The auxiliary object.
   * \return The target object \p lhs once modified.
   *
   * It relies on "lhs = L(lhs) / rhs".
   */
  template <typename L, typename R>
  L&
  operator/=(Object<L>& lhs, const Object<R>& rhs);


  /* \brief Default definition of the "mod equal" operator.
   *
   * \param[in,out] lhs The target object.
   * \param[in] rhs The auxiliary object.
   * \return The target object \p lhs once modified.
   *
   * It relies on "lhs = L(lhs) % rhs".
   */
  template <typename L, typename R>
  L&
  operator%=(Object<L>& lhs, const Object<R>& rhs);



# ifndef MLN_INCLUDE_ONLY

  // Plus equal.

  template <typename L, typename R>
  inline
  L&
  operator+=(Object<L>& lhs, const Object<R>& rhs)
  {
    typedef mln_trait_op_plus(L, R) P;
    mlc_converts_to(P, L)::check();
    return exact(lhs) = static_cast<L>(exact(lhs) + exact(rhs));
  }

  // Minus equal.

  template <typename L, typename R>
  inline
  L&
  operator-=(Object<L>& lhs, const Object<R>& rhs)
  {
    typedef mln_trait_op_minus(L, R) M;
    mlc_converts_to(M, L)::check();
    return exact(lhs) = static_cast<L>(exact(lhs) - exact(rhs));
  }

  // Times equal.

  template <typename L, typename R>
  inline
  L&
  operator*=(Object<L>& lhs, const Object<R>& rhs)
  {
    typedef mln_trait_op_times(L, R) T;
    mlc_converts_to(T, L)::check();
    return exact(lhs) = static_cast<L>(exact(lhs) * exact(rhs));
  }

  // Div equal.

  template <typename L, typename R>
  inline
  L&
  operator/=(Object<L>& lhs, const Object<R>& rhs)
  {
    typedef mln_trait_op_div(L, R) D;
    mlc_converts_to(D, L)::check();
    // Remark:
    // The code below gives a warning with g++-3.3 with the test file
    // tests/value/int_u8.cc.  It is normal since writing "i /= f"
    // means casting eventually float to int.  Yet what do we want in
    // that case?  A quiet compilation or a warning?
    return exact(lhs) = static_cast<L>(exact(lhs) / exact(rhs));
  }

  // Mod equal.

  template <typename L, typename R>
  inline
  L&
  operator%=(Object<L>& lhs, const Object<R>& rhs)
  {
    typedef mln_trait_op_mod(L, R) M;
    mlc_converts_to(M, L)::check();
    return exact(lhs) = static_cast<L>(exact(lhs) % exact(rhs));
  }

  // Unary plus.

  template <typename O>
  inline
  O
  operator+(const Object<O>& rhs)
  {
    return exact(rhs); // Cpy.
  }

  // Unary minus.

  template <typename O>
  inline
  mln_trait_op_minus(O, O)
  operator-(const Object<O>& rhs)
  {
    mlc_converts_to(literal::zero_t, O)::check();
    literal::zero_t* p_zero = 0;
    return O(*p_zero) - exact(rhs);
  }

  // Post-incrementation.

  template <typename O>
  inline
  O
  operator++(Object<O>& lhs, int)
  {
    O tmp(exact(lhs)); // Copy.
    ++exact(lhs);      // Pre-inc.
    // FIXME: Activate: mln_postcondition(exact(lhs) == tmp + literal::one);
    return tmp;
  }

  // Post-decrementation.

  template <typename O>
  inline
  O
  operator--(Object<O>& lhs, int)
  {
    O tmp(exact(lhs)); // Copy.
    --exact(lhs);      // Pre-dec.
    // FIXME: Activate: mln_postcondition(exact(lhs) == tmp - literal::one);
    return tmp;
  }

  // Pre-decrementation.

  template <typename O>
  inline
  O&
  operator--(Object<O>& rhs)
  {
    literal::one_t* p_one;
    exact(rhs) -= *p_one;
    return exact(rhs);
  }

  // Pre-incrementation.

  template <typename O>
  inline
  O&
  operator++(Object<O>& rhs)
  {
    literal::one_t* p_one;
    exact(rhs) += *p_one;
    return exact(rhs);
  }

  // Comparisons.

  template <typename O1, typename O2>
  inline
  mln_trait_op_neq(O1, O2)
  operator!=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return ! (exact(lhs) == exact(rhs));
  }

  template <typename O1, typename O2>
  inline
  mln_trait_op_greater(O1, O2)
    operator>(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return exact(rhs) < exact(lhs);
  }

  template <typename O1, typename O2>
  inline
  mln_trait_op_geq(O1, O2)
    operator>=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return exact(rhs) <= exact(lhs);
  }

  template <typename O1, typename O2>
  inline
  mln_trait_op_leq(O1, O2)
    operator<=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    // if partial ordering, this operator should be re-defined!
    return ! (exact(rhs) < exact(lhs));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_OPS_HH
