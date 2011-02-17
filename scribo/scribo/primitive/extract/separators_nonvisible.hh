// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///
/// \brief Find non visible separators (whitespaces)
///
/// \fixme To be cleaned up

#ifndef SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_NONVISIBLE_HH
# define SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_NONVISIBLE_HH

#include <mln/core/concept/image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/all.hh>

#include <mln/draw/line.hh>

#include <mln/data/wrap.hh>
#include <mln/data/fill.hh>
#include <mln/data/convert.hh>


#include <mln/labeling/colorize.hh>
#include <mln/labeling/relabel.hh>
#include <mln/labeling/blobs.hh>

#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/opening/structural.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/win/vline2d.hh>

#include <mln/logical/not.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/literal/colors.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/draw/box_plain.hh>

#include <mln/transform/influence_zone_geodesic.hh>

#include <mln/data/stretch.hh>

#include <mln/util/timer.hh>

#include <mln/norm/l1.hh>

#include <scribo/core/object_groups.hh>
#include <scribo/core/component_set.hh>
#include <scribo/primitive/extract/components.hh>

#include <scribo/primitive/extract/lines_h_pattern.hh>
#include <scribo/primitive/remove/separators.hh>

#include <scribo/preprocessing/denoise_fg.hh>
#include <scribo/preprocessing/rotate_90.hh>

#include <scribo/primitive/link/internal/compute_anchor.hh>

#include <scribo/primitive/link/internal/dmax_default.hh>

#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio_aligned.hh>

#include <scribo/primitive/group/from_double_link_any.hh>

#include <scribo/filter/object_links_top_aligned.hh>
#include <scribo/filter/object_groups_small.hh>
#include <scribo/filter/object_links_bottom_aligned.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>
#include <scribo/debug/decision_image.hh>





namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;


      /// \brief Find non visible separators (whitespaces)
      //
      template <typename I>
      mln_concrete(I)
      separators_nonvisible(const Image<I>& in_);



# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	using namespace primitive::link::internal;

	template <typename L, typename E>
	class single_dmax_ratio_aligned_functor_base
	  : public link_single_dmax_ratio_base<L, dmax_default, E>
	{
	  typedef link_single_dmax_ratio_base<L, dmax_default, E> super_;

	public:
	  typedef mln_site(L) P;

	  single_dmax_ratio_aligned_functor_base(
	    const mln_ch_value(L,bool)& input,
	    const component_set<L>& components,
	    unsigned dmax,
	    float min_angle,
	    float max_angle,
	    anchor::Type anchor_,
	    bool debug)
	    : super_(components,
		     anchor::Horizontal,
		     dmax_default(dmax)),
	      anchor(anchor_),
	      _debug_(debug)
	  {
	    (void) input; // FIXME : remove this argument
	    min_alpha_rad = (min_angle / 180.0f) * math::pi;
	    max_alpha_rad = (max_angle / 180.0f) * math::pi;

	    // if (_debug_)
	    // {
	    //   debug_ = data::convert(value::rgb8(), input);
	    //   debug_angle_ = data::convert(value::rgb8(), input);
	    // }
	  }

	  void compute_next_site_(P& p)
	  {
	    ++p.col();
	  }

	  void compute_next_site_f_(unsigned& p)
	  {
	    ++p;
	  }


	  mln_site(L)
	  start_point_(unsigned current_object, anchor::Type anchor)
	  {
	    return link::internal::compute_anchor(this->components_,
						  current_object, anchor);
	  }


	  inline
	  bool
	  valid_link_(unsigned current_object,
		      const P& start_point,
		      const P& p)
	  {
	    if (!super_::valid_link_(current_object, start_point, p))
	      return false;

	    box<P> b = this->components_(current_object).bbox();

	    // Distance between the two components.
	    float dist;

	    // current object is on the left.
	    if (p[this->direction_] > b.pmax()[this->direction_])
	      dist = math::abs(p[this->direction_] - b.pmax()[this->direction_]);
	    // current object is on the right.
	    else
	      dist = math::abs(p[this->direction_] - b.pmin()[this->direction_]);


	    int ldist = this->components_(current_object).bbox().width();

	    // Components are really close, so the angle is more permissive.
	    if (dist < 3 * ldist)
	    {
	      return
		filter::internal::component_aligned_rad(this->components_,
							current_object,
							this->labeled_image_(p),
							anchor,
							max_alpha_rad);
	    }


	    // Components are really far, so the angle is less permissive.
	    return
	      filter::internal::component_aligned_rad(this->components_,
						      current_object,
						      this->labeled_image_(p),
						      anchor,
						      min_alpha_rad);
	  }

	  void validate_link_(unsigned current_object,
			      const P& start_point,
			      const P& p,
			      anchor::Type anchor)
	  {
	    super_::validate_link_(current_object, start_point, p, anchor);

	    // if (_debug_)
	    // {
	    //   mln_site(L)
	    // 	p1 = link::internal::compute_anchor(this->components_,
	    // 					    current_object, anchor),
	    // 	p2 = link::internal::compute_anchor(this->components_,
	    // 					    this->labeled_image_(p),
	    // 			     anchor);
	    //   mln::draw::line(debug_, p1, p2, literal::green);


	    //   float
	    // 	angle = filter::internal::alignment_angle(this->components_,
	    // 						  current_object,
	    // 						  this->labeled_image_(p),
	    // 						  anchor);
	    //   angle = (angle * 180.0f) / math::pi;
	    //   angle = angle * 20.0f + 1.0f;
	    //   mln::draw::line(debug_angle_, p1, p2,
	    // 		      value::rgb8(unsigned(angle),
	    // 				  unsigned(angle),
	    // 				  unsigned(angle)));
	    // }
	  }

	  void invalidate_link_(unsigned current_object,
				const P& start_point,
				const P& p,
				anchor::Type anchor)
	  {
	    super_::invalidate_link_(current_object, start_point, p, anchor);

	    // if (_debug_)
	    // {
	    //   if (this->labeled_image_.domain().has(p) && this->labeled_image_(p) != 0)
	    //   {
	    // 	mln_site(L)
	    // 	  p1 = link::internal::compute_anchor(this->components_,
	    // 					      current_object, anchor),
	    // 	  p2 = link::internal::compute_anchor(this->components_,
	    // 					      this->labeled_image_(p),
	    // 			       anchor);
	    // 	if (this->labeled_image_.domain().has(p2)
	    // 	    && norm::l1_distance(p1.to_vec(), p2.to_vec()) < 300)
	    // 	{
	    // 	  mln::draw::line(debug_, p1, p2, literal::red);
	    // 	}


	    // 	float
	    // 	  angle = filter::internal::alignment_angle(this->components_,
	    // 						    current_object,
	    // 						    this->labeled_image_(p),
	    // 						    anchor);
	    // 	angle = (angle * 180.0f) / math::pi;
	    // 	angle = angle * 20.0f + 1.0f;
	    // 	mln::draw::line(debug_angle_, p1, p2,
	    // 			value::rgb8(unsigned(angle),
	    // 				    unsigned(angle),
	    // 				    unsigned(angle)));
	    //   }
	    // }
	  }


	  float min_alpha_rad;
	  float max_alpha_rad;
	  anchor::Type anchor;


	  // mln_ch_value(L, value::rgb8) debug_;
	  // mln_ch_value(L, value::rgb8) debug_angle_;
	  bool _debug_;
	};


	template <typename L>
	class single_right_dmax_ratio_aligned_functor
	  : public single_dmax_ratio_aligned_functor_base<L, single_right_dmax_ratio_aligned_functor<L> >
	{
	  typedef single_right_dmax_ratio_aligned_functor<L> self_t;
	  typedef single_dmax_ratio_aligned_functor_base<L, self_t> super_;

	public:
	  typedef mln_site(L) P;

	  single_right_dmax_ratio_aligned_functor(
	    const mln_ch_value(L, bool)& input,
	    const component_set<L>& components,
	    unsigned dmax,
	    float min_angle,
	    float max_angle,
	    anchor::Type anchor,
	    bool debug)
	    : super_(input, components, dmax, min_angle,
		     max_angle, anchor, debug)
	  {
	  }

	  void compute_next_site_(P& p)
	  {
	    ++p.col();
	  }

	  void compute_next_site_f_(unsigned& p)
	  {
	    ++p;
	  }

	};


	template <typename L>
	class single_left_dmax_ratio_aligned_functor
	  : public single_dmax_ratio_aligned_functor_base<L, single_left_dmax_ratio_aligned_functor<L> >
	{
	  typedef single_left_dmax_ratio_aligned_functor<L> self_t;
	  typedef single_dmax_ratio_aligned_functor_base<L, self_t> super_;

	public:
	  typedef mln_site(L) P;

	  single_left_dmax_ratio_aligned_functor(
	    const mln_ch_value(L, bool)& input,
	    const component_set<L>& components,
	    unsigned dmax,
	    float min_angle,
	    float max_angle,
	    anchor::Type anchor,
	    bool debug)
	    : super_(input, components, dmax, min_angle,
		     max_angle, anchor, debug)
	  {
	  }

	  void compute_next_site_(P& p)
	  {
	    --p.col();
	  }

	  void compute_next_site_f_(unsigned& p)
	  {
	    --p;
	  }


	};

      } // end of namespace scribo::primitive::extract::internal




      // FACADE

      template <typename I>
      mln_concrete(I)
      separators_nonvisible(const Image<I>& in_)
      {
	trace::entering("scribo::primitive::extract::separators_nonvisible");

	const I& in = exact(in_);
	mln_precondition(in.is_valid());
	typedef mln_value(I) Vi;
	mlc_is(Vi,bool)::check();

	bool _debug_ = false;
	unsigned
	  min_angle = 3,
	  max_angle = 5,
	  min_card = 3;

	util::timer t;
	util::timer gt;

	gt.start();


	// // Remove horizontal lines.
	// t.restart();

	// mln_concrete(I) hlines = primitive::extract::lines_h_pattern(in, 50, 3);
	// mln_concrete(I) input = primitive::remove::separators(in, hlines);

	// t_ = t;
	// std::cout << "Horizontal lines removed - " << t_ << std::endl;


	// Closing structural - Connect characters.
	t.start();

	win::hline2d vl(17);
	mln_concrete(I) input_clo = morpho::closing::structural(in, vl);

	float t_ = t;
	std::cout << "closing_structural - " << t_ << std::endl;

	// if (_debug_)
	// {
	//   // Restore input orientation.
	//   input = scribo::preprocessing::rotate_90(input, false);

	//   io::pbm::save(input_clo, "input_clo.pbm");
	// }

	// Rotate (OK)
	t.restart();
	input_clo = scribo::preprocessing::rotate_90(input_clo, false);
	t_ = t;
	std::cout << "rotate_90 - " << t_ << std::endl;



	/// Finding components.
	typedef value::int_u16 V;
	typedef mln_ch_value(I,V) L;

	t.restart();
	V ncomponents;
	component_set<L>
	  components = scribo::primitive::extract::components(input_clo, c8(),
							      ncomponents);
	t_ = t;
	std::cout << "extract::components - " << t_ << std::endl;

	// if (_debug_)
	//   io::pgm::save(data::convert(value::int_u8(), components.labeled_image()),
	// 		"lbl.pgm");


	unsigned dmax = 5;

	t.restart();
	object_links<L> top_right, bot_right;

	object_links<L> top_left, bot_left;


	// Top
	{
	  // Right
	  internal::single_right_dmax_ratio_aligned_functor<L>
	    functor(input_clo, components, dmax, min_angle, max_angle,
		    anchor::TopStrictLeft, _debug_);
//    top_right = primitive::link::impl::compute_fastest(functor, anchor::TopStrictLeft);
	  top_right = primitive::link::compute(functor, anchor::TopStrictLeft);

	  t.stop();


	  // if (_debug_)
	  // {
	  //   io::ppm::save(functor.debug_, "right_top.ppm");
	  //   io::ppm::save(functor.debug_angle_, "right_top_angle.ppm");
	  // }

	  t.resume();

	  // Left
	  internal::single_left_dmax_ratio_aligned_functor<L>
	    lfunctor(input_clo, components, dmax, min_angle, max_angle,
		     anchor::TopStrictLeft, _debug_);
	  top_left = primitive::link::compute(lfunctor, anchor::TopStrictLeft);


	  t.stop();

	  // if (_debug_)
	  // {
	  //   io::ppm::save(lfunctor.debug_, "left_top.ppm");
	  //   io::ppm::save(lfunctor.debug_angle_, "left_top_angle.ppm");

	  //   mln_ch_value(I, value::rgb8) output = duplicate(functor.debug_);
	  //   data::paste((lfunctor.debug_ | (pw::value(lfunctor.debug_) != pw::cst(literal::black)))
	  // 		| (pw::value(lfunctor.debug_) != pw::cst(literal::white)), output);

	  //   io::ppm::save(output, "left_right_top.ppm");
	  // }

	  t.resume();
	}


	// Bottom
	{
	  // Right
	  internal::single_right_dmax_ratio_aligned_functor<L>
	    functor(input_clo, components, dmax, min_angle, max_angle,
		    anchor::BottomStrictRight, _debug_);
	  bot_right = primitive::link::compute(functor, anchor::BottomStrictRight);
	  t.stop();

	  // if (_debug_)
	  // {
	  //   io::ppm::save(functor.debug_, "right_bot.ppm");
	  //   io::ppm::save(functor.debug_angle_, "right_bot_angle.ppm");
	  // }

	  t.resume();

	  // Left
	  internal::single_left_dmax_ratio_aligned_functor<L>
	    lfunctor(input_clo, components, dmax, min_angle, max_angle,
		     anchor::BottomStrictRight, _debug_);
	  bot_left = primitive::link::compute(lfunctor, anchor::BottomStrictRight);
	  t.stop();

	  // if (_debug_)
	  // {
	  //   io::ppm::save(lfunctor.debug_, "left_bot.ppm");
	  //   io::ppm::save(lfunctor.debug_angle_, "left_bot_angle.ppm");

	  //   mln_ch_value(I, value::rgb8) output = duplicate(functor.debug_);
	  //   data::paste((lfunctor.debug_ | (pw::value(lfunctor.debug_) != pw::cst(literal::black)))
	  // 		| (pw::value(lfunctor.debug_) != pw::cst(literal::white)), output);

	  //   io::ppm::save(output, "left_right_bot.ppm");
	  // }
	}


	t_ = t;
	std::cout << "links - " << t_ << std::endl;

	// Merge links and build CC groups
	t.restart();
	object_groups<L>
	  top_groups = primitive::group::from_double_link_any(top_left, top_right);
	object_groups<L>
	  bot_groups = primitive::group::from_double_link_any(bot_left, bot_right);
	t_ = t;
	std::cout << "group - " << t_ << std::endl;

	// Filter CC groups
	t.restart();
	top_groups = filter::object_groups_small(top_groups, min_card);
	bot_groups = filter::object_groups_small(bot_groups, min_card);
	t_ = t;
	std::cout << "small groups - " << t_ << std::endl;


	// Compute group bboxes
	t.restart();
	util::array<accu::shape::bbox<point2d> >
	  top_accu(top_groups.nelements()),
	  bot_accu(bot_groups.nelements());


	for_all_groups(c, top_groups)
	{
	  top_accu(top_groups(c)).take(components(c).bbox());
	  bot_accu(bot_groups(c)).take(components(c).bbox());
	}
	t_ = t;
	std::cout << "groups to group bboxes - " << t_ << std::endl;



	t.restart();
	mln_concrete(I) separators;
	initialize(separators, input_clo);

	// FIXME: any way to fill border AND data at the same time?
	data::fill(separators, false);
	extension::fill(separators, false);

	t_ = t;
	std::cout << "Initialize separators image - " << t_ << std::endl;

	t.restart();
	for_all_comp_data(d, top_accu)
	{
	  if (top_accu(d).is_valid())
	  {
	    mln::draw::line(separators,
			    top_accu(d).to_result().pmin(),
			    point2d(top_accu(d).to_result().pmin().row(),
				    top_accu(d).to_result().pmax().col()),
			    true);
	  }
	  if (bot_accu(d).is_valid())
	  {
	    mln::draw::line(separators,
			    point2d(bot_accu(d).to_result().pmax().row(),
				    bot_accu(d).to_result().pmin().col()),
			    bot_accu(d).to_result().pmax(),
			    true);
	  }

	}
	t_ = t;
	std::cout << "Drawing output image - " << t_ << std::endl;


	// if (_debug_)
	// {
	//   // Restore input orientation.
	//   mln_concrete(I) input = scribo::preprocessing::rotate_90(in, false);


	//   // Debug group bboxes (includes all bboxes before filtering)
	//   util::array<accu::shape::bbox<point2d> >
	//     btop_accu(top_groups.nelements()),
	//     bbot_accu(bot_groups.nelements());


	//   for_all_groups(c, top_groups)
	//   {
	//     btop_accu(top_groups(c)).take(components(c).bbox());
	//     bbot_accu(bot_groups(c)).take(components(c).bbox());
	//   }

	//   mln_ch_value(I, value::rgb8)
	//     wo_filtering = data::convert(value::rgb8(), input);

	//   for_all_comp_data(d, btop_accu)
	//   {
	//     if (btop_accu(d).is_valid())
	//     {
	//       mln::draw::line(wo_filtering,
	// 		      btop_accu(d).to_result().pmin(),
	// 		      point2d(btop_accu(d).to_result().pmin().row(),
	// 			      btop_accu(d).to_result().pmax().col()),
	// 		      literal::green);

	//     }
	//   }

	//   for_all_comp_data(d, bbot_accu)
	//   {
	//     if (bbot_accu(d).is_valid())
	//     {
	//       mln::draw::line(wo_filtering,
	// 		      point2d(bbot_accu(d).to_result().pmax().row(),
	// 			      bbot_accu(d).to_result().pmin().col()),
	// 		      bbot_accu(d).to_result().pmax(),
	// 		      literal::green);
	//     }

	//   }
	//   io::ppm::save(wo_filtering, "wo_filtering.ppm");

	//   mln_ch_value(I, value::rgb8) both = data::convert(value::rgb8(), input);

	//   for_all_comp_data(d, top_accu)
	//   {
	//     if (top_accu(d).is_valid()  ||  btop_accu(d).is_valid())
	//     {
	//       if (top_accu(d).is_valid())
	//       {
	// 	  mln::draw::line(both,
	// 			  top_accu(d).to_result().pmin(),
	// 			  point2d(top_accu(d).to_result().pmin().row(),
	// 				  top_accu(d).to_result().pmax().col()),
	// 			  literal::green);
	//       }
	//       else
	// 	if (btop_accu(d).is_valid())
	// 	  mln::draw::line(both,
	// 			  btop_accu(d).to_result().pmin(),
	// 			  point2d(btop_accu(d).to_result().pmin().row(),
	// 				  btop_accu(d).to_result().pmax().col()),
	// 			  literal::yellow);

	//     }
	//     if (bot_accu(d).is_valid() ||  bbot_accu(d).is_valid())
	//     {
	//       if (bot_accu(d).is_valid())
	//       {
	// 	  mln::draw::line(both,
	// 			  point2d(bot_accu(d).to_result().pmax().row(),
	// 				  bot_accu(d).to_result().pmin().col()),
	// 			  bot_accu(d).to_result().pmax(),
	// 			  literal::green);
	//       }
	//       else
	// 	if (bbot_accu(d).is_valid())
	// 	  mln::draw::line(both,
	// 			  point2d(bbot_accu(d).to_result().pmax().row(),
	// 				  bbot_accu(d).to_result().pmin().col()),
	// 			  bbot_accu(d).to_result().pmax(),
	// 			  literal::yellow);
	//     }

	//   }

	//   io::ppm::save(both, "both.ppm");
	//   io::pbm::save(separators, "separators.pbm");
	// }


	// Hit or miss
	{
	  // if (_debug_)
	  // {
	  //   mln_concrete(I) input_with_seps = duplicate(input_clo);
	  //   data::paste(separators | pw::value(separators), input_with_seps);

	  //   io::pbm::save(input_with_seps, "input_with_seps.pbm");
	  // }

	  t.restart();
	  unsigned length = 25;

	  dpoint2d
	    dp1(-21, 0),
	    dp2( 21, 0);

	  // Adjusting extension.
	  t.restart();
	  extension::adjust_fill(input_clo, length / 2, 0);

	  accu::count_value<bool> accu(true);
	  typedef mln_ch_value(I,unsigned) J;

	  J tmp = accu::transform_line(accu, input_clo, length, 1);

	  t_ = t;
	  std::cout << "* accu::transform_line - " << t_ << std::endl;

	  // if (_debug_)
	  //   io::pgm::save(data::convert(value::int_u8(), tmp), "tmp.pgm");


	  t.restart();
	  value::int_u8 nlabels;
	  mln_ch_value(I,value::int_u8)
	    sep_lbl = labeling::value(separators, true, c8(), nlabels);
	  t_ = t;
	  std::cout << "* labeling seps - " << t_ << std::endl;


	  t.restart();

	  util::array<bool> relbl(unsigned(nlabels) + 1, true);
	  relbl(0) = false;

	  unsigned invalid_ratio = unsigned(length * 0.30f);

	  extension::adjust_fill(tmp, 21, 0);

	  value::int_u8 *sep_lbl_ptr = sep_lbl.buffer() + sep_lbl.index_of_point(sep_lbl.domain().pmin());
	  bool *separators_ptr = separators.buffer() + separators.index_of_point(separators.domain().pmin());
	  unsigned *tmp_ptr = tmp.buffer() + tmp.index_of_point(tmp.domain().pmin());;
	  int idx1 = tmp.delta_index(dp1);
	  int idx2 = tmp.delta_index(dp2);

	  unsigned nrows = separators.nrows();
	  unsigned ncols = separators.ncols();

	  unsigned row_idx_sep_lbl = sep_lbl.delta_index(dpoint2d(+1, - ncols));
	  unsigned row_idx_separators = separators.delta_index(dpoint2d(+1, - ncols));
	  unsigned row_idx_tmp = tmp.delta_index(dpoint2d(+1, - ncols));

	  for (unsigned row = 0; row < nrows; ++row)
	  {
	    for (unsigned col = 0; col < ncols; ++col)
	    {
	      if (*separators_ptr)
	      {
		unsigned lbl = *sep_lbl_ptr;

		unsigned
		  top_count = *(tmp_ptr + idx1),
		  bot_count = *(tmp_ptr + idx2);

		// This site is wrapped between two lines of text so we don't
		// want it.
		if (top_count >= invalid_ratio + 1
		    && bot_count >= invalid_ratio + 1)
		{
		  relbl(lbl) = false;
		}
	      }

	      ++tmp_ptr;
	      ++sep_lbl_ptr;
	      ++separators_ptr;
	    }

	    tmp_ptr += row_idx_tmp;
	    sep_lbl_ptr += row_idx_sep_lbl;
	    separators_ptr += row_idx_separators;
	  }


	  t_ = t;
	  std::cout << "* reading data - " << t_ << std::endl;

	  t.restart();
	  labeling::relabel_inplace(sep_lbl, nlabels, relbl);
	  t_ = t;
	  std::cout << "* relabel_inplace - " << t_ << std::endl;

	  mln_concrete(I) output = data::convert(bool(), sep_lbl);

	  // if (_debug_)
	  // {
	  //   io::pbm::save(output, "separators_hom.pbm");
	  //   io::pbm::save(separators, "separators_filtered.pbm");

	  //   // value::int_u16 ncomps;
	  //   // component_set<L> comps = primitive::extract::components(output, c8(), ncomps);
	  //   // mln_ch_value(I, value::rgb8) both;

	  //   // both = data::convert(value::rgb8(), input);

	  //   // // Needed since the rotated image origin is (0,0).
	  //   // dpoint2d dp(input.domain().pcenter() - input_clo.domain().pcenter());

	  //   // for_all_comps(c, comps)
	  //   // {
	  //   //   box2d b = geom::rotate(comps(c).bbox(), -90, input_clo.domain().pcenter());
	  //   //   mln::draw::line(both,
	  //   // 		      b.pmin() + dp,
	  //   // 		      b.pmax() + dp,
	  //   // 		      literal::green);
	  //   // }
	  // }

	  gt.stop();
	  t_ = gt;
	  std::cout << "Non visible separators: " << t_ << std::endl;

	  trace::exiting("scribo::primitive::extract::separators_nonvisible");
	  return scribo::preprocessing::rotate_90(output, true);
	}
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_NONVISIBLE_HH
