/*!
 * \file   compute_histo.cc
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <mln/core/image/image.hh>
#include <mln/trait/value/comp.hh>

namespace mln
{
  namespace histo
  {

    template <typename I>
    struct compute_histo
    {
      Image<unsigned> operator()(const Image<I>& ima_) const;

      namespace internal
      {
      }

    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    ImageNd<dim, unsigned> // ? dim? imageNd?
    operator()(const Image<I>& ima_) const
    {
      I ima = exact(ima);
      typedef mln_value(I) V;

      // Create image Nd from value types of ima.
      algebra::vec<V::dim, unsigned> pmin, pmax;
      for (unsigned i = 0; i < V::dim; ++i)
      {
	typedef mln_trait_value_comp(V, i)::enc enc;
	pmin[i] = mln_min(enc);
	pmax[i] = mln_max(enc);
      }

      typedef box<site> _box; // ? site?
      _box box(pmin, pmax);
      ImageNd<V::dim, unsigned> out(box); // ? ImageNd?


      // Count occurences.
      data::fill(out, 0);

      mln_fwd_piter(box2d) p(ima.domain());
      for_all(p)
      {
	algebra::vec<V::dim, site> // ? site?
	for (unsigned i = 0; i < V::dim; ++i)
	  // comp() not implemented everywhere!
	  pt[i] = ima(p).comp(i);
	++out(pointNd(pt)); // ? pointNd?
      }

      return out;
    }

# endif // !MLN_INCLUDE_ONLY

  }
}
