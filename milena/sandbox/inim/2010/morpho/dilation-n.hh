#include <mln/core/concept/image.hh>

namespace mln
{

  namespace morpho
  {

    template <typename I, typename W>
    inline
    I
    dilation_n(const Image<I>& ima_,
	       const W& win,
	       const unsigned lambda)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      I out;
      initialize(out, ima);

      mln_piter(I) p(ima.domain());
      mln_qiter(W) q(win, p);

      for_all(p)
      {
	unsigned i = 0;
	for_all(q)
	  if (ima.has(q) && ima(q) != mln_min(mln_value(I)))
	    ++i;

	out(p) = (i >= lambda)
	  ? mln_max(mln_value(I))
	  : ima(p);
      }

      return out;
    }

  }
}
