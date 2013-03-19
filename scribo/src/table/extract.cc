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
#include <mln/io/ppm/save.hh>
#include <mln/io/dump/save.hh>
#include <mln/value/label_16.hh>
#include <mln/labeling/colorize.hh>
#include <mln/value/rgb8.hh>

#include <scribo/table/extract.hh>

#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  { "output.dump", "A label image stored as milena dump image." },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Extract tables from a binary image.",
				"input.pbm output.dump output.ppm",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 ncells;
  image2d<value::label_16>
    tables = scribo::table::extract(input, ncells).first();

  io::ppm::save(mln::labeling::colorize(value::rgb8(), tables, ncells),
		argv[3]);

  io::dump::save(tables, argv[2]);

}
