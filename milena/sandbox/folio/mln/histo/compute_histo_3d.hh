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

    template <typename T>
    struct compute_histo_3d
    {
      image3d<unsigned> operator()(const image2d<T>& ima) const;

      namespace internal
      {
      }

    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    image3d<unsigned>
    operator()(const image2d<T>& ima) const
    {
      typedef mln_trait_value_comp(T, 0)::enc enc_0; // R -> int_u8
      typedef mln_trait_value_comp(T, 1)::enc enc_1; // G -> int_u8
      typedef mln_trait_value_comp(T, 2)::enc enc_2; // B -> int_u8

      image3d<unsigned> out(box3d(point1d(mln_min(enc_0)),  // -> 0
				  point1d(mln_min(enc_1)),  // -> 0
				  point1d(mln_min(enc_2)),  // -> 0
				  point1d(mln_max(enc_0)),  // -> 255
				  point1d(mln_max(enc_1)),  // -> 255
				  point1d(mln_max(enc_2)))) // -> 255
	data::fill(out, 0);

      mln_fwd_piter(image2d<T>) p(ima.domain());
      for_all(p)
	++out(point3d(ima(p).comp(0), ima(p).comp(1), ima(p).comp(2)));

      return out;
    }

# endif // !MLN_INCLUDE_ONLY

  }
}
