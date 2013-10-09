// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE).
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
/// \brief Exercise mln::interpolated using mln::fun::x2v::bilinear.

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/interpolated.hh>
#include <mln/fun/x2v/bilinear.hh>

#include <mln/algebra/vec.hh>

#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  float tab[] = {1.f,  3.f,  5.f,  7.f,
		 4.f,  7.f,  10.f, 13.f,
		 7.f,  11.f, 15.f, 19.f,
		 10.f, 15.f, 20.f, 25.f};
  image2d<float> ima = make::image2d(tab);

  interpolated<image2d<float>, fun::x2v::bilinear> inter(ima);

  algebra::vec<2, float> v1 = make::vec(2.3f, 0.6f);
  algebra::vec<2, float> v2 = make::vec(3.2f, 1.8f);

  debug::println(ima);

  std::cout << "inter(" << v1 << ") = " << inter(v1) << std::endl;
  std::cout << "inter(" << v2 << ") = " << inter(v2) << std::endl;
}
