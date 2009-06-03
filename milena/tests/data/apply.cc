// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/data/apply.cc
 *
 * \brief Tests on mln::data::apply.
 */

#include <mln/core/image/image2d.hh>
#include <mln/data/apply.hh>
#include <mln/debug/iota.hh>
#include <mln/fun/v2v/saturate.hh>


int main()
{
  using namespace mln;

  image2d<int> ima(3, 3);
  int vs[3][3] = {
    { 2, 2, 3 },
    { 4, 5, 6 },
    { 6, 6, 6 }
  };

  image2d<int> ref(make::image(vs));
  debug::iota(ima);
  data::apply(ima, fun::v2v::saturate<int>(2, 6));
  box_fwd_piter_<point2d> p(ima.domain());
  for_all(p)
    mln_assertion(ima(p) == ref(p));
}
