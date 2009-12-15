// EXPERIMENTATION ON REGIONAL MAXIMA
// COUNTING THE IMAGE COLORS

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/math/sum.hh>
#include <mln/accu/math/count.hh>
#include <mln/accu/stat/histo3d_rgb.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>

#include <mln/algebra/vec.hh>

#include <mln/arith/diff_abs.hh>

#include <mln/core/macros.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>
#include <mln/data/transform.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/labeling/compute.hh>

#include <mln/literal/colors.hh>

#include <mln/morpho/opening/volume.hh>
#include <mln/morpho/elementary/dilation.hh>

#include <mln/opt/at.hh>

#include <mln/pw/cst.hh>

#include <mln/util/array.hh>
#include <mln/util/timer.hh>

#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rgb.hh>
#include <mln/value/int_u.hh>


// LISTER UN REPERTOIRE
// ECRIRE UN FICHIER DE SYNTHESE SUR LA CLASSE
//


// n < 8, n is the degree of quantification
template <unsigned n>
unsigned count_image_color(const std::string& image)
{
  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::image3d<t_lbl8>                        t_image3d_lbl8;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::image2d<unsigned>                      t_histo2d;
  typedef mln::fun::v2v::rgb8_to_rgbn<n>              t_rgb8_to_rgbn;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
  typedef mln::accu::meta::math::count                t_count_fun;

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_rgbn                                      input_rgbn;
  t_image2d_rgbn                                      output_rgbn;
  t_histo3d                                           histo;
  t_histo3d                                           opened;
  t_image3d_lbl8                                      label;
  t_image2d_lbl8                                      label_img;
  t_image3d_lbl8                                      dilated;
  t_lbl8                                              n_labels;

  mln::io::ppm::load(input_rgb8, image.c_str());

  unsigned nb_pixel   = input_rgb8.ncols() * input_rgb8.nrows();
  unsigned min_volume = (unsigned)(nb_pixel * 0.054);

  input_rgbn = mln::data::transform(input_rgb8, t_rgb8_to_rgbn());
  // FIXME UTILISER UN PREDICAT COMME FUN::CHESS
  histo      = mln::data::compute(t_histo3d_fun(), input_rgbn);
  return mln::data::compute(t_count_fun(), histo);
  // FIXME REALISER UN COUNT AVEC UN PREDICA SUR L'IMAGE
  //return nb_pixel;
//   opened     = mln::morpho::opening::volume(histo, mln::c6(), min_volume);
//   label      = mln::labeling::regional_maxima(opened, mln::c6(), n_labels);

//   return n_labels;
}


int main()
{
  typedef boost::filesystem::path                   t_path;
  //typedef boost::filesystem::initial_path<t_path()> t_init_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

  t_path full_path(ANNOTING_TYPED_IMG_PATH);

  std::cout << "entering "   << full_path << std::endl;

  if (//1 < argc &&
      boost::filesystem::exists(full_path) &&
      boost::filesystem::is_directory(full_path))
  {
    boost::filesystem::system_complete(full_path);
    const t_iter_path end_iter;
    unsigned count = 0;
    unsigned sum1  = 0;
    unsigned sum2  = 0;

    for (t_iter_path dir_iter(full_path); end_iter != dir_iter; ++dir_iter)
    {
      unsigned val = count_image_color<5>(dir_iter->path().string());

      ++count;
      sum1 += val;
      sum2 += val*val;

      std::cout << dir_iter->path().string() << " => " << val << std::endl;
      // FIXME NB COULEURS BRUTES
      // FIXME NB DE COULEURS
      // FIXME LISTE DES COULEURS
      // IMPORTANCES
    }

    unsigned mean = sum1 / count;
    unsigned var  = sum2 / count - mean * mean;

    std::cout << "mean : " << mean << std::endl;
    std::cout << "var  : " << var  << std::endl;
  }

  return 0;
}

