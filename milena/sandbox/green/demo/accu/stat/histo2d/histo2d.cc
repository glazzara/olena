// DEMO HISTO2D

#include <mln/clustering/kmean2d.hh>

#include <iostream>
#include <sstream>

#include <mln/img_path.hh>
#include <mln/pw/value.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/value_array.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb8_to_rgbn.hh>
#include <mln/fun/v2v/component.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/value/rg.hh>
#include <mln/fun/v2v/rgb_to_rg.hh>
#include <mln/fun/v2v/rg_to_rgb.hh>

#include <mln/accu/stat/histo2d.hh>
#include <mln/data/compute.hh>

#include <mln/arith/revert.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/elementary/opening.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/initialize.hh>
#include <mln/literal/colors.hh>
#include <mln/morpho/watershed/flooding.hh>


//
// Test kmean image processing chain.
// Ok pour la convergence.
// demander à Théo pour le temps (45 sec pour chaque launching)
//

std::string get_file_name(std::string fqn)
{
  size_t found = fqn.find_last_of("/\\");

  return fqn.substr(found+1);
}

void do_demo(const std::string& image)
{
  typedef mln::value::rg<8>                           t_rg8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rg8>                         t_image2d_rg8;
  typedef mln::fun::v2v::rgb_to_rg<8>                 t_rgb_to_rg;
  typedef mln::fun::v2v::rg_to_rgb<8>                 t_rg_to_rgb;
  typedef mln::image2d<unsigned>                      t_image2d_histo;

  t_image2d_rgb8                                      in_rgb8;
  t_image2d_rg8                                       in_rg8;
  t_image2d_histo                                     out;

  mln::io::ppm::load(in_rgb8, image.c_str());

  in_rg8 = mln::data::transform(in_rgb8, t_rgb_to_rg());
  out    = mln::data::compute(mln::accu::meta::stat::histo2d(), in_rg8);

  std::string saving_file_name = get_file_name(image);

  saving_file_name += ".histo.sh";
  mln::io::plot::save_image_sh(out, saving_file_name);
}


int main()
{
  std::string array[] = {SCRIBO_PPM_IMG_PATH"/mp00032c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00042c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00076c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00082c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00142c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00215c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00228c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00234c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00248c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00252c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00253c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00255c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00259c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00262c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00263c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00271c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00290c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00293c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00304c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00307c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00311c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00319c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00329c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00376c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00319c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00411c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00419c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00440c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00447c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00498c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00510c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00550c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00573c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00589c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00592c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00597c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00599c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00608c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00630c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/mp00631c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00028c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00031c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00034c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00036c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00037c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00039c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00040c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00043c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00046c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00049c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00055c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00057c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00063c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00065c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00068c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00072c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00073c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00081c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00083c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00089c_50p.ppm",
			 SCRIBO_PPM_IMG_PATH"/ta00090c_50p.ppm"};

  for (unsigned i = 0; i < 61; ++i)

  do_demo(array[i]);

  return 0;
}
