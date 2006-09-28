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

#ifndef XTD_LITERAL_HH
# define XTD_LITERAL_HH

# include <xtd/abstract/plain_nary_fun.hh>
# include <xtd/abstract/meta_nary_fun.hh>
# include <xtd/abstract/fun_nary_expr.hh>



namespace xtd
{

  namespace ERROR
  {
    struct FIXME;

  } // end of namespace xtd::ERROR



  /*! \class xtd::plain_literal_<T>
  **
  ** FIXME: doc
  */

  template <typename T> struct plain_literal_; // fwd decl

  template <typename T>
  struct fun_traits_< plain_literal_<T> >
  {
    typedef T res_type;
  };
  
  template <typename T>
  struct plain_literal_

    : private mlc::assert_< mlc_is_not_a(T, abstract::fun_),
			    xtd::ERROR::FIXME >,

      public abstract::plain_nary_fun_< 0, plain_literal_<T> >
  {
    const T value;

    plain_literal_(T value)
      : value(value)
    {} 

    T impl_op() const
    {
      return this->value;
    }
  };



  // equipment for xtd::meta_literal_<T>

  template <typename T> struct meta_literal_; // fwd decl

  template <typename T>
  struct res_< meta_literal_<T> >
  {
    typedef T ret;
  };


  /*! \class xtd::meta_literal_<T>
  **
  ** FIXME: doc
  */

  template <typename T>
  struct meta_literal_

    : private mlc::assert_< mlc_is_not_a(T, abstract::fun_),
			    xtd::ERROR::FIXME >,

      public abstract::meta_nary_fun_< 0, meta_literal_<T> >
  {
    const T value;

    meta_literal_(T value)
      : value(value)
    {} 

    T impl_calc() const
    {
      return this->value;
    }
  };



  // literal as fun expression
  // FIXME: this should be the only version (?)
  // FIXME: we should remove the plain version (?)

  template <typename T>
  struct literal_expr_;

  
  template <typename T>
  struct nargs_< literal_expr_<T> >
  {
    static const unsigned ret = 0;
  };


  template <typename T, typename Args>
  struct expr_res_< literal_expr_<T>, Args >
  {
    typedef T ret;
  };


  template <typename T>
  struct literal_expr_

    : private mlc::assert_< mlc_is_not_a(T, abstract::fun_),
			    xtd::ERROR::FIXME >,

      public abstract::fun_nary_expr_< 0, literal_expr_<T> >
  {
    const T value;

    literal_expr_(T value) :
      value(value)
    {}

    template <typename Args>
    T impl_eval(const Args&) const
    {
      return this->value;
    }
  };


  // FIXME: doc

  template <typename T>
  literal_expr_<T> lit(T value)
  {
    literal_expr_<T> tmp(value);
    return tmp;
  }


} // end of namespace xtd



#endif // ! XTD_LITERAL_HH
