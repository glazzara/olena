// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/dump/save.hh>
#include <mln/value/label_16.hh>
#include <mln/core/alias/neighb2d.hh>

#include <scribo/table/extract.hh>

#include <scribo/extract/primitive/lines_h_discontinued.hh>
#include <scribo/extract/primitive/lines_v_discontinued.hh>

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
    return usage(argv, "Remove tables from a binary image.",
		 "input.pbm output.pbm",
		 args_desc, "A binary image without tables.");

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef image2d<label_16> lbl_t;

  label_16 nhlines, nvlines;
  object_image(lbl_t)
    lbl_v = extract::primitive::lines_v_discontinued(input, c8(),
						     nvlines, 51, 8);
  object_image(lbl_t)
    lbl_h = extract::primitive::lines_h_discontinued(input, c8(),
						     nhlines, 51, 6);

  image2d<bool> input_notables
    = scribo::table::erase(input,
			   mln::make::couple(lbl_v.bboxes(),lbl_h.bboxes()));
  io::pbm::save(input_notables, argv[2]);

  trace::exiting("main");
}
