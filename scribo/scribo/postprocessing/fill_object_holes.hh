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

#ifndef SCRIBO_POSTPROCESSING_FILL_OBJECT_HOLES_HH
# define SCRIBO_POSTPROCESSING_FILL_OBJECT_HOLES_HH

/// \file
///
/// \brief Fill-in object small holes.

/// \FIXME share code with filter/object_groups_with_holes.hh
/// \FIXME Merge the two following routines.
/// \FIXME Use a size ratio in both overloads.

# include <sstream>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/routine/extend.hh>
# include <mln/core/image/dmorph/extended.hh>

# include <mln/data/fill.hh>
# include <mln/data/transform.hh>

# include <mln/extension/duplicate.hh>
# include <mln/extension/adjust_fill.hh>

# include <mln/draw/box_plain.hh>
# include <mln/util/array.hh>

# include <mln/accu/math/count.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/value/int_u16.hh>

# include <mln/labeling/blobs_and_compute.hh>

# include <mln/logical/not.hh>

# include <mln/transform/influence_zone_geodesic.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/primitive/group/apply.hh>


namespace scribo
{

  namespace postprocessing
  {

    using namespace mln;

    /*! \brief Fill-in object small holes.

     */
    template <typename I>
    inline
    mln_concrete(I)
    fill_object_holes(const Image<I>& input, float ratio);


    template <typename L>
    inline
    object_groups<L>
    fill_object_holes(const object_groups<L>& groups,
		      unsigned min_size);

# ifndef MLN_INCLUDE_ONLY


    // Utils

    namespace internal
    {

      unsigned my_find_root(image2d<unsigned>& parent, unsigned x)
      {
	if (parent.element(x) == x)
	  return x;
	return parent.element(x) = my_find_root(parent,
						parent.element(x));
      }


      template <typename L>
      mln_concrete(L)
      compute_bboxes_image(const component_set<L>& components)
      {
	typedef mln_psite(L) P;
	typedef mln_dpsite(P) D;

	const L& lbl = components.labeled_image();
	extension::adjust_fill(lbl, 1, 0);

	mln_concrete(L) output;
	initialize(output, lbl);
	data::fill(output, 0);

	for_all_comps(i, components)
	  if (components(i).is_valid())
	  {
	    mln_box(L) b = components(i).bbox();
	    b.enlarge(1);

	    unsigned
	      nrows = b.pmax().row() - b.pmin().row() + 1,
	      ncols = b.pmax().col() - b.pmin().col() + 1,
	      row_offset = lbl.delta_offset(D(+1, -ncols));

	    mln_value(L) *ptr = &output(b.pmin());
	    for (unsigned row = 0; row < nrows; ++row, ptr += row_offset)
	      for (unsigned col = 0; col < ncols; ++col)
		*ptr++ = i;
	  }

	extension::duplicate(output);

	return output;
      }


    } // end of namespace scribo::postprocessing::internal



    // Implementations

    namespace impl
    {

      namespace generic
      {

	template <typename L>
	inline
	object_groups<L>
	fill_object_holes(const object_groups<L>& groups,
			  unsigned min_size)
	{
	  trace::entering("scribo::postprocessing::impl::generic::fill_object_holes");

	  // Grouping groups and relabel the underlying labeled image.
	  // Groups are now considered as components.
	  component_set<L> components = primitive::group::apply(groups);

	  neighb2d nbh = c8();

	  image2d<unsigned> parent, card;
	  L bboxes_ima;

	  util::array<unsigned> bg_comps(
	    value::next(components.nelements()), 0);
	  util::array<bool> bg_comps_done(
	    value::next(components.nelements()), false);

	  mln::fun::i2v::array<bool>
	    to_keep(value::next(components.nelements()), false);

	  const L& lbl = components.labeled_image();

	  // init
	  {
	    extension::fill(lbl, mln_max(mln_value(L)));
//	extension::adjust_fill(components, nbh, mln_max(mln_value(L)));
	    initialize(parent, lbl);
	    data::fill(parent, 0u);

	    initialize(card, lbl);
	    data::fill(card, 1);
	    border::fill(card, 1);

	    // We want to label background components only in the
	    // group bounding boxes. Thus, this image is a labeling
	    // constraint.
	    bboxes_ima = internal::compute_bboxes_image(components);

	    to_keep(0) = true;
	  }

	  // 1st pass
	  {
	    util::array<int> dp = positive_offsets_wrt(lbl, nbh);
	    const unsigned n_nbhs = dp.nelements();

	    mln_bkd_pixter(const L) pxl(lbl); // Backward.
	    for_all(pxl)
	    {
	      unsigned p = pxl.offset();
	      if (bboxes_ima.element(p) == 0 || lbl.element(p) != literal::zero)
		continue;

	      parent.element(p) = p;
	      for (unsigned i = 0; i < n_nbhs; ++i)
	      {
		unsigned n = p + dp[i];
		if (bboxes_ima.element(n) == 0 || lbl.element(n) != literal::zero)
		  continue;

		unsigned r = internal::my_find_root(parent, n);
		if (r != p)
		{
		  parent.element(r) = p;
		  card.element(p) += card.element(r);
		}
	      } // for_all(n)

	    } // for_all(pxl)

	  }

	  // 2nd pass
	  {
	    unsigned kept = 0;
	    mln_fwd_pixter(const L) pxl(bboxes_ima); // Forward.
	    for_all(pxl)
	    {
	      unsigned p = pxl.offset();
	      if (parent.element(p) == 0) // Foreground, ignored.
		continue;

	      unsigned& parent_p = parent.element(p);

	      if (parent_p != p) // Not root => propagation
		parent_p = parent.element(parent_p);
	      else // Root
		if (card.element(p) < min_size)
		{
		  parent_p = 0;
		  continue;
		}

	      if (parent_p != 0 // Check parent again since
		  // parent's may have been set to
		  // 0.
		  && bboxes_ima.element(p) != literal::zero)
	      {
		mln_value(L) group_id = bboxes_ima.element(p);
		if (!bg_comps_done(group_id) && components(group_id).is_valid())
		{
		  if (bg_comps(group_id) == 0)
		  {
		    bg_comps(group_id) = parent_p;
		  }
		  else if (bg_comps(group_id) != parent_p)
		  {
		    bg_comps_done(group_id) = true;
		    to_keep(group_id) = true;
		    ++kept;
		  }
		}
	      }
	    }

	    if (kept == components.nelements())
	    {
	      trace::exiting("scribo::postprocessing::impl::generic::fill_object_holes");
	      return groups.duplicate();
	    }

	    object_groups<L> output = groups.duplicate();
	    for_all_groups(c, groups)
	      if (! to_keep(groups(c)))
		output(c) = 0;


	    trace::exiting("scribo::postprocessing::impl::generic::fill_object_holes");
	    return output;
	  }

	}

      } // end of namespace scribo::postprocessing::impl::generic

    } // end of namespace scribo::postprocessing::impl



    // Facade

    template <typename L>
    inline
    object_groups<L>
    fill_object_holes(const object_groups<L>& groups,
		      unsigned min_size)
    {
      trace::entering("scribo::postprocessing::fill_object_holes");

      mln_precondition(groups.is_valid());

      object_groups<L>
	output = impl::generic::fill_object_holes(groups, min_size);

      trace::exiting("scribo::postprocessing::fill_object_holes");
      return output;
    }


    template <typename I>
    inline
    mln_concrete(I)
    fill_object_holes(const Image<I>& input_, float ratio)
    {
      trace::entering("scribo::postprocessing::fill_object_holes");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mlc_is(mln_value(I), bool)::check();

      mln_concrete(I) output = duplicate(input);

      typedef value::int_u16 L;
      typedef mln_ch_value(I, L) Li;
      typedef accu::math::count<mln_site(Li)> A;

      typedef util::couple<Li, util::couple<util::array<unsigned>,
	util::array<A> > > res_t;

      // Holes card Image

//      std::cout << "> Holes card image" << std::endl;

      L nlabels;

      res_t res = labeling::blobs_and_compute(input, c8(), nlabels, A());

      util::array<unsigned>& holes_card = res.second().first();
      mln_ch_value(I, unsigned)
	holes = data::transform(res.first(), holes_card);



      // Threshold Image

//      std::cout << "> Threshold image" << std::endl;


      I input_i = logical::not_(input);
      res = labeling::blobs_and_compute(input_i, c8(), nlabels, A());

      util::array<unsigned>& card = res.second().first();
      for (unsigned i = 1; i < card.size(); ++i)
	card(i) = unsigned(round(card(i) * ratio));

      mln_ch_value(I, unsigned)
	thres = data::transform(res.first(), card);
      thres = transform::influence_zone_geodesic(thres, c8());



      // Thresholding

//      std::cout << "> Thresholding" << std::endl;

      I hole_mask;
      initialize(hole_mask, holes);
      data::fill(hole_mask, false);
      mln_piter(I) p(input.domain());
      for_all(p)
	if (holes(p))
	  hole_mask(p) = holes(p) < thres(p);


      // Cleanup

//      std::cout << "> Cleanup" << std::endl;

      data::fill((output | pw::value(hole_mask)).rw(), false);


      trace::exiting("scribo::postprocessing::fill_object_holes");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::postprocessing

} // end of namespace scribo


#endif // ! SCRIBO_POSTPROCESSING_FILL_OBJECT_HOLES_HH
