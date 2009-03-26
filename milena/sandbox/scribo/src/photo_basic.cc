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

#include <mln/essential/2d.hh>

#include <scribo/text/extract_bboxes.hh>
#include <scribo/text/grouping/group_with_single_left_link.hh>
#include <scribo/text/grouping/group_with_single_right_link.hh>
#include <scribo/text/grouping/group_from_double_link.hh>
#include <scribo/filter/small_components.hh>
#include <scribo/filter/large_components.hh>
#include <scribo/filter/thin_bboxes.hh>
#include <scribo/filter/thick_bboxes.hh>
#include <scribo/util/text.hh>

#include <scribo/make/debug_filename.hh>
#include <scribo/debug/save_textbboxes_image.hh>
#include <scribo/debug/save_linked_textbboxes_image.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  scribo::make::internal::debug_filename_prefix = "photo_basic";

  image2d<bool> input;
  io::pbm::load(input, argv[1]);
  logical::not_inplace(input);

  typedef scribo::util::text<image2d<value::label_16> > text_t;
  value::label_16 nbboxes;
  text_t textbboxes = text::extract_bboxes(input, c8(), nbboxes);

  text_t filtered_textbboxes
    = scribo::filter::small_components(textbboxes, 6);

  filtered_textbboxes
    = scribo::filter::thin_bboxes(filtered_textbboxes, 3);

  filtered_textbboxes
    = scribo::filter::thick_bboxes(filtered_textbboxes,
	  math::min(input.ncols(), input.nrows()) / 6);


  mln::util::array<unsigned> left_link
    = text::grouping::group_with_single_left_link(filtered_textbboxes, 30);
  mln::util::array<unsigned> right_link
    = text::grouping::group_with_single_right_link(filtered_textbboxes, 30);

  std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;
  scribo::debug::save_linked_textbboxes_image(input,
					      filtered_textbboxes, left_link, right_link,
					      literal::red, literal::cyan, literal::yellow,
					      literal::green,
					      scribo::make::debug_filename("links.ppm"));

 scribo::debug::save_textbboxes_image(input, filtered_textbboxes.bboxes(),
				       literal::red,
				       scribo::make::debug_filename("test_graph_filtered_text.ppm"));
  text_t grouped_textbboxes
    = text::grouping::group_from_double_link(filtered_textbboxes, left_link, right_link);

  std::cout << "AFTER - nbboxes = " << grouped_textbboxes.nbboxes().next() << std::endl;


  scribo::debug::save_textbboxes_image(input, grouped_textbboxes.bboxes(),
				       literal::red,
				       scribo::make::debug_filename("test_graph_grouped_text.ppm"));


}

