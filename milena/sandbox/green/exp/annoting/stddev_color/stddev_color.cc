// COMPUTING THE STDEV OF THE COLORS

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/max_site.hh>
#include <mln/accu/stat/histo1d.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>
#include <mln/data/transform.hh>

#include <mln/fun/v2v/rgb8_to_int_u8.hh>

#include <mln/io/ppm/load.hh>

#include <mln/math/sqr.hh>

#include <mln/opt/at.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/int_u.hh>


// FIXME REPARER L'ACCUMULATEUR MAX_SITE
// FIXME PENSER LA VARIANCE SUR UN HISTOGRAMME NORMALISE, VOIR LES RESULTATS

float r(short p, unsigned histo_p, short x, unsigned histo_x)
{
  float result = mln::math::sqr(((float)histo_x / histo_p) * (x-p));

  return result;
}

float stddev_color(const std::string& image)
{
  typedef mln::point1d                                t_point1d;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image1d<unsigned>                      t_histo1d;
  typedef mln::fun::v2v::rgb8_to_int_u8               t_rgb8_to_int_u8;
  typedef mln::accu::meta::stat::histo1d              t_histo1d_fun;
  typedef mln::accu::max_site<t_histo1d>              t_max_site_fun;

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_int_u8                                    input_int_u8;
  t_histo1d                                           histo;
  t_point1d                                           max_site;

  mln::io::ppm::load(input_rgb8, image.c_str());
  input_int_u8 = mln::data::transform(input_rgb8, t_rgb8_to_int_u8());
  histo        = mln::data::compute(t_histo1d_fun(), input_int_u8);

  // Find the peak of the histogram
  unsigned v_max = mln::opt::at(histo, 0);
  short    p_max = 0;

  mln_piter_(t_histo1d) p(histo.domain());

  for_all(p)
  {
    if (v_max < histo(p))
    {
      v_max = histo(p);
      p_max = p.ind();
    }
  }

  // Compute the specific stddev

  float stddev_low = 0.0;
  float stddev_up  = 0.0;
  float stddev     = 0.0;

  if (250 > p_max)
    for (short i = p_max+1; i < p_max+6; ++i)
      stddev_up += r(p_max, mln::opt::at(histo,p_max),
		     i, mln::opt::at(histo,i));

  if (5 < p_max)
    for (short i = p_max-1; i > p_max-6; --i)
      stddev_low += r(p_max, mln::opt::at(histo,p_max),
		      i, mln::opt::at(histo,i));

  stddev = (250 < p_max)? stddev_low : (5 > p_max)? stddev_up :
    (stddev_low + stddev_up)/2;

  return stddev;
}

int main()
{
  typedef boost::filesystem::path                   t_path;
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
      float count = 0;
      float sum1  = 0;
      float sum2  = 0;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	float val = stddev_color(dir_iter->path().string());

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
