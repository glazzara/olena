// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR F PARTICULAR PURPOSE.  See the GNU
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

#ifndef MLN_FUN_UNARY_HH
# define MLN_FUN_UNARY_HH

# include <mln/core/concept/meta_function.hh>
# include <mln/fun/spe/unary.hh>
# include <mln/trait/next/solve.hh>

namespace mln
{

  namespace fun
  {

    // Forward declarations, for composition with unary::operator()(Fun)
    struct compose;

    template <typename F>
    struct unary;

    namespace internal
    {

      template <typename T>
      struct unary_with {};

      template <bool has_param, typename M, typename T>
      struct unary_with_param_helper;

      template <typename M, typename T>
      struct unary_with_param_helper<false, M, T>
      {
	typedef mln::fun::spe::unary<M, T> function;

	static function inst(const M&)
	{
	  return function();
	};

      };

      template <typename M, typename T>
      struct unary_with_param_helper<true, M, T>
      {
	typedef mln::fun::spe::unary<M, T> function;

	static function inst(const M& m)
	{
	  return function(m.parameter());
	};

      };

      template <bool has_lvalue, typename M, typename T>
      struct unary_with_lvalue_helper;

      template <typename M, typename T>
      struct unary_with_lvalue_helper<false, M, T>
      : unary_with_param_helper<mln_trait_fun_is_parametrable_(M)::value, M, T>
      {
	typedef unary_with_param_helper<mln_trait_fun_is_parametrable_(M)::value, M, T> super;
	typedef typename super::function function;

	static typename function::result call(const M& m, const T& x)
	{
	  function f(super::inst(m));
	  return f(x);
	}
      };

      template <typename M, typename T>
      struct unary_with_lvalue_helper<true, M, T>
      : unary_with_lvalue_helper<false, M, T>
      {
	typedef unary_with_lvalue_helper<false, M, T> super;
	typedef typename super::function function;

	static typename function::lresult lcall(const M& m, T& x)
	{
	  function f(super::inst(m));
	  return f(x);
	}

	static void set(const M& m, typename function::lvalue v, const T& x)
	{
	  function f(super::inst(m));
	  f.set(v, x);
	}
      };

      template <typename M, typename T>
      struct unary_with_helper
      : unary_with_lvalue_helper<mln_trait_fun_is_assignable__1comma_(mln::fun::spe::unary<M, T>)::value, M, T>
      {
      };

    } // end of namespace mln::fun::internal

    template <typename F>
    struct unary: mln::Meta_Function_v2v< F >
    {

      template <typename T>
      struct with {
	typedef mln_trait_nunary(internal::unary_with<F>, T) impl;
	typedef typename impl::function ret;
      };

      template <typename T>
      typename with<T>::ret::result operator()(const T& v) const
      {
	return with<T>::impl::call(exact(*this), v);
      }

      template <typename T>
      typename with<T>::ret::lresult operator()(T& v) const
      {
	return with<T>::impl::lcall(exact(*this), v);
      }

      template <typename T, typename R>
      void set(T& v, const R& r) const
      {
	with<T>::impl::set(exact(*this), v, r);
      }

    };

    template <typename F, typename P>
    struct unary_param: unary<F>
    {
      typedef P param;

      unary_param() {};
      unary_param(const param& p) : p_(p) {};

      void init(const param& p)
      {
	p_ = p;
      }

      param& parameter()
      {
	return p_;
      }

      const param& parameter() const
      {
	return p_;
      }

    protected:
      param p_;

    };

    template <typename F>
    struct unary_param<F, F>: unary<F>
    {
      typedef F param;

      void init(const param& p)
      {
	exact(*this) = p;
      }

      param& parameter()
      {
	return exact(*this);
      }

      const param& parameter() const
      {
	return exact(*this);
      }

    };

  } // end of namespace mln::fun

  namespace trait
  {

    namespace next
    {

      // Any type
      template <typename F, typename T>
      struct set_unary_< mln::fun::internal::unary_with<F>, mln::Object, T>
      {
	typedef mln::fun::internal::unary_with_helper<F, T> ret;
      };

      // Meta Function
      template <typename F, typename G>
      struct set_unary_< mln::fun::internal::unary_with<F>, mln::Meta_Function, G>
      {
	// FIXME: Workaround for cyclic references (unary -> unary_with -> compose -> unary)
	template <typename T>
	struct identity
	{
	  typedef T ret;
	};

	typedef set_unary_ ret;
	typedef typename identity<mln::fun::compose>::ret::template with<F, G>::ret function;

	static typename function::result call(const F& f, const G& g)
	{
	  function tmp;
	  return tmp(f, g);
	}
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

} // end of namespace mln

#endif /* ! MLN_FUN_UNARY_HH */
