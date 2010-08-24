// Test de l'opérateur de Millet RGB-64
// Composante RGB sur 2 bits, puis histogramme de 64 composantes
// L'image est la résultante

#include <iostream>
#include <sstream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/compute.hh>
#include <mln/data/paste.hh>
#include <mln/data/transform.hh>

#include <mln/debug/println.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/img_path.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>

const char *get_name(const char *base, const unsigned i, const unsigned j)
{
  std::ostringstream name;

  name << base;
  name << i;
  name << "_";
  name << j;
  name << ".ppm";

  return name.str().c_str();
}

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

  // IMAGE SPLITTING PHASE
  mln::box2d     domain = input_rgb2.domain();
  mln::point2d   pmin   = domain.pmin();
  mln::point2d   pmax   = domain.pmax();

  unsigned       sz_row = (pmax.row() - pmin.row())/ 3;
  unsigned       sz_col = (pmax.col() - pmin.col())/ 3;

  std::cout << domain << std::endl;

  // Divide the domain in nine sub-domains.
  for (unsigned i = 0; i < 3; ++i)
    for (unsigned j = 0; j < 3; ++j)
    {
      mln::point2d min(pmin.row()+sz_row*i,pmin.col()+sz_col*j);
      mln::point2d max(pmin.row()+sz_row*(i+1),pmin.col()+sz_col*(j+1));
      mln::box2d   dom(min,max);

      std::cout << dom << std::endl;

      // Save it
      t_image2d_rgb2 input_1o9_rgb2(dom);

      mln::data::paste(input_rgb2 | dom, input_1o9_rgb2);
      mln::io::ppm::save(input_1o9_rgb2, get_name("output",i,j));

      histo      = mln::data::compute(t_histo3d_fun(), input_1o9_rgb2);

      // PRINTING PHASE
      mln::debug::println(histo);
    }

}
