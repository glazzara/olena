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



  // -----------------------------  fun_p2b_<F>


  // Fwd decl.
  template <typename F> struct fun_p2b_;

  // Category.
  namespace internal
  {
    template <typename F>
    struct set_category_of_< fun_p2b_<F> >
    {
      typedef stc::is< Function_p2b > ret;
    };
  }

  // Class.
  template <typename F>
  struct fun_p2b_ : public Function_p2b< fun_p2b_<F> >
  {
    typedef oln_arg_of_(F) argument;
    typedef oln_res_of_(F) result;

    fun_p2b_(F f) : f_(f) {}

    result operator()(argument arg) const
    {
      return this->f_(arg);
    }

  private:
    F f_;
  };

  // Specialization.
  template <typename I>
  struct fun_p2b_< Binary_Image<I> > : public Function_p2b< fun_p2b_< Binary_Image<I> > >
  {
    typedef const oln_point(I)& argument;
    typedef       oln_rvalue(I) result;

    fun_p2b_(const Binary_Image<I>& ima)
      : ima_(exact(ima))
    {}

    result operator()(argument arg) const
    {
      return this->ima_(arg);
    }

  private:
    I ima_;
  };



  // -----------------------------  fun_v2v_<F>


  // Fwd decl.
  template <typename F> struct fun_v2v_;

  // Category.
  namespace internal
  {
    template <typename F>
    struct set_category_of_< fun_v2v_<F> >
    {
      typedef stc::is< Function_v2v > ret;
    };
  }

  // Class.
  template <typename F>
  struct fun_v2v_ : public Function_v2v< fun_v2v_<F> >
  {
    typedef oln_arg_of_(F) argument;
    typedef oln_res_of_(F) result;

    fun_v2v_(F f) : f_(f) {}

    result operator()(argument arg) const
    {
      return this->f_(arg);
    }

  private:
    F f_;
  };


  // functorize_v2v
  template <typename R, typename A>
  fun_v2v_<R (*)(A)>
  functorize_v2v(R (*f)(A))
  {
    fun_v2v_<R (*)(A)> tmp(f);
    return tmp;
  }


} // end of namespace oln


#endif // ! OLN_CORE_GEN_FUN_HH
