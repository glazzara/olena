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

#include <libgen.h>
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/tr_image.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/colorize.hh>

#include <mln/data/stretch.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/math/min.hh>

#include <mln/logical/not.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/fun/v2v/rgb_to_int_u.hh>

#include <mln/data/wrap.hh>

#include <mln/draw/box.hh>

#include <mln/geom/translate.hh>

#include <scribo/draw/bounding_boxes.hh>

#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/binarization/sauvola.hh>

#include <scribo/primitive/extract/components.hh>

#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/link/with_single_right_link.hh>

#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/group/from_double_link.hh>
#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/filter/objects_with_holes.hh>
#include <scribo/filter/object_links_bbox_h_ratio.hh>
#include <scribo/filter/object_links_bbox_overlap.hh>

#include <scribo/filter/common/objects_photo.hh>

#include <scribo/filter/object_groups_small.hh>
#include <scribo/filter/object_groups_v_thickness.hh>

#include <scribo/debug/decision_image.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>

#include <scribo/debug/usage.hh>

#include <scribo/preprocessing/split_bg_fg.hh>

#include <scribo/make/debug_filename.hh>

#include <mln/util/timer.hh>
#include <mln/core/var.hh>


#include <scribo/src/afp/components.hh>
#include <scribo/src/afp/link.hh>
#include <scribo/src/afp/regroup.hh>

#include <scribo/core/line_set.hh>

const char *args_desc[][2] =
{
  { "input.ppm", "A color image." },
  { "debug_output_dir", "Directory were debug images will be saved" },
  { "lambda", "Lambda value used for foreground extraction" },
  {0, 0}
};


namespace mln
{

  struct mask_non_text : Function_v2v<mask_non_text>
  {
    typedef value::rgb8 result;
    typedef image2d<bool> I;

    mask_non_text(const image2d<bool>& mask)
      : mask_(mask), p_(mask_)
    {
      p_.start();
    }

    result operator()(const result& v) const
    {
      bool b = p_.val();
      p_.next();
      if (!b)
	return v / 2;
      else
	return v;

    }

    I mask_;
    mutable mln_pixter_(I) p_;
  };


  template <typename I, typename L>
  mln_concrete(I)
  compute_highlight_image(const I& input_rgb,
			  const scribo::component_set<L>& components)
  {
    mln_ch_value(I, bool) mask;
    initialize(mask, input_rgb);
    data::fill(mask, false);

    for_all_comps(i, components)
      if (components(i).is_valid())
	data::fill((mask | components(i).bbox()).rw(), true);

    mask_non_text f(mask);
    mln_concrete(I) output = data::transform(input_rgb, f);

    for_all_comps(i, components)
      if (components(i).is_valid())
	mln::draw::box(output, components(i).bbox(), literal::red);

    return output;
  }

  template <typename I, typename L>
  mln_concrete(I)
  compute_text_image(const I& input_rgb,
		     const scribo::component_set<L>& grouped_objects)
  {
    unsigned shift = 5;
    float height = 1, width = 0;
    for_all_comps(i, grouped_objects)
      if (grouped_objects(i).is_valid())
      {
	height += grouped_objects(i).bbox().nrows() + shift;
	width = math::max(static_cast<float>(grouped_objects(i).bbox().ncols()),
			  width);
      }
    if (width == 0)
      width = 1;

    I output(height, width);
    data::fill(output, literal::black);

    algebra::vec<2, float> dv;
    dv[0] = 0;
    dv[1] = 0;
    for_all_comps(i, grouped_objects)
      if (grouped_objects(i).is_valid())
      {
	mln_VAR(tmp, duplicate(input_rgb | grouped_objects(i).bbox()));

	typedef fun::x2x::translation<mln_site_(I)::dim, float> trans_t;
	trans_t trans(dv - grouped_objects(i).bbox().pmin().to_vec());

	mln_domain(I)
	  tr_box(grouped_objects(i).bbox().pmin().to_vec() + trans.t(),
		 grouped_objects(i).bbox().pmax().to_vec() + trans.t());

	tr_image<mln_domain(I), tmp_t, trans_t> tr_ima(tr_box, tmp, trans);

	data::paste(tr_ima, output);
	dv[0] += grouped_objects(i).bbox().nrows() + shift;
      }

    return output;
  }

} // end of namespace mln


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc < 3 || argc > 8)
    return scribo::debug::usage(argv,
				"Find text in a photo.\n\n\
Common usage: ./pbm_text_in_photo input.pbm output.ppm 1 1 1",
				"input.pbm output.ppm <Bg comp filter enabled> <small group filter enabled> <thin group filter enabled> [debug_output_dir] [lambda]",
				args_desc,
				"A color image where the text is highlighted.");

  std::string out_base_dir;
  bool debug = false;
  if (argc > 6)
  {
    scribo::make::internal::debug_filename_prefix = argv[6];
    debug = true;
    out_base_dir = argv[6];
  }

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);


  mln::util::timer timer_, global_t_;
  float t_;



  typedef image2d<value::label_16> L;

  /// Finding components.
  timer_.restart();

  typedef component_set<L> Obj;
  Obj filtered_components;

  {
    mln::util::array<std::pair<box2d, std::pair<point2d, unsigned> > > attribs;

    value::label_16 ncomponents;
    L components = extract_components(input, ncomponents, attribs);

    filtered_components = Obj(components, ncomponents, attribs);
  }

  t_ = timer_;
  std::cout << "Object extracted " << t_ << " - " << filtered_components.nelements() << " components" << std::endl;

  /// linking potential components
  timer_.restart();
  mln::util::couple<object_links<L>, object_links<L> >
    links = primitive::link::left_right(filtered_components);

  object_links<L>& left_link = links.first();
  object_links<L>& right_link = links.second();

  t_ = timer_;
  std::cout << "Link done " << t_ << std::endl;



  // Validating left and right links.
  timer_.restart();
  object_links<L>
    merged_links = primitive::link::merge_double_link(left_link, right_link);
  t_ = timer_;
  std::cout << "Right/Left Validation. " << t_ << std::endl;


  // Remove links if bboxes have too different sizes.
  timer_.restart();
  object_links<L>
    hratio_filtered_links = filter::object_links_bbox_h_ratio(merged_links,
							      1.50f);

  //Remove links if bboxes overlap too much.
  object_links<L> overlap_filtered_links
    = filter::object_links_bbox_overlap(hratio_filtered_links, 0.80f);

  t_ = timer_;
  std::cout << "Components links filtered. " << t_ << std::endl;

  timer_.restart();
  object_groups<L>
    groups = primitive::group::from_single_link(overlap_filtered_links);



//  Apply grouping in a temporary image (for debug purpose).

  t_ = timer_;
  std::cout << "Components grouped. " << t_ << std::endl;

  std::cout << "Filtering groups..." << std::endl;

  mln::util::timer g_timer;

  timer_.restart();
  // Remove components part of groups with strictly less than 3 components.

  g_timer.start();
  object_groups<L> filtered_small_groups;
  if (argc > 4 && atoi(argv[4]) != 0)
  {
    std::cout << "** Using group too small" << std::endl;
    filtered_small_groups = filter::object_groups_small(groups, 3);
  }
  else
    filtered_small_groups = groups;
  t_ = g_timer;
  std::cout << "Small groups removed " << t_ << std::endl;



  // Remove components part of groups having a mean thickness lower than 8.
  g_timer.restart();
  object_groups<L> filtered_thin_groups;
  if (argc > 5 && atoi(argv[5]) != 0)
  {
    std::cout << "** Using group too thin" << std::endl;
    filtered_thin_groups
      = filter::object_groups_v_thickness(filtered_small_groups, 8);
  }
  else
    filtered_thin_groups = filtered_small_groups;
  t_ = g_timer;
  std::cout << "Groups too thin " << t_ << std::endl;



  /// Apply grouping in the object image.
  g_timer.restart();

//   groups = primitive::group::regroup_left(filtered_components,
// 					  filtered_thin_groups,
// 					  30);

  component_set<L>
    grouped_components = primitive::group::apply(filtered_thin_groups);

  t_ = g_timer;
  std::cout << "Group applied to object image " << t_ << std::endl;

  /// Components have been grouped. We try to link groups together.
  /// This time a single link is enough since non-wanted components have
  /// been removed.
  g_timer.restart();

  left_link = primitive::link::left(grouped_components, 30);


  /// Grouping groups.
  groups = primitive::group::from_single_link(left_link);


  grouped_components = primitive::group::apply(groups);

  t_ = g_timer;
  std::cout << "Link and group again " << t_ << std::endl;

  timer_.stop();

  timer_.resume();

  g_timer.restart();


  /// Filter grouped objects not having enough background components.
  if (argc > 3 && atoi(argv[3]) != 0)
  {
    std::cout << "** Using objects_with_two_holes" << std::endl;
    grouped_components = scribo::filter::components_with_two_holes(grouped_components, 5);
    t_ = g_timer;
    std::cout << "Objects_with_holes " << t_ << std::endl;
  }


  t_ = timer_;
  std::cout << "Objects groups filtered. " << t_ << std::endl;


  t_ = global_t_;
  std::cout << "*** Result computed " << t_ << std::endl;

  timer_.restart();
  std::cout << "Saving result..." << std::endl;
  io::ppm::save(mln::labeling::colorize(value::rgb8(),
					grouped_components.labeled_image(),
					grouped_components.nelements()),
		argv[2]);


  scribo::line_set<L>
    lines = scribo::line_set<L>(filtered_thin_groups);


  // Bboxes + line infos
  {
    std::ofstream file(scribo::make::debug_filename("text_bboxes.txt").c_str());

    for_all_lines(l, lines)
      if (lines(l).tag() != line::Merged
	  && lines(l).tag() != line::Ignored
	  && lines(l).tag() != line::Pathological)
      {
	file << lines(l).bbox().pmin().row() << " "
	     << lines(l).bbox().pmin().col() << " "
	     << lines(l).bbox().pmax().row() << " "
	     << lines(l).bbox().pmax().col()
//           << " "
// 	     << lines(l).card()              << " "
// 	     << lines(l).baseline()          << " "
// 	     << lines(l).x_height()          << " "
// 	     << lines(l).median()            << " "
// 	     << lines(l).d_height()          << " "
// 	     << lines(l).a_height()          << " "
// 	     << lines(l).char_space()        << " "
// 	     << lines(l).char_width()
	     << std::endl;
      }

    file.close();
  }


  trace::exiting("main");
  return grouped_components.nelements() != 0;
}
