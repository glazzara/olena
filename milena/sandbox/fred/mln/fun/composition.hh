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

#ifndef MLN_FUN_COMPOSITION_HH
# define MLN_FUN_COMPOSITION_HH

# include <mln/fun/unary.hh>
# include <mln/fun/binary.hh>

namespace mln
{
  // Composition
  namespace fun
  {

    namespace internal
    {

      // Compositions may take this has initialization parameter
      template <typename F, typename G>
      struct composition_param
      {
	composition_param(const F& f, const G& g) : f_(f), g_(g) {}
	composition_param() {}

	F f_;
	G g_;
      };

      // Composition types...
      template <template <class> class CatF,  typename F,
		template <class> class CatG, typename G>
      struct composition;

      // Meta
      template <typename F, typename G>
      struct composition<mln::Meta_Function_v2v, F, mln::Meta_Function_v2v, G>
      : mln::fun::unary_param< composition<mln::Meta_Function_v2v, F, mln::Meta_Function_v2v, G>, composition_param<F, G> >
      {
	typedef mln::fun::unary_param< composition<mln::Meta_Function_v2v, F, mln::Meta_Function_v2v, G>, composition_param<F, G> > super;

	composition() {};
	composition(const typename super::param& p) : super(p) {};

	typedef composition exact_type;
      };

      template <typename F, typename G>
      struct composition<mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G>
      : mln::fun::binary_param< composition<mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G>, composition_param<F, G> >
      {
	typedef mln::fun::binary_param< composition<mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G>, composition_param<F, G> > super;

	composition() {};
	composition(const typename super::param& p) : super(p) {};

	typedef composition exact_type;
      };

      // Concrete
      template <typename F, typename G>
      struct composition<mln::Meta_Function_v2v, F, mln::Function_v2v, G>
      {
	typedef mln::fun::spe::unary< composition<mln::Meta_Function_v2v, F, mln::Function_vv2v, G>, typename G::argument> exact_type;
      };

      template <typename F, typename G>
      struct composition<mln::Meta_Function_v2v, F, mln::Function_vv2v, G>
      {
	typedef mln::fun::spe::binary< composition<mln::Meta_Function_v2v, F, mln::Function_vv2v, G>,
				       typename G::argument1, typename G::argument2> exact_type;
      };

      // Unary compositions implementation inherit from composition_unary_impl...
      template <bool has_lvalue, typename F, typename F_spe, typename G, typename G_spe>
      struct composition_unary_impl_helper;

      template <typename F, typename F_spe, typename G, typename G_spe>
      struct composition_unary_impl_helper<false, F, F_spe, G, G_spe>
      {
	typedef typename G_spe::argument argument;
	typedef typename F_spe::result   result;
	typedef composition_param<F, G> param;

	composition_unary_impl_helper() {}
	composition_unary_impl_helper(const param& p) : f_(p.f_), g_(p.g_) {}

	void init(const param& p)
	{
	  f_ = p.f_;
	  g_ = p.g_;
	}

	result read(const argument& x) const
	{
	  return this->f_(this->g_(x));
	}

      protected:
	F f_;
	G g_;
      };

      template <typename F, typename F_spe, typename G, typename G_spe>
      struct composition_unary_impl_helper<true, F, F_spe, G, G_spe>
      : composition_unary_impl_helper<false, F, F_spe, G, G_spe>
      {
	typedef composition_unary_impl_helper<false, F, F_spe, G, G_spe> super;
	typedef typename G_spe::lvalue   lvalue;

	composition_unary_impl_helper() {}
	composition_unary_impl_helper(const typename super::param& p) : super(p) {}

	void write(lvalue l, const typename super::result& x) const
	{
	  typename G_spe::result r(this->g_(l));

	  this->f_.set(r, x);
	  this->g_.set(l, r);
	}
      };

      template <typename F, typename F_spe, typename G, typename G_spe>
      struct composition_unary_impl
      : composition_unary_impl_helper<mln_trait_fun_is_assignable_(G_spe)::value, F, F_spe, G, G_spe>
      {
	typedef composition_unary_impl_helper<mln_trait_fun_is_assignable_(G_spe)::value, F, F_spe, G, G_spe> super;

	composition_unary_impl() {}
	composition_unary_impl(const typename super::param& p) : super(p) {}
      };

      // Binary compositions implementation inherit from composition_binary_inherit...
      template <typename F, typename F_spe, typename G, typename G_spe>
      struct composition_binary_impl
      {
	typedef typename G_spe::argument1 argument1;
	typedef typename G_spe::argument2 argument2;
	typedef typename F_spe::result   result;
	typedef composition_param<F, G> param;

	composition_binary_impl() {}
	composition_binary_impl(const param& p) : f_(p.f_), g_(p.g_) {}

	void init(const param& p)
	{
	  f_ = p.f_;
	  g_ = p.g_;
	}

	result read(const argument1& a, const argument2& b) const
	{
	  return this->f_(this->g_(a, b));
	}

      protected:
	F f_;
	G g_;
      };

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun

  namespace trait
  {

    namespace next
    {

      template <typename F, typename G, typename T>
      struct set_precise_unary_<mln::fun::internal::composition<mln::Meta_Function_v2v, F, mln::Meta_Function_v2v, G>, T>
      {
	typedef typename G::template with<T>::ret G_fun;
	typedef typename F::template with<typename G_fun::result>::ret F_fun;

	typedef mln::fun::internal::composition_unary_impl<F, F_fun, G, G_fun> ret;
      };

      template <typename F, typename G, typename T1, typename T2>
      struct set_precise_binary_<mln::fun::internal::composition<mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G>, T1, T2>
      {
	typedef typename G::template with<T1, T2>::ret G_fun;
	typedef typename F::template with<typename G_fun::result>::ret F_fun;

	typedef mln::fun::internal::composition_binary_impl<F, F_fun, G, G_fun> ret;
      };

      template <typename F, typename G>
      struct set_precise_unary_<mln::fun::internal::composition<mln::Meta_Function_v2v, F, mln::Function_v2v, G>,
				typename G::argument>
      {
	typedef typename F::template with<typename G::result>::ret F_fun;

	typedef mln::fun::internal::composition_unary_impl<F, F_fun, G, G> ret;
      };

      template <typename F, typename G>
      struct set_precise_binary_<mln::fun::internal::composition<mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G>,
				 typename G::argument1, typename G::argument2>
      {
	typedef typename F::template with<typename G::result>::ret F_fun;

	typedef mln::fun::internal::composition_binary_impl<F, F_fun, G, G> ret;
      };

    }  // end of namespace mln::trait::next

  }  // end of namespace mln::trait

} // end of namespace mln

#endif /* ! MLN_FUN_COMPOSITION_HH */
