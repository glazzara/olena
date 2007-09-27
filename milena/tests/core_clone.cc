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

/*! \file tests/core_clone.cc
 *
 * \brief Tests on mln::clone.
 */

#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

#include <mln/core/clone.hh>



int main()
{
  using namespace mln;
  image2d_b<int> ima(3, 3, 51);
  debug::iota(ima);

  {
    box2d b = make::box2d(2,2);
    std::cout << border::get( clone(ima | b) ) << std::endl;

    debug::println(ima | b);
    debug::println(clone(ima | b));
  }

  {
    image2d_b<int> ima_ = clone(ima);
    std::cout << ima_.border() << std::endl;
    ima_(make::point2d(1,1)) = 51;
    debug::println(ima);
  }
  
  {
    image2d_b<int> ima_( ima );
    ima_(make::point2d(1,1)) = 51;
    debug::println(ima);
  }

}
