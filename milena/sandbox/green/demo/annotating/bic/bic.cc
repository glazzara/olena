// Test de l'opérateur de Millet BIC
// Composante RGB sur 6 bits, puis on distingue les points intérieurs des points
// extérieurs. Les points intérieurs ont la même couleur que leur 4-voisinage.
// Puis on construit les 2 histogrammes (intérieur/extérieur)
#include <iostream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/initialize.hh>
#include <mln/core/var.hh>

#include <mln/data/compute.hh>
#include <mln/data/convert.hh>
#include <mln/data/transform.hh>
#include <mln/data/fill.hh>

#include <mln/debug/println.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/img_path.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/literal/colors.hh>

#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>

int main()
{
  typedef mln::value::rgb<3>                 t_rgb3;
  typedef mln::value::rgb8                   t_rgb8;
  typedef mln::neighb2d                      t_neighb2d;
  typedef mln::image2d<t_rgb3>               t_image2d_rgb3;
  typedef mln::image2d<t_rgb8>               t_image2d_rgb8;
  typedef mln::image3d<unsigned>             t_histo3d;
  typedef mln::fun::v2v::rgb8_to_rgbn<3>     t_rgb8_to_rgb3;
  typedef mln::accu::meta::stat::histo3d_rgb t_histo3d_fun;

  t_image2d_rgb8                             input_rgb8;
  t_image2d_rgb3                             input_rgb3;
  t_image2d_rgb3                             interior_rgb3;
  t_image2d_rgb3                             exterior_rgb3;
  t_histo3d                                  histo_exterior;
  t_histo3d                                  histo_interior;

  // IMAGE LOADING PHASE
  std::cout << "Image loading phase ..." << std::endl;
  mln::io::ppm::load(input_rgb8, SCRIBO_PPM_IMG_PATH"/mp00082c_50p.ppm");
  input_rgb3 = mln::data::transform(input_rgb8, t_rgb8_to_rgb3());

  mln::initialize(interior_rgb3, input_rgb3);
  mln::initialize(exterior_rgb3, input_rgb3);

  mln::data::fill(interior_rgb3, mln::literal::black);
  mln::data::fill(exterior_rgb3, mln::literal::black);

  mln_piter_(t_image2d_rgb3) p(input_rgb3.domain());
  mln_niter_(t_neighb2d)     n(mln::c4(), p);

  for_all(p)
  {
    bool is_interior = true;

    for_all(n)
      is_interior = is_interior && (input_rgb3(p) == input_rgb3(n));

    if (is_interior)
      interior_rgb3(p) = input_rgb3(p);
    else
      exterior_rgb3(p) = input_rgb3(p);
  }

  histo_interior = mln::data::compute(t_histo3d_fun(), interior_rgb3);
  histo_exterior = mln::data::compute(t_histo3d_fun(), exterior_rgb3);

  //mln::io::plot::save_image_sh(histo, "histo.sh");

  // PRINTING PHASE
  mln::debug::println(histo_interior);
  mln::debug::println(histo_exterior);
}
