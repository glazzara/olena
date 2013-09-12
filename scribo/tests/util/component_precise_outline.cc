// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/io/pgm/load.hh>

#include <scribo/util/component_precise_outline.hh>

#include <mln/draw/line.hh>
#include "tests/data.hh"

int main()
{
  using namespace mln;
  using namespace scribo;

  static const point2d ref_compress[] = {
    point2d(7,6), point2d(7,5),
    point2d(6,5), point2d(6,6),
    point2d(5,6), point2d(5,8),
    point2d(6,8), point2d(6,9),
    point2d(7,9), point2d(7,8),
    point2d(8,8), point2d(8,6)
  };


  static const point2d ref_no_compress[] = {
    point2d(7,6), point2d(7,5),
    point2d(6,5), point2d(6,6),
    point2d(5,6), point2d(5,7),
    point2d(5,8), point2d(6,8),
    point2d(6,9), point2d(7,9),
    point2d(7,8), point2d(8,8),
    point2d(8,7), point2d(8,6)
  };

  static const point2d ref_lbl_no_compress[] = {
    point2d(7,6), point2d(7,5),
    point2d(6,5), point2d(6,6),
    point2d(5,6), point2d(5,7),
    point2d(5,8), point2d(6,8),
    point2d(6,9), point2d(7,9),
    point2d(7,8), point2d(8,8),
    point2d(8,7), point2d(8,6)
  };

  static const point2d ref_lbl_compress[] = {
    point2d(7,6), point2d(7,5),
    point2d(6,5), point2d(6,6),
    point2d(5,6), point2d(5,8),
    point2d(6,8), point2d(6,9),
    point2d(7,9), point2d(7,8),
    point2d(8,8), point2d(8,6)
  };


  // Compute an outline of a single object.
  // Do compress output points.
  {
    std::string f = SCRIBO_IMG_DIR "/single_object.pbm";

    image2d<bool> input;
    io::pbm::load(input, f);

    p_array<point2d> par = scribo::util::component_precise_outline(input);

    for (unsigned i = 0; i < par.nsites(); ++i)
      mln_assertion(par[i] == ref_compress[i]);
  }

  // Compute an outline of a single object.
  // Do NOT compress output points.
  {
    std::string f = SCRIBO_IMG_DIR "/single_object.pbm";

    image2d<bool> input;
    io::pbm::load(input, f);

    p_array<point2d> par = scribo::util::component_precise_outline(input, true, false);

    for (unsigned i = 0; i < par.nsites(); ++i)
      mln_assertion(par[i] == ref_no_compress[i]);
  }


  // Compute an outline of a labeled object among others.
  // Do NOT compress output points.
  {
    std::string f = SCRIBO_IMG_DIR "/several_objects.pgm";

    image2d<value::int_u8> input;
    io::pgm::load(input, f);

    p_array<point2d> par = scribo::util::component_precise_outline(input, 1, false);

    for (unsigned i = 0; i < par.nsites(); ++i)
      mln_assertion(par[i] == ref_lbl_no_compress[i]);
  }

  // Compute an outline of a labeled object among others.
  // Do compress output points.
  {
    std::string f = SCRIBO_IMG_DIR "/several_objects.pgm";

    image2d<value::int_u8> input;
    io::pgm::load(input, f);

    p_array<point2d> par = scribo::util::component_precise_outline(input, 1, true);

    for (unsigned i = 0; i < par.nsites(); ++i)
      mln_assertion(par[i] == ref_lbl_compress[i]);
  }
}
