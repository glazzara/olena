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

#ifndef EXTENDED_LITERAL_HH
# define EXTENDED_LITERAL_HH

# include <xtd/abstract/plain_fun.hh>



namespace xtd
{

  /*! \class xtd::literal_<T>
  **
  ** FIXME: doc
  */

  template <typename T> struct literal_; // fwd decl

  template <typename T>
  struct fun_traits_< literal_<T> >
  {
    typedef T res_type;
  };
  
  template <typename T>
  struct literal_ : public abstract::plain_nary_fun_< 0, literal_<T> >
  {
    const T value;

    literal_(const T& value)
      : value(value)
    {} 

    T impl_op() const
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

    : public abstract::nary_fun_expr_< 0, literal_expr_<T> >
  {
    const T value;

    literal_expr_(const T& value) :
      value(value)
    {}

    template <typename Args>
    T impl_eval(const Args&) const
    {
      return this->value;
    }
  };


} // end of namespace xtd



#endif // ! EXTENDED_LITERAL_HH
