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

/*! \file tests/fast_median.cc
 *
 * \brief Test on mln::level::fast_median.
 */

#include <mln/core/image2d_b.hh>
#include <mln/core/win/rectangle2d.hh>

#include <mln/io/load_pgm.hh>
#include <mln/io/save_pgm.hh>

#include <mln/value/int_u8.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/level/fast_median.hh>

#include <mln/core/dpoints_pixter.hh>
#include <mln/core/pixel.hh>


namespace mln
{

  template <typename I, typename W>
  void test(I& input, const W& win)
  {
    mln_point(I) p;
    p.row() = p.col() = 1;

    {
      mln_qixter(I, W) qix(input, win, p);
      for_all(qix)
	std::cout << qix.val() << ' ';
      std::cout << " :  " << qix.center_val() << std::endl;
    }

    {
      pixel<I> pix(input, p);
      mln_qixter(I, W) qix(input, win, pix);
      for_all(qix)
	std::cout << qix.val() << ' ';
      std::cout << " :  " << qix.center_val() << std::endl;
    }
  }

}



int main()
{
  using namespace mln;
  using value::int_u8;

//   {
//     win::rectangle2d rect(3, 3);
//     border::thickness = 4;
//     image2d_b<int_u8> ima(3, 3);
//     debug::iota(ima);
//     debug::println(ima);
//     test(ima, rect);
//   }


  {
    win::rectangle2d rect(51, 51);
    border::thickness = 52;
    
    image2d_b<int_u8>
      lena = io::load_pgm("../img/lena.pgm"),
      out(lena.domain());
    
    level::fast_median(lena, rect, out);
    io::save_pgm(out, "out.pgm");
  }

}
