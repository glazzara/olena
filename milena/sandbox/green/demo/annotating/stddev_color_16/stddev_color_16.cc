// COMPUTING THE STDEV OF THE COLORS

#include <iostream>
#include <sstream>

#include <mln/img_path.hh>

#include <mln/accu/max_site.hh>
#include <mln/accu/stat/histo1d.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/debug/println.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>
#include <mln/data/transform.hh>
#include <mln/data/paste.hh>

#include <mln/fun/v2v/rgb8_to_int_u8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/math/sqr.hh>

#include <mln/opt/at.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/int_u.hh>


float r(short p, unsigned histo_p, short x, unsigned histo_x)
{
  float result = mln::math::sqr(((float)histo_x / histo_p) * (x-p));

  return result;
}

unsigned stddev_color(mln::image2d<mln::value::int_u8> input_int_u8,
		      const char *name_histo,
		      const char *name_image)
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

  t_histo1d                                           histo;

  std::cout << "histo : " << name_histo << std::endl;
  std::cout << "image : " << name_image << std::endl;

  histo        = mln::data::compute(t_histo1d_fun(), input_int_u8);

  mln::io::pgm::save(input_int_u8, name_image);
  mln::io::plot::save_image_sh(histo, name_histo);
  mln::debug::println(histo);

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

  std::cout << "max_site    : " << p_max << std::endl;
  std::cout << "h(max_site) : " << v_max << std::endl;
  std::cout << "stddev_up   : " << stddev_up << std::endl;
  std::cout << "stddev_low  : " << stddev_low << std::endl;
  std::cout << "stddev      : " << stddev << std::endl;

  return 0;
}

unsigned stddev_color_16(const std::string& image)
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

  mln::io::ppm::load(input_rgb8, image.c_str());
  input_int_u8 = mln::data::transform(input_rgb8, t_rgb8_to_int_u8());

  // IMAGE SPLITTING PHASE
  mln::box2d     domain = input_int_u8.domain();
  mln::point2d   pmin   = domain.pmin();
  mln::point2d   pmax   = domain.pmax();

  unsigned       sz_row = (pmax.row() - pmin.row())/ 4;
  unsigned       sz_col = (pmax.col() - pmin.col())/ 4;

  std::cout << domain << std::endl;

  // Divide the domain in nine sub-domains.
  for (unsigned i = 0; i < 4; ++i)
    for (unsigned j = 0; j < 4; ++j)
    {
      mln::point2d min(pmin.row()+sz_row*i,pmin.col()+sz_col*j);
      mln::point2d max(pmin.row()+sz_row*(i+1),pmin.col()+sz_col*(j+1));
      mln::box2d   dom(min,max);

      std::cout << dom << std::endl;

      // Save it
      t_image2d_int_u8   input_1o16_int_u8(dom);
      std::ostringstream name_histo("");
      std::ostringstream name_image("");

      name_histo << "histo" << i << "_" << j << ".sh";
      name_image << "image" << i << "_" << j << ".ppm";

      mln::data::paste(input_int_u8 | dom, input_1o16_int_u8);

      stddev_color(input_1o16_int_u8,
		   name_histo.str().c_str(),
		   name_image.str().c_str());
    }

  return 0;
}


int main()
{
//   unsigned val = stddev_color_16(ANNOTATING_PHOTO_IMG_PATH "/photo01.ppm");
  unsigned val = stddev_color_16(ANNOTATING_LOGO_IMG_PATH "/logo06.ppm");

//   std::cout << "nb color : " << val << std::endl;

  return 0;
}

