// Copyright (C) 2007,2008,2009,2010 EPITA LRDE
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
/// \brief This is a demonstrator for kmean_rgb.
///
/// The code is provided as is. It is an alpha version, so very very
/// experimental. Take care !!

#include <iostream>
#include <sstream>

#include <mln/clustering/kmean_rgb.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image2d.hh>

#include <mln/data/transform.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/img_path.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>


/// \brief The real code for launching the kmean optimised algorithm.
///
/// \param[in] image     : the color image.
/// \param[in] k_center  : the number of centers.
/// \param[in] n_times   : the number of launching.
/// \param[in] watch_dog : the number of iterations to manage the convergence.
///
/// This is the real lauching code. In fact, the stuff consists in loading the
/// image, setting the parameters, calling the routine, extracting the results
/// and saving them for the user.
///
/// \fixme The result is not safe because we don't test kmean.is_valid()
void do_demo(const std::string& image,
	     const unsigned     k_center,
	     const unsigned     n_times,
	     const unsigned     watch_dog)
{
  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<5>                          t_rgb5;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgb5>                        t_image2d_rgb5;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;

  t_image2d_rgb8                                      img_rgb8;
  t_image2d_rgb5                                      img_rgb5;
  t_image2d_lbl8                                      img_lbl8;

  mln::io::ppm::load(img_rgb8, image.c_str());
  img_rgb5 = mln::data::transform(img_rgb8,mln::fun::v2v::rgb8_to_rgbn<5>());
  img_lbl8 = mln::clustering::kmean_rgb<double,5>(img_rgb5,
						  k_center,
						  watch_dog,
						  n_times);

  mln::io::pgm::save(img_lbl8, "label.pgm");
}

/// \brief Front hand for the demonstrator.
///
/// \param[in] image     : the color image.
/// \param[in] k_center  : the number of centers.
/// \param[in] n_times   : the number of launching.
/// \param[in] watch_dog : the number of iterations to manage the convergence.
///
/// The front hand enables the setting of the default values for the
/// parameters. it allows us to print the real parameter set used by
/// the demonstrator.
void demo(const std::string& image     = OLENA_IMG_PATH"/house.ppm",
	  const unsigned     k_center  = 3,
	  const unsigned     n_times   = 3,
	  const unsigned     watch_dog = 3)
{
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Launching the demo with these parameters" << std::endl;
  std::cout << "image     : " << image                    << std::endl;
  std::cout << "k_center  : " << k_center                 << std::endl;
  std::cout << "n_times   : " << n_times                  << std::endl;
  std::cout << "watch_dog : " << watch_dog                << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  do_demo(image, k_center, n_times, watch_dog);
}


/// \brief Convert character get in the input stream to unsigned integer.
///
/// \param[in]  status : the exiting flag tell us to exit without doing any job.
/// \param[in]  arg    : the input argument to convert.
/// \param[out] val    : the converted value to output.
///
/// \return a flag that tell us if every thing is ok, if the result can be used.
bool char_to_unsigned(const bool status, const char *arg, unsigned& val)
{
  bool result = false;

  if (status)
  {
    std::istringstream arg_stream(arg);

    arg_stream >> val;

    result = !arg_stream.fail();
  }

  return result;
}


/// \brief Convert character get in the input stream to string.
///
/// \param[in]  status : the exiting flag tell us to exit without doing any job.
/// \param[in]  arg    : the input argument to convert.
/// \param[out] val    : the converted value to output.
///
/// \return a flag that tell us if every thing is ok, if the result can be used.
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


/// \brief Print usage recommandation for this binary.
///
/// \param[in]  argc : the exiting flag tell us to exit without doing any job.
/// \param[in]  arg    : the input argument to convert.
void usage(const int argc, const char *args[])
{
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "argc    : " << argc                       << std::endl;

  for (int i = 0; i < argc; ++i)
    std::cout << "args[" << i << "] : " << args[i] << std::endl;

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "usage: kmean1d [image [k_center [n_times [watch_dog]]]]"
	    << std::endl;
  std::cout << "pbm image          (points to work with)" << std::endl;
  std::cout << "unsigned k_center  (number of centers)"   << std::endl;
  std::cout << "unsigned n_times   (number of launching)" << std::endl;
  std::cout << "unsigned watch_dog (convergence loop)"    << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}


/// \brief The main routine launchs the kmean optimised algoritm.
///
/// \param[in] image     : the color image.
/// \param[in] k_center  : the number of centers.
/// \param[in] n_times   : the number of launching.
/// \param[in] watch_dog : the number of iterations to manage the convergence.
///
/// All the parameters are optional, but we must respect strictly a
/// specific order. In fact, we can launch five programs with some
/// default parameters: demo(), demo(image), demo(image,k_center),
/// demo(image,k_center,n_times) and
/// demo(image,k_center,n_times,watch_dog).
int main(const int argc, const char *args[])
{
  std::string image("top");
  unsigned    k_center;
  unsigned    watch_dog;
  unsigned    n_times;
  bool        status = true;

  switch (argc)
  {
  case 5:  status = char_to_unsigned(status, args[4], watch_dog);
  case 4:  status = char_to_unsigned(status, args[3], n_times);
  case 3:  status = char_to_unsigned(status, args[2], k_center);
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
    case 3: demo(image, k_center); break;
    case 4: demo(image, k_center, n_times); break;
    case 5: demo(image, k_center, n_times, watch_dog); break;
    }
  }
  else
    usage(argc, args);

  return 0;
}
