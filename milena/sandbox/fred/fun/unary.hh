#ifndef UNARY_HH
# define UNARY_HH

# include <mln/trait/solve.hh>
# include <mln/fun/essential.hh>
# include <mln/fun/internal/resolve.hh>
# include "meta_function.hh"

namespace mln
{
  // UNARY
  namespace fun
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
	
	// FIXME: argument or lvalue? ~~~
	unary_modifier(argument& x)
	: x_(&x)
	{
	}
	
	result to_result() const
	{
	  return Impl::read(*x_);
	};
	
	operator result() const
	{
	  return to_result();
	};

	const result& operator = (const result& r) const
	{
	  Impl::write(*x_, r);
	  return r;
	}
	
      private:
	argument *x_;
      };
    }

    template <template <class> class Fun, typename T>
    struct unary : mln::Function_v2v< Fun<T> >
    {
      // FIXME: mln_fun_internal_resolve? Fun<T> is not defined at this point...
      //	so mln_is_a(Fun<T>, Function) won't work.
      typedef typename mln::trait::solve_unary< Fun, T >::ret impl;
      
      typedef typename impl::result          result;
      typedef typename impl::argument        argument;
      typedef typename impl::lvalue          lvalue;
      typedef internal::unary_modifier<impl> lresult;
      
      result operator () (const argument& value) const
      {
	return impl::read(value);
      }
      
      lresult operator () (argument& value) const
      {
	return lresult(value);
      }
      
      void set(lvalue l, const result& r) const
      {
	impl::write(l, r);
      }
    };
  }

  namespace trait
  {
    
    namespace fun
    {

      /// Find correct implementation
      template <typename F>
      struct is_assignable
      {
	typedef metal::true_ ret;
      };
      
    }
    
  }
  
  namespace meta
  {
    template <template <class> class F>
    struct unary : mln::Meta_Function< unary<F> >
    {
      template <typename T>
      typename F<T>::result operator()(const T& v) const
      {
	F<T> tmp;
	return tmp(v);
      }
      
      template <typename T>
      typename F<T>::lresult operator()(T& v) const
      {
	F<T> tmp;
	return tmp(v);
      }
      
      template <typename T>
      struct with
      {
	typedef F<T> ret;
      };
    };    
  }
  
}

template <typename Impl>
std::ostream& operator << (std::ostream& o, const mln::fun::internal::unary_modifier<Impl>& m)
{
  return o << m.to_result();
}

#endif /* ! UNARY_HH */
