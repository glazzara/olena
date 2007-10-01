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

#ifndef MLN_FUN_OPS_HH
# define MLN_FUN_OPS_HH

/*! \file mln/fun/ops.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>
# include <mln/fun/internal/selector.hh>



# define mln_decl_binary_expr_(In, Out, Name, Symbol)				\
										\
  namespace fun									\
  {										\
										\
    template <typename L, typename R>						\
    struct Name##_##Out##_expr_							\
      : public Function_##Out < Name##_##Out##_expr_<L,R> >			\
    {										\
      typedef mln_result(L) result;						\
										\
      Name##_##Out##_expr_(const L& l, const R& r)				\
	: l_(l), r_(r)								\
      {										\
      }										\
										\
      template <typename P>							\
      result operator()(const P& p) const					\
      {										\
	return l_(p) Symbol r_(p);						\
      }										\
										\
    protected:									\
      const L l_;								\
      const R r_;								\
    };										\
										\
  }										\
										\
  template <typename L, typename R>						\
  fun::Name##_##Out##_expr_<L,R>						\
  operator Symbol (const Function_##In<L>& lhs, const Function_##In<R>& rhs)	\
  {										\
    fun::Name##_##Out##_expr_<L,R> tmp(exact(lhs), exact(rhs));			\
    return tmp;									\
  }										\
										\
  struct dummy


# define mln_decl_unary_expr_(In, Out, Name, Symbol)		\
								\
  namespace fun							\
  {								\
								\
    template <typename F>					\
    struct Name##_##Out##_expr_					\
      : public Function_##Out< Name##_##Out##_expr_<F> >	\
    {								\
      typedef mln_result(F) result;				\
								\
      Name##_##Out##_expr_(const F& f)				\
	: f_(f)							\
      {								\
      }								\
								\
      template <typename P>					\
      result operator()(const P& p) const			\
      {								\
	return Symbol f_(p);					\
      }								\
								\
    protected:							\
      const F f_;						\
    };								\
								\
  }								\
								\
  template <typename F>						\
  fun::Name##_##Out##_expr_<F>					\
  operator Symbol (const Function_##In<F>& f)			\
  {								\
    fun::Name##_##Out##_expr_<F> tmp(exact(f));			\
    return tmp;							\
  }								\
								\
  struct dummy



namespace mln
{
  // -> p2v

  mln_decl_binary_expr_(p2v, p2b,  eq, ==);
  mln_decl_binary_expr_(p2v, p2b, neq, !=);

  mln_decl_binary_expr_(p2v, p2b, less, <);
  mln_decl_binary_expr_(p2v, p2b, leq , <=);
  mln_decl_binary_expr_(p2v, p2b, geq,  >=);
  mln_decl_binary_expr_(p2v, p2b, greater, >);

  mln_decl_binary_expr_(p2b, p2b, and, &&);
  mln_decl_binary_expr_(p2b, p2b, or, ||);
  mln_decl_binary_expr_(p2b, p2b, xor, ^);

  mln_decl_unary_expr_(p2b, p2b, not, !);

  mln_decl_binary_expr_(p2v, p2v, plus, +);
  mln_decl_binary_expr_(p2v, p2v, minus, -);
  mln_decl_binary_expr_(p2v, p2v, times, *);
  mln_decl_binary_expr_(p2v, p2v, div, /);
  mln_decl_binary_expr_(p2v, p2v, mod, %);

  mln_decl_unary_expr_(p2v, p2v, uplus, +);
  mln_decl_unary_expr_(p2v, p2v, uminus, -);

  // -> v2b

  mln_decl_binary_expr_(v2v, v2b, eq,  ==);
  mln_decl_binary_expr_(v2v, v2b, neq, !=);

  mln_decl_binary_expr_(v2v, v2b, less, <);
  mln_decl_binary_expr_(v2v, v2b, leq,  <=);
  mln_decl_binary_expr_(v2v, v2b, geq,  >=);
  mln_decl_binary_expr_(v2v, v2b, greater, >);

  mln_decl_binary_expr_(v2b, v2b, and, &&);
  mln_decl_binary_expr_(v2b, v2b, or, ||);
  mln_decl_binary_expr_(v2b, v2b, xor, ^);

  mln_decl_unary_expr_(v2b, v2b, not, !);

} // end of namespace mln


#endif // ! MLN_FUN_OPS_HH
