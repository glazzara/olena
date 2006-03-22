// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef EXTENDED_OPS_EXPR_HH
# define EXTENDED_OPS_EXPR_HH

# include <mlc/case.hh>

# include <xtd/abstract/fun_expr.hh>
# include <xtd/mexpr.hh>
# include <xtd/math/arith.hh>


namespace xtd
{

//   namespace abstract
//   {


  // +

    template <typename L, typename R>
    xtd::m2expr_<xtd::plus_type, L, R>
    operator + (const xtd::abstract::fun_expr_<L>& lexpr,
		const xtd::abstract::fun_expr_<R>& rexpr)
    {
      xtd::m2expr_<xtd::plus_type, L, R> tmp(lexpr, rexpr);
      return tmp;
    }

  // HERE

//     template <typename R>
//     xtd::m2expr_<xtd::plus_type, m0expr<literal_<int> >, R>
//     operator + (int i,
// 		const xtd::abstract::fun_expr_<R>& rexpr)
//     {
//       xtd::m2expr<xtd::plus_type, L, R> tmp;
//       return tmp;
//     }


  // *

    template <typename L, typename R>
    xtd::m2expr_<xtd::mult_type, L, R>
    operator * (const xtd::abstract::fun_expr_<L>& lexpr,
		const xtd::abstract::fun_expr_<R>& rexpr)
    {
      xtd::m2expr_<xtd::mult_type, L, R> tmp(lexpr, rexpr);
      return tmp;
    }

//   } // end of namespace xtd::abstract

} // end of namespace xtd


#endif // ! EXTENDED_OPS_EXPR_HH
