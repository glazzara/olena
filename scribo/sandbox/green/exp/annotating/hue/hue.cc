// Copyright (C) 2007, 2008, 2009, 2010 EPITA LRDE
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
///
/// \brief Implement the Millet hue operator [millet.phd.2008.pdf]
///
/// This is the Millet code for moving from RGB space to Hue
/// one. Formulae are classical, we can find them on the web.
///
/// Val = max(R,G,B).
/// Sat = (max(R,G,B) - min(R,G,B))/max(R,G,B).
/// IF R = max(R,G,B) THEN Hue = 60 * [(V-B)/(max(R,G,B)-min(R,G,B))].
/// IF G = max(R,G,B) THEN Hue = 60 * [2 + (B-R)/(max(R,G,B)-min(R,G,B))].
/// IF B = max(R,G,B) THEN Hue = 60 * [4 + (R-G)/(max(R,G,B)-min(R,G,B))].


#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/stat/histo1d.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>
#include <mln/data/transform.hh>

#include <mln/literal/colors.hh>
#include <mln/literal/grays.hh>

#include <mln/math/max.hh>
#include <mln/math/min.hh>

#include <mln/opt/at.hh>

#include <mln/geom/nsites.hh>

#include <mln/fun/v2v/rgb_to_hue_map.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>



/// \brief Label a grey value.
///
/// \param[in] int_u8 the grey value.
///
/// \return the reference color for this grey value.
///
/// Classify grey value in three class, white, black and medium_gray.
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


/// \brief Label color with orange reference or brown reference.
///
/// \param[in] color the rgb8 color.
/// \param[in] sat the HSV saturation.
/// \param[in] val the HSV value.
///
/// \return the reference color with which it has been associated to.
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

/// \brief Label between yellow and green color.
///
/// \param[in] color the rgb8 color.
/// \param[in] val the HSV value.
///
/// \return the reference color associated to that color.
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


/// \brief Label hue color.
///
/// \param[in] hue the HSV hue canal for a pixel.
///
/// \return a hue segmentation of the hue wheel.
///
/// The old classification given by Millet is commented. Mine is ready
/// to use.  We need to change reference colors to make the
/// primary colors, the secondary ones, etc...
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


//   if (14 > hue)
//     result = mln::literal::red;
//   else if (29 > hue)
//     result = mln::literal::orange;
//   else if (45 > hue)
//     result = mln::literal::yellow;
//   else if (113 > hue)
//     result = mln::literal::green;
//   else if (149 > hue)
//     result = mln::literal::cyan;
//   else if (205 > hue)
//     result = mln::literal::blue;
//   else if (235 > hue)
//     result = mln::literal::violet;
//   else if (242 > hue)
//     result = mln::literal::pink;
//   else
//     result = mln::literal::red;

  return result;
}


/// \brief Sum all the bins of the histogram.
///
/// \param[in] img the histogram based on image.
///
/// \return the sum of the overall bins.
///
/// Sum evry bins and return the result.
template <typename I>
unsigned count_histo(const mln::Image<I>& img_)
{
  const   I& img = exact(img_);

  mln_precondition(img.is_valid());

  unsigned     result = 0;

  mln_piter(I) p(img.domain());

  for_all(p)
    result += img(p);

  return result;
}

/// \brief Find the peak of the histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the bin which contains the greatest value.
///
/// Compute the position of the peak of the histogram. To do this, we
/// view evrey bin and we maintain the maxima of the values and the
/// position. At the end, we return the position which contains the
/// greatest value.
template <typename I>
unsigned peak_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the peak of the histogram
  unsigned v_max = mln::opt::at(histo, 0);
  short    p_max = 0;

  mln_piter(I) p(histo.domain());

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

/// \brief Compute the average of the histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the average of the histogram.
///
/// Compute the mean of the histogram by summing the values of each
/// bin ponderated by its frequency. The result is divided by the sum
/// of the frequencies.
template <typename I>
unsigned mean_histo(const mln::Image<I>& histo_)
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


/// \brief The hue test.
///
/// \param[in] input the name of the input image.
/// \param[in] output the name of the histogram to output.
/// \param[in] tmp the name of the hue map to output.
///
/// \return the proportion of pixels that pass the test.
///
/// Load the input image, transform it to get the hue_map and compute
/// the histogram. Then count the number of pixels that are between
/// peak-threshold and peak +threshold. Print the peak and the
/// percentage of pixels around the pic.
float hue_test(const std::string input,
	       const std::string output,
	       const std::string tmp,
	       const unsigned threshold)

{
  typedef mln::fun::v2v::rgb_to_hue_map<8>   t_rgb_to_hue_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  unsigned                         cnt1;
  unsigned                         cnt2;
  float                            prop;
  unsigned                         peak;
  mln::value::rgb8                 color;

  mln::io::ppm::load(input_rgb8, input.c_str());

  map    = mln::data::transform(input_rgb8, t_rgb_to_hue_map());
  histo  = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  peak   = mean_histo(histo); //peak_histo(histo);
  color  = label_hue(peak);
  cnt1   = count_histo(histo | mln::box1d(mln::point1d(peak-threshold),
					  mln::point1d(peak+threshold)));
  cnt2   = mln::geom::nsites(input_rgb8);
  prop   = ((100.0 * cnt1) / cnt2);

  mln::io::plot::save_image_sh(histo, output.c_str());
  mln::io::pgm::save(map, tmp.c_str());
  std::cout << "peak  = " << peak  << std::endl;
  std::cout << "color = " << color << std::endl;

  return prop;
}


/// \brief Main entry.
///
/// It is a front end which managing the iteration through one image
/// data base with boost. Save hue map and hue histogram.
int main()
{
  typedef boost::filesystem::path                   t_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

  t_path full_path[] = {t_path(ICDAR_20P_PPM_IMG_PATH)};

  for (int i = 0; i < 1; ++i)
  {
    std::cout << "entering "   << full_path[i] << std::endl;

    if (boost::filesystem::exists(full_path[i]) &&
	boost::filesystem::is_directory(full_path[i]))
    {
      boost::filesystem::system_complete(full_path[i]);
      const t_iter_path end_iter;
      float             prop = 0.0;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	// concatenation de chaine
	t_path directory(ANNOTATING_ICDAR_RET_PATH);
	t_path leaf   = dir_iter->path().leaf();
	t_path output = change_extension(directory / leaf, ".sh");
	t_path tmp    = change_extension(directory / leaf, ".pgm");

	prop = hue_test(dir_iter->path().string(),
			output.string(),
			tmp.string(),
			20);

	std::cout << output << " : " << prop << std::endl;
	std::cerr << output << " : " << prop << std::endl;
      }
    }
  }

  return 0;
}
