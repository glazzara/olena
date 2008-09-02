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

/*! \file tests/morpho/contrast.cc
 *
 * \brief Test on mln::morpho::contrast.
 */

#include <mln/core/image2d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_s.hh>

#include <mln/morpho/contrast.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  win::rectangle2d rect(3, 3);
  border::thickness = 1;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/tiny.pgm");
  /* FIXME: Have dilation take the border into account (as erosion
     does), and fill it with min_value(V), sot that the post-condition
     in opening works.  */
  image2d<int_u8> out = morpho::contrast(lena, rect);
  io::pgm::save(out, "out.pgm");

  {
    // Self-duality test.
    // FIXME: Likewise.
    image2d<int_u8> out_ =
      morpho::complementation( morpho::contrast( morpho::complementation(lena),
						 rect )
			       );
    mln_assertion(out_ == out);
  }

}
