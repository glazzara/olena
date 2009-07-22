#ifndef GRAYSCALE_HH
# define GRAYSCALE_HH

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>
# include <mln/data/transform.hh>
# include <mln/literal/colors.hh>

namespace mln
{
  struct lum : Function_v2v<lum>
  {
      typedef value::int_u8 result;
      result operator()(const value::rgb8& c) const
      {
        return (c.red() + c.green() + c.blue()) / 3;
      }
  };
}

namespace binarization
{
  using namespace mln;

  void grayscale(const image2d<value::rgb8>& src,
                 image2d<value::int_u8>& dst);
}

# include <grayscale.hxx>

#endif /* !GRAYSCALE_HH */
