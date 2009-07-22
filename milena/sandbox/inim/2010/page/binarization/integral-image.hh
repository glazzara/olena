#ifndef INTEGRAL_IMAGE_HH
# define INTEGRAL_IMAGE_HH

# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/int_u32.hh>
# include <functors.hh>

namespace binarization
{
  using namespace mln;

  template< typename T >
  class IntegralImage
  {
    public:
      template<class F>
      IntegralImage(const image2d<T>& i, F func);
      ~IntegralImage();

//      const unsigned long long& operator()(int row, int col) const;

      unsigned long long operator()(int row, int col) const;

    private:
      unsigned long long **img_;
      int nrows_;
      int ncols_;
  };
}

# include "integral-image.hxx"

#endif /* !INTEGRAL_IMAGE_HH */
