#ifndef COMPOSE_HH
# define COMPOSE_HH

# include "composition.hh"

namespace mln
{
  // Composition
  namespace fun
  {
    struct compose : binary<compose> {};

    namespace internal
    {

      template <template <class> class CatF, typename F, template <class> class CatG, typename G>
      struct compose_helper
      {
	typedef F argument1;
	typedef G argument2;

	typedef typename composition<CatF, F, CatG, G>::exact_type result;
	typedef typename result::param param;

	static result read(const F& f, const G& g)
	{
	  return result(param(f, g));
	}
      };

    }
  }

  namespace trait
  {

    namespace next
    {

      // All kinds of supported compositions (meta : unary) with (meta or not : unary or binary)
      template <typename F, typename G>
      struct set_binary_< mln::fun::compose, mln::Meta_Function_v2v, F, mln::Meta_Function_v2v, G>
      {
	typedef mln::fun::internal::compose_helper<mln::Meta_Function_v2v, F, mln::Meta_Function_v2v, G> ret;
      };

      template <typename F, typename G>
      struct set_binary_< mln::fun::compose, mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G>
      {
	typedef mln::fun::internal::compose_helper<mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G> ret;
      };

      template <typename F, typename G>
      struct set_binary_< mln::fun::compose, mln::Meta_Function_v2v, F, mln::Function_v2v, G>
      {
	typedef mln::fun::internal::compose_helper<mln::Meta_Function_v2v, F, mln::Function_v2v, G> ret;
      };

      template <typename F, typename G>
      struct set_binary_< mln::fun::compose, mln::Meta_Function_v2v, F, mln::Function_vv2v, G>
      {
	typedef mln::fun::internal::compose_helper<mln::Meta_Function_v2v, F, mln::Function_vv2v, G> ret;
      };

    }

  }

}

#endif /* ! COMPOSE_HH */