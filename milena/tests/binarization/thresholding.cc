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

/// \file tests/binarization/thresholding.cc
/// \brief Test on mln::binarization::threshold.

#include <mln/core/image2d.hh>
#include <mln/binarization/threshold.hh>
#include <mln/level/all.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/save.hh>

#include "tests/data.hh"

int main(int argc, char **)
{
  using namespace mln;
  using value::int_u8;
 
  { 
    image2d<int_u8> lena;
    io::pgm::load(lena, MLN_IMG_DIR "lena.pgm");
    // FIXME: argc?  Weird.
    io::pbm::save(binarization::threshold(lena, argc), "out1.pgm");
  }

  {
    image2d<int_u8> l;
    image2d<int> lena;
    io::pgm::load(l, MLN_IMG_DIR "img/lena.pgm");
    
    level::paste(l, lena);
    
    // FIXME: argc?  Weird.
    io::pbm::save(binarization::threshold(lena, argc), "out2.pgm");
  }
}
