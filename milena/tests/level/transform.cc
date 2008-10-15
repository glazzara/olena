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

/*! \file tests/level/transform.cc
 *
 * \brief Tests on mln::level::transform
 */

#include <cmath>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/flat_image.hh>
#include <mln/core/image/image_if.hh>

#include <mln/level/transform.hh>
#include <mln/debug/iota.hh>
#include <mln/core/var.hh>

#include <mln/fun/p2b/chess.hh>

struct mysqrt : mln::Function_v2v<mysqrt>
{
  typedef unsigned short result;
  result operator()(unsigned short c) const
  {
    return result( std::sqrt(float(c)) );
  }
};



int main()
{
  using namespace mln;
  const unsigned size = 1000;

  {
    image2d<unsigned short> ima(size, size);
    image2d<unsigned short> out(size, size);

    (std::cout << "iota... ").flush();
    debug::iota(ima);
    std::cout << "done" << std::endl;

    (std::cout << "transform... ").flush();
    level::transform(ima, mysqrt(), out);
    std::cout << "done" << std::endl;

    (std::cout << "checking... ").flush();
    box_fwd_piter_<point2d> p(out.domain());
    for_all(p)
        mln_assertion((unsigned short)std::sqrt(ima(p)) == out(p));
    std::cout << "done" << std::endl;
  }

  {
    flat_image<short, box2d> ima(5, make::box2d(size, size));
    image2d<unsigned short> out(size, size);

    (std::cout << "fill... ").flush();
    level::fill_with_value(ima, 51);
    std::cout << "done" << std::endl;

    (std::cout << "transform... ").flush();
    level::transform(ima, mysqrt(), out);
    std::cout << "done" << std::endl;

    (std::cout << "checking... ").flush();
    box2d::piter p(out.domain());
    for_all(p)
      mln_assertion((unsigned short)std::sqrt(ima(p)) == out(p));
    std::cout << "done" << std::endl;
  }

  {
    typedef image2d<unsigned short> I;
    typedef image_if<I, fun::p2b::chess_t> II;

    I ima(size, size);
    I out(size, size);
    II ima_if = ima | fun::p2b::chess;

    level::fill_with_value(ima, 0);
    (std::cout << "iota... ").flush();
    debug::iota(ima);
    std::cout << "done" << std::endl;;

    (std::cout << "transform... ").flush();
    level::transform(ima_if, mysqrt(), out);
    std::cout << "done" << std::endl;

    (std::cout << "checking... ").flush();
    II::piter p(ima_if.domain());
    for_all(p)
      mln_assertion((unsigned short)std::sqrt(ima_if(p)) == out(p));
    std::cout << "done" << std::endl;
  }

}
