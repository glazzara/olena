/*!
 * \file   classify_with_histo.hh
 * \author etiennefolio <ornthalas@gmail.com>
 */


#ifndef CLASSIFY_WITH_HISTO_HH_
# define CLASSIFY_WITH_HISTO_HH_

namespace mln
{

  namespace histo
  {

    template <typename T, typename C>
    image2d<C>
    classify_with_histo_rgb(image2d<T>& ref,
			    image3d<C>& regions)
    {
      image2d<C> out;
      initialize(out, ref);

      mln_fwd_piter(image2d<T>) p(ref.domain());
      for_all(p)
      {
	// get 3d point in regions image.
	point3d p3 = point3d(ref(p).red(), ref(p).green(), ref(p).blue());
	out(p) = regions(p3); // copy the label in out's pixel.
      }

      return out;
    }

  } // end of namespace mln::histo

} // end of namespace mln

#endif /* !CLASSIFY_WITH_HISTO_HH_ */
