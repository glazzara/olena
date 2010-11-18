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
/// This demonstrator is the first step before using
/// regional_maxima. Its purpose is to work on histogram filtering and
/// on the whole image processing chain.
///
/// \fixme: this source doesn't compile. I've no time to fix it. It's not
/// so important because it's just a basic sample, just keep in mind the
/// method to reach the result, it will be reuse for regional_maxima.

#include <iostream>
#include <sstream>

#include <mln/accu/stat/histo1d.hh>

#include <mln/arith/revert.hh>

#include <mln/core/alias/neighb1d.hh>
#include <mln/core/alias/window1d.hh>
#include <mln/core/macros.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/compute.hh>
#include <mln/data/transform.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/labeling/colorize.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/labeling/regional_maxima.hh>

#include <mln/linear/convolve.hh>
//#include <mln/linear/gaussian.hh>

#include <mln/literal/colors.hh>

#include <mln/morpho/elementary/closing.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/elementary/opening.hh>
#include <mln/morpho/opening/structural.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/img_path.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_8.hh>

#include <mln/win/disk1d.hh>


/// \brief Compute the gaussian probability to obtain a given value.
///
/// \param[in] x     the value from which we want the probability.
/// \param[in] mean  the mean parameter of the gaussian distribution.
/// \param[in] sigma the sqrt(variance) parameter of the gaussian.
/// \return the probability to obtain the x value.
///
/// Implements the standard 1d equation of the gaussian.
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


/// \brief Merge input image with label one.
///
/// \param[in] input    the window (filter) where to put the probability values.
/// \param[in] label  the size of the filter.
/// \return the merged image.
///
/// This function build a new image in which the input pixels are
/// replaced by the label associated to their grey level.
mln::image2d<mln::value::label_8>
merge(const mln::image2d<mln::value::int_u8>&  input,
      const mln::image1d<mln::value::label_8>& label)
{
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

  return output;
}

/// \brief This is the wathersed method for segmenting the histogram.
///
/// \param[in] image the input image.
///
/// The function returns nothing but writes different images on the
/// filesystem. The 3 outputs are the image label, the random coloring
/// of the image label and the average tone of the label image
/// (replace every label by the grey average of the group to which
/// it's belong to) .
void do_demo(const std::string& image)
{
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::image1d<t_lbl8>                        t_image1d_lbl8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::image1d<unsigned>                      t_image1d_unsigned;

  t_image2d_int_u8                                    img;
  t_image1d_unsigned                                  histo;
  t_image1d_unsigned                                  filtered;
  t_image1d_unsigned                                  reverted;
  t_image1d_lbl8                                      label;
  t_image2d_lbl8                                      output;
  t_image2d_int_u8                                    mean;
  t_image2d_rgb8                                      colorized;
  t_lbl8                                              n_labels;


  //---------------------------------------------------------------------------
  // LOADING PHASE
  //---------------------------------------------------------------------------
  mln::io::pgm::load(img, image.c_str());
  mln::io::pgm::save(img,  "img.pgm");


  //---------------------------------------------------------------------------
  // HISTO COMPUTING PHASE
  //---------------------------------------------------------------------------
  histo = mln::data::compute(mln::accu::meta::stat::histo1d(), img);
  mln::io::plot::save_image_sh(histo, "histo.sh");


  //---------------------------------------------------------------------------
  // HISTO FILTERING PHASE
  //---------------------------------------------------------------------------

  // a) GAUSSIAN FILTERING
  double ws[41];
  gaussian_filter(ws, 41, 6.0);
  filtered = mln::linear::convolve(histo, mln::make::w_window1d(ws));

  // b) MORPHOLOGICAL ELEMENTARY OPENING
  // filtered = mln::morpho::elementary::opening(histo, mln::c4());

  // c) MORPHOLOGICAL STRUCTURAL OPENING (C4,C8 ??)
  // filtered = mln::morpho::opening::structural(histo, mln::win_c8p());

  // d) MORPHOLOGICAL STRUCTURAL DISK OPENING (SIZE = 11/21/31/41/51 ??)
  // mln::win::disk1d disk1d(21);
  // filtered = mln::morpho::opening::structural(histo, disk1d);

  mln::io::plot::save_image_sh(filtered, "filtered.sh");


  //---------------------------------------------------------------------------
  // HISTO LABELING PHASE
  //---------------------------------------------------------------------------
  // a) watershed
  reverted = mln::arith::revert(histo);
  label    = mln::morpho::watershed::flooding(reverted, mln::c4(), n_labels);
  // b) regional maxima
  // label = mln::labeling::regional_maxima(opened, mln::c8(), n_labels);
  mln::io::pgm::save(label, "label.pgm");


  //---------------------------------------------------------------------------
  // FUZZY PHASE
  //---------------------------------------------------------------------------
  dilated    = mln::morpho::elementary::dilation(label, mln::c8());
  mln::io::pgm::save(dilated, "dilated.pgm");


  //---------------------------------------------------------------------------
  // OUTPUT PHASE
  //---------------------------------------------------------------------------
  output    = merge(house_rg8, label);
  mean      = mln::labeling::mean_values(img, label, nlabels);
  colorized = mln::labeling::colorize(rgb8(), label);

  mln::io::pgm::save(output,    "output.pgm");
  mln::io::pgm::save(mean,      "mean.pgm");
  mln::io::ppm::save(colorized, "colorized.ppm");
}


/// \brief This is the launcher of the method.
///
/// \param[in] image the input image.
///
/// The launcher is the good place for debugging input arguments or
/// setting defaults one.
void demo(const std::string& image     = SCRIBO_PGM_IMG_PATH"/mp00082c_50p.pgm")
{
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Launching the demo with these parameters" << std::endl;
  std::cout << "image     : " << image                    << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  do_demo(image);
}


/// \bief The usage function of the binary.
///
/// \param[in] argc the binary number of arguments.
/// \param[in] args the binary array of arguments.
///
/// This binary requires one input image to work. It is a grey level
/// image in pgm format.
void usage(const int argc, const char *args[])
{
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "argc    : " << argc                       << std::endl;

  for (int i = 0; i < argc; ++i)
    std::cout << "args[" << i << "] : " << args[i] << std::endl;

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "usage: wateshed [image]"                  << std::endl;
  std::cout << "pgm image          (image to label)"      << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}


/// \brief Convert a c string parameter to string object like.
///
/// \param[in]  status the flag to prevent bad work.
/// \param[in]  arg    the c string to convert in string object.
/// \param[out] val    the return string object.
/// \return a flag to say if every thing goes well.
///
/// Convert a c string (char *) in string object (std::string) in order
/// to simplify some string operations.
bool char_to_string(const bool status, const char *arg, std::string& val)
{
  bool result = false;

  if (status)
  {
    std::istringstream arg_stream(arg);

    arg_stream >> val;

    return !arg_stream.fail();
  }

  return result;
}


/// \brief Segment by the watershed method histogram space.
///
/// \param[in] img the input image.
/// \return merge image.
///
/// This is the main entry of the program. It takes an image as input
/// parameter and computes its histogram. The method segments the
/// histogram in order to build color classes (even we stay in grey
/// level). Merging image is returned where pixels have been replaced
/// by some attribute of the class (like the average color).
int main(const int argc, const char *args[])
{
  std::string image("top");
  bool        status = true;

  switch (argc)
  {
    case 2:  status = char_to_string(status, args[1], image); break;
    case 1:  status = true; break;
    default: status = false;
  }

  if (status)
  {
    switch (argc)
    {
      case 1: demo(); break;
      case 2: demo(image); break;
    }
  }
  else
    usage(argc, args);

  return 0;
}
