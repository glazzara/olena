/*!
 * \file   compute_histo_rgb.cc
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <iostream>
#include <mln/trait/value/comp.hh>

namespace mln
{
  namespace histo
  {

    template <typename C, typename T>
    image3d<C> compute_histo_rgb(image2d<T> ima)
    {
      typedef mln_trait_value_comp(T, 0)::enc enc_0; // R -> int_u8
      typedef mln_trait_value_comp(T, 1)::enc enc_1; // G -> int_u8
      typedef mln_trait_value_comp(T, 2)::enc enc_2; // B -> int_u8

      image3d<unsigned> out(box3d(point3d(mln_min(enc_0),    // -> 0
					  mln_min(enc_1),    // -> 0
					  mln_min(enc_2)),   // -> 0
				  point3d(mln_max(enc_0),    // -> 255
					  mln_max(enc_1),    // -> 255
					  mln_max(enc_2)))); // -> 255
      data::fill(out, 0);

      // count
      mln_fwd_piter(image2d<T>) p(ima.domain());
      for_all(p)
	++out(point3d(ima(p).red(), ima(p).green(), ima(p).blue()));

      // return
      return out;
    }

  }
}
