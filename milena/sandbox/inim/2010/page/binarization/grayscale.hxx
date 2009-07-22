#ifndef GRAYSCALE_HXX
# define GRAYSCALE_HXX

# include <grayscale.hh>

namespace binarization
{
  void grayscale(const image2d<value::rgb8>& src,
                 image2d<value::int_u8>& dst)
  {
    dst = data::transform(src, lum());
  }
}

#endif /* !GRAYSCALE_HXX */
