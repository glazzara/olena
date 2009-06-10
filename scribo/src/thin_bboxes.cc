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

#include <mln/essential/2d.hh>
#include <scribo/text/extract_lines.hh>
#include <scribo/filter/thin_objects.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 1)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = "thin_bboxes";

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nlines;
  typedef scribo::util::text<image2d<value::label_16> > text_t;
  text_t lines = scribo::text::extract_lines(input, c8(), nlines);

  text_t filtered_lines = scribo::filter::thin_objects(lines, 5);

  scribo::debug::save_textbboxes_image(input, filtered_lines.bboxes(),
				       literal::red,
				       scribo::make::debug_filename("thickness_filter"));
}
