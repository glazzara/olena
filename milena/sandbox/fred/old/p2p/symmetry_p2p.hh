#include <mln/core/contract.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/macros.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/dpoint2d.hh>
#include <mln/core/var.hh>

namespace mln
{

  template <typename P>
  class symmetry_t : public Function_v2v< symmetry_t<P> >
  {
  public:
    typedef P result;
    typedef symmetry_t<P> inverse;

    symmetry_t (unsigned dim)
      : dim_ (dim)
    {
      mln_assertion(dim < P::dim);
    };

    P operator() (const P& point) const
    {
      P pt = point;
      pt[dim_] = -pt[dim_];
      return pt;
    };

    inverse inv () const
    {
      return *this;
    };

  private:
    const unsigned dim_;
  };

  template <typename P>
  symmetry_t<P> flip (unsigned dim)
  {
    symmetry_t<P> f (dim);
    return f;
  }

}