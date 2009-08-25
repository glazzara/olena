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
#include <mln/util/graph.hh>
#include <mln/value/label_16.hh>
#include <mln/io/pbm/load.hh>
#include <mln/literal/colors.hh>

#include <scribo/core/object_image.hh>

#include <scribo/primitive/extract/objects.hh>
#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/link/with_graph.hh>
#include <scribo/primitive/group/from_graph.hh>
#include <scribo/filter/small_objects.hh>
#include <scribo/util/text.hh>

#include <scribo/make/debug_filename.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>

#include <scribo/tests/data.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  std::string img;
  if (argc < 2)
  {
    usage(argv[0]);
    img = SCRIBO_IMG_DIR "/text_to_group_and_clean.pbm";
  }
  else
    img = argv[1];

  scribo::make::internal::debug_filename_prefix = argv[0];

  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  typedef object_image(L) text_t;
  text_t text = primitive::extract::objects(input, c8(), nbboxes);

  mln::util::graph g = primitive::link::with_graph(text, 30);


  mln_assertion(nbboxes == 12u);
//  std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;
//  scribo::debug::save_linked_bboxes_image(input,
//					      text, g,
//					      literal::red, literal::cyan,
//					      "test_graph_left_linked.ppm");

  object_groups<L> groups = primitive::group::from_graph(text, g);

  text_t grouped_text = primitive::group::apply(text, groups);
//  std::cout << "AFTER - nbboxes = " << grouped_text.nbboxes().next() << std::endl;
//
//  scribo::debug::save_bboxes_image(input, grouped_text.bboxes(),
//				       literal::red,
//				       "test_graph_grouped_text.ppm");

  mln_assertion(grouped_text.nlabels() == 6u);

  text_t
    filtered_text = scribo::filter::small_objects(grouped_text, 20);

  mln_assertion(filtered_text.nlabels() == 2u);

//  scribo::debug::save_bboxes_image(input, filtered_text.bboxes(),
//				       literal::red,
//				       "test_graph_filtered_text.ppm");

}
