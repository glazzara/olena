#ifndef BINARY_HH
# define BINARY_HH

# include <mln/fun/essential.hh>
# include "meta_function.hh"
# include "nsolve.hh"
# include "assignability.hh"

namespace mln
{

  namespace fun
  {

    namespace spe
    {

      // Forward declaration
      template <typename Fun, typename T1, typename T2>
      struct binary;

      namespace impl
      {

	template <typename Fun, typename T1, typename T2>
	struct binary_impl : mln::Function_v2v< binary<Fun, T1, T2> >
	{
	  typedef mln_trait_nbinary(Fun, T1, T2) impl;

	  typedef typename impl::argument1 argument1;
	  typedef typename impl::argument2 argument2;
	  typedef typename impl::result    result;

	  binary_impl()
	  : impl_()
	  {
	  }

	  binary_impl(const impl& f)
	  : impl_(f)
	  {
	  }

	  result operator () (const argument1& a, const argument2& b) const
	  {
	    return this->impl_.read(a, b);
	  }

	protected:
	  impl impl_;
	};

      } // end of namespace mln::fun::spe::impl

      template <typename Fun, typename T1, typename T2>
      struct binary
      : impl::binary_impl<Fun, T1, T2>
      {
	typedef impl::binary_impl<Fun, T1, T2> super;

	binary()
	: super()
	{
	}

	binary(const typename super::impl& f)
	: super(f)
	{
	}

	using super::operator();
      };

    } // end of namespace mln::fun::spe

    template <typename F>
    struct binary : mln::Meta_Function_vv2v< binary<F> >
    {

      template <typename T1, typename T2>
      struct with
      {
	typedef spe::binary<F, T1, T2> ret;
      };

      template <typename T1, typename T2>
      typename with<T1, T2>::ret::result operator()(const T1& a, const T2& b) const
      {
	typename with<T1, T2>::ret tmp;
	return tmp(a, b);
      }

    };

    template <typename F, typename P>
    struct binary_param: binary<F>
    {
      typedef P param;

      binary_param() {};
      binary_param(const param& p) : p_(p) {};

      void init(const param& p)
      {
	p_ = p;
      }

      param& parameter()
      {
	return p_;
      }

      protected:
	param p_;
    };

    template <typename F>
    struct binary_param<F, F>: binary<F>
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
    };

  } // end of namespace mln::fun

} // end of namespace mln

#endif /* ! BINARY_HH */
