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
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>

#include <scribo/extract/primitive/objects.hh>
#include <scribo/text/grouping/group_with_single_left_link.hh>
#include <scribo/text/grouping/group_with_single_right_link.hh>
#include <scribo/text/grouping/group_from_double_link.hh>
#include <scribo/filter/small_objects.hh>
#include <scribo/filter/thin_objects.hh>
#include <scribo/filter/thick_objects.hh>

#include <scribo/make/debug_filename.hh>
#include <scribo/debug/save_textbboxes_image.hh>
#include <scribo/debug/save_linked_textbboxes_image.hh>

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

  scribo::make::internal::debug_filename_prefix = "photo";

  if (argc != 3)
    return usage(argv, "Find text in a binarized photo.", "input.pbm output.ppm",
		 args_desc, "A color image where the text is highlighted.");

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef image2d<value::label_16> L;
  value::label_16 nobjects;
  object_image(L) objects = scribo::extract::primitive::objects(input, c8(), nobjects);

  object_image(L) filtered_objects
    = scribo::filter::small_objects(objects, 6);

  filtered_objects
    = scribo::filter::thin_objects(filtered_objects, 3);

  filtered_objects
    = scribo::filter::thick_objects(filtered_objects,
	  math::min(input.ncols(), input.nrows()) / 6);


  mln::util::array<unsigned> left_link
    = text::grouping::group_with_single_left_link(objects, 30);
  mln::util::array<unsigned> right_link
    = text::grouping::group_with_single_right_link(objects, 30);

  std::cout << "BEFORE - nobjects = " << nobjects << std::endl;
//  scribo::debug::save_linked_textbboxes_image(input,
//					      filtered_textbboxes, left_link, right_link,
//					      literal::red, literal::cyan, literal::yellow,
//					      literal::green,
//					      scribo::make::debug_filename("links.ppm"));
//
// scribo::debug::save_textbboxes_image(input, filtered_textbboxes.bboxes(),
//				       literal::red,
//				       scribo::make::debug_filename("test_graph_filtered_text.ppm"));
  object_image(L) grouped_objects
    = text::grouping::group_from_double_link(filtered_objects, left_link, right_link);

  std::cout << "AFTER - nobjects = " << grouped_objects.nlabels() << std::endl;


//  scribo::debug::save_textbboxes_image(input, grouped_textbboxes.bboxes(),
//				       literal::red,
//				       scribo::make::debug_filename("test_graph_grouped_text.ppm"));
//
  io::ppm::save(mln::labeling::colorize(value::rgb8(), grouped_objects, grouped_objects.nlabels()),
		argv[2]);
  trace::exiting("main");
}
