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

/*! \file tests/labeling/level.cc
 *
 * \brief Test on mln::labeling::level.
 */

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>

#include <mln/accu/count.hh>
#include <mln/accu/compute.hh>
#include <mln/labeling/level.hh>
#include <mln/level/paste.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/image_if.hh>

#include <mln/debug/println.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena = io::pgm::load<int_u8>(MLN_IMG_DIR "/tiny.pgm");
  image2d<bool> lvl(lena.domain());

  unsigned n, npixels = 0;
  for (unsigned l = 0; l <= 255; ++l)
    {
      image2d<unsigned> labels = labeling::level(lena, l, c4(), n);
      unsigned npix =
	accu::compute<accu::count>(labels | (pw::value(labels) != pw::cst(0u)));
      npixels += npix;
    }
  mln_assertion(npixels == lena.nsites());
}
