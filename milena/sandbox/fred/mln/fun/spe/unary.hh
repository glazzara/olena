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

#ifndef MLN_FUN_SPE_UNARY_HH
# define MLN_FUN_SPE_UNARY_HH

# include <mln/core/concept/function.hh>
# include <mln/trait/next/solve.hh>
# include <mln/trait/fun.hh>

namespace mln
{

  namespace fun
  {

    namespace spe
    {

      namespace internal
      {

	template <typename Impl>
	struct unary_modifier
	{
	  typedef typename Impl::result   result;
	  typedef typename Impl::argument argument;
	  typedef typename Impl::lvalue   lvalue;
	  typedef unary_modifier          lresult;

	  // FIXME: "argument& x" or "lvalue x" directly? ~~~
	  unary_modifier(const Impl& impl, argument& x)
	  : x_(&x), impl_(&impl)
	  {
	  }

	  result to_result() const
	  {
	    return impl_->read(*x_);
	  };

	  operator result() const
	  {
	    return to_result();
	  };

	  const result& operator = (const result& r) const
	  {
	    impl_->write(*x_, r);
	    return r;
	  }

	private:
	  argument *x_;
	  const Impl *impl_;
	};

      } // end of namespace mln::fun::internal

      // Forward declaration (defined in mln/fun/unary.hh)
      template <typename Fun, typename T>
      struct unary;

      namespace impl
      {

	template <bool set, typename Fun, typename T>
	struct unary_impl_set;

	template <typename Fun, typename T>
	struct unary_impl_set<false, Fun, T> : mln::Function_v2v< unary<Fun, T> >
	{
	  typedef mln_trait_nunary(Fun, T) impl;

	  typedef typename impl::argument argument;
	  typedef typename impl::result   result;
	  typedef mln_trait_fun_param(impl) param_;
	  typedef mlc_if(mlc_equal(param_, void), impl, param_) init_param;

	  unary_impl_set() {}

	  unary_impl_set(const init_param& p) : impl_(p) {}

	  result operator () (const argument& value) const
	  {
	    return this->impl_.read(value);
	  }

	protected:
	  impl impl_;
	};

	template <typename Fun, typename T>
	struct unary_impl_set<true, Fun, T> : unary_impl_set<false, Fun, T>
	{
	  typedef unary_impl_set<false, Fun, T> super;
	  typedef typename super::impl      impl;

	  typedef typename impl::argument        argument;
	  typedef typename impl::result          result;
	  typedef typename impl::lvalue          lvalue;
	  typedef mln::fun::spe::internal::unary_modifier<impl> lresult;

	  unary_impl_set() {}
	  unary_impl_set(const typename super::init_param& p) : super(p) {}

	  void set(lvalue l, const result& r) const
	  {
	    this->impl_.write(l, r);
	  }

	  lresult operator () (argument& value) const
	  {
	    return lresult(this->impl_, value);
	  }

	  using super::operator();
	};

	template <bool set, typename Fun, typename T>
	struct unary_impl_param;

	template <typename Fun, typename T>
	struct unary_impl_param<false, Fun, T>
	: impl::unary_impl_set<mln_trait_fun_is_assignable_(mln_trait_nunary(Fun, T))::value, Fun, T>
	{
	  typedef impl::unary_impl_set<mln_trait_fun_is_assignable_(mln_trait_nunary(Fun, T))::value, Fun, T> super;

	  unary_impl_param() {}
	  unary_impl_param(const typename super::init_param& p) : super(p) {}
	};

	template <typename Fun, typename T>
	struct unary_impl_param<true, Fun, T>
	: unary_impl_param<false, Fun, T>
	{
	  typedef unary_impl_param<false, Fun, T> super;

	  unary_impl_param() {}
	  unary_impl_param(const typename super::init_param& p) : super(p) {}

	  typedef typename super::param_ param;

	  void init(const param& p)
	  {
	    this->impl_.init(p);
	  }
	};

      } // end of namespace mln::fun::spe::impl

      template <typename Fun, typename T>
      struct unary
      : impl::unary_impl_param<mln_trait_fun_is_parametrable_(mln_trait_nunary(Fun, T))::value, Fun, T>
      {
	typedef impl::unary_impl_param<mln_trait_fun_is_parametrable_(mln_trait_nunary(Fun, T))::value, Fun, T>
		super;

	unary() {}
	unary(const typename super::init_param& p) : super(p) {}

	using super::operator();
      };

    } // end of namespace mln::fun::spe

  } // end of namespace mln::fun

  template <typename Impl>
  std::ostream& operator << (std::ostream& o, const mln::fun::spe::internal::unary_modifier<Impl>& m)
  {
    return o << m.to_result();
  }

} // end of namespace mln

#endif /* ! UNARY_HH */
