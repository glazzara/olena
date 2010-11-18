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
/// \brief Implement the Millet achromatism operator [millet.phd.2008.pdf]
///
/// An image is said to be achromatic when have the RGB channel which
/// are quite identicals.

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

#include <mln/math/max.hh>
#include <mln/math/min.hh>

#include <mln/geom/nsites.hh>

#include <mln/fun/v2v/rgb_to_achromatism_map.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/value/rgb8.hh>


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

/// \brief Decide if an image is achromatic or not [Millet].
///
/// \param[in] input the input image.
/// \param[in] output the histogram of the achromatic map.
/// \param[in] threshold  the distance threshold used for equality.
///
/// This is an improving of the Millet test. The original test
/// compares rgb values to each other and look at differences greater
/// than the threshold.  If the number of pixel that pass the test are
/// greater than 99.0, then the image is declared achromatic. In fact,
/// there is few variations between the three channels, so we can say
/// that it is like a grey image. We can with no too distortions
/// replace the color image by the grey one. The improving is in first
/// creating a map of the difference. As we can't keep the free
/// differences between channels, we look at reducing the test and we
/// find an equivalent one based on the difference between minima
/// channel value and the maxima channel value. After the map is
/// ready, we make binarization with the threshold. Then we compute
/// the histogram 1d for every pixels of the map that are greater the
/// threshold. Then, we count pixels in the histogram and traduce the
/// count in percentage to compare to the second threshold. Details
/// are saved in files and printed in the screen.
float achromatism_test(const std::string input,
		       const std::string output,
		       const unsigned threshold)

{
  typedef mln::fun::v2v::rgb_to_achromatism_map<8>   t_rgb_to_achromatism_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  unsigned                         cnt1;
  unsigned                         cnt2;
  float                            prop;

  mln::io::ppm::load(input_rgb8, input.c_str());

  map    = mln::data::transform(input_rgb8, t_rgb_to_achromatism_map());
  histo  = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  cnt1   = count_histo(histo | mln::box1d(mln::point1d(0),
					  mln::point1d(threshold)));
  cnt2   = mln::geom::nsites(input_rgb8);
  prop   = ((100.0 * cnt1) / cnt2);

  mln::io::plot::save_image_sh(histo, output.c_str());

  return prop;
}


/// \brief The main entry.
///
/// Deal with boost library for walking over image database
/// directories. Call the image processing routine and compute some
/// stats on the result.
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
	t_path directory(ANNOTATING_RET_PATH);
	t_path leaf = dir_iter->path().leaf();
	t_path output = change_extension(directory / leaf, ".sh");

	prop = achromatism_test(dir_iter->path().string(),
				output.string(),
				11);

	std::cout << output << " : " << prop << std::endl;
	std::cerr << output << " : " << prop << std::endl;
      }
    }
  }

  return 0;
}
