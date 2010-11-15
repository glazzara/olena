// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
/// \brief Sample code to use the red/green visualization.
///
/// \fixme There is a lot of routines in display_histo.hh, but only
/// two of them have been call here. It's a poor job from me, but i've
/// the time to do it know.

#include <iostream>
#include <mln/accu/stat/histo3d_rgb.hh>
#include <mln/core/macros.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/data/compute.hh>
#include <mln/data/transform.hh>
#include <mln/display/display_histo.hh>
#include <mln/fun/v2v/rgb8_to_rgbn.hh>
#include <mln/img_path.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rgb.hh>

int main()
{
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::int_u<4>                        t_int_u4;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<4>                          t_rgb4;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image2d<t_int_u4>                      t_image2d_int_u4;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgb4>                        t_image2d_rgb4;
  typedef mln::image2d<bool>                          t_image2d_bool;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::fun::v2v::rgb8_to_rgbn<4>              t_rgb8_to_rgb4;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;

  t_image2d_rgb8                                      i0_input; // input rgb8
  t_image2d_rgb4                                      i1_input; // input rgbn
  t_histo3d                                           h1_input; // histo input
  t_image2d_int_u8                                    p1_histo1;// histo proj1
  t_image2d_rgb4                                      p1_histo2;// histo proj2
  t_rgb4                                              red(mln::literal::red);

  mln::io::ppm::load(i0_input, OLENA_IMG_PATH"/lena.ppm");

  i1_input  = mln::data::transform(i0_input, t_rgb8_to_rgb4());
  h1_input  = mln::data::compute(t_histo3d_fun(), i1_input);
  p1_histo1 = mln::display::display_histo3d_unsigned(h1_input);
  p1_histo2 = mln::display::display3_histo3d_unsigned(h1_input, red);

  mln::io::pgm::save(p1_histo1, "proj1.pgm");
  mln::io::ppm::save(p1_histo2, "proj2.ppm");

  return 0;
}
