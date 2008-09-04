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

/*! \file tests/accu/max.cc
 *
 * \brief Tests on mln::accu::max.
 */

#include <mln/core/image/image2d.hh>
#include <mln/debug/iota.hh>

#include <mln/accu/nil.hh>
#include <mln/accu/max.hh>
#include <mln/accu/p.hh>
#include <mln/accu/v.hh>
#include <mln/accu/compute.hh>

#include <mln/level/compute.hh>


int main()
{
  using namespace mln;
  image2d<int> ima(3, 3);
  debug::iota(ima);
  mln_assertion(level::compute< accu::max >(ima) == 9);
  mln_assertion(level::compute< accu::max_<int> >(ima) == 9);

  accu::compute< accu::nil >(ima); // No-op.

  // FIXME : what's the difference between
  //  accu::compute< accu::max >(ima);

  mln_assertion( accu::compute< accu::val<accu::max> >(ima) == 9);

//   std::cout << accu::compute< accu::max >(ima)
// 	    << std::endl;
}
