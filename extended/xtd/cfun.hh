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

#ifndef EXTENDED_CFUN_HH
# define EXTENDED_CFUN_HH

# include <xtd/abstract/plain_nary_fun.hh>
# include <xtd/abstract/meta_nary_fun.hh>




namespace xtd
{


  template <typename F> struct plain_cfun_; // fwd decl


  template <typename R>
  struct fun_traits_< R(*)() >
  {
    typedef R res_type;
  };

  template <typename R, typename A>
  struct fun_traits_< R(*)(A) >
  {
    typedef R res_type;
    typedef A arg_type;
  };

  template <typename R, typename A1, typename A2>
  struct fun_traits_< R(*)(A1, A2) >
  {
    typedef R res_type;
    typedef A1 arg1_type;
    typedef A2 arg2_type;
  };

  template <typename R, typename A1, typename A2, typename A3>
  struct fun_traits_< R(*)(A1, A2, A3) >
  {
    typedef R res_type;
    typedef A1 arg1_type;
    typedef A2 arg2_type;
    typedef A3 arg3_type;
  };


  template <typename F>
  struct fun_traits_< plain_cfun_<F> > : fun_traits_<F>
  {
  };


  
  /*! \class xtd::plain_cfun_< R(*)() >
  **
  ** FIXME: doc
  */

  template <typename R>
  struct plain_cfun_ < R(*)() >

    : public abstract::plain_nary_fun_< 0, plain_cfun_<R(*)()> >
  {
    typedef R(*cfun_type)();
    const cfun_type f;

    plain_cfun_(const cfun_type& f)
      : f(f)
    {} 

    R impl_op() const
    {
      return this->f();
    }
  };


  template <typename R>
  plain_cfun_<R(*)()> mk_plain_fun( R(*f)() )
  {
    plain_cfun_<R(*)()> tmp(f);
    return tmp;
  }


  
  /*! \class xtd::plain_cfun_< R(*)(A) >
  **
  ** FIXME: doc
  */

  template <typename R, typename A>
  struct plain_cfun_ < R(*)(A) >

    : public abstract::plain_nary_fun_< 1, plain_cfun_<R(*)(A)> >
  {
    typedef R(*cfun_type)(A);
    const cfun_type f;

    plain_cfun_(const cfun_type& f)
      : f(f)
    {} 

    R impl_op(const A& a) const
    {
      return this->f(a);
    }
  };


  template <typename R, typename A>
  plain_cfun_<R(*)(A)> mk_plain_fun( R(*f)(A) )
  {
    plain_cfun_<R(*)(A)> tmp(f);
    return tmp;
  }



  
  /*! \class xtd::plain_cfun_< R(*)(A1,A2) >
  **
  ** FIXME: doc
  */

  template <typename R, typename A1, typename A2>
  struct plain_cfun_ < R(*)(A1,A2) >

    : public abstract::plain_nary_fun_< 2, plain_cfun_<R(*)(A1,A2)> >
  {
    typedef R(*cfun_type)(A1,A2);
    const cfun_type f;

    plain_cfun_(const cfun_type& f)
      : f(f)
    {} 

    R impl_op(const A1& a1, const A2& a2) const
    {
      return this->f(a1, a2);
    }
  };


  template <typename R, typename A1, typename A2>
  plain_cfun_<R(*)(A1,A2)> mk_plain_fun( R(*f)(A1,A2) )
  {
    plain_cfun_<R(*)(A1,A2)> tmp(f);
    return tmp;
  }


  
  /*! \class xtd::plain_cfun_< R(*)(A1,A2,A3) >
  **
  ** FIXME: doc
  */

  template <typename R, typename A1, typename A2, typename A3>
  struct plain_cfun_ < R(*)(A1,A2,A3) >

    : public abstract::plain_nary_fun_< 3, plain_cfun_<R(*)(A1,A2,A3)> >
  {
    typedef R(*cfun_type)(A1,A2,A3);
    const cfun_type f;

    plain_cfun_(const cfun_type& f)
      : f(f)
    {} 

    R impl_op(const A1& a1, const A2& a2, const A3& a3) const
    {
      return this->f(a1, a2, a3);
    }
  };


  template <typename R, typename A1, typename A2, typename A3>
  plain_cfun_<R(*)(A1,A2,A3)> mk_plain_fun( R(*f)(A1,A2,A3) )
  {
    plain_cfun_<R(*)(A1,A2,A3)> tmp(f);
    return tmp;
  }








  /*! \class xtd::meta_cfun_<F>
  **
  ** FIXME: doc
  */

  template <typename F>
  struct meta_cfun_;



  /*! \class xtd::meta_cfun_< R_(*)() >
  **
  ** FIXME: doc
  */

  template <typename R_>
  struct res_< meta_cfun_<R_(*)()> >
  {
    typedef R_ ret;
  };

  template <typename R_>
  struct meta_cfun_ < R_(*)() >

    : public abstract::meta_nary_fun_< 0, meta_cfun_<R_(*)()> >
  {
    typedef R_(*cfun_type)();
    const cfun_type f;

    meta_cfun_(const cfun_type& f)
      : f(f)
    {} 

    R_ impl_calc() const
    {
      return this->f;
    }
  };

  template <typename R>
  meta_cfun_<R(*)()> mk_fun( R(*f)() )
  {
    meta_cfun_<R(*)()> tmp(f);
    return tmp;
  }



  /*! \class xtd::meta_cfun_< R_(*)(A_) >
  **
  ** FIXME: doc
  */

  template <typename R_, typename A_,
	    typename A>
  struct res_< meta_cfun_<R_(*)(A_)>,
	       A >
  {
    typedef R_ ret;
  };

  template <typename R_, typename A_>
  struct meta_cfun_ < R_(*)(A_) >

    : public abstract::meta_nary_fun_< 1, meta_cfun_<R_(*)(A_)> >
  {
    typedef R_(*cfun_type)(A_);
    const cfun_type f;

    meta_cfun_(const cfun_type& f)
      : f(f)
    {} 

    template <typename A>
    R_ impl_calc(const A& a) const
    {
      // FIXME: check that conversion "A -> A_" exists
      return this->f(a);
    }
  };

  template <typename R, typename A>
  meta_cfun_<R(*)(A)> mk_fun( R(*f)(A) )
  {
    meta_cfun_<R(*)(A)> tmp(f);
    return tmp;
  }



  /*! \class xtd::meta_cfun_< R_(*)(A1_,A2_) >
  **
  ** FIXME: doc
  */

  template <typename R_, typename A1_, typename A2_,
	    typename A1, typename A2>
  struct res_< meta_cfun_<R_(*)(A1_,A2_)>,
	       A1, A2 >
  {
    typedef R_ ret;
  };

  template <typename R_, typename A1_, typename A2_>
  struct meta_cfun_ < R_(*)(A1_,A2_) >

    : public abstract::meta_nary_fun_< 2, meta_cfun_<R_(*)(A1_,A2_)> >
  {
    typedef R_(*cfun_type)(A1_,A2_);
    const cfun_type f;

    meta_cfun_(const cfun_type& f)
      : f(f)
    {} 

    template <typename A1, typename A2>
    R_ impl_calc(const A1& a1, const A2& a2) const
    {
      // FIXME: check that conversions "Ai -> Ai_" exist
      return this->f(a1, a2);
    }
  };

  template <typename R, typename A1, typename A2>
  meta_cfun_<R(*)(A1,A2)> mk_fun( R(*f)(A1,A2) )
  {
    meta_cfun_<R(*)(A1,A2)> tmp(f);
    return tmp;
  }



  /*! \class xtd::meta_cfun_< R_(*)(A1_,A2_,A3_) >
  **
  ** FIXME: doc
  */

  template <typename R_, typename A1_, typename A2_, typename A3_,
	    typename A1, typename A2, typename A3>
  struct res_< meta_cfun_<R_(*)(A1_,A2_,A3_)>,
	       A1, A2, A3 >
  {
    typedef R_ ret;
  };

  template <typename R_, typename A1_, typename A2_, typename A3_>
  struct meta_cfun_ < R_(*)(A1_,A2_,A3_) >

    : public abstract::meta_nary_fun_< 3, meta_cfun_<R_(*)(A1_,A2_,A3_)> >
  {
    typedef R_(*cfun_type)(A1_,A2_,A3_);
    const cfun_type f;

    meta_cfun_(const cfun_type& f)
      : f(f)
    {} 

    template <typename A1, typename A2, typename A3>
    R_ impl_calc(const A1& a1, const A2& a2, const A3& a3) const
    {
      // FIXME: check that conversions "Ai -> Ai_" exist
      return this->f(a1, a2, a3);
    }
  };

  template <typename R, typename A1, typename A2, typename A3>
  meta_cfun_<R(*)(A1,A2,A3)> mk_fun( R(*f)(A1,A2,A3) )
  {
    meta_cfun_<R(*)(A1,A2,A3)> tmp(f);
    return tmp;
  }



} // end of namespace xtd



#endif // ! EXTENDED_CFUN_HH
