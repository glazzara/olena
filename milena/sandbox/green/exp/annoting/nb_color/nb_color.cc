// COUNTING THE IMAGE COLORS

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/math/count.hh>
#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/binarization/threshold.hh>

#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/compute.hh>
#include <mln/data/transform.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

// #include <mln/morpho/opening/volume.hh>

#include <mln/io/ppm/load.hh>

#include <mln/pw/value.hh>
#include <mln/pw/cst.hh>

#include <mln/util/timer.hh>

#include <mln/value/rgb8.hh>
//#include <mln/value/rgb.hh>

// FIXME
// Pour la classification des images, pour gagner du temps,
// peut être peut-on réduire toutes les images à 100x100 avec 2^5 couleurs ?

// n < 8, n is the degree of quantification
template <unsigned n>
unsigned count_image_color(const std::string& image)
{
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::image3d<bool>                          t_histo3d_bool;
  typedef mln::fun::v2v::rgb8_to_rgbn<n>              t_rgb8_to_rgbn;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
  typedef mln::accu::meta::math::count                t_count_fun;

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_rgbn                                      input_rgbn;
  t_image2d_rgbn                                      output_rgbn;
  t_histo3d                                           histo;
//   t_histo3d                                           opened;
  t_histo3d_bool                                      filtered;

  mln::io::ppm::load(input_rgb8, image.c_str());

  unsigned nb_pixel   = input_rgb8.ncols() * input_rgb8.nrows();
  unsigned min_volume = (unsigned)(nb_pixel * 0.0001);
  unsigned nb_color   = 0;

//  input_rgbn = mln::data::transform(input_rgb8, t_rgb8_to_rgbn());
  histo      = mln::data::compute(t_histo3d_fun(), input_rgb8);
//   opened     = mln::morpho::opening::volume(histo, mln::c6(), min_volume);
  filtered   = mln::binarization::threshold(histo, min_volume);
  nb_color   = mln::data::compute(t_count_fun(),
				 (filtered|(mln::pw::value(filtered)!=0)).rw());

  return nb_color;
}

int main()
{
  typedef boost::filesystem::path                   t_path;
  //typedef boost::filesystem::initial_path<t_path()> t_init_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

  t_path full_path[] = {t_path(ANNOTING_BILL_IMG_PATH),
			t_path(ANNOTING_FAX_IMG_PATH),
			t_path(ANNOTING_HANDWRITTEN_IMG_PATH),
			t_path(ANNOTING_LOGO_IMG_PATH),
			t_path(ANNOTING_MAP_IMG_PATH),
			t_path(ANNOTING_PHOTO_IMG_PATH),
			t_path(ANNOTING_SCREENSHOT_IMG_PATH),
			t_path(ANNOTING_SLIDE_IMG_PATH),
			t_path(ANNOTING_TYPED_IMG_PATH)};

  for (int i = 0; i < 9; ++i)
  {
    std::cerr << "entering "   << full_path[i] << std::endl;
    std::cout << "entering "   << full_path[i] << std::endl;

    if (boost::filesystem::exists(full_path[i]) &&
	boost::filesystem::is_directory(full_path[i]))
    {
      boost::filesystem::system_complete(full_path[i]);
      const t_iter_path end_iter;
      unsigned count = 0;
      unsigned sum1  = 0;
      unsigned sum2  = 0;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	unsigned val = count_image_color<8>(dir_iter->path().string());

	++count;
	sum1 += val;
	sum2 += val*val;

	std::cout << dir_iter->path().string() << " => " << val << std::endl;
	// FIXME NB COULEURS BRUTES
	// FIXME NB DE COULEURS
	// FIXME LISTE DES COULEURS
	// IMPORTANCES
      }

      float mean = sum1 / count;
      float var  = ((float)sum2 / count) - (mean * mean);

      std::cout << "mean : " << mean << std::endl;
      std::cout << "var  : " << var  << std::endl;
    }
  }

  return 0;
}
