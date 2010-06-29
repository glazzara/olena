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

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/math/min.hh>

#include <mln/logical/not.hh>

#include <mln/fun/v2v/rgb_to_int_u.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/draw/box.hh>

#include <mln/geom/translate.hh>

#include <scribo/binarization/sauvola.hh>

#include <scribo/draw/bounding_boxes.hh>

#include <scribo/primitive/extract/objects.hh>

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
			  const object_image<L>& objects)
  {
    mln_ch_value(I, bool) mask;
    initialize(mask, input_rgb);
    data::fill(mask, false);

    for_all_components(i, objects.bboxes())
      data::fill((mask | objects.bbox(i)).rw(), true);

    mask_non_text f(mask);
    mln_concrete(I) output = data::transform(input_rgb, f);

    for_all_components(i, objects.bboxes())
      mln::draw::box(output, objects.bbox(i), literal::red);

    return output;
  }


  template <typename I, typename L>
  mln_concrete(I)
  compute_text_image(const I& input_rgb,
		     const object_image<L>& grouped_objects)
  {
    const util::array<mln_domain(L)>& bboxes = grouped_objects.bboxes();

    unsigned shift = 5;
    float height = 1, width = 0;
    for_all_components(i, bboxes)
    {
      height += bboxes(i).nrows() + shift;
      width = math::max(static_cast<float>(bboxes(i).ncols()), width);
    }
    if (width == 0)
      width = 1;

    I output(height, width);
    data::fill(output, literal::black);

    algebra::vec<2, float> dv;
    dv[0] = 0;
    dv[1] = 0;
    for_all_ncomponents(i, grouped_objects.nlabels())
    {
      mln_VAR(tmp, duplicate(input_rgb | grouped_objects.bbox(i)));

      typedef fun::x2x::translation<mln_site_(I)::dim, float> trans_t;
      trans_t trans(dv - grouped_objects.bbox(i).pmin().to_vec());

      mln_domain(I) tr_box(grouped_objects.bbox(i).pmin().to_vec() + trans.t(),
			   grouped_objects.bbox(i).pmax().to_vec() + trans.t());

      tr_image<mln_domain(I), tmp_t, trans_t> tr_ima(tr_box, tmp, trans);

      data::paste(tr_ima, output);
      dv[0] += grouped_objects.bbox(i).nrows() + shift;
    }

    return output;
  }

} // end of namespace mln


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3 && argc != 4 && argc != 5 && argc != 6)
    return scribo::debug::usage(argv,
				"Find text in a photo.",
				"input.ppm output.ppm [bg/fg] [debug_output_dir] [lambda]",
				args_desc,
				"A color image where the text is highlighted.");

  if (argc > 4)
    scribo::make::internal::debug_filename_prefix = argv[4];

  trace::entering("main");

  image2d<value::rgb8> input_rgb;
  io::ppm::load(input_rgb, argv[1]);


  unsigned lambda;
  if (argc == 6)
    lambda = atoi(argv[5]);
  else
    lambda = 1.2 * (input_rgb.nrows() + input_rgb.ncols());


  util::timer timer_;

  // Extract foreground
  image2d<value::int_u8> intensity_ima;
  std::cout << "Extracting foreground..." << std::endl;
  timer_.start();

  if (argc > 3 && atoi(argv[3]) != 0)
  {
    // Extract foreground
    timer_.start();
    image2d<value::rgb8>
      fg = preprocessing::split_bg_fg(input_rgb,
				      lambda,
				      32).second();
    intensity_ima = data::transform(fg, mln::fun::v2v::rgb_to_int_u<8>());
    float t_ = timer_;
    std::cout << "Foreground extracted. " << t_ << std::endl;
  }
  else
  {
    timer_.start();
    intensity_ima = data::transform(input_rgb,
				    mln::fun::v2v::rgb_to_int_u<8>());
    float t_ = timer_;
    std::cout << "Intensity image " << t_ << std::endl;
  }
  float t_ = timer_;
  std::cout << "Foreground extracted. " << t_ << std::endl;

  // Binarize foreground to use it in the processing chain.
  // FIXME: TOO SLOW!
  std::cout << "Binarizing foreground..." << std::endl;
  timer_.restart();
  image2d<bool> input = binarization::sauvola(intensity_ima, 11);
  io::pbm::save(input, "input.pbm");
  t_ = timer_;
  std::cout << "Foreground binarized. " << t_ << std::endl;



  typedef image2d<value::label_16> L;

  /// Finding objects.
  std::cout << "Extracting objects..." << std::endl;
  timer_.restart();
  value::label_16 nobjects;
  object_image(L)
    objects = scribo::primitive::extract::objects(input, c8(), nobjects);
  t_ = timer_;
  std::cout << "Object extracted" << t_ << std::endl;

  /// First filtering.
  std::cout << "Filtering objects..." << std::endl;
  timer_.restart();
  object_image(L) filtered_objects = filter::common::objects_photo(objects);
  t_ = timer_;
  std::cout << "Object filtered" << t_ << std::endl;



  /// linking potential objects
  std::cout << "Linking objects..." << std::endl;
  timer_.restart();
  object_links<L> left_link
    = primitive::link::with_single_left_link(filtered_objects, 30);
  t_ = timer_;
  std::cout << "Left Link done" << t_ << std::endl;

  timer_.restart();
  object_links<L> right_link
    = primitive::link::with_single_right_link(filtered_objects, 30);
  t_ = timer_;
  std::cout << "Right Link done" << t_ << std::endl;



#ifndef NOUT
  if (argc > 3)
  {
    std::cerr << "BEFORE - nobjects = " << nobjects << std::endl;
    scribo::debug::save_linked_bboxes_image(input,
					    filtered_objects,
					    left_link, right_link,
					    literal::red, literal::cyan,
					    literal::yellow,
					    literal::green,
					    scribo::make::debug_filename("links.ppm"));
  }
#endif



  // Validating left and right links.
  timer_.restart();
  object_links<L>
    merged_links = primitive::link::merge_double_link(filtered_objects,
						      left_link,
						      right_link);
  t_ = timer_;
  std::cout << "Right/Left Validation. " << t_ << std::endl;


  // Remove links if bboxes have too different sizes.
  std::cout << "Filtering object links..." << std::endl;
  timer_.restart();
  object_links<L>
    hratio_filtered_links = filter::object_links_bbox_h_ratio(filtered_objects,
							      merged_links,
							      1.50f);




#ifndef NOUT
  if (argc > 3)
  {
    image2d<value::rgb8>
      hratio_decision_image = scribo::debug::decision_image(input,
							    merged_links,
							    hratio_filtered_links);
    io::ppm::save(hratio_decision_image,
		  scribo::make::debug_filename("hratio_links_decision_image.ppm"));
  }
#endif




  //Remove links if bboxes overlap too much.
  object_links<L> overlap_filtered_links
    = filter::object_links_bbox_overlap(filtered_objects,
					hratio_filtered_links,
					0.80f);




#ifndef NOUT
  if (argc > 3)
  {
    image2d<value::rgb8> overlap_decision_image
      = scribo::debug::decision_image(input,
				      hratio_filtered_links,
				      overlap_filtered_links);
    io::ppm::save(overlap_decision_image,
		  scribo::make::debug_filename("overlap_links_decision_image.ppm"));
  }
#endif


  t_ = timer_;
  std::cout << "Objects links filtered. " << t_ << std::endl;

  std::cout << "Grouping objects..." << std::endl;
  timer_.restart();
  object_groups<L>
    groups = primitive::group::from_single_link(filtered_objects,
						overlap_filtered_links);



  /// Apply grouping in a temporary image (for debug purpose).
  object_image(L)
    raw_group_image = primitive::group::apply(filtered_objects,
					      filter::object_groups_small(groups, 2));

  t_ = timer_;
  std::cout << "Objects grouped. " << t_ << std::endl;

#ifndef NOUT

  if (argc > 3)
    scribo::debug::save_bboxes_image(input,
				     raw_group_image.bboxes(),
				     literal::red,
				     scribo::make::debug_filename("group_image.ppm"));
#endif // !NOUT

  std::cout << "Filtering groups..." << std::endl;

  util::timer g_timer;

  timer_.restart();
  // Remove objects part of groups with strictly less than 3 objects.

  g_timer.start();
  object_groups<L>
    filtered_small_groups = filter::object_groups_small(groups, 3);
  t_ = g_timer;
  std::cout << "Small groups removed " << t_ << std::endl;



#ifndef NOUT
  image2d<value::rgb8> decision_image;
  if (argc > 3)
  {
    decision_image = scribo::debug::decision_image(input,
						   groups,
						   filtered_small_groups);
    io::ppm::save(decision_image,
		  scribo::make::debug_filename("small_groups_filter.ppm"));
  }
#endif



  // Remove objects part of groups having a mean thickness lower than 8.
  g_timer.restart();
  object_groups<L> filtered_thin_groups
    = filter::object_groups_v_thickness(filtered_small_groups, 8);
  t_ = g_timer;
  std::cout << "Groups too thin " << t_ << std::endl;


#ifndef NOUT
  if (argc > 3)
  {
    decision_image = scribo::debug::decision_image(input,
						   groups,
						   filtered_thin_groups);

    io::ppm::save(decision_image,
		  scribo::make::debug_filename("thin_groups_filter.ppm"));
  }
#endif



  /// Apply grouping in the object image.
  g_timer.restart();

  object_image(L)
    grouped_objects = primitive::group::apply(filtered_objects,
					      filtered_thin_groups);

  t_ = g_timer;
  std::cout << "Group applied to object image " << t_ << std::endl;

  /// Objects have been grouped. We try to link groups together.
  /// This time a single link is enough since non-wanted objects have
  /// been removed.
  g_timer.restart();
  left_link
    = primitive::link::with_single_left_link(grouped_objects, 30);

  /// Grouping groups.
  groups = primitive::group::from_single_link(grouped_objects, left_link);


  grouped_objects = primitive::group::apply(grouped_objects, groups);

  t_ = g_timer;
  std::cout << "Link and group again " << t_ << std::endl;

  timer_.stop();

  io::ppm::save(mln::labeling::colorize(value::rgb8(),
					grouped_objects,
					grouped_objects.nlabels()),
		scribo::make::debug_filename("out_before_hole_filter.ppm"));


  timer_.resume();

  g_timer.restart();
  /// Filter grouped objects not having enough background components.
  grouped_objects = scribo::filter::objects_with_holes_slow(grouped_objects, 2);
//  grouped_objects = scribo::filter::objects_with_holes(grouped_objects, 2, 2);
  t_ = g_timer;
  std::cout << "Objects_with_holes " << t_ << std::endl;

  t_ = timer_;
  std::cout << "Objects groups filtered. " << t_ << std::endl;

#ifndef NOUT
  if (argc > 3)
  {
    scribo::draw::bounding_boxes(decision_image, raw_group_image, literal::yellow);
    scribo::draw::bounding_boxes(decision_image, grouped_objects, literal::blue);
    io::ppm::save(decision_image, scribo::make::debug_filename("decision_image.ppm"));

    std::cerr << "AFTER - nobjects = " << grouped_objects.nlabels() << std::endl;
  }
#endif


  std::cout << "Saving result..." << std::endl;
  io::ppm::save(mln::labeling::colorize(value::rgb8(),
					grouped_objects,
					grouped_objects.nlabels()),
		argv[2]);

#ifndef NOUT
  io::ppm::save(compute_highlight_image(input_rgb, grouped_objects),
		scribo::make::debug_filename("orig_with_bboxes.ppm"));
//   scribo::debug::save_bboxes_image(input_rgb, grouped_objects.bboxes(),
// 				   literal::red,
// 				   scribo::make::debug_filename("orig_with_bboxes.ppm"));
#endif

  io::ppm::save(compute_text_image(input_rgb, grouped_objects),
		scribo::make::debug_filename("out_text.ppm"));


  std::cout << "# objects = " << grouped_objects.nlabels() << std::endl;

  trace::exiting("main");
  return grouped_objects.nlabels() != 0;
}
