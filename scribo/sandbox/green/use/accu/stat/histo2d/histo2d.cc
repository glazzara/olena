/// \file
///
/// \brief Minimal code for building 2d image histogram version.
///

#include <mln/accu/stat/histo2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/data/compute.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_rg.hh>
#include <mln/img_path.hh>
#include <mln/io/ppm/load.hh>
#include <mln/value/rg.hh>
#include <mln/value/rgb8.hh>

int main()
{
  typedef mln::value::rg<8>		t_rg8;
  typedef mln::value::rgb8		t_rgb8;
  typedef mln::fun::v2v::rgb_to_rg<8>	t_rgb_to_rg;
  typedef mln::image2d<t_rg8>		t_image2d_rg8;
  typedef mln::image2d<t_rgb8>		t_image2d_rgb8;
  typedef mln::image2d<unsigned>	t_histo;
  t_image2d_rgb8			img_rgb8;
  t_image2d_rg8				img_rg8;
  t_histo				histo;

  mln::io::ppm::load(img_rgb8, OLENA_IMG_PATH"/lena.ppm");
  img_rg8 = mln::data::transform(img_rgb8, t_rgb_to_rg());
  histo   = mln::data::compute(mln::accu::meta::stat::histo2d(), img_rg8);

  return 0;
}
