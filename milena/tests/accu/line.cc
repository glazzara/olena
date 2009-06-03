// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/*! \file tests/accu/line.cc
 *
 * \brief Tests on mln::accu::line.
 */

#include <mln/accu/line.hh>
#include <mln/accu/min_h.hh>

#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>
#include <mln/debug/iota.hh>



int main()
{
  using namespace mln;
  using namespace mln::value;

  typedef image2d<unsigned char> I;
  I ima(1, 5);
  debug::iota(ima);

  I out(ima.domain());
  point2d p_start(0, 0);
  accu::line< accu::meta::min_h, 1 >(ima,
				     p_start, ima.ncols(),
				     0, // half_length
				     out);
  mln_assertion(out == ima);
}
