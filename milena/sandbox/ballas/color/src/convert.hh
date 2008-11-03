/*! \file src/convert.hh
 *
 * Method that convert an rgb image 2d into gray level
 */

#ifndef SRC_CONVERT_HH
# define SRC_CONVERT_HH

# include <mln/value/int_u8.hh>
# include <mln/value/int_u16.hh>
# include <mln/value/int_s16.hh>
# include <mln/value/rgb8.hh>

# include <mln/core/image/image2d.hh>

// Convert function
void convert_to_gl(mln::image2d<mln::value::int_u16>& ima,
                   const mln::image2d<mln::value::rgb8>& data)
{
  mln_piter_(mln::image2d<mln::value::int_u16>) p(ima.domain());
  for_all(p)
    ima(p) = (int) (data(p).red() * 0.3 +
                    data(p).green() * 0.58 +
                    data(p).blue() * 0.12);
}

void convert_to_gl(mln::image2d<mln::value::int_s16>& ima,
                   const mln::image2d<mln::value::rgb8>& data)
{
  mln_piter_(mln::image2d<mln::value::int_s16>) p(ima.domain());
  for_all(p)
    ima(p) = (int) (data(p).red() * 0.3 +
                    data(p).green() * 0.58 +
                    data(p).blue() * 0.12);
}

#endif // !SRC_CONVERT_HH
