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
/// \brief Minimal code for building HSL 3d image histogram version.
//
/// \fixme This code should compile but it doesn't.
///

#include <mln/accu/stat/histo3d_hsl.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/data/compute.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_hsl.hh>
#include <mln/img_path.hh>
#include <mln/io/ppm/load.hh>
#include <mln/value/hsl.hh>
#include <mln/value/rgb8.hh>

int main()
{
  typedef mln::value::rgb8                    t_rgb;
  typedef mln::value::hsl_f                   t_hsl;
  typedef mln::fun::v2v::f_rgb_to_hsl_<t_hsl> t_rgb_to_hsl;
  mln::image2d<t_rgb>                         img_rgb;
  mln::image2d<t_hsl>                         img_hsl;
  mln::image3d<unsigned>                      histo;

  mln::io::ppm::load(img_rgb, OLENA_IMG_PATH"/lena.ppm");
  img_hsl = mln::data::transform(img_rgb, t_rgb_to_hsl());
  //histo =mln::data::compute(mln::accu::meta::stat::histo3d_hsl<7>(), img_hsl);
  //histo =mln::data::compute(mln::accu::stat::histo3d_hsl<7,t_hsl>(), img_hsl);
  mln::accu::stat::histo3d_hsl<7,t_hsl>();
  return 0;
}
