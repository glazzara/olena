// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/border/mirror.cc
 *
 * \brief Tests on mln::border::mirror.
 */

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/border/mirror.hh>

#include <mln/debug/println_with_border.hh>

using namespace mln;

int
main (void)
{
  {
    image1d<int> im(2, 3);
    debug::iota(im);
    border::mirror(im);
    mln_assertion(im[0] == 2);
    mln_assertion(im[1] == 2);
    mln_assertion(im[2] == 1);
    mln_assertion(im[3] == 1);
    mln_assertion(im[4] == 2);
    mln_assertion(im[5] == 2);
    mln_assertion(im[6] == 1);
    mln_assertion(im[7] == 1);
  }


  image2d<int> ima(2, 3, 2);

  debug::iota(ima);
  border::mirror(ima);

  mln_assertion(ima[ 0] == 1);
  mln_assertion(ima[ 1] == 1);
  mln_assertion(ima[ 2] == 4);
  mln_assertion(ima[ 3] == 5);
  mln_assertion(ima[ 4] == 6);
  mln_assertion(ima[ 5] == 3);
  mln_assertion(ima[ 6] == 3);
  mln_assertion(ima[ 7] == 1);
  mln_assertion(ima[ 8] == 1);
  mln_assertion(ima[ 9] == 1);
  mln_assertion(ima[10] == 2);
  mln_assertion(ima[11] == 3);
  mln_assertion(ima[12] == 3);
  mln_assertion(ima[13] == 3);
  mln_assertion(ima[14] == 2);
  mln_assertion(ima[15] == 1);
  mln_assertion(ima[16] == 1);
  mln_assertion(ima[17] == 2);
  mln_assertion(ima[18] == 3);
  mln_assertion(ima[19] == 3);
  mln_assertion(ima[20] == 2);
  mln_assertion(ima[21] == 5);
  mln_assertion(ima[22] == 4);
  mln_assertion(ima[23] == 4);
  mln_assertion(ima[24] == 5);
  mln_assertion(ima[25] == 6);
  mln_assertion(ima[26] == 6);
  mln_assertion(ima[27] == 5);
  mln_assertion(ima[28] == 4);
  mln_assertion(ima[29] == 4);
  mln_assertion(ima[30] == 4);
  mln_assertion(ima[31] == 5);
  mln_assertion(ima[32] == 6);
  mln_assertion(ima[33] == 6);
  mln_assertion(ima[34] == 6);
  mln_assertion(ima[35] == 4);
  mln_assertion(ima[36] == 4);
  mln_assertion(ima[37] == 1);
  mln_assertion(ima[38] == 2);
  mln_assertion(ima[39] == 3);
  mln_assertion(ima[40] == 6);
  mln_assertion(ima[41] == 6);
}

