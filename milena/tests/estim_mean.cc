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

/*! \file tests/estim_mean.cc
 *
 * \brief Tests on mln::estim::mean.
 */

#include <mln/core/image2d_b.hh>
#include <mln/value/int_u8.hh>

#include <mln/debug/iota.hh>
#include <mln/estim/mean.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d_b<int_u8> ima(3, 3);
  debug::iota(ima);
  // 1 2 3
  // 4 5 6
  // 7 8 9
  mln_assertion(estim::mean(ima) == 5);

  // FIXME: Add example on accu::mean used several times.
}
