#ifndef UNARY_HH
# define UNARY_HH

# include <mln/fun/essential.hh>
# include "meta_function.hh"
# include "nsolve.hh"
# include "assignability.hh"

namespace mln
{

  namespace fun
  {

    // Forward declaration, for composition with unary::operator()(Fun)
    struct compose;

    namespace internal
    {

      template <typename Impl>
      struct unary_modifier
      {
	typedef typename Impl::result   result;
	typedef typename Impl::argument argument;
	typedef typename Impl::lvalue   lvalue;
	typedef unary_modifier          lresult;

	// FIXME: argument or lvalue? ~~~
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

    namespace spe
    {

      // Forward declaration
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
	  typedef internal::unary_modifier<impl> lresult;

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

      template <typename M, typename T>
      struct unary_with_helper : unary_with_param_helper<mln_trait_fun_is_parametrable_(M)::value, M, T>
      {
	typedef unary_with_param_helper<mln_trait_fun_is_parametrable_(M)::value, M, T> super;
	typedef typename super::function function;

	static typename function::result call(const M& m, const T& x)
	{
	  function f(super::inst(m));
	  return f(x);
	}

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

    }

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

    }
  }

  template <typename Impl>
  std::ostream& operator << (std::ostream& o, const mln::fun::internal::unary_modifier<Impl>& m)
  {
    return o << m.to_result();
  }

} // end of namespace mln

#endif /* ! UNARY_HH */
