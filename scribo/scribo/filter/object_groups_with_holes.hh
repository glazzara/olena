// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_FILTER_OBJECT_GROUPS_WITH_HOLES_HH
# define SCRIBO_FILTER_OBJECT_GROUPS_WITH_HOLES_HH

/// \file
///
/// \brief Remove groups not having at least two background components
/// of a minimum size.

# include <sstream>

# include <mln/core/image/image2d.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/routine/extend.hh>
# include <mln/core/image/dmorph/extended.hh>

# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>

# include <mln/extension/duplicate.hh>
# include <mln/extension/adjust_fill.hh>

# include <mln/draw/box_plain.hh>
# include <mln/util/array.hh>

# include <mln/accu/math/count.hh>

# include <mln/fun/i2v/array.hh>


# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/object_groups.hh>

# include <scribo/primitive/group/apply.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /*! \brief Remove groups not having at least two background
     *  components of \p min_size pixels.

     */
    template <typename L>
    inline
    object_groups<L>
    object_groups_with_holes(const object_groups<L>& components,
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
	mln::extension::adjust_fill(lbl, 1, 0);

	mln_concrete(L) output;
	initialize(output, lbl);
	data::fill(output, 0);

	for_all_comps(i, components)
	  if (components(i).is_valid())
	  {
	    mln_box(L) b = components(i).bbox();
	    b.enlarge(1);

	    unsigned
	      nrows = geom::nrows(b),
	      ncols = geom::ncols(b),
	      row_offset = lbl.delta_offset(D(+1, -ncols));

	    mln_value(L) *ptr = &output(b.pmin());
	    for (unsigned row = 0; row < nrows; ++row, ptr += row_offset)
	      for (unsigned col = 0; col < ncols; ++col)
		*ptr++ = i;
	  }

	extension::duplicate(output);

	return output;
      }


    } // end of namespace scribo::filter::internal



    // Implementations

    namespace impl
    {

      namespace generic
      {

	template <typename L>
	inline
	object_groups<L>
	object_groups_with_holes(const object_groups<L>& groups,
				 unsigned min_size)
	{
	  trace::entering("scribo::filter::impl::generic::object_groups_with_holes");

	  // Grouping groups and relabel the underlying labeled image.
	  // Groups are now considered as components.
          //
	  // The relabeling function used in group::apply is kept in
	  // order to map properly the groups and their new component
	  // id.
	  //
	  mln::fun::i2v::array<mln_value(L)> group_2_comp;
	  component_set<L>
	    components = primitive::group::apply(groups, group_2_comp);

	  neighb2d nbh = c8();

	  mln_ch_value(L,unsigned) parent, card;
	  L bboxes_ima;

	  // Will store the first background component id associated
	  // to a group.
	  mln::util::array<unsigned> bg_comps(
	    value::next(components.nelements()), 0);

	  // Will 'True' if a group has at least two background
	  // components.
	  fun::i2v::array<bool>
	    to_keep(value::next(components.nelements()), false);

	  const L& lbl = components.labeled_image();

	  // init
	  {
	    // The border should be considered as the background.
	    extension::fill(lbl, literal::zero);

	    initialize(parent, lbl);
	    data::fill(parent, 0u);
	    border::fill(parent, 0u);

	    initialize(card, lbl);
	    data::fill(card, 1);
	    border::fill(card, 1);

	    // We want to label background components only in the
	    // group/component bounding boxes. This image is a
	    // labeling constraint.
	    bboxes_ima = internal::compute_bboxes_image(components);

//	    to_keep(0) = true;

	  } // end of init


	  // 1st pass
	  {
	    mln::util::array<int> dp = positive_offsets_wrt(lbl, nbh);
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

	  } // end of 1st pass

	  unsigned kept = 0;


	  // 2nd pass
	  {
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
		if (!to_keep(group_id))
		{
		  // The group must have at least 2 different
		  // background components.
		  //
		  // This is the first background component found.
		  if (bg_comps(group_id) == 0)
		  {
		    bg_comps(group_id) = parent_p;
		  }
		  // This is the second background component found.
		  else if (bg_comps(group_id) != parent_p)
		  {
		    to_keep(group_id) = true;
		    ++kept;
		  }
		}
	      }
	    } // for_all(pxl)

	  } // end of 2nd pass


	  // No groups need to be invalidated.
	  if (kept == components.nelements())
	  {
	    trace::exiting("scribo::filter::impl::generic::object_groups_with_holes");
	    return groups.duplicate();
	  }

	  // Invalidate groups when necessary.
	  object_groups<L> output = groups.duplicate();
	  for_all_groups(c, groups)
	    if (! to_keep(group_2_comp(c)))
	      output(c).invalidate();

	  trace::exiting("scribo::filter::impl::generic::object_groups_with_holes");
	  return output;

	}

      } // end of namespace scribo::filter::impl::generic

    } // end of namespace scribo::filter::impl



    // Facade

    template <typename L>
    inline
    object_groups<L>
    object_groups_with_holes(const object_groups<L>& groups,
			     unsigned min_size)
    {
      trace::entering("scribo::filter::object_groups_with_holes");

      mln_precondition(groups.is_valid());

      object_groups<L>
	output = impl::generic::object_groups_with_holes(groups, min_size);

      trace::exiting("scribo::filter::object_groups_with_holes");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECT_GROUPS_WITH_HOLES_HH
