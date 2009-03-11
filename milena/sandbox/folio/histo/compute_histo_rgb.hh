/*!
 * \file   compute_histo_rgb.cc
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

namespace mln
{
  namespace histo
  {

    template <typename C, typename T>
    image3d<C> compute_histo_rgb(image2d<T> ima)
    {
      // out
      image3d<C> out(mln_max(C),
		     mln_max(C),
		     mln_max(C));
      data::fill(out, mln_min(C));

      // count
      mln_fwd_piter(image2d<T>) p(ima.domain());
      for_all(p)
	++out(point3d(ima(p).red(), ima(p).green(), ima(p).blue()));

      // return
      return out;
    }

  }
}
