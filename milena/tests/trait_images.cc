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

/*! \file tests/trait_images.cc
 *
 * \brief Tests on mln::trait::images.
 */

#include <mln/core/image2d.hh>

#include <mln/core/sub_image.hh>
#include <mln/core/image_if.hh>
#include <mln/fun/p2b/chess.hh>


int main()
{
  using namespace mln;

  typedef image2d<int> I;

  std::cout << "image2d: ";
  mln::trait::image::print<I>(std::cout);

  typedef sub_image<I, box2d> Isub;
  std::cout << std::endl
	    << "sub_image< image2d >: ";
  mln::trait::image::print< Isub >(std::cout);

  std::cout << std::endl
	    << "image_if< image2d >: ";
  mln::trait::image::print< image_if<I, fun::p2b::chess_t> >(std::cout);


  std::cout << std::endl
	    << "image_if< sub_image< image2d > >: ";
  mln::trait::image::print< image_if<Isub, fun::p2b::chess_t> >(std::cout);
}
