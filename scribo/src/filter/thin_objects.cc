// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/core/alias/neighb2d.hh>
#include <mln/data/convert.hh>
#include <mln/io/pbm/all.hh>
#include <mln/value/label_16.hh>

#include <scribo/filter/thin_objects.hh>
#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  { "min_thin", "The minimum object thinness value. Objects with bounding\
box hight or width less than or equal to this value are removed." },
  {0, 0}
};

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Filter too thin objects",
				"input.pbm min_thin output.pbm",
				args_desc,
				"A binary image.");

  trace::entering("main");

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  value::label_16 nobjects;
  typedef object_image(mln_ch_value_(I,value::label_16)) obj_ima_t;
  obj_ima_t objects
    = scribo::primitive::extract::objects(input, c8(), nobjects);

  obj_ima_t filtered = scribo::filter::thin_objects(objects, atoi(argv[2]));
  io::pbm::save(data::convert(bool(), filtered), argv[3]);

  trace::exiting("main");

}
