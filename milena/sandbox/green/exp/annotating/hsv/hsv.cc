// HSV TEST CF MILLET 2008

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/stat/histo1d.hh>

#include <mln/arith/minus.hh>
#include <mln/arith/div.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/convert.hh>
#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>
#include <mln/data/transform.hh>

#include <mln/literal/zero.hh>
#include <mln/literal/colors.hh>
#include <mln/literal/grays.hh>

#include <mln/math/max.hh>
#include <mln/math/min.hh>
#include <mln/math/sqr.hh>
#include <mln/math/sqrt.hh>

#include <mln/opt/at.hh>

#include <mln/geom/nsites.hh>

#include <mln/fun/v2v/rgb_to_hue_map.hh>
#include <mln/fun/v2v/rgb_to_saturation_map.hh>
#include <mln/fun/v2v/rgb_to_value_map.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/value/rgb8.hh>

//============================================================================//
//                         HISTOGRAM
//============================================================================//

template <typename I>
mln_value(I) count_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) result = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
    result += histo(p);

  return result;
}

template <typename I>
mln_value(I) sum_frequency_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) sum = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
    sum += histo(p);

  return sum;
}

template <typename I>
mln_value(I) count_null_frequency_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) count = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
    if (0 == histo(p))
      count++;

  return count;
}

template <typename I>
mln_coord(mln_site_(I)) peak_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the peak of the histogram
  mln_value(I)            v_max = mln::opt::at(histo, mln::literal::zero);
  mln_coord(mln_site_(I)) p_max = mln::literal::zero;
  mln_piter(I)            p(histo.domain());

  for_all(p)
  {
    if (v_max < histo(p))
    {
      v_max = histo(p);
      p_max = p.ind();
    }
  }

  return p_max;
}

template <typename I>
mln_value(I) max_frequency_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the value of the peak from the histogram
  mln_value(I)            max = mln::opt::at(histo, mln::literal::zero);
  mln_piter(I)            p(histo.domain());

  for_all(p)
  {
    max = mln::math::max(histo(p),max);
  }

  return max;
}

template <typename I>
float mean_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the mean of the histogram
  float sum   = 0;
  float mean  = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum  += histo(p);
    mean += p.ind()*histo(p);
  }

  mean = mean / sum;

  return mean;
}

template <typename I>
float cmp_equi_frequency_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the mean of the histogram
  float sum   = 0;
  float var   = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum += histo(p);
    var += mln::math::sqr(histo(p) - (1/256.0));
  }

  var = var / sum;

  return var;
}

template <typename I>
float var_histo(const mln::Image<I>& histo_, float mean)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the mean of the histogram
  float sum   = 0;
  float var   = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum  += histo(p);
    var  += mln::math::sqr(p.ind() - mean) * histo(p);
  }

  var = var / sum;

  return var;
}

template <typename I>
float mean_frequency_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the mean of the histogram
  float sum   = 0;
  float mean  = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum++;
    mean += histo(p);
  }

  mean = mean / sum;

  return mean;
}

template <typename I>
float stddev_frequency_histo(const mln::Image<I>& histo_, float mean)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the var of the histogram
  float sum  = 0;
  float var  = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum++;
    var += mln::math::sqr(histo(p)-mean);
  }

  var = mln::math::sqrt(var / sum);

  return var;
}


//============================================================================//
//                         HUE TEST
//============================================================================//

mln::value::rgb8 label_val(const mln::value::int_u8 val)
{
  mln::value::rgb8 result;

  if (82 > val)
    result = mln::literal::black;
  else if (179 > val)
    result= mln::literal::medium_gray;
  else
    result = mln::literal::white;

  return result;
}


mln::value::rgb8 label_orange_or_brown(const mln::value::rgb8   color,
				       const mln::value::int_u8 sat,
				       const mln::value::int_u8 val)
{
  mln::value::rgb8 result;

  if (mln::literal::orange == color)
  {
    unsigned dist_orange = mln::math::abs(sat - 184)
      + mln::math::abs(val - 65);

    unsigned dist_brown  = mln::math::abs(sat - 255)
      + mln::math::abs(val - 125);

    if (dist_orange < dist_brown)
      result = mln::literal::orange;
    else
      result = mln::literal::brown;
  }
  else
    result = color;

  return result;
}

mln::value::rgb8 label_yellow_or_green(const mln::value::rgb8   color,
				       const mln::value::int_u8 val)
{
  mln::value::rgb8 result;

  if (mln::literal::yellow == color)
  {
    // Is it green or yellow ?
    if (80 > val)
      result = mln::literal::green;
    else
      result = mln::literal::yellow;
  }
  else
    return color;

  return result;
}

mln::value::rgb8 label_hue(const mln::value::int_u8 hue)
{
  mln::value::rgb8 result;


  if (10 > hue)
    result = mln::literal::red;
  else if (32 > hue)
    result = mln::literal::orange;
  else if (53 > hue)
    result = mln::literal::yellow;
  else if (74 > hue)
    result = mln::literal::green; // chartreuse
  else if (96 > hue)
    result = mln::literal::green;
  else if (116 > hue)
    result = mln::literal::green;// turquoise, aigue-marine
  else if (138 > hue)
    result = mln::literal::green; // cyan
  else if (159 > hue)
    result = mln::literal::blue; // azur
  else if (181 > hue)
    result = mln::literal::blue;
  else if (202 > hue)
    result = mln::literal::violet;
  else if (223 > hue)
    result = mln::literal::pink;
  else // if (244 > hue)
    result = mln::literal::red;

  return result;
}

float hue_test(const std::string input,
	       const std::string output,
	       const std::string tmp,
	       const short threshold)

{
  typedef mln::fun::v2v::rgb_to_hue_map<8>   t_rgb_to_hue_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  mln::image1d<float>              histo_float;
  float                            cnt1;
  float                            cnt2;
  float                            prop;
  short                            peak;
  mln::value::rgb8                 color;
  float                            sum;
  mln::point1d                     inf;
  mln::point1d                     sup;

  mln::io::ppm::load(input_rgb8, input.c_str());

  map         = mln::data::transform(input_rgb8, t_rgb_to_hue_map());
  histo       = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  peak        = mean_histo(histo); //peak_histo(histo);
  color       = label_hue(peak);
  inf         = mln::point1d(mln::math::max(0,   peak-threshold));
  sup         = mln::point1d(mln::math::min(255, peak+threshold));
  cnt1        = count_histo(histo_float|mln::box1d(inf,sup));
  cnt2        = count_histo(histo_float);
  prop        = ((100.0 * cnt1) / cnt2);

  mln::io::plot::save_image_sh(histo_float, output.c_str());
  mln::io::pgm::save(map, tmp.c_str());
//  std::cout << "peak  = " << peak  << std::endl;
//  std::cout << "color = " << color << std::endl;

  return prop;
}

//============================================================================//
//                         SATURATION TEST
//============================================================================//

float saturation_test(const std::string input,
		      const std::string output,
		      const std::string tmp,
		      const short threshold)

{
  typedef mln::fun::v2v::rgb_to_saturation_map<8>    t_rgb_to_saturation_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  mln::image1d<float>              histo_float;
  float                            cnt1;
  float                            cnt2;
  float                            sum;
  float                            prop;

  mln::io::ppm::load(input_rgb8, input.c_str());

  map         = mln::data::transform(input_rgb8, t_rgb_to_saturation_map());
  histo       = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  cnt1        = count_histo(histo_float | mln::box1d(mln::point1d(0),
						     mln::point1d(threshold)));
  cnt2        = count_histo(histo_float);
  prop        = ((100.0 * cnt1) / cnt2);

  mln::io::plot::save_image_sh(histo_float, output.c_str());
  mln::io::pgm::save(map, tmp.c_str());

  return prop;
}

//============================================================================//
//                         VALUE TEST
//============================================================================//

// calcul de contribution
float r(short p, unsigned histo_p, short x, unsigned histo_x)
{
  float result = mln::math::sqr(((float)histo_x / histo_p) * (x-p));

  return result;
}

template <typename I>
float stddev3(const mln::Image<I>& histo_, unsigned peak)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Compute stddev

  float stddev = 0.0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    stddev  += r((short)peak, mln::opt::at(histo,peak), p.ind(), histo(p));
  }

  return stddev;
}

template <typename I>
float stddev2(const mln::Image<I>& histo_, unsigned peak, unsigned limit)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  float stddev_low = 0.0;
  float stddev_up  = 0.0;
  float ret        = 0.0;

  // A transformer avec des iterators

  if (250 > peak)
    stddev_up  = stddev3(histo |mln::box1d(mln::point1d(peak+1),
					   mln::point1d(peak+limit)), peak);

  if (5 < peak)
    stddev_low = stddev3(histo |mln::box1d(mln::point1d(peak-limit),
					   mln::point1d(peak-1)), peak);

  ret = (250 < peak)? stddev_low : (5 > peak)? stddev_up :
    (stddev_low + stddev_up)/2;

  return ret;
}

// DOC:
// la discrimination entre la base AFP et la base ICDAR peut se faire en
// étudiant la forme des densités des niveaux de gris.
// Les images naturelles semblent avoir un spectre recouvrant
// en général les 256 niveaux de gris alors que les images de documents ont
// une présence importante du fond. Dans le cadre d'une densité, ce qui est
// alloué sur le fond ne peut se retrouver ailleurs. Une comparaison avec la
// densité équiprobable nous renseigne donc sur la nature des images.
// Il semble néanmoins qu'un certain nombre d'images défient ce dispositif.
// Par exemple des gros plans sur des zones mono-teintée (ski, voile,site web).




float value_test(const std::string input,
		 const std::string output,
		 const std::string tmp,
		 const short threshold)

{
  typedef mln::fun::v2v::rgb_to_value_map<8>   t_rgb_to_value_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  mln::image1d<float>              histo_float;
  float                            cnt1;
  float                            cnt2;
  float                            prop;
  float                            sum;
  float                            prop4;
  short                            peak;
  mln::point1d                     inf;
  mln::point1d                     sup;


  mln::io::ppm::load(input_rgb8, input.c_str());

  map         = mln::data::transform(input_rgb8, t_rgb_to_value_map());
  histo       = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  prop4       = cmp_equi_frequency_histo(histo_float);
  peak        = peak_histo(histo); // mean_histo(histo);
  //prop        = stddev2(histo, peak, threshold);
  inf         = mln::point1d(mln::math::max(0,   peak-threshold));
  sup         = mln::point1d(mln::math::min(255, peak+threshold));
  cnt1        = count_histo(histo_float|mln::box1d(inf,sup));
  cnt2        = count_histo(histo_float);
  prop        = ((100.0 * cnt1) / cnt2);

  std::cerr << "peak = " << peak << std::endl;
  std::cerr << "inf  = " << inf  << std::endl;
  std::cerr << "sup  = " << sup  << std::endl;
  std::cerr << "cnt1 = " << cnt1 << std::endl;
  std::cerr << "cnt2 = " << cnt2 << std::endl;
  std::cerr << "prop = " << prop << std::endl;
  std::cerr << "prop4= " << prop4 << std::endl;

  mln::io::plot::save_image_sh(histo_float, output.c_str());
  mln::io::pgm::save(map, tmp.c_str());

  return prop;
}

//============================================================================//
//                         MAIN
//============================================================================//


int main()
{
  typedef boost::filesystem::path                   t_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

//   t_path full_path[] = {t_path(ICDAR_20P_TEXT_ONLY_IMG_PATH),
//  			t_path(ICDAR_20P_TEXT_COLOR_IMG_PATH),
//  			t_path(ICDAR_20P_TEXT_PHOTO_IMG_PATH)};

  t_path full_path[] = {t_path(AFP_PPM_IMG_PATH)};

  std::cout << "#!/usr/bin/gnuplot"                       << std::endl;
  std::cout << "set terminal x11 persist 1"               << std::endl;
  std::cout << "#HUE - SATURATION - VALUE"                << std::endl;
  std::cout << "plot '-' using 1:2 with point notitle,\\" << std::endl;
  std::cout << "     '-' using 1:2 with point notitle,\\" << std::endl;
  std::cout << "     '-' using 1:2 with point notitle"    << std::endl;

  for (int i = 0; i < 1; ++i)
  {
    if (boost::filesystem::exists(full_path[i]) &&
	boost::filesystem::is_directory(full_path[i]))
    {
      boost::filesystem::system_complete(full_path[i]);
      const t_iter_path end_iter;
      float             prop = 0.0;
      t_path            directory;
      t_path            leaf;
      t_path            output;
      t_path            tmp;

      std::cerr << "entering "   << full_path[i] << std::endl;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	std::cerr << dir_iter->path() << std::endl;
	// concatenation de chaine
//	directory = (ANNOTATING_ICDAR_HUE_RET_PATH);
	directory = (ANNOTATING_AFP_HUE_RET_PATH);
	leaf      = dir_iter->path().leaf();
	output    = change_extension(directory / leaf, ".sh");
	tmp       = change_extension(directory / leaf, ".pgm");

	prop = hue_test(dir_iter->path().string(),
			output.string(),
			tmp.string(),
			20);

	std::cout << prop << " ";

//	directory = (ANNOTATING_ICDAR_SAT_RET_PATH);
	directory = (ANNOTATING_AFP_SAT_RET_PATH);
	leaf      = dir_iter->path().leaf();
	output    = change_extension(directory / leaf, ".sh");
	tmp       = change_extension(directory / leaf, ".pgm");

	prop = saturation_test(dir_iter->path().string(),
			       output.string(),
			       tmp.string(),
			       25);

	std::cout << prop << " ";

//	directory = (ANNOTATING_ICDAR_VAL_RET_PATH);
	directory = (ANNOTATING_AFP_VAL_RET_PATH);
	leaf      = dir_iter->path().leaf();
	output    = change_extension(directory / leaf, ".sh");
	tmp       = change_extension(directory / leaf, ".pgm");

	prop = value_test(dir_iter->path().string(),
			  output.string(),
			  tmp.string(),
			  15);

	std::cout << prop  << " ";
	std::cout << "# "  << dir_iter->path().leaf() << std::endl;
      }
      std::cout << "e"   << std::endl;
    }
  }

  return 0;
}
