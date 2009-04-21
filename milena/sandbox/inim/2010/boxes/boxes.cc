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
#include <mln/debug/colorize.hh>
#include <mln/debug/println.hh>

#include <scribo/text/extract_bboxes.hh>
#include <scribo/text/grouping/group_from_single_link.hh>
#include <scribo/text/grouping/group_with_single_left_link.hh>
#include <scribo/text/grouping/group_with_single_right_link.hh>
#include <scribo/debug/save_linked_textbboxes_image.hh>
#include <scribo/text/grouping/group_from_double_link.hh>
#include <scribo/filter/small_components.hh>

#include <scribo/debug/save_textbboxes_image.hh>
#include <scribo/make/debug_filename.hh>

#include <mln/logical/not.hh>
#include <mln/io/dump/save.hh>

#include <mln/io/pgm/save.hh>
#include <mln/fun/l2l/wrap.hh>

#include <mln/math/abs.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " input.pbm output.dump" << std::endl;
  return 1;
}

using namespace scribo;
using namespace mln;

typedef image2d<value::label_16> scribo_image;

template< typename T >
double vec_y_norm(const T& t)
{
  return mln::math::abs(t[1] / t[0]);
}

template< typename T >
double bboxes_y_mean(const T& bboxes)
{
  double mean = 0.;

  for (unsigned i = 1; i < bboxes.nelements(); i++)
  {
    mln_VAR(s, bboxes[i].pmax() - bboxes[i].pmin());
    mean += (s[1] / s[0]);
  }

  mean /= static_cast<double>(bboxes.nelements());

  return mean;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = "extract_text_double_link";

  std::cout << "Debug 1" << std::endl;

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  std::cout << "Debug 2" << std::endl;

  logical::not_inplace(input);

  std::cout << "Debug 3" << std::endl;

  value::label_16 nbboxes;
  scribo::util::text<image2d<value::label_16> >
    text = text::extract_bboxes(input, c8(), nbboxes);

  std::cout << "Debug 4" << std::endl;

  text = filter::small_components(text,4);

  std::cout << "Debug 5" << std::endl;

  mln::util::array<unsigned>
    left_link  = text::grouping::group_with_single_left_link(text, 30),
    right_link = text::grouping::group_with_single_right_link(text, 30);

  std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;


  std::cout << "Debug 6" << std::endl;

  scribo::debug::save_linked_textbboxes_image(input,
					      text, left_link, right_link,
					      literal::red, literal::cyan, literal::yellow,
					      literal::green,
					      scribo::make::debug_filename("links.ppm"));

  std::cout << "Debug 7" << std::endl;

  // Validation.

  scribo::util::text< scribo_image > grouped_text
	= text::grouping::group_from_double_link(text, left_link, right_link);

  std::cout << "AFTER double grouping - nbboxes = " << grouped_text.bboxes().nelements() << std::endl;

  io::dump::save(grouped_text.label_image(), argv[2]);

  io::pgm::save(level::transform(grouped_text.label_image(),
				 fun::l2l::wrap<value::int_u8>()),
		"tmp.pgm");

  std::cout << "Debug 8" << std::endl;







  mln_VAR(b, grouped_text.bboxes());
  std::cout << "Bounding boxes: " << std::endl << b << std::endl;

  mln_VAR(m, grouped_text.mass_centers());
  std::cout << "Mass centers: " << std::endl << m << std::endl;

  // Mean of the y components of the bounding boxes
  double mean = bboxes_y_mean(b);
  std::cout << "Mean: " << mean << std::endl;

  mln::util::array< unsigned > lines(m.size());

  // Suppose that every component is on a different line
  for (unsigned i = 1; i < m.size(); i++)
    lines[i] = i;

  // Merge the lines if they are too close
  for (unsigned i = 1; i < m.size(); i++)
    for (unsigned j = i + 1; j < m.size(); j++)
    {
      if (i == j)
        continue;

      double y_norm_diff = vec_y_norm(m[j] - m[i]);
//       std::cout << "y_norm_diff: " << y_norm_diff << std::endl;
      if (y_norm_diff > 1000)
      {
        std::cout << "(" << i << "," << j << ")" << std::endl;
        continue;
      }
      if (y_norm_diff < mean * 2)
      {
        lines[j] = lines[i];
//         std::cout << "Associating " << i << " to " << j << std::endl;
      }
    }

  std::cout << "Lines: " << std::endl << lines << std::endl;

//   scribo::util::text< scribo_image > grouped_lines
// 	= text::grouping::group_from_single_link(text, lines);








  scribo::debug::save_textbboxes_image(input, grouped_text.bboxes(),
				       literal::red,
				       scribo::make::debug_filename("boxes.ppm"));

  std::cout << "Debug 9" << std::endl;
}

