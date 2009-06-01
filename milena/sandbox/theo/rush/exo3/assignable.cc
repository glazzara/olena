#include <mln/core/concept/all.hh>
#include <mln/fun/all.hh>
#include <mln/trait/all.hh>
#include <mln/fun/meta/green.hh>
#include <mln/fun/v2w2v/cos.hh>
#include <mln/fun/v2w_w2v/norm.hh>

// Main definition, use solve_unary to find "implementation"
// Usage: assignable< mln::fun::v2w2v::cos<double > cos;
template <typename F>
struct assignable : mln::trait::solve_unary<assignable, F>::ret
{
};

// Meta for assignable
// Usage: meta::assignable< mln::fun::v2w2v::cos > meta_cos;
namespace meta
{
  template <template <class> class T>
  struct assignable
  {
    template <typename V>
    struct with
    {
      typedef ::assignable< T<V> > ret;
    };

    template <typename V>
    typename with<V>::ret::result
    operator() (const V& value) const
    {
      return typename with<V>::ret()(value);
    }

    template <typename V>
    typename with<V>::ret::lresult
    operator() (V& value) const
    {
      return typename with<V>::ret()(value);
    }
  };

  template <template <class, class> class T>
  struct assignable_bin
  {
    template <typename V, typename R>
    struct with
    {
      typedef ::assignable< T<V, R> > ret;
    };
  };
};

// Tricks to play with templates
namespace tricky
{
  // Disgusting: used to subsitute only according to certain condition (sfinae)
  template <typename C1,        typename C2 = void,
	    typename C3 = void, typename C4 = void>
  struct subst_test
  {
    typedef void ret;
  };

  // Reinstantiate a template with another parameter
  template <typename Tmpl, typename E>
  struct reinstantiate;

  template <template <class> class Tmpl, typename T, typename E>
  struct reinstantiate<Tmpl<T>, E>
  {
    typedef Tmpl<E> ret;
  };

  template <typename T, template <class> class>
  struct if_is_templated;

  template <template <class> class Tmpl, typename M>
  struct if_is_templated< Tmpl<M>, Tmpl >
  {
  };
}

// Internal namespace for assignable functor
namespace internal
{
  namespace assignable
  {
    //
    template <typename F, typename X, typename V>
    void
    compute_inv(const mln::Function_v2v<F>& f, X& x, const V& v)
    {
      x = mln::exact(f).f_1(v, x);
    };

    template <typename F, typename X, typename V>
    void
    compute_inv(const mln::Function_v2v<F>& f, X& x, const V& v)
    {
      x = mln::exact(f).f_1(v);
    };

    template <typename F, typename X>
    struct modifier
    {
      modifier(X& x, const F& f)
	: x_(&x), f_(&f)
      {
      }

      operator const X& () const
      { return *x_; };

      operator X& ()
      { return *x_; };

      template <typename V>
      const V& operator= (const V& v)
      {
	compute_inv(*f_, *x_, v);
	return v;
      }

    private:
      X *x_;
      const F* f_;
    };

    // Solve modifiable type: do we need a modifier or can we directly set thru lvalue ?
    template <typename F, typename V>
    struct solve_value
    {
      typedef typename F::result ret;
    };

    template <typename F>
    struct solve_value<F, typename tricky::subst_test< typename F::value >::ret >
    {
      typedef typename F::value ret;
    };

    // Solve modifiable type: do we need a modifier or can we directly set thru lvalue ?
    template <typename F, typename Dummy>
    struct solve_lresult
    {
      typedef modifier<F, typename F::result> ret;

      template <typename V>
      static inline
      ret
      modify(V& value, const F& thru)
      {
	return ret(value, thru);
      }
    };

    template <typename F>
    struct solve_lresult<F, typename tricky::subst_test< tricky::if_is_templated<typename F::F, mln::function> >::ret >
    {
      typedef typename F::lresult ret;

      template <typename X>
      static inline
      ret
      modify(X& value, const F& thru)
      {
	(void)thru;
	return mln::function<F>().write(value);
      }
    };

    template <typename F>
    struct function_assign : public tricky::reinstantiate<typename F::category, function_assign<F> >::ret
    {
      typedef F function;
      typedef typename function::result result;
      typedef typename solve_value<function, void>::ret value;
      typedef solve_lresult<function, void> lresult_solve;
      typedef typename lresult_solve::ret lresult;

      inline
      result
      operator() (const value& t) const
      {
	return f_.read(t);
      }

      inline
      lresult
      operator() (value& t)
      {
	return lresult_solve::modify(t, f_);
      }

    protected:
      F f_;
    };
  }
}

namespace mln
{

  namespace trait
  {

    template <typename F>
    struct set_unary_< ::assignable, Function_v2v, F >
    {
      typedef ::internal::assignable::function_assign<F> ret;
    };

    template <typename F>
    struct set_unary_< ::assignable, Function_v2v, F >
    {
      typedef ::internal::assignable::function_assign<F> ret;
    };

  };

};

#define print_dbg(expr) std::cout << #expr "\t\t == " << (expr) << std::endl
int main()
{
  mln::value::rgb8 p;
  double x;

  assignable< mln::fun::v2w2v::cos<double> > cos;
  assignable< mln::meta::red<mln::value::rgb8> > red;
  assignable< mln::meta::green<mln::value::rgb8> > green;
  assignable< mln::meta::blue<mln::value::rgb8> > blue;

  meta::assignable< mln::fun::v2w2v::cos > meta_cos;
  meta::assignable< mln::meta::green > meta_green;

  print_dbg(cos(x) = 0);
  print_dbg(x);
  print_dbg(meta_cos(x) = 1);
  print_dbg(x);

  print_dbg(red(p) = 0);
  print_dbg(green(p) = 127);
  print_dbg(blue(p) = 255);
  print_dbg(p);

  print_dbg(meta_green(p) = 0);
  print_dbg(p);

};