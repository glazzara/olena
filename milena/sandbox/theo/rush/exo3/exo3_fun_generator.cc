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
#include <mln/level/sort_offsets.hh>
#include <mln/canvas/morpho/attribute_filter.hh>
#include <mln/canvas/morpho/internal/find_root.hh>

namespace exo3
{
  // Se code est un peu ... sale, et tente de répondre aux problèmes
  // techniques apparus lors de la présentation (bonus ?).

  // mln::trait::solve se spécialise pour des template <class T>
  // en premier paramètre.
  // Afin de transmettre plusieurs types, on passe par
  // T = couple<TExact, TAux>. On 'hérite' de la catégorie de TExact.
  // Utiliser pour le générateur de (méta)fonctions.
  template <typename TExact, typename TAux>
  struct couple
  {
    typedef TExact Exact;
    typedef typename mln::category<Exact>::ret category;

    typedef TAux Aux;
  };

  // Crée un type unique vide pour un type donné, évite une instantation ?!
  // (inutile probablement, dans le doute...).
  template <typename U>
  struct uniq
  {
  };

  // Get attribute from attribute "E" exact.
  template <typename E>
  typename E::result
  get_attribute(uniq<E>, const E& exact)
  {
    return exact.to_result();
  }

  // area
  template <typename I, typename E>
  unsigned get_attribute(uniq< mln::morpho::attribute::card<I> >, const E& exact)
  {
    return exact.area();
  }

  // volume
  template <typename I, typename E>
  unsigned get_attribute(uniq< mln::morpho::attribute::volume<I> >, const E& exact)
  {
    return exact.volume();
  }
}

namespace mln
{
  namespace fun
  {
    namespace v2v
    {
      template <typename T> struct from_accu_t;
      template <typename T> struct solve_from_accu;
    }

    namespace meta
    {
      template <template <class> class A>
      struct from_accu
      {
	template <typename T>
        typename fun::v2v::from_accu_t< exo3::couple<T, A<T> > >::result
	operator()(const T& t1) const
        {
	  fun::v2v::from_accu_t< exo3::couple<T, A<T> > > f;
          return f(t1);
	}

	template <typename T>
	struct with
	{
	  typedef fun::v2v::from_accu_t< exo3::couple<T, A<T> > > ret;
	};
      };
    }
  }

  namespace trait
  {
    template <typename TE, typename TA>
    struct set_unary_< fun::v2v::from_accu_t, Accumulator, exo3::couple<TE, TA> >
    {
      typedef set_unary_< fun::v2v::from_accu_t, Accumulator, exo3::couple<TE, TA> > ret;
      typedef unsigned result;

      static
      result
      exec(const TE& t1)
      {
	return exo3::get_attribute(exo3::uniq<TA>(), t1);
      }
    };

  } // mln::trait

  namespace fun
  {
    namespace v2v
    {
      template <typename T>
      struct from_accu_t;

      template <typename TE, typename TA>
      struct from_accu_t< exo3::couple<TE, TA> > : mln::Function_v2v< from_accu_t< exo3::couple<TE, TA> > >
      {
	typedef mln_fun_internal_resolve(from_accu_t) impl;
	typedef typename impl::result result;

	result
	operator()(const TE& t1) const
	{
	  return impl::exec(t1);
	}
      };

      template <typename A>
      struct solve_from_accu
      {
	// FIXME: Not sure whether inheritance is the right way...
	// But templated typedef are not currently available.
	template <typename E>
	struct ret : from_accu_t< exo3::couple< E, A > >
	{
	};
      };
    }
  }

}

namespace exo3
{
  template <typename F, typename G>
  struct functors_compose_
    : mln::Function_p2v< functors_compose_<F, G> >
  {
    typedef typename F::result result;

    functors_compose_() {}
    functors_compose_(const F& f, const G& g)
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

}

namespace mln
{

  namespace trait
  {

    template <typename TG, typename TA>
    struct set_unary_<fun::v2v::from_accu_t, mln::Function_p2v, exo3::couple<TG, TA> >
    {
      typedef set_unary_<fun::v2v::from_accu_t, mln::Function_p2v, exo3::couple<TG, TA> > ret;
      typedef exo3::functors_compose_<fun::v2v::from_accu_t< exo3::couple<typename TG::result, TA> >, TG> result;

      static
      result
      exec(const TG& g)
      {
	return exo3::functors_compose_<fun::v2v::from_accu_t< exo3::couple<typename TG::result, TA> >, TG>
	  (fun::v2v::from_accu_t<exo3::couple<typename TG::result, TA> >(), g);
      }
    };
  }

}

// Modified copy of mln::canvas::morpho::attribute_filter
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

  // Surcharger les operateurs uniquement pour les builtins...
  // l'idée est de provoquer une sfinae. J'ai pas trouvé mieux :(
  // (pas vu d'opérateurs <, >, ... pour les fonctions utilisant mln::trait::solve)

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
  operator op (const mln::Function_p2v<F>& f, const C& c)		\
  {									\
    return exact(f) op mln::pw::cst(c);					\
  }									\
									\
  template <typename F, typename C>					\
  mln_trait_op_##name(							\
    templatize(typename exo3_filter::ret_builtin< typename mln::category< C >::ret, mln::pw::cst_<C> >::ret), \
    F									\
    )									\
  operator op (const C& c, const mln::Function_p2v<F>& f)		\
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
  std::cerr << "usage: " << argv[0] << " input.pgm sharpness minvol minarea (inclusive or)"
	    << std::endl;
  abort();
}

int main(int argc, char **argv)
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  float	l_sharpness;
  unsigned l_minvol;
  unsigned l_minarea;
  I input;

  if (argc < 5)
    usage(argv);

  io::pgm::load(input, argv[1]);

  l_sharpness = atof(argv[2]);
  l_minvol = atoi(argv[3]);
  l_minarea = atoi(argv[4]);

  mln_VAR(sharp, exo3_filter::attribute_filter(input, c4(),
					       level::sort_psites_decreasing(input),
					       morpho::attribute::sharpness<I>(), l_sharpness));

  mln_VAR(s, pw::value(sharp));

  mln::fun::meta::from_accu< mln::morpho::attribute::volume > vol;
  mln::fun::meta::from_accu< mln::morpho::attribute::card > area;

  io::pbm::save((vol(s) > l_minvol || area(s) > l_minarea) | input.domain(), "out.pbm");

}