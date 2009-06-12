// Copyright (C) 2009 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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
#include <mln/labeling/colorize.hh>
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
#include <mln/fun/v2v/wrap.hh>
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
double
bboxes_height_mean(const T& bboxes)
{
  double mean = 0.;
  unsigned count = 0;

  for (unsigned i = 1; i < bboxes.nelements(); i++)
  {
    mln_VAR(s, bboxes[i].pmax() - bboxes[i].pmin());
    if (s[1] < 10 || s[1] > 300)
      continue;

    mean += s[1];
    count++;
  }

  mean /= static_cast<double>(count);

  return mean;
}


template <typename I>
scribo::util::text<I>
group_from_line_array(const scribo::util::text<I>& text,
		      mln::util::array<unsigned>& array)
{
  /// Accumulator array
  mln::util::array< accu::shape::bbox<mln_site(I)> > tboxes;
  tboxes.resize(text.bboxes().nelements());

  /// Adding bounding boxes to accumulator
  for (unsigned label = 0; label < array.size(); ++label)
    for (unsigned i = label; i < array.size(); i++)
    {
      if (label != array[i])
	continue;

      tboxes[label].take(text.bbox(i));
    }

  fun::i2v::array<unsigned> f;
  convert::from_to(array, f);

  /// Finding valid bounding boxes
  mln::util::array< box<mln_site(I)> > bresult;
  bresult.append(box<mln_site(I)>());
  for_all_components(i, tboxes)
    if (tboxes[i].is_valid())
      bresult.append(tboxes[i]);

  /// Labelizing (FIXME)
  mln_value(I) new_nbboxes;
  I new_lbl = labeling::relabel(text.label_image(),
                                text.nbboxes(),
                                mln::make::relabelfun(f, text.nbboxes(),
                                                      new_nbboxes));

  scribo::util::text<I> result(bresult, new_lbl, new_nbboxes);
  return result;
}


int main(int argc, char* argv[])
{
  if (argc != 3)
    return usage(argv[0]);

  std::cout << "# Line detection algorithm (boxes)" << std::endl << std::endl;

  scribo::make::internal::debug_filename_prefix = "extract_text_double_link";

  /// Loading image
  image2d<bool> input;
  io::pbm::load(input, argv[1]);
  logical::not_inplace(input);

  /// Extract boxes
  value::label_16 nbboxes;
  scribo::util::text<image2d<value::label_16> >
    text = text::extract_bboxes(input, c8(), nbboxes);  
  text = filter::small_components(text,4);

  /// Grouping components
  mln::util::array<unsigned>
    left_link  = text::grouping::group_with_single_left_link(text, 30),
    right_link = text::grouping::group_with_single_right_link(text, 30);

  std::cout << "* Before validation: " << nbboxes << " boxes" << std::endl;


  scribo::debug::save_linked_textbboxes_image(input,
					      text, left_link, right_link,
					      literal::red,
					      literal::cyan,
					      literal::yellow,
					      literal::green,
					      scribo::make::debug_filename("links.ppm"));


  // Validation
  scribo::util::text< scribo_image > grouped_text
    = text::grouping::group_from_double_link(text, left_link, right_link);

  std::cout << "* After validation: " << grouped_text.bboxes().nelements()
	    << " boxes" << std::endl;

  /// Save grouped text image
  io::pgm::save(data::transform(grouped_text.label_image(),
				 fun::v2v::wrap<value::int_u8>()),
		"words.pgm");

  mln_VAR(b, grouped_text.bboxes());
  mln_VAR(m, grouped_text.mass_centers());

  // Word height mean
  double word_height_mean = bboxes_height_mean(b);
  std::cout << "* Word height mean: " << word_height_mean << std::endl;


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

      double component_y_diff = mln::math::abs(m[j][0] - m[i][0]);

      if (component_y_diff < 0.3 * word_height_mean)
	lines[j] = lines[i];
    }

  scribo::util::text< scribo_image > grouped_lines
    = group_from_line_array(grouped_text, lines);

  std::cout << "* Line number: " << grouped_lines.bboxes().nelements()
	    << std::endl;

  scribo::debug::save_textbboxes_image(input, grouped_text.bboxes(),
				       literal::red,
				       scribo::make::debug_filename("boxes.ppm"));

  /// Does not seem to work (FIXME)
  io::pgm::save(data::transform(grouped_lines.label_image(),
				 fun::l2l::wrap<value::int_u8>()),
		"fixme.ppm");

  /// Alternative save
  scribo::debug::save_textbboxes_image(input, grouped_lines.bboxes(),
				       literal::red,
				       argv[2]);
}
