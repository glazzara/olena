// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file tests/linear/log.cc
 *
 * \brief Tests on mln::linear.
 */

#include <mln/core/image2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/border/thickness.hh>
#include <mln/linear/log.hh>

#include <mln/estim/min_max.hh>
#include <mln/level/stretch.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 2;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");

  image2d<int> tmp(lena.domain());
  linear::LoG_5x5(lena, tmp);
  {
    int min, max;
    estim::min_max(tmp, min, max);
    // FIXME: This assertion fails.  (Maybe the input file has been
    // changed, but this line was not updated?)
    mln_assertion(min == -929 && max == 1458);
  }

  image2d<int_u8> out(lena.domain());
  level::stretch(tmp, out);
  io::pgm::save(out, "out.pgm");
  {
    int_u8 min, max;
    estim::min_max(out, min, max);
    mln_assertion(min == 0u && max == 255u);
  }
}
