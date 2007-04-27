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

#ifndef OLN_CORE_GEN_TERNARY_FUN_HH
# define OLN_CORE_GEN_TERNARY_FUN_HH

# include <oln/core/concept/function.hh>



namespace oln
{


  namespace internal
  {

    template <bool b, typename B, typename T, typename F>
    struct ternary_base_;

    template <typename B, typename T, typename F>
    struct ternary_base_< true, B,T,F > : public Function_p2b< ternary_fun_<B,T,F> >
    {
    };

    template <typename B, typename T, typename F>
    struct ternary_base_< false, B,T,F > : public Function_p2v< ternary_fun_<B,T,F> >
    {
    };

  } // end of namespace oln::internal



  template <typename B, typename T, typename F>
  class ternary_fun_
    :
    public internal::ternary_base_< mlc_is_a(oln_result(T), bool), B,T,F >
  {
  public:
    typedef oln_argument(B) argument;
    typedef oln_result(T)   result; // FIXME: oln_promote_trait( oln_result(T), oln_result(F) )?

    ternary_fun_(const B& b, const T& t, const F& f);

    result operator()(argument arg) const;

  protected:
    B b_;
    T t_;
    F f_;
  };

  
  template <typename B, typename T, typename F>
  ternary_fun_<B,T,F> ternary_fun(const Function_p2b<B>& b,
				  const Function_p2v<T>& t, const Function_p2v<F>& f);



# ifndef OLN_INCLUDE_ONLY

  // ternary_fun_<B,T,F>

  template <typename B, typename T, typename F>
  ternary_fun_<B,T,F>::ternary_fun_(const B& b, const T& t, const F& f)
    : b_(b),
      t_(t),
      f_(f)
  {
  }

  template <typename B, typename T, typename F>
  typename ternary_fun_<B,T,F>::result
  ternary_fun_<B,T,F>::operator()(argument arg) const
  {
    return this->b_(arg) ? this->t_(arg) : this->f_(arg);
  }

  // ternary_fun

  template <typename B, typename T, typename F>
  ternary_fun_<B,T,F>
  ternary_fun(const Function_p2b<B>& b,
	      const Function_p2v<T>& t, const Function_p2v<F>& f)
  {
    // FIXME: Add static asserts here!
    ternary_fun_<B,T,F> tmp(exact(b), exact(t), exact(f));
    return tmp;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_TERNARY_FUN_HH
