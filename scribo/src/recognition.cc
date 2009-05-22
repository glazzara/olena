// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/labeling/colorize.hh>
#include <mln/labeling/wrap.hh>

#include <mln/util/timer.hh>
#include <mln/util/array.hh>

#include <mln/io/txt/save.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/label_16.hh>

#include <mln/core/alias/neighb2d.hh>

#include <scribo/text/extract_bboxes.hh>
#include <scribo/text/grouping/group_with_several_left_links.hh>
#include <scribo/text/grouping/group_with_several_right_links.hh>
#include <scribo/debug/save_linked_textbboxes_image.hh>
#include <scribo/text/grouping/group_from_double_link.hh>
#include <scribo/filter/small_components.hh>
#include <scribo/filter/thin_bboxes.hh>
#include <scribo/text/recognition.hh>

#include <scribo/debug/save_textbboxes_image.hh>
#include <scribo/make/debug_filename.hh>

#include <scribo/preprocessing/unskew.hh>


int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> <output.txt>" << std::endl;
  return 1;
}


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc < 3)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = argv[2];

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  input = preprocessing::unskew(input);

  /// Extract text.
  value::label_16 nbboxes;
  scribo::util::text<image2d<value::label_16> > text
      = text::extract_bboxes(input, c8(), nbboxes);
  text = filter::small_components(text,4);
  text = filter::thin_bboxes(text,2);
  mln::util::array<unsigned> left_link
	= text::grouping::group_with_several_left_links(text, 30);
  mln::util::array<unsigned> right_link
	= text::grouping::group_with_several_right_links(text, 30);
  text = text::grouping::group_from_double_link(text, left_link, right_link);

  io::ppm::save(mln::labeling::colorize(value::rgb8(),
			labeling::wrap(text.label_image())),
		scribo::make::debug_filename("lbl_color.ppm"));
  io::pgm::save(labeling::wrap(text.label_image()),
      scribo::make::debug_filename("lbl.pgm"));

  image2d<char> ima_txt = scribo::text::recognition(text, "fra");

  io::txt::save(ima_txt, argv[2]);
}

