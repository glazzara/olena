// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file scribo/src/filter/small_objects.cc
///
/// Filter too small objects.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/level/convert.hh>
#include <mln/io/pbm/all.hh>
#include <mln/value/label_16.hh>

#include <scribo/filter/small_objects.hh>
#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  { "min_area", "The minimum object area value. Objects with an area less than \
or equal to this value are removed." },
  {0, 0}
};

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
    return usage(argv, "Filter too small objects", "input.pbm min_area output.pbm",
		 args_desc, "A binary image.");

  trace::entering("main");

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  value::label_16 nobjects;
  typedef object_image(mln_ch_value_(I,value::label_16)) obj_ima_t;
  obj_ima_t objects
    = scribo::extract::primitive::objects(input, c8(), nobjects);

  obj_ima_t filtered = scribo::filter::small_objects(objects, atoi(argv[2]));
  io::pbm::save(level::convert(bool(), filtered), argv[3]);

  trace::exiting("main");

}
