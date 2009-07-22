#ifndef INTEGRAL_IMAGE_HXX
# define INTEGRAL_IMAGE_HXX

# include "integral-image.hh"

namespace binarization
{
  template<typename T>
  template<class F>
  IntegralImage<T>::IntegralImage(const image2d<T>& i, F func)
    : img_ (NULL),
      nrows_ (i.nrows()),
      ncols_ (i.ncols())
  {
    img_ = static_cast<unsigned long long**>(malloc(sizeof (unsigned long long*) * nrows_));
    for (int n = 0; n < nrows_; ++n)
      img_[n] = static_cast<unsigned long long*>(malloc(sizeof (unsigned long long) * ncols_));

    // FIXME Overflow when super big image?
    // unsigned long long not cool for portability
    // Look for I.I. in the litterature
    img_[0][0] = func(i(point2d(0, 0)));

    for (def::coord row = 1; row < static_cast<def::coord>(i.nrows()); ++row)
      img_[row][0] = (*this)(row - 1, 0)
        + func(i(point2d(row, 0)));

    for (def::coord col = 1; col < static_cast<def::coord>(i.ncols()); ++col)
      img_[0][col] = (*this)(0, col - 1)
        + func(i(point2d(0, col)));

    for (def::coord row = 1; row < static_cast<def::coord>(i.nrows()); ++row)
      for (def::coord col = 1; col < static_cast<def::coord>(i.ncols()); ++col)
        img_[row][col] = (*this)(row - 1, col)
          + (*this)(row, col - 1)
          - (*this)(row - 1, col - 1)
          + func(i(point2d(row, col)));
  }

  template< typename T >
  IntegralImage<T>::~IntegralImage()
  {
    for (int n = 0; n < nrows_; ++n)
      free(img_[n]);
    free(img_);
  }

  template< typename T >
  unsigned long long
  IntegralImage<T>::operator()(int row,
                               int col) const
  {
    return img_[row][col];
  }

}
#endif /* !INTEGRAL_IMAGE_HXX */
