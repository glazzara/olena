#include <mln/core/image/image2d.hh>
#include <mln/core/macros.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/dpoint2d.hh>
#include <mln/core/var.hh>

namespace mln
{

  template <typename P>
  class translate_t : public Function_p2p< translate_t<P> >
  {
  public:
    typedef P result;
    typedef translate_t<P> inverse;

    translate_t (mln_delta(P) dp)
      : dp_ (dp) {};

    P operator() (const P& point) const
    {
      P pt = point;
      return (pt + dp_);
    };

    inverse inv () const
    {
      translate_t<P> tmp(-dp_);
      return tmp;
    };

  private:
    const mln_delta(P) dp_;
  };

  template <typename D>
  translate_t<mln_site(D)> translate (const Gdpoint<D>& delta)
  {
    translate_t<mln_site(D)> f (exact (delta));
    return f;
  }

}