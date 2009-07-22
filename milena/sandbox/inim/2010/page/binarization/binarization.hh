#ifndef BINARIZATION_HH
# define BINARIZATION_HH

# include <algorithm>
# include <cmath>

# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/int_u32.hh>

# include <integral-image.hh>
# include <functors.hh>

namespace binarization
{
  using namespace mln;

  /**
   * Sauvola thresholding binarization
   * using Integral Images.
   *
   * \param[in] src The source image
   * \param[out] dst The destination image
   * \param[in] w Value of the window size
   */

  template<typename T1, typename T2>
  void
  sauvola_threshold(const image2d<T1>& src,
                    image2d<T2>& dst,
                    unsigned int w);
}

# include <binarization.hxx>

#endif /* !BINARIZATION_HH */
