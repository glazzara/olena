// Copyright (C) 2009, 2014 EPITA Research and Development Laboratory (LRDE).
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

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/unproject_image.hh>

#include <mln/fun/v2v/projection.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>



int main()
{
  using namespace mln;

  image1d<int> ima(3);
  debug::iota(ima);

  debug::println(ima);
  std::cout << std::endl;

  fun::v2v::projection<point2d, 0> f;

  typedef unproject_image< image1d<int>,
                           box2d,
                           fun::v2v::projection<point2d, 0u> > ima__t;
  ima__t ima_ = unproject(ima, make::box2d(3, 3), f);
  debug::println(ima_);

  ima_(point2d(1,1)) = 9;
  debug::println(ima_);

  debug::println(ima);
}
