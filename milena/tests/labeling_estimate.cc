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

/*! \file tests/labeling_foreground.cc
 *
 * \brief Test on mln::labeling::foreground.
 */

#include <mln/core/image2d_b.hh>
#include <mln/core/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/pw/all.hh>

#include <mln/io/pgm/load.hh>
#include <mln/labeling/foreground.hh>
#include <mln/labeling/estimate.hh>
#include <mln/accu/count.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d_b<int_u8> lena = io::pgm::load("../img/tiny.pgm"),
    out(lena.domain());

  unsigned n;
  labeling::foreground((pw::value(lena) > pw::cst(127)) | lena.domain(),
		       c4(), out, n);
  mln_assertion(n == 14);

  unsigned sum = 0;
  for (int_u8 i = 0; i <= n; ++i)
    sum += labeling::estimate< accu::count >(out, i);
  // FIXME: use of pix so that (center, mean) etc.
  mln_assertion(sum == lena.npoints());
}
