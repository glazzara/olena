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

#ifndef XTD_BIND_HH
# define XTD_BIND_HH

# include <xtd/abstract/fun_nary_expr.hh>
# include <xtd/literal.hh>
# include <xtd/mexpr.hh>
# include <xtd/arg.hh>


namespace xtd
{


  namespace abstract
  {

    // fun_nary_expr_< 2, E >::bind_i

    template <typename E>
    template <typename T>
    m2expr_< E, literal_expr_<T>, arg_<2> >
    fun_nary_expr_< 2, E >::bind_1(T value) const
    {
      typedef m2expr_< E, literal_expr_<T>, arg_<2> > ret;
      ret tmp(this->exact(), lit(value), arg_<2>());
      return tmp;
    }

    template <typename E>
    template <typename T>
    m2expr_< E, arg_<1>, literal_expr_<T> >
    fun_nary_expr_< 2, E >::bind_2(T value) const
    {
      typedef m2expr_< E, arg_<1>, literal_expr_<T> > ret;
      ret tmp(this->exact(), arg_<1>(), lit(value));
      return tmp;
    }


    // fun_nary_expr_< 3, E >::bind_i

    template <typename E>
    template <typename T>
    m3expr_< E, literal_expr_<T>, arg_<2>, arg_<3> >
    fun_nary_expr_< 3, E >::bind_1(T value) const
    {
      typedef m3expr_< E, literal_expr_<T>, arg_<2>, arg_<3> > ret;
      ret tmp(this->exact(), lit(value), arg_<2>(), arg_<3>());
      return tmp;
    }

    template <typename E>
    template <typename T>
    m3expr_< E, arg_<1>, literal_expr_<T>, arg_<3> >
    fun_nary_expr_< 3, E >::bind_2(T value) const
    {
      typedef m3expr_< E, arg_<1>, literal_expr_<T>, arg_<3> > ret;
      ret tmp(this->exact(), arg_<1>(), lit(value), arg_<3>());
      return tmp;
    }

    template <typename E>
    template <typename T>
    m3expr_< E, arg_<1>, arg_<2>, literal_expr_<T> >
    fun_nary_expr_< 3, E >::bind_3(T value) const
    {
      typedef m3expr_< E, arg_<1>, arg_<2>, literal_expr_<T> > ret;
      ret tmp(this->exact(), arg_<1>(), arg_<2>(), lit(value));
      return tmp;
    }

  } // end of namespace xtd::abstract

} // end of namespace xtd



#endif // ! XTD_BIND_HH
