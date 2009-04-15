/*!
 * \file   compute_histo.cc
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <mln/core/image/image.hh>
#include <mln/trait/value/comp.hh>

#include <mln/trait/image_from_grid.hh>

namespace mln
{
  namespace histo
  {

    template <typename C, typename I>
    Image<C> compute_histo(Image<I> ima_)
    {
      I ima = exact(ima);
      typedef mln_value(I) V;
      typedef mln_regular_grid_from_dim(V::dim + 1) G;
      typedef mln_image_from_grid(G, C) O;
      O out;

      for (unsigned i = 0; i < V::dim + 1; ++i)
      {
	typedef mln_trait_value_comp(I, i)::enc enc[i];
	// FIXME: define here the domain of `out'.
	// out(mln_max(enc[0]) + abs(mln_min(enc[0])) + 1,
	//     mln_max(enc[1]) + abs(mln_min(enc[1])) + 1,
	//     mln_max(enc[2]) + abs(mln_min(enc[2])) + 1);
      }
      data::fill(out, mln_min(C));

      // count
      mln_fwd_piter(box2d) p(ima.domain());
      for_all(p)
	// FIXME: call macro comp()?
	// FIXME: pointnd
	++out(point3d(ima(p).comp(0), ima(p).comp(1), ima(p).comp(2)));

      // return
      return out;
    }

  }
}
