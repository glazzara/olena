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

#ifndef EXTENDED_ARG_HH
# define EXTENDED_ARG_HH

# include <mlc/assert.hh>
# include <mlc/is_a.hh>

# include <xtd/args.hh>
# include <xtd/mexpr.hh>



namespace xtd
{


  namespace internal
  {

    // FIXME: doc

    template <unsigned i> struct impl_arg_;

    template <>
    struct impl_arg_<1>
    {
      template <typename Args>
      typename Args::arg1_type impl_eval(const Args& args) const
      {
	mlc::assert_< mlc_is_a(Args, xtd::abstract::args) >::check();
	return args.arg1;
      }
    };

    template <>
    struct impl_arg_<2>
    {
      template <typename Args>
      typename Args::arg2_type impl_eval(const Args& args) const
      {
	mlc::assert_< mlc_is_a(Args, xtd::abstract::args) >::check();
	return args.arg2;
      }
    };

    template <>
    struct impl_arg_<3>
    {
      template <typename Args>
      typename Args::arg3_type impl_eval(const Args& args) const
      {
	mlc::assert_< mlc_is_a(Args, xtd::abstract::args) >::check();
	return args.arg3;
      }
    };

  } // end of namespace xtd::internal




  /*! \class xtd::arg_<i>
  **
  ** FIXME: placeholder for the ith argument
  */


  template <unsigned i> struct arg_;

  template <unsigned i>
  struct nargs_< arg_<i> >
  {
    static const unsigned ret = i;
  };

  
  template <typename Args>
  struct expr_res_< arg_<1>, Args >
  {
    typedef typename Args::arg1_type ret;
  };
  
  template <typename Args>
  struct expr_res_< arg_<2>, Args >
  {
    typedef typename Args::arg2_type ret;
  };
  
  template <typename Args>
  struct expr_res_< arg_<3>, Args >
  {
    typedef typename Args::arg3_type ret;
  };
  

  template <unsigned i>
  struct arg_

    : public abstract::nary_fun_expr_< i, arg_<i> >,

      public internal::impl_arg_<i>

  // FIXME: add something like ": private mlc::assert_< (i > 1 and i <= 3) >"
  {
    typedef internal::impl_arg_<i> super;
    using super::impl_eval;
  };


  // placeholders:

  const arg_<1> _1 = arg_<1>();
  const arg_<2> _2 = arg_<2>();
  const arg_<3> _3 = arg_<3>();


} // end of namespace xtd


#endif // ! EXTENDED_ARG_HH
