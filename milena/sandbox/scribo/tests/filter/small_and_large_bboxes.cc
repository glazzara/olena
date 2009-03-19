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
#include <scribo/text/grouping/group_with_multiple_links.hh>
#include <scribo/text/grouping/group_from_multiple_links.hh>
#include <scribo/filter/small_components.hh>

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

  if (argc < 1)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = argv[0];

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nbboxes;
  util::array<box2d> textbboxes = text::extract_bboxes(input, c8(), nbboxes);

  util::graph g = text::grouping::group_with_multiple_links(input,
							    c8(), nbboxes,
							    textbboxes, 30);

  std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;
  scribo::debug::save_linked_textbboxes_image(input,
					      textbboxes, g,
					      literal::red, literal::cyan,
					      "test_multiple_links_left_linked.ppm");

  util::array<box2d> grouped_textbboxes
    = text::grouping::group_from_multiple_links(textbboxes, g);

  std::cout << "AFTER - nbboxes = " << grouped_textbboxes.nelements() << std::endl;

  scribo::debug::save_textbboxes_image(input, grouped_textbboxes,
				       literal::red,
				       "test_multiple_links_grouped_text.ppm");

  util::array<box2d> filtered_textbboxes
    = scribo::filter::small_components(grouped_textbboxes, 6);

  scribo::debug::save_textbboxes_image(input, filtered_textbboxes,
				       literal::red,
				       "test_multiple_links_filtered_text.ppm");

}

