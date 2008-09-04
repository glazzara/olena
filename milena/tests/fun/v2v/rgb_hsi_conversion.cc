// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file tests/fun/v2v/rgb_hsi_conversion.cc
/// \brief Test RGB-to-HSI conversion.

#include <cmath>

#include <mln/value/hsi.hh>
#include <mln/fun/v2v/rgb_to_hsi.hh>

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/level/transform.hh>

#include "tests/data.hh"


template <typename I1, typename I2>
float rms(const mln::Image<I1>& ima1_, const mln::Image<I2>& ima2_)
{
  const I1& ima1 = exact(ima1_);
  const I2& ima2 = exact(ima2_);

  mln_precondition(ima1.has_data() && ima2.has_data());

  double sum = 0, nb = 0;

  mln_piter(I1) p(ima1.domain());
  for_all(p)
  {
    mln_value(I1) c1 = ima1(p);
    mln_value(I2) c2 = ima2(p);
    double
      distred = c2.red() - c1.red(),
      distgreen = c2.green() - c1.green(),
      distblue = c2.blue() - c1.blue();

    ++nb;
    sum += distred * distred + distblue * distblue + distgreen * distgreen;
  }

  if (nb == 0)
    return 0;

  return std::sqrt(sum / nb);
}


int main()
{
  using namespace mln;

  image2d<value::rgb8> lena;
  io::ppm::load(lena, MLN_IMG_DIR "/lena.ppm");

  image2d<value::hsi_f> lena_hsi = level::transform(lena,
						      fun::v2v::f_rgb_to_hsi_f);

  image2d<value::rgb8> lena_rgb = level::transform(lena_hsi,
						   fun::v2v::f_hsi_to_rgb_3x8);

  double err = rms(lena, lena_rgb);
  std::cout << "err: " << err << std::endl;
}

