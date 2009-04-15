/*!
 * \file   compute_histo_3d.cc
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/trait/value/comp.hh>

namespace mln
{
  namespace histo
  {

    template <typename C, typename T>
    image3d<C> compute_histo_3d(image2d<T> ima)
    {
      // out
      typedef mln_trait_value_comp(T, 0)::enc enc_0;
      typedef mln_trait_value_comp(T, 1)::enc enc_1;
      typedef mln_trait_value_comp(T, 2)::enc enc_2;
      image3d<C> out(mln_max(enc_0) + abs(mln_min(enc_0)) + 1,
		     mln_max(enc_1) + abs(mln_min(enc_1)) + 1,
		     mln_max(enc_2) + abs(mln_min(enc_2)) + 1);
      data::fill(out, mln_min(C));

      // count
      mln_fwd_piter(box2d) p(ima.domain());
      for_all(p)
	// FIXME: call macro comp()?
	++out(point3d(ima(p).comp(0), ima(p).comp(1), ima(p).comp(2)));

      // return
      return out;
    }

  }
}
