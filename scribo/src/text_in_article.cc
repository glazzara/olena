// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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


#include <libgen.h>
#include <fstream>
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/dump/save.hh>

#include <mln/math/min.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/int_u16.hh>

#include <mln/draw/box.hh>
#include <mln/draw/line.hh>

#include <mln/data/convert.hh>

#include <mln/extension/adjust.hh>

#include <mln/accu/stat/median_h.hh>

#include <mln/labeling/colorize.hh>
#include <mln/labeling/relabel.hh>

//#include <scribo/draw/bounding_boxes.hh>
#include <scribo/draw/bounding_box_links.hh>

#include <scribo/primitive/extract/components.hh>

#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

#include <scribo/primitive/group/apply.hh>
// #include <scribo/primitive/group/from_double_link.hh>
#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/filter/object_links_bbox_h_ratio.hh>

#include <scribo/filter/objects_small.hh>
// #include <scribo/filter/objects_thin.hh>
// #include <scribo/filter/objects_thick.hh>

// #include <scribo/filter/object_groups_small.hh>

// #include <scribo/debug/decision_image.hh>
// #include <scribo/debug/save_bboxes_image.hh>
// #include <scribo/debug/save_bboxes_image.hh>
// #include <scribo/debug/save_linked_bboxes_image.hh>

#include <scribo/core/line_set.hh>
#include <scribo/io/xml/save_text_lines.hh>


#include <scribo/debug/usage.hh>

#include <scribo/make/debug_filename.hh>

//#include <scribo/text/recognition.hh>

#include <scribo/text/merging.hh>



const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'False' for object, 'True'\
for the background." },
  { "debug_dir", "Output directory for debug image" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3 && argc != 4)
    return scribo::debug::usage(argv,
				"Find text lines using left/right validation and display x-height in a binarized article.",
				"input.pbm out.ppm <debug_dir>",
				args_desc,
				"A color image. The following colors are used : dark blue for object bboxes, orange for single object bboxes, purple for group bboxes and light blue for x-height.");

  if (argc == 4)
    scribo::make::internal::debug_filename_prefix = argv[3];

  trace::entering("main");


  image2d<bool> input;
  mln::io::pbm::load(input, argv[1]);

  typedef value::label_16 V;
  typedef image2d<V> L;

  /// Finding components.
  std::cout << "Finding components..." << std::endl;
  V ncomponents;
  component_set<L>
    components = scribo::primitive::extract::components(input,
							c8(),
							ncomponents);

  /// First filtering.
  std::cout << "Filtering components..." << std::endl;
  component_set<L> filtered_components
    = scribo::filter::components_small(components, 6);

  {
    unsigned none = 0, ignored = 0;
    for_all_comps(i, filtered_components)
    {
      if (filtered_components.info(i).tag() == component::Ignored)
	++ignored;
      else
	++none;
    }
    std::cout << "stats - none = " << none << " - ignored = " << ignored << std::endl;

  }

  /// Linking potential objects
  std::cout << "Linking objects..." << std::endl;
  object_links<L> left_link
    = primitive::link::with_single_left_link_dmax_ratio(filtered_components, 2);
  object_links<L> right_link
    = primitive::link::with_single_right_link_dmax_ratio(filtered_components, 2);

  // Validating left and right links.
  object_links<L>
    merged_links = primitive::link::merge_double_link(filtered_components,
						      left_link,
						      right_link);

// #ifndef NOUT
//   if (argc == 4)
//   {
//     image2d<value::rgb8> output = data::convert(value::rgb8(), input);
//     scribo::draw::bounding_box_links(output,
// 				     merged_links,
// 				     literal::green);

//     mln::util::array<bool> drawn(static_cast<unsigned>(filtered_components.nelements()) + 1, 0);
//     for_all_comps(i, filtered_components)
//       if (filtered_components(i).tag() != component::Ignored)
//       {
// 	if (merged_links[i] == i && ! drawn(i))
// 	{
// 	  mln::draw::box(output, filtered_components(i).bbox(), literal::orange);
// 	  drawn[i] = true;
// 	}
// 	else
// 	{
// 	  mln::draw::box(output, filtered_components(i).bbox(), literal::blue);
// 	  mln::draw::box(output, filtered_components(merged_links[i]).bbox(), literal::blue);
// 	  drawn[i] = true;
// 	  drawn[merged_links[i]] = true;
// 	}
//       }

//     mln::io::ppm::save(output, scribo::make::debug_filename("links.ppm"));
//   }
// #endif

  // Remove links if bboxes have too different sizes.
  object_links<L> hratio_filtered_links
    = filter::object_links_bbox_h_ratio(filtered_components,
					merged_links,
					2.5f);




// #ifndef NOUT
//   if (argc == 4)
//   {
//     image2d<value::rgb8>
//       hratio_decision_image = scribo::debug::decision_image(input,
// 							    merged_links,
// 							    hratio_filtered_links);
//     io::ppm::save(hratio_decision_image,
// 		  scribo::make::debug_filename("hratio_links_decision_image.ppm"));
//   }
// #endif



//   // FIXME: from_single_link should return a packed object_groups?
//   //
//   //######
  object_groups<L>
    groups = primitive::group::from_single_link(filtered_components,
						hratio_filtered_links);
//   value::label_16 n_groups;
//   mln::fun::i2v::array<value::label_16>
//     groups_packed = mln::make::relabelfun(groups,
// 					  filtered_objects.nlabels(),
// 					  n_groups);
//   //######



//   mln::util::array<line_stats_extra> line_stats;
  line_set<L>
    lines = scribo::make::line_set(hratio_filtered_links, groups);




  // Bboxes image.
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);

    for_all_lines(l, lines)
      if (lines(l).tag() != line::Ignored)
	mln::draw::box(output, lines(l).bbox(), literal::red);

    mln::io::ppm::save(output,
		       scribo::make::debug_filename("step1_bboxes.ppm"));
  }

  // Bboxes enlarged
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);

    for_all_lines(l, lines)
      if (lines(l).tag() != line::Ignored)
      {
	if (text::internal::looks_like_a_text_line(lines(l)))
	{
	  box2d
	    b = text::internal::enlarge(lines(l).bbox(),
					text::internal::delta_of_line(lines(l)));
	  b.crop_wrt(input.domain());
	  mln::draw::box(output, b, literal::green);
	}
	else
	  mln::draw::box(output, lines(l).bbox(), literal::red);
      }

    mln::io::ppm::save(output,
		       scribo::make::debug_filename("step1_bboxes_enlarged.ppm"));
  }

  // Looks like a text line
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);

    for_all_lines(l, lines)
      if (lines(l).tag() != line::Ignored)
      {
	if (text::internal::looks_like_a_text_line(lines(l)))
	  mln::draw::box(output, lines(l).bbox(), literal::green);
	else
	  mln::draw::box(output, lines(l).bbox(), literal::red);
      }

    mln::io::ppm::save(output,
		       scribo::make::debug_filename("step1_looks_like_a_text_line.ppm"));
  }


  // Bboxes + line infos
  {
    std::ofstream file(scribo::make::debug_filename("step1_bboxes_100p.txt").c_str());
    std::ofstream file_50p(scribo::make::debug_filename("step1_bboxes_50p.txt").c_str());

    for_all_lines(l, lines)
      if (lines(l).tag() != line::Merged
	  && lines(l).tag() != line::Ignored
	  && lines(l).tag() != line::Pathological)
      {
	file << lines(l).bbox().pmin().row() << " "
	     << lines(l).bbox().pmin().col() << " "
	     << lines(l).bbox().pmax().row() << " "
	     << lines(l).bbox().pmax().col() << " "
	     << lines(l).card()              << " "
	     << lines(l).baseline()          << " "
	     << lines(l).x_height()          << " "
	     << lines(l).median()            << " "
	     << lines(l).d_height()          << " "
	     << lines(l).a_height()          << " "
	     << lines(l).char_space()        << " "
	     << lines(l).char_width()        << std::endl;

	file_50p << lines(l).bbox().pmin().row() / 2 << " "
		 << lines(l).bbox().pmin().col() / 2 << " "
		 << lines(l).bbox().pmax().row() / 2 << " "
		 << lines(l).bbox().pmax().col() / 2 << std::endl;
      }

    file.close();
    file_50p.close();
  }


   std::cout << "Merging lines..." << std::endl;
   lines = scribo::text::merging(input.domain(), lines);

  // Median and base lines.
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);

    for_all_lines(l, lines)
    {
      if (lines(l).tag() == line::Pathological)
	mln::draw::box(output, lines(l).bbox(), literal::orange);
      else if (lines(l).tag() != line::Merged)
      {
	if (lines(l).card() > 1)
	{
	  point2d
	    b_top(lines(l).baseline() - lines(l).median(), lines(l).bbox().pmin().col()),
	    e_top(lines(l).baseline() - lines(l).median(), lines(l).bbox().pmax().col()),
	    b_bot(lines(l).baseline(), lines(l).bbox().pmin().col()),
	    e_bot(lines(l).baseline(), lines(l).bbox().pmax().col());

	  mln::draw::line(output, b_top, e_top, literal::blue);
	  mln::draw::line(output, b_bot, e_bot, literal::cyan);

//	  std::cout << lines(l) << std::endl;
	}
 	mln::draw::box(output, lines(l).bbox(), literal::purple);
      }
    }

    mln::io::ppm::save(output, scribo::make::debug_filename("step2_x_height.ppm"));
  }


  // Bboxes image.
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);

    for_all_lines(l, lines)
      if (lines(l).tag() != line::Merged
	  && lines(l).tag() != line::Ignored
	  && lines(l).tag() != line::Pathological)
      {
	mln::draw::box(output, lines(l).bbox(), literal::red);
      }

    mln::io::ppm::save(output, argv[2]);
  }


//   // line label image.
//   {
//     util::array<V>
//       f(static_cast<unsigned>(filtered_components.nelements()) + 1, 0);
//     for_all_lines(l, lines)
//       if (lines(l).tag() != line::Merged
// 	  && lines(l).tag() != line::Ignored
// 	  && lines(l).tag() != line::Pathological)
//       {
// 	for_all_elements(c, lines(l).components())
// 	  f(lines(l).components()[c]) = l;
//       }
//     L line_label = duplicate(filtered_components.labeled_image());
//     V nlbl = filtered_components.nelements();
//     mln::labeling::relabel_inplace(line_label, nlbl, f);

//     mln::io::ppm::save(labeling::colorize(value::rgb8(), line_label),
// 		       scribo::make::debug_filename("labeled_lines.ppm"));
//     mln::io::dump::save(line_label, scribo::make::debug_filename("labeled_lines.dump"));
//   }



//   scribo::io::xml::save_text_lines(argv[1], lines, "out.xml");







//  scribo::text::recognition(lines, "fra", "out.txt");


//   // Display median character space.
//   {
//     image2d<value::rgb8> output = data::convert(value::rgb8(), input);
//     typedef mln::value::int_u<8> median_t;
//     typedef mln::accu::stat::median_h<median_t> accu_t;
//     util::array<accu_t>
//       lspace_med(static_cast<unsigned>(grouped_objects.nlabels()) + 1);

//     for_all_components(i, filtered_objects.bboxes())
//       if (groups_packed(i) != 0)
//       {
// 	if (hratio_filtered_links(i) != i)
// 	{
// 	  unsigned
// 	    space = filtered_objects.bbox(i).pmin().col() - filtered_objects.bbox(hratio_filtered_links(i)).pmax().col();

// 	  lspace_med(groups_packed(i)).take(space);

// 	}
//       }

//     std::cout << "Drawing median character space" << std::endl;
//     for_all_components(i, filtered_objects.bboxes())
//       if (groups_packed(i) != 0 && lspace_med(groups_packed(i)).card() > 1)
//       {
// 	unsigned med = lspace_med(groups_packed(i)).to_result();

// 	mln::draw::box(output, grouped_objects.bbox(groups_packed(i)),
// 		       literal::purple);

// 	point2d
// 	  beg = filtered_objects.bbox(i).pmax(),
// 	  end = beg;
// 	beg.row() = filtered_objects.bbox(i).pmin().row();
// 	mln::draw::line(output, beg, end, literal::cyan);
// 	beg.col() += med;
// 	end.col() += med;
// 	mln::draw::line(output, beg, end, literal::cyan);

//       }

//     io::ppm::save(output, "median_wspace.ppm");

//   }

//   trace::exiting("main");
}
