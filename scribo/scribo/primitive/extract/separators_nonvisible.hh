// Copyright (C) 2010, 2011, 2012 EPITA Research and Development
// Laboratory (LRDE)
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


#ifndef SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_NONVISIBLE_HH
# define SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_NONVISIBLE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/pw/all.hh>

# include <mln/accu/count_value.hh>

# include <mln/draw/line.hh>

# include <mln/data/fill.hh>
# include <mln/data/convert.hh>

# include <mln/labeling/relabel.hh>
# include <mln/labeling/value.hh>

# include <mln/morpho/closing/structural.hh>

# include <mln/literal/colors.hh>

# include <mln/value/label_16.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>

# include <mln/util/timer.hh>

# include <scribo/core/object_groups.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/def/lbl_type.hh>

# include <scribo/debug/logger.hh>

# include <scribo/filter/object_groups_small.hh>
# include <scribo/filter/object_links_bottom_aligned.hh>
# include <scribo/filter/object_links_top_aligned.hh>

# include <scribo/preprocessing/rotate_90.hh>

# include <scribo/primitive/extract/components.hh>

# include <scribo/primitive/link/internal/dmax_default.hh>
# include <scribo/primitive/link/with_single_right_link_dmax_ratio_aligned.hh>
# include <scribo/primitive/link/with_single_left_link_dmax_ratio_aligned.hh>

# include <scribo/primitive/group/from_double_link_any.hh>




namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;
      using namespace scribo::debug;


      /*! \brief Find non visible separators. Based on components
	alignments.

	\ingroup extractprimitiveseps
      */
      template <typename I>
      mln_concrete(I)
      separators_nonvisible(const Image<I>& in_);



# ifndef MLN_INCLUDE_ONLY

      // FACADE

      template <typename I>
      mln_concrete(I)
      separators_nonvisible(const Image<I>& in_)
      {
	mln_trace("scribo::primitive::extract::separators_nonvisible");

	const I& in = exact(in_);
	mln_precondition(in.is_valid());
	typedef mln_value(I) Vi;
	mlc_is(Vi,bool)::check();

	typedef scribo::def::lbl_type V;
	typedef mln_ch_value(I,V) L;

	unsigned
	  min_angle = 3,
	  max_angle = 5,
	  min_card = 3;

	// mln::util::timer t;
	// mln::util::timer gt;

	// gt.start();

	bool _debug_ = logger().is_at_level(AuxiliaryResults);

	// Closing structural - Connect characters.
	// t.start();

	win::hline2d vl(17);
	mln_concrete(I) input_clo = morpho::closing::structural(in, vl);

	// float t_ = t;
	// std::cout << "closing_structural - " << t_ << std::endl;

	// Debug
	logger().log_image(AuxiliaryResults, input_clo, "input_clo");

	// Rotate (OK)
	// t.restart();
	input_clo = scribo::preprocessing::rotate_90(input_clo, false);
	// t_ = t;
	// std::cout << "rotate_90 - " << t_ << std::endl;



	/// Finding components.

	// t.restart();
	V ncomponents;
	component_set<L>
	  components = scribo::primitive::extract::components(input_clo, c8(),
							      ncomponents);
	// t_ = t;
	// std::cout << "extract::components - " << t_ << std::endl;

	// Debug
	logger().log_image(AuxiliaryResults,
			   data::convert(value::int_u8(),
					 components.labeled_image()),
			   "lbl");
	unsigned dmax = 5;

	// t.restart();
	object_links<L> top_right, bot_right;

	object_links<L> top_left, bot_left;


	typedef link::internal::dmax_default Dmax_F;
	// Top
	{
	  // Right
	  link::internal::single_right_dmax_ratio_aligned_functor<L,Dmax_F>
	    functor(components, Dmax_F(dmax), min_angle, max_angle,
		    anchor::TopStrictLeft);
	  top_right = primitive::link::compute(functor, anchor::TopStrictLeft);

	  // t.stop();

	  // Debug
	  logger().log_image(AuxiliaryResults, functor.debug_, "right_top");
	  logger().log_image(AuxiliaryResults, functor.debug_angle_,
			     "right_top_angle");

	  // t.resume();

	  // Left
	  link::internal::single_left_dmax_ratio_aligned_functor<L,Dmax_F>
	    lfunctor(components, Dmax_F(dmax), min_angle, max_angle,
		     anchor::TopStrictLeft);
	  top_left = primitive::link::compute(lfunctor, anchor::TopStrictLeft);


 	  // t.stop();

	  // Debug
	  if (_debug_)
	  {
	    logger().log_image(AuxiliaryResults, functor.debug_, "left_top");
	    logger().log_image(AuxiliaryResults, functor.debug_angle_,
			       "left_top_angle");

	    mln_ch_value(I, value::rgb8) output = duplicate(functor.debug_);
	    data::paste((lfunctor.debug_
			 | (pw::value(lfunctor.debug_) != pw::cst(literal::black)))
	    		| (pw::value(lfunctor.debug_) != pw::cst(literal::white)), output);

	    logger().log_image(AuxiliaryResults, output, "left_right_top");
	  }

	  // t.resume();
	}


	// Bottom
	{
	  // Right
	  link::internal::single_right_dmax_ratio_aligned_functor<L,Dmax_F>
	    functor(components, Dmax_F(dmax), min_angle, max_angle,
		    anchor::BottomStrictRight);
	  bot_right = primitive::link::compute(functor, anchor::BottomStrictRight);
	  // t.stop();

	  // Debug
	  if (_debug_)
	  {
	    logger().log_image(AuxiliaryResults, functor.debug_, "right_bot");
	    logger().log_image(AuxiliaryResults, functor.debug_angle_,
			       "right_bot_angle");
	  }

	  // t.resume();

	  // Left
	  link::internal::single_left_dmax_ratio_aligned_functor<L,Dmax_F>
	    lfunctor(components, Dmax_F(dmax), min_angle, max_angle,
		     anchor::BottomStrictRight);
	  bot_left = primitive::link::compute(lfunctor, anchor::BottomStrictRight);
	  // t.stop();

	  // Debug
	  if (_debug_)
	  {
	    logger().log_image(AuxiliaryResults, functor.debug_, "left_bot");
	    logger().log_image(AuxiliaryResults, functor.debug_angle_,
			       "left_bot_angle");

	    mln_ch_value(I, value::rgb8) output = duplicate(functor.debug_);
	    data::paste((lfunctor.debug_
	    		 | (pw::value(lfunctor.debug_) != pw::cst(literal::black)))
	    		| (pw::value(lfunctor.debug_) != pw::cst(literal::white)), output);

	    logger().log_image(AuxiliaryResults, output, "left_right_bot");
	  }
	}


	// t_ = t;
	// std::cout << "links - " << t_ << std::endl;

	// Merge links and build CC groups
	// t.restart();
	object_groups<L>
	  top_groups = primitive::group::from_double_link_any(top_left, top_right);
	object_groups<L>
	  bot_groups = primitive::group::from_double_link_any(bot_left, bot_right);
	// t_ = t;
	// std::cout << "group - " << t_ << std::endl;

	// Filter CC groups
	// t.restart();
	top_groups = filter::object_groups_small(top_groups, min_card);
	bot_groups = filter::object_groups_small(bot_groups, min_card);
	// t_ = t;
	// std::cout << "small groups - " << t_ << std::endl;

	// Compute group bboxes

	// t.restart();
	mln_concrete(I) separators;
	initialize(separators, input_clo);

	// FIXME: any way to fill border AND data at the same time?
	data::fill(separators, false);
	extension::fill(separators, false);

	// t_ = t;
	// std::cout << "Initialize separators image - " << t_ << std::endl;

	// t.restart();
	for_all_groups(d, top_groups)
	  if (top_groups(d).is_valid())
	  {
	    mln::draw::line(separators,
			    top_groups(d).bbox().pmin(),
			    point2d(top_groups(d).bbox().pmin().row(),
				    top_groups(d).bbox().pmax().col()),
			    true);
	  }


	for_all_groups(d, bot_groups)
	  if (bot_groups(d).is_valid())
	  {
	    mln::draw::line(separators,
			    point2d(bot_groups(d).bbox().pmax().row(),
				    bot_groups(d).bbox().pmin().col()),
			    bot_groups(d).bbox().pmax(),
			    true);
	  }


	// t_ = t;
	// std::cout << "Drawing output image - " << t_ << std::endl;


	if (_debug_)
	{
	  // Restore input orientation.
	  mln_concrete(I) input = scribo::preprocessing::rotate_90(in, false);

	  mln_ch_value(I, value::rgb8)
	    wo_filtering = data::convert(value::rgb8(), input);

	  for_all_groups(d, top_groups)
	    if (top_groups(d).is_valid())
	    {
	      mln::draw::line(wo_filtering,
			      top_groups(d).bbox().pmin(),
			      point2d(top_groups(d).bbox().pmin().row(),
				      top_groups(d).bbox().pmax().col()),
			      literal::green);
	    }

	  for_all_groups(d, bot_groups)
	    if (bot_groups(d).is_valid())
	    {
	      mln::draw::line(wo_filtering,
			      point2d(bot_groups(d).bbox().pmax().row(),
				      bot_groups(d).bbox().pmin().col()),
			      bot_groups(d).bbox().pmax(),
			      literal::green);
	    }

	  logger().log_image(AuxiliaryResults, wo_filtering, "wo_filtering");
	  logger().log_image(AuxiliaryResults, separators, "separators");
	}


	// Hit or miss
	{
	  if (_debug_)
	  {
	    mln_concrete(I) input_with_seps = duplicate(input_clo);
	    data::paste(separators | pw::value(separators), input_with_seps);

	    logger().log_image(AuxiliaryResults, input_with_seps, "input_with_seps");
	  }

	  // t.restart();
	  unsigned length = 25;

	  dpoint2d
	    dp1(-21, 0),
	    dp2( 21, 0);

	  // Adjusting extension.
	  // t.restart();
	  extension::adjust_fill(input_clo, length / 2, 0);

	  accu::count_value<bool> accu(true);
	  typedef mln_ch_value(I,unsigned) J;

	  J tmp = accu::transform_line(accu, input_clo, length, 1);

	  // t_ = t;
	  // std::cout << "* accu::transform_line - " << t_ << std::endl;


	  // t.restart();
	  value::int_u8 nlabels;
	  mln_ch_value(I,value::int_u8)
	    sep_lbl = labeling::value(separators, true, c8(), nlabels);
	  // t_ = t;
	  // std::cout << "* labeling seps - " << t_ << std::endl;


	  // t.restart();

	  mln::util::array<bool> relbl(unsigned(nlabels) + 1, true);
	  relbl(0) = false;

	  unsigned invalid_ratio = unsigned(length * 0.30f);

	  extension::adjust_fill(tmp, 21, 0);

	  value::int_u8 *sep_lbl_ptr = sep_lbl.buffer()
	    + sep_lbl.offset_of_point(sep_lbl.domain().pmin());
	  bool *separators_ptr = separators.buffer()
	    + separators.offset_of_point(separators.domain().pmin());
	  unsigned *tmp_ptr = tmp.buffer() + tmp.offset_of_point(tmp.domain().pmin());;
	  int idx1 = tmp.delta_offset(dp1);
	  int idx2 = tmp.delta_offset(dp2);

	  unsigned
	    nrows = separators.nrows(),
	    ncols = separators.ncols();

	  unsigned
	    row_idx_sep_lbl = sep_lbl.delta_offset(dpoint2d(+1, - ncols)),
	    row_idx_separators = separators.delta_offset(dpoint2d(+1, - ncols)),
	    row_idx_tmp = tmp.delta_offset(dpoint2d(+1, - ncols));

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


	  // t_ = t;
	  // std::cout << "* reading data - " << t_ << std::endl;

	  // t.restart();
	  labeling::relabel_inplace(sep_lbl, nlabels, relbl);
	  // t_ = t;
	  // std::cout << "* relabel_inplace - " << t_ << std::endl;

	  mln_concrete(I) output = data::convert(bool(), sep_lbl);

	  if (_debug_)
	  {
	    logger().log_image(AuxiliaryResults, output,
			       "separators_filtered");
	  }

	  // gt.stop();
	  // t_ = gt;
	  // std::cout << "Non visible separators: " << t_ << std::endl;

	  return scribo::preprocessing::rotate_90(output, true);
	}
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_NONVISIBLE_HH
