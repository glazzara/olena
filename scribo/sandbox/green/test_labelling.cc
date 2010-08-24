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


#include <mln/io/plot/save_histo_sh.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_8.hh>
#include <mln/core/alias/neighb1d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/make/w_window1d.hh>
#include <mln/math/pi.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/elementary/closing.hh>
#include <mln/pw/value.hh>
#include <mln/linear/convolve.hh>
#include <mln/linear/gaussian.hh>


/// ... to complete

/// \brief Compute the gaussian probability to obtain a given value.
///
/// \param[in] x     the value from which we want the probability.
/// \param[in] mean  the mean parameter of the gaussian distribution.
/// \param[in] sigma the sqrt(variance) parameter of the gaussian.
/// \return the probability to obtain the x value.
///
/// Implements the standard equation of the gaussian.

double gaussian_distribution(const double x,
			     const double mean,
			     const double sigma)
{
  double num    = exp(-0.5*mln::math::sqr((x - mean)/sigma));
  double denom  = sigma*mln::math::sqrt(2*mln::math::pi);
  double result = num/denom;

  return result;
}

/// \brief Make a 1d gaussian filter.
///
/// \param[out] ws    the window (filter) where to put the probability values.
/// \param[in]  size  the size of the filter.
/// \param[in]  sigma the gaussian parameter for the standard deviation.
/// \return the probability to obtain the x value.
///
/// Approach the gaussian distribution for a few discrete sites, and
/// then normilize the results to be sure that the sum obtain 1.

void gaussian_filter(double ws[], const unsigned size, const double sigma)
{
  int h = size/2;

  for (int i = -h; i <= h; ++i)
  {
    ws[i+h] = gaussian_distribution(i, 0.0, sigma);
  }

  double sum = 0.0;

  for (unsigned i = 0; i < size; ++i)
    sum += ws[i];

  for (unsigned i = 0; i < size; ++i)
    ws[i] /= sum;
}

/// \brief Make a 1d gaussian filter.
///
/// \param[out] ws    the window (filter) where to put the probability values.
/// \param[in]  size  the size of the filter.
/// \param[in]  sigma the gaussian parameter for the standard deviation.
/// \return the probability to obtain the x value.
///
/// Approach the gaussian distribution for a few discrete sites, and
/// then normilize the results to be sure that the sum obtain 1.

/// The aim of this function is to rebuild an label image2d from the segmenting
/// image of the histogram (label) and the original image (input).
/// label_image2d / for each grey tone, associate its label.

mln::image2d<mln::value::label_8>
build_8bits(const mln::image2d<mln::value::int_u8>&  input,
	    const mln::image1d<mln::value::label_8>& label)
{
  mln::trace::entering("build_8bits");
  mln_precondition(label.is_valid());
  mln_precondition(input.is_valid());

  mln::image2d<mln::value::label_8>             output;

  mln::initialize(output, input);

  mln_piter_(mln::image2d<mln::value::int_u8>)  pi(input.domain());
  mln_piter_(mln::image2d<mln::value::label_8>) po(output.domain());

  for_all_2(pi, po)
  {
    mln::value::int_u8 val = input(pi);
    unsigned           grp = label(mln::point1d(val));

    output(po) = grp;
  }

  mln::trace::exiting("build_8bits");
  return output;
}


void test_8bits_classifying()
{
  typedef mln::value::int_u8                int_u8;
  typedef mln::value::label_8               label_8;
  typedef mln::value::rgb8                  rgb8;
  typedef mln::accu::stat::mean<double>     mean;

  mln::image2d<int_u8>        img_ref;
  mln::image2d<int_u8>        img_out;
  mln::image2d<rgb8>          img_rgb;
  mln::image1d<unsigned>      img_res;
  mln::image1d<double>        img_smooth;
  mln::image1d<label_8>       labels;
  label_8                     nlabels;

  //-----------------------------------------------------
  // Loading the scribo image and computing its histogram
  //-----------------------------------------------------

  std::cout << "(08 bits) LOADING HISTOGRAM" << std::endl;

  //  mln::io::pgm::load(img_ref, OLENA_IMG_PATH"/lena.pgm");
  mln::io::pgm::load(img_ref, SCRIBO_IMG_PATH"/mp00082c_50p_8bits.pgm");
  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u8>(), img_ref);
  mln::io::plot::save_histo_sh(img_res, "histo0_8bits.sh");


  //-----------------------------------------------------
  // Smoothing the histogram with a gaussian filter
  //-----------------------------------------------------

  std::cout << "(08 bits) SMOOTHING HISTOGRAM" << std::endl;

  double ws[41];
  gaussian_filter(ws, 41, 6.0);
  img_smooth = mln::linear::convolve(img_res, mln::make::w_window1d(ws));
  mln::io::plot::save_histo_sh(img_smooth, "histo1_8bits.sh");


  //-----------------------------------------------------
  // Segmenting the histogram with the watershed method
  //-----------------------------------------------------

  std::cout << "SEGMENTING HISTOGRAM" << std::endl;

  /*
  labels = mln::labeling::regional_maxima(img_smooth, mln::c2(), nlabels);
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_histo_sh(labels, "histo2_8bits.sh");
  */

  // need to revert the histogram
  labels = mln::morpho::watershed::flooding(img_smooth, mln::c2(), nlabels);
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_histo_sh(labels, "histo2_8bits.sh");


  //-----------------------------------------------------
  // Rebuilding the image with the mean of each region
  //-----------------------------------------------------

  std::cout << "(08 bits) BUILDING OUTPUT" << std::endl;

  mln::image2d<label_8>img_label = build_8bits(img_ref, labels);

  std::cout << "(08 bits) COLORING OUTPUT" << std::endl;

  img_out = mln::labeling::mean_values(img_ref, img_label, nlabels);
  img_rgb = mln::labeling::colorize(rgb8(), img_label);

  mln::io::pgm::save(img_out, "out_8bits.pgm");
  mln::io::ppm::save(img_rgb, "color_8bits.pgm");

  //labels = mln::morpho::elementary::dilation(labels, mln::c2());
  //mln::io::plot::save_histo_sh(labels, "histo3.sh");
  //mln::io::plot::save(labels, "labelized.data");
}


/// The aim of this function is to rebuild an label image2d from the segmenting
/// image of the histogram (label) and the original image (input).
/// label_image2d / for each grey tone, associate its label.

mln::image2d<mln::value::label_8>
build_14bits(const mln::image2d<mln::value::int_u<14> >&  input,
	     const mln::image1d<mln::value::label_8>&     label)
{
  mln::trace::entering("build_14bits");
  mln_precondition(label.is_valid());
  mln_precondition(input.is_valid());

  mln::image2d<mln::value::label_8>             output;

  mln::initialize(output, input);

  mln_piter_(mln::image2d<mln::value::int_u<14> >) pi(input.domain());
  mln_piter_(mln::image2d<mln::value::label_8>) po(output.domain());

  for_all_2(pi, po)
  {
    mln::value::int_u<14> val = input(pi);
    unsigned              grp = label(mln::point1d(val));

    output(po) = grp;
  }

  mln::trace::exiting("build_14bits");
  return output;
}


void test_14bits_classifying()
{
  typedef mln::value::int_u16               int_u16;
  typedef mln::value::int_u<14>             int_u14;
  typedef mln::value::label_8               label_8;
  typedef mln::value::rgb8                  rgb8;
  typedef mln::accu::stat::mean<double>     mean;

  mln::image2d<int_u16>       img_fst;
  mln::image2d<int_u14>       img_ref;
  mln::image2d<int_u14>       img_out;
  mln::image2d<rgb8>          img_rgb;
  mln::image1d<unsigned>      img_res;
  mln::image1d<double>        img_smooth;
  mln::image1d<label_8>       labels;
  label_8                     nlabels;

  //-----------------------------------------------------
  // Loading the scribo image and computing its histogram
  //-----------------------------------------------------

  std::cout << "(14 bits) LOADING HISTOGRAM" << std::endl;

  //mln::io::pgm::load(img_fst, OLENA_IMG_PATH"/lena_16.pgm");
  mln::io::pgm::load(img_fst, SCRIBO_IMG_PATH"/mp00082c_50p_16bits.pgm");
  img_ref = mln::data::transform(img_fst, mln::fun::v2v::int_u16_to_int_u14());
  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u14>(), img_ref);
  mln::io::plot::save_histo_sh(img_res, "histo0_14bits.sh");


  //-----------------------------------------------------
  // Smoothing the histogram with a gaussian filter
  //-----------------------------------------------------

  std::cout << "(14 bits) SMOOTHING HISTOGRAM" << std::endl;

  double ws[401];
  gaussian_filter(ws, 401, 50.0);
  img_smooth = mln::linear::convolve(img_res, mln::make::w_window1d(ws));
  mln::io::plot::save_histo_sh(img_smooth, "histo1_14bits.sh");


  //-----------------------------------------------------
  // Segmenting the histogram with the watershed method
  //-----------------------------------------------------

  std::cout << "(14 bits) SEGMENTING HISTOGRAM" << std::endl;

  /*
  labels = mln::labeling::regional_maxima(img_smooth, mln::c2(), nlabels);
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_histo_sh(labels, "histo2.sh");
  */


  labels = mln::morpho::watershed::flooding(img_smooth, mln::c2(), nlabels);
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_histo_sh(labels, "histo2_14bits.sh");


  //-----------------------------------------------------
  // Rebuilding the image with the mean of each region
  //-----------------------------------------------------

  std::cout << "(14 bits) BUILDING OUTPUT" << std::endl;

  mln::image2d<label_8>img_label = build_14bits(img_ref, labels);

  std::cout << "(14 bits) COLORING OUTPUT" << std::endl;

  img_out = mln::labeling::mean_values(img_ref, img_label, nlabels);
  img_rgb = mln::labeling::colorize(rgb8(), img_label);

  mln::io::pgm::save(img_out, "out_14bits.pgm");
  mln::io::ppm::save(img_rgb, "color_14bits.pgm");

  //labels = mln::morpho::elementary::dilation(labels, mln::c2());
  //mln::io::plot::save_histo_sh(labels, "histo3.sh");
  //mln::io::plot::save(labels, "labelized.data");
}

int main()
{
  test_8bits_classifying();

  test_14bits_classifying();

  return 0;
}
