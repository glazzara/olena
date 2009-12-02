// Test de l'opérateur de Millet RGB-64
// Composante RGB sur 2 bits, puis histogramme de 64 composantes
// L'image est la résultante

#include <iostream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/image/image2d.hh>

#include <mln/data/compute.hh>
#include <mln/data/transform.hh>

#include <mln/debug/println.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/img_path.hh>

#include <mln/io/ppm/load.hh>

#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>

int main()
{
  typedef mln::fun::v2v::rgb8_to_rgbn<2>     t_rgb8_to_rgb2;
  typedef mln::value::rgb8                   t_rgb8;
  typedef mln::value::rgb<2>                 t_rgb2;
  typedef mln::image2d<t_rgb8>               t_image2d_rgb8;
  typedef mln::image2d<t_rgb2>               t_image2d_rgb2;
  typedef mln::image3d<unsigned>             t_histo3d;
  typedef mln::accu::meta::stat::histo3d_rgb t_histo3d_fun;

  t_image2d_rgb8                             input_rgb8;
  t_image2d_rgb2                             input_rgb2;
  t_histo3d                                  histo;

  // IMAGE LOADING PHASE
  std::cout << "Image loading phase ..." << std::endl;
  mln::io::ppm::load(input_rgb8, SCRIBO_PPM_IMG_PATH"/mp00082c_50p.ppm");
  input_rgb2 = mln::data::transform(input_rgb8, t_rgb8_to_rgb2());
  histo      = mln::data::compute(t_histo3d_fun(), input_rgb2);

  // PRINTING PHASE
  mln::debug::println(histo);
}
