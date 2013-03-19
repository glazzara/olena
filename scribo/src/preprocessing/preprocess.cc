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

#include <mln/core/image/image2d.hh>

#include <mln/data/convert.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>

#include <scribo/binarization/global_threshold_auto.hh>
#include <scribo/preprocessing/deskew.hh>
#include <scribo/filter/objects_small.hh>
#include <scribo/filter/objects_thin.hh>

#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pgm", "A gray-level image." },
  {0, 0}
};

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Generic image preprocessing",
				"input.pgm output.pbm",
				args_desc);

  mln_trace("main");

  typedef image2d<value::int_u8> I;
  I input;
  io::pgm::load(input, argv[1]);

  image2d<bool>
    input_bw = scribo::binarization::global_threshold_auto(input);

  value::label_16 nlabels;
  input_bw = scribo::filter::components_small(input_bw, c8(),
					      nlabels, 3);
  input_bw = scribo::filter::components_thin(input_bw, c8(),
					     nlabels, 1);

  image2d<value::int_u8>
    input_gl = data::convert(value::int_u8(), input_bw);
  input_gl = scribo::preprocessing::deskew(input_gl);
  input_bw = data::convert(bool(), input_gl);

  io::pbm::save(input_bw, argv[2]);


}
