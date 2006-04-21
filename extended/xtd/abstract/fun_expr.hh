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

#ifndef EXTENDED_ABSTRACT_FUN_EXPR_HH
# define EXTENDED_ABSTRACT_FUN_EXPR_HH

# include <xtd/abstract/fun.hh>
# include <xtd/res.hh>
# include <xtd/args.hh>




namespace xtd
{

  namespace ERROR
  {

    struct INTERNAL_ILL_FORMED_CALL_TO_xtd_fun_expr_eval;

  } // end of namespace xtd::ERROR



  namespace abstract
  {



    /*! \class xtd::abstract::fun_expr_<E>
    **
    ** Abstract base class for function expressions.  Parameter E is
    ** the exact type of the function expression.
    */

    template <typename E>
    class fun_expr_

      : public fun_<E>
    {
    public:

      template <typename Args>
      xtd_expr_res(E, Args)
	eval(const Args& arglist) const
      {
	mlc::assert_< mlc_is_a(Args, xtd::abstract::args),
	              xtd::ERROR::INTERNAL_ILL_FORMED_CALL_TO_xtd_fun_expr_eval >::check();
	return this->exact().impl_eval(arglist);
      }

    protected:
      fun_expr_() {}
    };


  } // end of namespace xtd::abstract


} // end of namespace xtd



#endif // ! EXTENDED_ABSTRACT_FUN_EXPR_HH
