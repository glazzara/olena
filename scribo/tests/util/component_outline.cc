// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

// \file

#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/io/pbm/load.hh>

#include <scribo/util/component_outline.hh>

#include "tests/data.hh"

int main()
{
  using namespace mln;
  using namespace scribo;

  static const point2d ref[] = {
    point2d(5,8), point2d(5,6),
    point2d(6,5), point2d(7,5),
    point2d(8,6), point2d(8,8),
    point2d(7,9), point2d(6,9)
  };

  std::string f = SCRIBO_IMG_DIR "/single_object.pbm";

  image2d<bool> input;
  io::pbm::load(input, f);

  p_array<point2d> par = scribo::util::component_outline(input, 0.2);

  for (unsigned i = 0; i < par.nsites(); ++i)
    mln_assertion(par[i] == ref[i]);

  return 0;
}
