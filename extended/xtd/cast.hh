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

#ifndef EXTENDED_CAST_HH
# define EXTENDED_CAST_HH

# include <xtd/abstract/plain_nary_fun.hh>
# include <xtd/abstract/meta_nary_fun.hh>
# include <xtd/mexpr.hh>



namespace xtd
{

  /*! \class xtd::plain_cast_<Dest>
  **
  ** FIXME: doc
  */

  template <typename Dest, typename Src> struct plain_cast_; // fwd decl

  template <typename Dest, typename Src>
  struct fun_traits_< plain_cast_<Dest, Src> >
  {
    typedef Src arg_type;
    typedef Dest res_type;
  };
  
  template <typename Dest, typename Src>
  struct plain_cast_ : public abstract::plain_nary_fun_< 1, plain_cast_<Dest, Src> >
  {
    Dest impl_op(const Src& arg) const
    {
      return Dest(arg);
    }
  };




  // equipment for xtd::meta_cast_<Dest>

  template <typename Dest> struct meta_cast_; // fwd decl

  template <typename Dest, typename A>
  struct res_< meta_cast_<Dest>, A >
  {
    typedef Dest ret;
  };


  /*! \class xtd::meta_cast_<Dest>
  **
  ** FIXME: doc
  */

  template <typename Dest>
  struct meta_cast_ : public abstract::meta_nary_fun_< 1, meta_cast_<Dest> >
  {
    template <typename A>
    Dest impl_calc(const A& a) const
      // ---------
    {
      return Dest(a);
    }
  };



  /*! \function xtd::cast_<Dest>(arg)
  **
  ** This function mimics the behavior of the method:
  **
  **    xtd::meta_nary_fun_<1, E>::operator(const A& a)
  **
  ** The method is thus turned into a function where E, the
  ** unary meta function actually is meta_cast_<Dest>.
  */

  template <typename Dest, typename A>
  typename xtd::case_< xtd::tag::fun_operator_1,
		       mlc::pair_< meta_cast_<Dest>,
				   A >
                     >::ret::res
  cast_(const A& a)
  {
    typedef typename xtd::case_< xtd::tag::fun_operator_1,
                                 mlc::pair_< meta_cast_<Dest>, A> >::ret case_t;
    static const meta_cast_<Dest> target;
    return case_t::impl(target, a);
  }



} // end of namespace xtd



#endif // ! EXTENDED_CAST_HH
