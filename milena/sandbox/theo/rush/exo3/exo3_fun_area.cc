#include <mln/core/image/image2d.hh>
#include <mln/core/concept/function.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>
#include <mln/value/int_u8.hh>
#include <mln/pw/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/trait/all.hh>
#include <mln/fun/internal/resolve.hh>
#include <mln/morpho/attribute/card.hh>
#include <mln/morpho/attribute/sharpness.hh>
#include <mln/data/sort_offsets.hh>
#include <mln/canvas/morpho/attribute_filter.hh>
#include <mln/canvas/morpho/internal/find_root.hh>

namespace mln
{
  namespace fun
  {
    namespace v2v
    {
      template <typename T> struct area;
    }

    /// Meta function for area calculation
    namespace meta
    {

      struct area
      {
	template <typename T>
        typename fun::v2v::area<T>::result
	operator()(const T& t1) const
        {
	  fun::v2v::area<T> f;
          return f(t1);
	}

	template <typename T>
	struct with
	{
	  typedef fun::v2v::area<T> ret;
	};
      };

    } // end of namespace mln::fun::meta

    /// Function for area calculation
    namespace v2v
    {

      template <typename T>
      struct area : mln::Function_v2v< area<T> >
      {
	typedef mln_fun_internal_resolve(area) impl;
	typedef typename impl::result result;

	result
	operator()(const T& t1) const
	{
	  return impl::exec(t1);
	}
      };

    } // end of namespace mln::fun::v2v

    /// Function_v2v composition
    namespace p2v
    {
      template <typename F, typename G>
      struct composition_t
	: mln::Function_v2v< composition_t<F, G> >
      {
	typedef typename F::result result;

	composition_t() {}
	composition_t(const F& f, const G& g)
	  : f_(f), g_(g)
	{
	}

	template <typename T>
	result
	operator()(const T& p) const
	{
	  return f_(g_(p));
	}

      protected:
	F f_;
	G g_;
      };

    } // end of namespace mln::fun::p2v

  } // end of namespace mln::fun

  /// fun::v2v::area trait for composition
  namespace trait
  {
    template <typename T>
    struct set_unary_< fun::v2v::area, Accumulator, T >
    {
      typedef set_unary_< fun::v2v::area, Accumulator, T > ret;
      typedef unsigned result;

      static
      result
      exec(const T& t1)
      {
	return t1.area();
      }
    };

    template <typename I>
    struct set_precise_unary_< fun::v2v::area, mln::morpho::attribute::card<I> >
    {
      typedef set_precise_unary_< fun::v2v::area, mln::morpho::attribute::card<I> > ret;
      typedef unsigned result;

      static
      result
      exec(const mln::morpho::attribute::card<I>& t1)
      {
	return t1.to_result();
      }
    };

    template <typename G>
    struct set_unary_<fun::v2v::area, mln::Function_v2v, G>
    {
      typedef set_unary_<fun::v2v::area, mln::Function_v2v, G> ret;
      typedef fun::p2v::composition_t<fun::v2v::area<typename G::result>, G> result;

      static
      result
      exec(const G& g)
      {
	return fun::p2v::composition_t<fun::v2v::area<typename G::result>, G>
	  (fun::v2v::area<typename G::result>(), g);
      }
    };
  }

}

// Modified copy of mln::canvas::morpho::attribute_filter
// for purpose of testing.
namespace exo3_filter
{
  using namespace mln;

  template <typename I, typename N, typename S, typename A>
  mln_ch_value(I, A)
  attribute_filter (const Image<I>& input_,
		    const Neighborhood<N>& nbh_,
		    const Site_Set<S>& s_,
		    const Accumulator<A>& a_,
		    const typename A::result& lambda)
  {
    const I& input = exact(input_);
    const N& nbh   = exact(nbh_);
    const S& s     = exact(s_);
    (void)a_; // To avoid warning at compilation

    // Local type.
    typedef mln_psite(I) P;

    mln_ch_value(I, bool) deja_vu;
    mln_ch_value(I, bool) activity;
    mln_ch_value(I, P)    parent;
    mln_ch_value(I, A)    data;

    {
      initialize(deja_vu, input);
      data::fill(deja_vu, false);
      initialize(activity, input);
      data::fill(activity, true);
      initialize(parent, input);
      initialize(data, input);
    }
    {
      mln_fwd_piter(S) p(s);
      mln_niter(N) n(nbh, p);

      for_all(p)
      {
	{
	  parent(p) = p;
	  mln::canvas::morpho::impl::take_as_init (data(p), input, p);
	}

	for_all(n)
	  if (input.domain().has(n) && deja_vu(n))
	    {
	      P r = mln::canvas::morpho::impl::generic::find_root(parent, n);
	      if (r != p)
		{
		  if (input(r) == input(p) || (activity(r) && (data(r) < lambda)))
		    {
		      data(p).take(data(r));
		      parent(r) = p;
		      if (activity(r) == false)
			activity(p) = false;
		    }
		  else
		    {
		      activity(p) = false;
		    }
		}
	    }
	deja_vu(p) = true;
      }
    }
    // Second pass.
    {
      mln_bkd_piter(S) p(s);
      for_all(p)
	if (parent(p) != p)
	  data(p) = data(parent(p));
    }
    return data;
  }
} // end of namespace exo3_filter

namespace exo3_filter
{
  // Le but est de surcharger les operateurs uniquement pour les Built_In<>...
  // l'idée est de provoquer une sfinae. J'ai pas trouvé mieux :(
  // (pas vu d'opérateurs <, >, ... pour les Function<> utilisant mln::trait::solve)

  // C is Category, R is return type
  template <typename C, typename R>
  struct ret_builtin;

  template <typename T, typename R>
  struct ret_builtin< value::Built_In< T > , R>
  {
    typedef R ret;
  };

} // end of namespace exo3_filter

#define templatize(...) __VA_ARGS__

#define op_for_cst(name, op)						\
  template <typename F, typename C>					\
  mln_trait_op_##name(							\
    F,									\
    templatize(typename exo3_filter::ret_builtin< typename mln::category< C >::ret, mln::pw::cst_<C> >::ret) \
    )									\
  operator op (const mln::Function_v2v<F>& f, const C& c)		\
  {									\
    return exact(f) op mln::pw::cst(c);					\
  }									\
									\
  template <typename F, typename C>					\
  mln_trait_op_##name(							\
    templatize(typename exo3_filter::ret_builtin< typename mln::category< C >::ret, mln::pw::cst_<C> >::ret), \
    F									\
    )									\
  operator op (const C& c, const mln::Function_v2v<F>& f)		\
  {									\
    return mln::pw::cst(c) or exact(f);					\
  }

op_for_cst(less, <)
op_for_cst(greater, >)
op_for_cst(leq, <=)
op_for_cst(geq, >=)
op_for_cst(eq,  ==)
op_for_cst(neq, !=)

#undef op_for_cst

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm sharpness minarea maxarea"
	    << std::endl;
  abort();
}

int main(int argc, char **argv)
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  float	l_sharpness;
  unsigned l_minarea;
  unsigned l_maxarea;
  I input;

  if (argc < 5)
    usage(argv);

  io::pgm::load(input, argv[1]);

  l_sharpness = atof(argv[2]);
  l_minarea = atoi(argv[3]);
  l_maxarea = atoi(argv[4]);

  mln_VAR(sharp, exo3_filter::attribute_filter(input, c4(),
					       data::sort_psites_decreasing(input),
					       morpho::attribute::sharpness<I>(), l_sharpness));

  mln_VAR(s, pw::value(sharp));

  mln::fun::meta::area area;
  io::pbm::save((area(s) > l_minarea && area(s) < l_maxarea) | input.domain(), "out.pbm");
}