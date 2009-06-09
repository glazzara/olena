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

#include <iostream>

#include <mln/core/image/image2d.hh>

#include <mln/util/array.hh>

#include <mln/io/txt/save.hh>
#include <mln/io/pbm/load.hh>

#include <mln/value/label_16.hh>

#include <mln/core/alias/neighb2d.hh>

#include <scribo/extract/primitive/objects.hh>

#include <scribo/text/grouping/group_with_several_left_links.hh>
#include <scribo/text/grouping/group_with_several_right_links.hh>
#include <scribo/text/grouping/group_from_double_link.hh>
#include <scribo/filter/small_objects.hh>
#include <scribo/filter/thin_objects.hh>
#include <scribo/text/recognition.hh>

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

  if (argc != 2)
    return usage(argv, "Text extraction and recognition", "input.pbm",
		 args_desc, "The text is printed on the standard output.");

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  /// Extract text.
  typedef mln_ch_value_(image2d<bool>,value::label_16) lbl_t;
  value::label_16 nbboxes;
  object_image(lbl_t)
    objects = scribo::extract::primitive::objects(input, c8(), nbboxes);

  /// Filter non interesting objects
  objects = filter::small_objects(objects, 4);
  objects = filter::thin_objects<lbl_t>(objects, 2);

  /// Group objects.
  mln::util::array<unsigned> left_link
	= text::grouping::group_with_several_left_links<lbl_t>(objects, 30);
  mln::util::array<unsigned> right_link
	= text::grouping::group_with_several_right_links(objects, 30);
  objects = text::grouping::group_from_double_link(objects, left_link, right_link);

  /// Try to recognize text in grouped objects.
  scribo::text::recognition(objects, "fra");

  trace::exiting("main");
}
