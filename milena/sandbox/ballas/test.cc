// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#include <oln/core/2d/image2d.hh>
#include <oln/core/3d/image3d.hh>
#include <oln/core/1d/image1d.hh>
#include <oln/data/fill.hh>


using namespace oln;

int main(void)
{
  image1d<char> ima1d1(5);
  image1d<int> ima1d2(5);
  image1d<float> ima1d3(5);
  image1d<double> ima1d4(5);

  data::fill(ima1d1, 5);
  data::fill(ima1d2, 5);
  data::fill(ima1d3, 5);
  data::fill(ima1d4, 5);

  image2d<char> ima2d1(5, 5);
  image2d<int> ima2d2(5, 5);
  image2d<float> ima2d3(5, 5);
  image2d<double> ima2d4(5, 5);

  data::fill(ima2d1, 5);
  data::fill(ima2d2, 5);
  data::fill(ima2d3, 5);
  data::fill(ima2d4, 5);

  image3d<char> ima3d1(5, 5, 5);
  image3d<int> ima3d2(5, 5, 5);
  image3d<float> ima3d3(5, 5, 5);
  image3d<double> ima3d4(5, 5, 5);

  data::fill(ima3d1, 5);
  data::fill(ima3d2, 5);
  data::fill(ima3d3, 5);
  data::fill(ima3d4, 5);

  return 0;
}
