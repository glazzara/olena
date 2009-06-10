// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/accu/line.hh>
#include <mln/accu/stat/min_h.hh>

#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>
#include <mln/debug/iota.hh>



int main()
{
  using namespace mln;
  using namespace mln::value;

  typedef image2d<unsigned char> I;
  I ima(1, 5);
  debug::iota(ima);

  I out(ima.domain());
  point2d p_start(0, 0);
  accu::line< accu::meta::stat::min_h, 1 >(ima,
				     p_start, ima.ncols(),
				     0, // half_length
				     out);
  mln_assertion(out == ima);
}
