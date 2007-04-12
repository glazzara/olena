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

#ifndef OLN_CORE_GEN_FUN_HH
# define OLN_CORE_GEN_FUN_HH

# include <oln/core/internal/category_of.hh>
# include <oln/core/concept/function.hh>
# include <oln/core/concept/image.hh>
# include <oln/core/concept/point.hh>
# include <oln/core/concept/value.hh>



// FIXME: Add "fun" to names below.
# define oln_arg_of_(T) typename oln::internal::argument_of_< T >::ret
# define oln_res_of_(T) typename oln::internal::result_of_< T >::ret



namespace oln
{

  namespace internal
  {

    // result_of_

    template <typename F>
    struct result_of_
    {
      typedef typename F::result ret;
    };

    template <typename R, typename A>
    struct result_of_< R (*)(A) >
    {
      typedef R ret;
    };


    // argument_of_

    template <typename F>
    struct argument_of_
    {
      typedef typename F::argument ret;
    };

    template <typename R, typename A>
    struct argument_of_< R (*)(A) >
    {
      typedef A ret;
    };


  } // end of namespace oln::internal



  //  fun_p2b_<F>
  // -------------


  template <typename F>
  struct fun_p2b_ : public Function_p2b< fun_p2b_<F> >
  {
    typedef oln_arg_of_(F) argument;
    typedef oln_res_of_(F) result;

    fun_p2b_(const F& f);
    result operator()(argument arg) const;

  private:
    F f_;
  };
 
  template <typename B, typename P>
  fun_p2b_<B (*)(P)>
  functorize_p2b(B (*f)(P));
 

  // Specialization.

  template <typename I>
  struct fun_p2b_< Binary_Image<I> >
    :
    public Function_p2b< fun_p2b_< Binary_Image<I> > >
  {
    typedef const oln_point(I)& argument;
    typedef       oln_rvalue(I) result;

    fun_p2b_(const I& ima);
    oln_rvalue(I) operator()(const oln_point(I)& arg) const;

  private:
    I ima_;
  };

  template <typename I>
  fun_p2b_< Binary_Image<I> >
  functorize_p2b(const Binary_Image<I>& ima);



  //  fun_p2v_<F>
  // -------------


  template <typename F>
  struct fun_p2v_ : public Function_p2v< fun_p2v_<F> >
  {
    typedef oln_arg_of_(F) argument;
    typedef oln_res_of_(F) result;

    fun_p2v_(const F& f);
    result operator()(argument arg) const;

  private:
    F f_;
  };

  template <typename V, typename P>
  fun_p2v_<V (*)(P)>
  functorize_p2v(V (*f)(P));


  //  fun_v2v_<F>
  // -------------


  template <typename F>
  struct fun_v2v_ : public Function_v2v< fun_v2v_<F> >
  {
    typedef oln_arg_of_(F) argument;
    typedef oln_res_of_(F) result;

    fun_v2v_(const F& f);
    result operator()(argument arg) const;

  private:
    F f_;
  };

  template <typename Vr, typename Va>
  fun_v2v_<Vr (*)(Va)>
  functorize_v2v(Vr (*f)(Va));



# ifndef OLN_INCLUDE_ONLY

  // fun_p2b_<F>

  template <typename F>
  fun_p2b_<F>::fun_p2b_(const F& f)
    : f_(f)
  {
  }
 
  template <typename F>
  typename fun_p2b_<F>::result
  fun_p2b_<F>::operator()(typename fun_p2b_<F>::argument arg) const
  {
    return this->f_(arg);
  }

  template <typename B, typename P>
  fun_p2b_<B (*)(P)>
  functorize_p2b(B (*f)(P))
  {
    typedef mlc_basic(P) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check();
    // FIXME: Add static check about B being Boolean.
    fun_p2b_<B (*)(P)> tmp(f);
    return tmp;
  }

  template <typename I>
  fun_p2b_< Binary_Image<I> >::fun_p2b_(const I& ima)
    : ima_(ima)
  {
  }

  template <typename I>
  oln_rvalue(I)
  fun_p2b_< Binary_Image<I> >::operator()(const oln_point(I)& arg) const
  {
    return this->ima_(arg);
  }

  template <typename I>
  fun_p2b_< Binary_Image<I> >
  functorize_p2b(const Binary_Image<I>& ima)
  {
    fun_p2b_< Binary_Image<I> > tmp(exact(ima));
    return tmp;
  }

  // fun_p2v_<F>
 
  template <typename F>
  fun_p2v_<F>::fun_p2v_(const F& f)
    : f_(f)
  {
  }

  template <typename F>
  typename fun_p2v_<F>::result
  fun_p2v_<F>::operator()(typename fun_p2v_<F>::argument arg) const
  {
    return this->f_(arg);
  }

  template <typename V, typename P>
  fun_p2v_<V (*)(P)>
  functorize_p2v(V (*f)(P))
  {
    typedef mlc_basic(P) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    fun_p2v_<V (*)(P)> tmp(f);
    return tmp;
  }

  // fun_v2v_<F>
 
  template <typename F>
  fun_v2v_<F>::fun_v2v_(const F& f)
    : f_(f)
  {
  }
  
  template <typename F>
  typename fun_v2v_<F>::result
  fun_v2v_<F>::operator()(typename fun_v2v_<F>::argument arg) const
  {
    return this->f_(arg);
  }

  template <typename Vr, typename Va>
  fun_v2v_<Vr (*)(Va)>
  functorize_v2v(Vr (*f)(Va))
  {
    fun_v2v_<Vr (*)(Va)> tmp(f);
    return tmp;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_FUN_HH
