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
/// \brief Minimal code to use the optimized version of the kmean3d algorithm.
///

#include <iostream>
#include <mln/clustering/kmean3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb8_to_rgbn.hh>
#include <mln/img_path.hh>
#include <mln/io/ppm/load.hh>
#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>


int main()
{
  typedef mln::clustering::kmean3d<double,5> t_kmean;
  typedef mln::value::rgb8                   t_rgb8;
  typedef mln::value::rgb<5>                 t_rgb5;
  typedef mln::image2d<t_rgb8>               t_image2d_rgb8;
  typedef mln::image2d<t_rgb5>               t_image2d_rgb5;
  typedef mln::fun::v2v::rgb8_to_rgbn<5>     t_rgb8_to_rgb5;

  t_image2d_rgb8                             img_rgb8;
  t_image2d_rgb5                             img_rgb5;

  mln::io::ppm::load(img_rgb8, OLENA_IMG_PATH"/house.ppm");

  img_rgb5=mln::data::transform(img_rgb8, t_rgb8_to_rgb5());

  t_kmean                                    kmean(img_rgb5, 3);

  kmean.launch_n_times();

  return 0;
}
