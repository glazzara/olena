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

/*! \file tests/io/fits/fits.cc
 *
 * \brief Test on mln::io::fits::load.
 */

#include <mln/core/image/image2d.hh>

#include <mln/level/compare.hh>
#include <mln/debug/println.hh>

#include <mln/io/fits/load.hh>
#include <mln/io/pfm/save.hh>
#include <mln/io/pfm/load.hh>

#include "tests/data.hh"

int main()
{
  using namespace mln;
  {
    image2d<float> fits_in = io::fits::load(MLN_IMG_DIR "/test.fits");

    io::pfm::save(fits_in, "out.pfm");
    image2d<float> pfm = io::pfm::load("out.pfm");
    mln_assertion(pfm == fits_in);

    io::pfm::save(fits_in, "out2.pfm");
    image2d<float> pfm2 = io::pfm::load("out2.pfm");
    mln_assertion(fits_in == pfm2);
  }
}
