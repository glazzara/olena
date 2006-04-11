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

#ifndef EXTENDED_ARGS_HH
# define EXTENDED_ARGS_HH

# include <xtd/internal/mlc.hh>



// FIXME: doc

# define xtd_nargs(F) xtd::internal::get_nargs_< F >::ret



namespace xtd
{


  namespace abstract
  {

    // fwd decl
    template <typename E> struct fun_expr_;


    /*! \class xtd::abstract::args
    **
    ** FIXME: Abstract base class for args_<..>
    */

    struct args
    {
    protected:
      args() {}
    };

  } // end of namespace xtd::abstract




  /*! \class xtd::args_<A1, ..>
  **
  ** FIXME: to pack arguments
  */

  // three args

  template <typename A1 = mlc::none,
	    typename A2 = mlc::none,
	    typename A3 = mlc::none>
  struct args_ : public abstract::args
  {
    args_(const A1& arg1, const A2& arg2, const A3& arg3)
      : arg1(arg1),
	arg2(arg2),
	arg3(arg3)
    {}
    const A1 arg1;
    const A2 arg2;
    const A3 arg3;
    typedef A1 arg1_type;
    typedef A2 arg2_type;
    typedef A3 arg3_type;
  };

  template <typename A1, typename A2, typename A3>
  args_<A1,A2,A3> mk_args(const A1& arg1, const A2& arg2, const A3& arg3)
  {
    args_<A1,A2,A3> tmp(arg1, arg2, arg3);
    return tmp;
  }


  // no arg

  template <>
  struct args_<> : public abstract::args
  {
    args_()
    {}
    typedef mlc::none arg1_type;
    typedef mlc::none arg2_type;
    typedef mlc::none arg3_type;
  };

  args_<> mk_args()
  {
    args_<> tmp;
    return tmp;
  }


  // a single arg

  template <typename A1>
  struct args_< A1 > : public abstract::args
  {
    args_(const A1& arg1)
      : arg1(arg1)
    {}
    const A1 arg1;
    typedef A1 arg1_type;
    typedef mlc::none arg2_type;
    typedef mlc::none arg3_type;
  };

  template <typename A1>
  args_<A1> mk_args(const A1& arg1)
  {
    args_<A1> tmp(arg1);
    return tmp;
  }


  // a couple of args

  template <typename A1, typename A2>
  struct args_< A1, A2 > : public abstract::args
  {
    args_(const A1& arg1, const A2& arg2)
      : arg1(arg1),
	arg2(arg2)
    {}
    const A1 arg1;
    const A2 arg2;
    typedef A1 arg1_type;
    typedef A2 arg2_type;
    typedef mlc::none arg3_type;
  };

  template <typename A1, typename A2>
  args_<A1,A2> mk_args(const A1& arg1, const A2& arg2)
  {
    args_<A1,A2> tmp(arg1, arg2);
    return tmp;
  }






  namespace ERROR
  {

    struct SPECIALIZATION_OF_xtd_nargs_IS_FOR_xtd_fun_expr_ONLY;
    struct SPECIALIZATION_OF_xtd_nargs_NOT_FOUND_FOR_AN_xtd_fun_expr;

  } // end of namespace xtd::ERROR



  /*! \class xtd::nargs_<F>
  **
  ** FIXME: doc
  */

  template <typename F>
  struct nargs_ : public mlc::undefined
  {
  };


  namespace internal
  {

    template <typename F>
    struct get_nargs_

      : // private mlc::assert_< mlc_is_a(F, xtd::abstract::fun_expr_),
	//	                 xtd::ERROR::SPECIALIZATION_OF_xtd_nargs_IS_FOR_xtd_fun_expr_ONLY >,
        // FIXME: the static assertion above does *not* compile...

	private mlc::assert_< mlc_is_not_a(xtd::nargs_<F>, mlc::undefined),
			      xtd::ERROR::SPECIALIZATION_OF_xtd_nargs_NOT_FOUND_FOR_AN_xtd_fun_expr >
    {
      static const unsigned ret = xtd::nargs_<F>::ret;
    };

  } // end of xtd::internal




} // end of namespace xtd


#endif // ! EXTENDED_ARGS_HH
