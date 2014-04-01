// Copyright (C) 2007, 2008, 2009, 2014 EPITA Research and Development
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

/// \brief Exercise mln::pw::value.

#include <mln/core/image/image2d.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>

int main()
{
  using namespace mln;

  image2d<int> ima(3, 3);
  data::fill(ima, 0);

  point2d p(1, 1);
  ima(p) = 51;
  mln_assertion((pw::value(ima) == pw::cst(51))(p));
  mln_assertion((pw::value(ima) == 51)(p));
}
