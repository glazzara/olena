// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/dump/save.hh>
#include <mln/value/label_16.hh>
#include <mln/core/alias/neighb2d.hh>

#include <scribo/table/erase.hh>

#include <scribo/primitive/extract/lines_h_discontinued.hh>
#include <scribo/primitive/extract/lines_v_discontinued.hh>

#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;
  using mln::value::label_16;

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Remove tables from a binary image.",
				"input.pbm output.pbm",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef value::label_16 V;
  typedef image2d<V> L;

  /// Extracting vertical and horizontal lines.
  V nhlines, nvlines;
  component_set<L>
    lbl_v = primitive::extract::lines_v_discontinued(input, c8(),
						     nvlines, 51, 8);
  component_set<L>
    lbl_h = primitive::extract::lines_h_discontinued(input, c8(),
						     nhlines, 51, 6);

  /// Reconstruct and erase tables.
  image2d<bool> input_notables
    = scribo::table::erase(input, lbl_h, lbl_v);

  io::pbm::save(input_notables, argv[2]);

}
