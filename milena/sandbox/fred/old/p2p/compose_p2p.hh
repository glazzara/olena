#include <mln/core/image/image2d.hh>
#include <mln/core/macros.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/dpoint2d.hh>
#include <mln/core/var.hh>

namespace mln
{

  template <typename P, typename F, typename G>
  class compose_t : public Function_v2v< compose_t<P, F, G> >
  {
  public:
    typedef P result;
    typedef compose_t<P, typename G::inverse, typename F::inverse> inverse;

    compose_t (const F& f, const G& g)
      : f_ (f), g_ (g) {};

    P operator() (const P& point) const
    {
      return (f_ (g_ (point)));
    };

    inverse inv () const
    {
      inverse comp(g_.inv (), f_.inv ());
      return comp;
    };

  private:
    const F f_;
    const G g_;
  };

  template <typename F, typename G>
  compose_t<typename F::result, F, G> operator* (const Function_v2v< F >& f,
				       const Function_v2v< G >& g)
  {
    compose_t<typename F::result, F, G> comp (exact (f), exact (g));
    return comp;
  }

}