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

/*! \file tests/sub_image.cc
 *
 * \brief Tests on mln::sub_image.
 */

#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/inplace.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/level/fill.hh>
#include <mln/debug/println.hh>

#include <mln/core/image2d_b.hh>
#include <mln/core/point2d.hh>
#include <mln/debug/println.hh>
#include <mln/util/graph.hh>
#include <mln/io/ppm/save.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/core/image_if_value.hh>
#include <mln/debug/iota.hh>

namespace mln
{
  template <typename I, typename J>
  void
  color (Image<I>& ima_, Image<J>& out_)
  {
    I& ima = exact (ima_);
    J& out = exact (out_);

    level::fill (out, value::rgb8(255, 0, 0));

    {
      mln_piter(I) p (ima.domain ());
      for_all (p)
	{
	  out(p) = value::rgb8(ima(p));
	}
    }
  }
}

int main()
{
  using namespace mln;

  image2d_b<value::int_u8> ima(3,3);
  debug::iota(ima);
  image2d_b<value::rgb8> out(ima.domain ().bbox ());
  std::cout << ima.domain() << std::endl;
  color(inplace (ima | 6), out);
  io::ppm::save(out, "out.ppm");
  debug::println(out);
}
