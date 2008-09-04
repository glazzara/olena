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

/*! \file tests/core/image/plain.cc
 *
 * \brief Test on mln::plain.
 */

#include <mln/core/image/image2d.hh>
#include <mln/core/image/plain.hh>

#include <mln/value/int_u8.hh>
#include <mln/level/compare.hh>
#include <mln/level/fill.hh>


#include <mln/debug/println.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  {
    plain< image2d<int_u8> > lena
      = io::pgm::load<int_u8>(MLN_IMG_DIR "/lena.pgm");

    image2d<int_u8> ima;
    image2d<int_u8> ima2;

    ima = lena;

    ima(make::point2d(0,0)) = 124;

    mln_assertion(ima != lena);

    ima2 = lena;
    ima = lena;

    mln_assertion(ima == ima2);

    ima(make::point2d(0,0)) = 124;
    mln_assertion(ima != ima2);

    ima = ima2;

    ima(make::point2d(0,0)) = 124;
    mln_assertion(ima == ima2);

  }

  {
    image2d<int_u8> lena(12,12);
    level::fill(lena, 42);

    point2d p1(0,0);
    point2d p2(5,4);

    image2d< image2d<int_u8> > ima(2,2);
    level::fill(ima, lena);
    // The 4 pixels of ima share the same data (holded by lena).
    // Then this update will affect the 4 pixels of ima.
    ima(p1)(p2) = 0;
    // ima(0,0)(p2) == ima(0,1)(p2) == ima(1,0)(p2) == ima(1,1)(p2) == 0

    image2d< plain< image2d<int_u8> > > ima_plain(2,2);
    level::fill(ima_plain, plain< image2d<int_u8> >(lena));
    // The 4 pixels of ima_plain are instances of plain<
    // image2d<int_u8> >.  So each of them hold their own data. Then
    // this update will affect just one pixel at the coordinate 0,0 of
    // ima_plain.
    ima_plain(p1)(p2) = 0;
    // ima_plain(0,0)(p2) == 0
    // ima_plain(0,1)(p2) == 0
    // ima_plain(1,0)(p2) == 0
    // ima_plain(1,1)(p2) == 0

  }

}
