// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_FUN_SPE_UNARY_HH
# define MLN_FUN_SPE_UNARY_HH

# include <mln/core/concept/function.hh>
# include <mln/trait/next/solve.hh>
# include <mln/trait/functions.hh>

namespace mln
{

  namespace fun
  {

    namespace spe
    {

      // Wrapper for bijective functions
      template <typename Fun, typename T>
      struct lwrapper
      {
	typedef typename Fun::result   result;
	typedef typename Fun::argument argument;
	typedef typename Fun::lvalue   lvalue;
	typedef lwrapper               lresult;

	lwrapper(const Fun& f, T& x)
	: x_(&x), f_(&f)
	{
	}

	result to_result() const
	{
	  return (*f_)(*const_cast<const T*>(x_));
	};

	operator result() const
	{
	  return to_result();
	};

	const result& operator = (const result& r) const
	{
	  argument x(*x_);
	  f_->set(x, r);
	  *x_ = x;

	  return r;
	}

	private:
	  T *x_;
	  const Fun *f_;
      };

      template <typename Fun>
      struct lwrapper<Fun, typename Fun::argument>
      {
	typedef typename Fun::result   result;
	typedef typename Fun::argument argument;
	typedef typename Fun::lvalue   lvalue;
	typedef lwrapper               lresult;

	lwrapper(const Fun& f, argument& x)
	: x_(&x), f_(&f)
	{
	}

	result to_result() const
	{
	  return (*f_)(*const_cast<const argument*>(x_));
	};

	operator result() const
	{
	  return to_result();
	};

	const result& operator = (const result& r) const
	{
	  f_->set(*x_, r);

	  return r;
	}

	private:
	  argument *x_;
	  const Fun *f_;
      };

      template <typename Fun, typename Any, typename T>
      struct lwrapper<Fun, lwrapper<Any, T> >
      {
	typedef typename Fun::result   result;
	typedef typename Fun::argument argument;
	typedef typename Fun::lvalue   lvalue;
	typedef lwrapper               lresult;

	lwrapper(const Fun& f, const lwrapper<Any, T>& m)
	: m_(m), f_(&f)
	{
	}

	result to_result() const
	{
	  return (*f_)(m_.to_result());
	};

	operator result() const
	{
	  return to_result();
	};

	const result& operator = (const result& r) const
	{
	  argument m(m_);
	  f_->set(m, r);
	  m_ = m;

	  return r;
	}

	private:
	  const lwrapper<Any, T> m_;
	  const Fun *f_;
      };

      template <typename Fun, typename T>
      struct unary;

      namespace impl
      {

	template <bool param, bool set, typename Fun, typename T>
	struct unary_impl;

	template <typename Fun, typename T>
	struct unary_impl<false, false, Fun, T> : Function_v2v< unary<Fun, T> >
	{
	  typedef Fun flag;
	  typedef mln_trait_nunary(Fun, T) def;

	  typedef typename def::argument argument;
	  typedef typename def::result   result;

	  result operator () (const argument& value) const
	  {
	    return def::read(value);
	  }

	  template <typename U>
	  void init(const U& value)
	  {
	    (void) value;
	  };

	};

	template <typename Fun, typename T>
	struct unary_impl<false, true, Fun, T> : unary_impl<false, false, Fun, T>
	{
	  typedef unary_impl<false, false, Fun, T> super;
	  typedef typename super::def::lvalue lvalue;

	  template <typename U>
	  struct lresult_with
	  {
	    typedef mln::fun::spe::lwrapper< unary<Fun, T>, U> ret;
	  };

	  typedef typename lresult_with<typename super::argument>::ret lresult;

	  void set(lvalue l, const typename super::result& r) const
	  {
	    super::def::write(l, r);
	  }

	  using super::operator ();

	  lresult apply_rw(typename super::argument& value) const
	  {
	    return lresult(exact(*this), value);
	  }

	  template <typename U>
	  typename lresult_with<U>::ret apply_rw(U& value) const
	  {
	    return typename lresult_with<U>::ret(exact(*this), value);
	  }

	  lresult operator () (typename super::argument& value) const
	  {
	    return apply_rw(value);
	  }
	};

	template <typename Fun, typename T>
	struct unary_impl<true, false, Fun, T> : Function_v2v< unary<Fun, T> >
	{
	  typedef Fun flag;
	  typedef mln_trait_nunary(Fun, T) def;

	  typedef typename def::argument argument;
	  typedef typename def::result   result;

	  typedef mln_trait_fun_param(unary_impl) param;
	  typedef mln_trait_fun_storage(unary_impl) storage;

	  result operator () (const argument& value) const
	  {
	    return def::read(this->state_, value);
	  }

	  template <typename U>
	  void init(const U& value)
	  {
	    state_ = mln::trait::function::internal::introspect::has_storage_t<def, void>::compute(value);
	  };

	  stored<storage>& state()
	  {
	    return state_;
	  }

	  const stored<storage>& state() const
	  {
	    return state_;
	  }

	protected:
	  stored<storage> state_;
	};

	template <typename Fun, typename T>
	struct unary_impl<true, true, Fun, T> : unary_impl<true, false, Fun, T>
	{
	  typedef unary_impl<true, false, Fun, T> super;
	  typedef typename super::def::lvalue lvalue;

	  template <typename U>
	  struct lresult_with
	  {
	    typedef mln::fun::spe::lwrapper< unary<Fun, T>, U> ret;
	  };

	  typedef typename lresult_with<typename super::argument>::ret lresult;

	  void set(lvalue l, const typename super::result& r) const
	  {
	    super::def::write(this->state(), l, r);
	  }

	  using super::operator ();

	  lresult apply_rw(typename super::argument& value) const
	  {
	    return lresult(exact(*this), value);
	  }

	  template <typename U>
	  typename lresult_with<U>::ret apply_rw(U& value) const
	  {
	    return typename lresult_with<U>::ret(exact(*this), value);
	  }

	  lresult operator () (typename super::argument& value) const
	  {
	    return apply_rw(value);
	  }
	};

      } // end of namespace mln::fun::spe::impl

      template <typename Fun, typename T>
      struct unary
      : impl::unary_impl<mlc_or(mln_trait_fun_is_parametrable(mln_trait_nunary(Fun, T)), mln_trait_fun_is_parametrable(Fun))::value,
			 mln_trait_fun_is_assignable_(mln_trait_nunary(Fun, T))::value, Fun, T>
      {
	typedef mln_trait_nunary(Fun, T) def;
	typedef impl::unary_impl<mlc_or(mln_trait_fun_is_parametrable(def), mln_trait_fun_is_parametrable(Fun))::value,
				 mln_trait_fun_is_assignable_(def)::value,
				 Fun,
				 T>
		super;

	unary() {}

	template <typename U>
	unary(const U& param)
	{
	  this->init(param);
	}

	using super::operator();
      };

    } // end of namespace mln::fun::spe

  } // end of namespace mln::fun

} // end of namespace mln

template <typename F, typename T>
std::ostream& operator << (std::ostream& o, const mln::fun::spe::lwrapper<F, T>& m)
{
  return o << m.to_result();
}

#endif // ! MLN_FUN_SPE_UNARY_HH
