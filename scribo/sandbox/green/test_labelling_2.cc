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

#include <mln/clustering/kmean2d.hh>

#include <iostream>
#include <sstream>

#include <mln/img_path.hh>
#include <mln/pw/value.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/value_array.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb8_to_rgbn.hh>
#include <mln/fun/v2v/component.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/value/rg.hh>
#include <mln/fun/v2v/rgb_to_rg.hh>
#include <mln/fun/v2v/rg_to_rgb.hh>

#include <mln/accu/stat/histo2d.hh>
#include <mln/data/compute.hh>

#include <mln/arith/revert.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/elementary/opening.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/initialize.hh>
#include <mln/literal/colors.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/opening/structural.hh>

#include <mln/core/alias/window2d.hh>
#include <mln/win/disk2d.hh>


//
// Test kmean image processing chain.
// Ok pour la convergence.
// demander à Théo pour le temps (45 sec pour chaque launching [house,3 cl.])
//
void do_demo(const std::string& image,
	     const unsigned     k_center,
	     const unsigned     n_times,
	     const unsigned     watch_dog)
{
  typedef mln::clustering::kmean2d<double,8>          t_kmean;
  typedef mln::value::rg<8>                           t_rg8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rg8>                         t_image2d_rg8;
  typedef mln::fun::v2v::rgb_to_rg<8>                 t_rgb_to_rg;
  typedef mln::fun::v2v::rg_to_rgb<8>                 t_rg_to_rgb;

  t_image2d_rgb8                                      img_rgb8;
  t_image2d_rgb8                                      point_img_rgb8;
  t_image2d_rgb8                                      mean_img_rgb8;
  t_image2d_rgb8                                      mean_dbg_rgb8;
  t_image2d_rg8                                       img_rg8;

  // Read input.
  mln::io::ppm::load(img_rgb8, image.c_str());
  img_rg8   = mln::data::transform(img_rgb8, t_rgb_to_rg());

  // Call kmean.
  t_kmean kmean(img_rg8, k_center, watch_dog, n_times);

  mln::trace::quiet = false;
  kmean.launch_n_times();

  // Get outputs.
  t_kmean::t_point_img    point_img    = kmean.get_point();
  t_kmean::t_color_dbg    color_img    = kmean.get_color_dbg();
  t_kmean::t_mean_dbg     mean_img     = kmean.get_mean_dbg();
  t_kmean::t_label_dbg    label_img    = kmean.get_label_dbg();
  t_kmean::t_variance_cnv variance_cnv = kmean.get_variance_cnv();
  t_kmean::t_mean_cnv     mean_cnv     = kmean.get_mean_cnv();

  // Convert outputs.
  point_img_rgb8 = mln::data::transform(point_img, t_rg_to_rgb());
  mean_img_rgb8  = mln::data::transform(mean_img,  t_rg_to_rgb());

  mln::io::ppm::save(mean_img_rgb8,  "mean.ppm");
  mln::io::ppm::save(color_img,      "color.ppm");
  mln::io::pgm::save(label_img,      "label.pgm");
  mln::io::ppm::save(point_img_rgb8, "point.ppm");

  mln::io::plot::save_image_sh(mean_cnv,     "mean_cnv.sh");
  mln::io::plot::save_image_sh(variance_cnv, "variance_cnv.sh");
}


void demo(const std::string& image     = OLENA_IMG_PATH"/house.ppm",
	  const unsigned     k_center  = 3,
	  const unsigned     n_times   = 10,
	  const unsigned     watch_dog = 10)
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

mln::image2d<mln::value::rgb<8> >
merge(const mln::image2d<mln::value::rg<8> >&  input,
      const mln::image2d<mln::value::label_8>& label)
{
  mln::image2d<mln::value::rgb8> output;

  mln::initialize(output, input);

  mln_piter_(mln::image2d<mln::value::rg<8> >)  p_rg(input.domain());
  mln_piter_(mln::image2d<mln::value::rgb8>)    p_rgb(output.domain());

  for_all_2(p_rg, p_rgb)
  {
    if (0 < label(mln::point2d(input(p_rg).red(), input(p_rg).green())))
    {
      output(p_rgb).red()   = input(p_rg).red();
      output(p_rgb).green() = input(p_rg).green();
      output(p_rgb).blue()  = label(mln::point2d(input(p_rg).red(),
						 input(p_rg).green()));
    }
    else
      output(p_rgb) = mln::literal::red;
    //output(p_rgb) = mln::literal::black;
  }

  return output;
}




void usage(const int argc, const char *args[])
{
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "argc    : " << argc                       << std::endl;

  for (int i = 0; i < argc; ++i)
    std::cout << "args[" << i << "] : " << args[i] << std::endl;

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "usage: kmean2d [image [k_center [n_times [watch_dog]]]]"
	    << std::endl;
  std::cout << "pbm image          (points to work with)" << std::endl;
  std::cout << "unsigned k_center  (number of centers)"   << std::endl;
  std::cout << "unsigned n_times   (number of launching)" << std::endl;
  std::cout << "unsigned watch_dog (convergence loop)"    << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

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

//
// Regional maxima image processing chain.
//
void do_demo_regional2(const std::string& image,
		       const unsigned     k_center,
		       const unsigned     n_times,
		       const unsigned     watch_dog)
{
  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::rg<8>                           t_rg8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image2d<t_rg8>                         t_image2d_rg8;
  typedef mln::image2d<unsigned>                      t_histo2d;
  typedef mln::fun::v2v::rgb_to_rg<8>                 t_rgb_to_rg;
  typedef mln::fun::v2v::rg_to_rgb<8>                 t_rg_to_rgb;

  t_image2d_rgb8                                      img_rgb8;
  t_image2d_rgb8                                      output;
  t_image2d_rgb8                                      img_cast;
  t_image2d_rg8                                       img_rg8;
  t_histo2d                                           histo;
  t_histo2d                                           filtered;
  t_image2d_lbl8                                      label;
  t_lbl8                                              n_labels;


  // IMAGE LOADING PHASE
  mln::io::ppm::load(img_rgb8, image.c_str());
  img_rg8   = mln::data::transform(img_rgb8, t_rgb_to_rg());
  img_cast  = mln::data::transform(img_rg8,  t_rg_to_rgb());

  mln::io::ppm::save(img_cast,  "red_green.ppm");


  // HISTO COMPUTING AND FILTERING PHASE
  histo    = mln::data::compute(mln::accu::meta::stat::histo2d(), img_rg8);

  mln::io::plot::save_image_sh(histo, "histo.sh");

  // FILTERING
  // a) one pixel with c4 or c8
  // histo   = mln::morpho::elementary::opening(histo, mln::c4());

  // b) structural element with c4 or c8

  filtered   = mln::morpho::opening::structural(histo, mln::win_c8p());

/*
  // c) structural element with disk

  // 11 ==> trop peu
  // 21 ???
  // 31 ???
  // 51 ==> beaucoup trop (2 labels)
  mln::win::disk2d disk2d(21);

  histo    = mln::morpho::opening::structural(histo, disk2d);

*/
  mln::io::plot::save_image_sh(histo, "filtered.sh");


  // LABELING PHASE
  label    = mln::labeling::regional_maxima(filtered, mln::c8(), n_labels);
  label    = mln::morpho::elementary::dilation(label, mln::c8());
  mln::io::pgm::save(label, "label.pgm");


  // OUTPUT PHASE
  output   = merge(img_rg8, label);
  mln::io::ppm::save(output,  "merge.ppm");
}



