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
#include <scribo/text/grouping/group_from_single_link.hh>

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

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nbboxes;
  util::array<box2d> textbboxes = text::extract_bboxes(input, c8(), nbboxes);

  {
    std::cout << "* Left grouping" << std::endl;
    util::array<unsigned> left_link
	= text::grouping::group_with_single_left_link(input,
						      c8(), nbboxes,
						      textbboxes, 30);

    std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;
    scribo::debug::save_linked_textbboxes_image(input,
						textbboxes, left_link,
						literal::red, literal::cyan,
						"test_single_left_link_linked.ppm");

    util::array< box<point2d> > grouped_textbboxes
	  = text::grouping::group_from_single_link(textbboxes, left_link);

    std::cout << "AFTER - nbboxes = " << grouped_textbboxes.nelements() << std::endl;

    scribo::debug::save_textbboxes_image(input, grouped_textbboxes,
					 literal::red,
					 "test_single_left_link_grouped_text.ppm");
  }

  {
    std::cout << "* Left grouping" << std::endl;
    util::array<unsigned> right_link
	= text::grouping::group_with_single_right_link(input,
						      c8(), nbboxes,
						      textbboxes, 30);

    std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;
    scribo::debug::save_linked_textbboxes_image(input,
						textbboxes, right_link,
						literal::red, literal::cyan,
						"test_single_right_link_linked.ppm");

    util::array< box<point2d> > grouped_textbboxes
	  = text::grouping::group_from_single_link(textbboxes, right_link);

    std::cout << "AFTER - nbboxes = " << grouped_textbboxes.nelements() << std::endl;

    scribo::debug::save_textbboxes_image(input, grouped_textbboxes,
					 literal::red,
					 "test_single_right_link_grouped_text.ppm");
  }


}

