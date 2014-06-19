// Copyright (C) 2009, 2010, 2011, 2013, 2014 EPITA Research and Development
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

#ifndef SCRIBO_FILTER_OBJECTS_WITH_HOLES_HH
# define SCRIBO_FILTER_OBJECTS_WITH_HOLES_HH

/// \file
///
/// \brief Remove components having a minimum number of holes.

# include <sstream>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/routine/extend.hh>
# include <mln/core/image/dmorph/extended.hh>

# include <mln/extension/duplicate.hh>

# include <mln/draw/box_plain.hh>
# include <mln/util/array.hh>

# include <mln/labeling/blobs_and_compute.hh>

# include <mln/accu/math/count.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>

# include <mln/data/convert.hh>

# include <mln/labeling/background.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>
# include <scribo/filter/internal/compute.hh>

# include <mln/data/fill.hh>
# include <mln/data/paste.hh>

# include <mln/util/timer.hh>

# include <mln/value/label_16.hh>


#include <mln/debug/filename.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /*! \brief Remove components having a minimum number of holes.

      \param[in] components A component set.
      \param[in] min_holes_count If a component have at least \p
                                 min_holes_count holes it is
                                 invalidated.
      \param[in] min_size The minimum hole area to take a hole into
                          account.

      \return A component where the component having too much holes
	      are invalidated.

      \ingroup grpalgofiltercomp
     */
    template <typename L>
    component_set<L>
    objects_with_holes(const component_set<L>& components,
		       unsigned min_holes_count,
		       unsigned min_size);

    /*! \brief Remove components having at least two holes.

      This is a fastest version since it is optimized for 2 holes
      detection.

      \param[in] components A component set.
      \param[in] min_size The minimum hole area to take a hole into
                          account.

      \return A component where the component having at least two
	      holes are invalidated.

      \ingroup grpalgofiltercomp
     */
    template <typename L>
    inline
    component_set<L>
    components_with_two_holes(const component_set<L>& components,
			      unsigned min_size);


# ifndef MLN_INCLUDE_ONLY

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


    } // end of namespace scribo::filter::internal



    /*!
      Label the background and count the number of background
      components in an object bounding box.
     */
    template <typename L>
    inline
    component_set<L>
    objects_with_holes(const component_set<L>& components,
		       unsigned min_holes_count,
		       unsigned min_size)
    {
      mln_trace("scribo::filter::objects_with_holes");

      typedef component_set<L> O;
      neighb2d nbh = c4();

      image2d<unsigned> parent, card;
      L bboxes_ima;

      util::array<util::set<unsigned> > bg_comps(
	value::next(components.nelements()));

      fun::i2v::array<bool>
	to_keep(value::next(components.nelements()),
		false);

      const L& lbl = components.labeled_image();

      // init
      {
	extension::adjust_fill(lbl, nbh, mln_max(mln_value(L)));
	initialize(parent, lbl);
	data::fill(parent, 0u);

	initialize(card, lbl);
	data::fill(card, 1);


	// FIXME: Improve.
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

      // FIXME: Iterate over another label when a label is marked as
      // "to be kept".

      // 2nd pass
      {
	unsigned kept = 0;
	mln_fwd_pixter(const L) pxl(bboxes_ima); // Forward.
	for_all(pxl)
	{
	  unsigned p = pxl.offset();

	  // Foreground, ignored.
	  if (parent.element(p) == 0 || bboxes_ima.element(p) == literal::zero)
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

// 	  if (bboxes_ima.element(p) != literal::zero)
// 	  {
	    mln_value(L) object_id = bboxes_ima.element(p);
	    if (parent_p != 0 // Check parent again since
		              // parent's may have been set to
		              // 0.
		&& bg_comps(object_id).nelements() < min_holes_count)
	    {
	      if (! bg_comps(object_id).has(parent_p))
	      {
		bg_comps(object_id).insert(parent_p);

		if (bg_comps(object_id).nelements() == min_holes_count)
		{
		  to_keep(object_id) = true;
		  ++kept;
		}
	      }
	    }
// 	  }
	}

	if (kept == components.nelements())
	{
	  return components;
	}

	component_set<L> output = components.duplicate();
	output.update_tags(to_keep, component::Ignored);

	return output;
      }


    }


    template <typename L>
    inline
    component_set<L>
    components_with_two_holes(const component_set<L>& components,
			      unsigned min_size)
    {
      mln_trace("scribo::filter::objects_with_holes");

      typedef component_set<L> O;
      neighb2d nbh = c8();

      image2d<unsigned> parent, card;
      L bboxes_ima;

      util::array<unsigned> bg_comps(
	value::next(components.nelements()), 0);
      util::array<bool> bg_comps_done(
	value::next(components.nelements()), false);

      fun::i2v::array<bool>
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
	    mln_value(L) object_id = bboxes_ima.element(p);
	    if (!bg_comps_done(object_id) && components(object_id).is_valid())
	    {
	      if (bg_comps(object_id) == 0)
	      {
		bg_comps(object_id) = parent_p;
	      }
	      else if (bg_comps(object_id) != parent_p)
	      {
		  bg_comps_done(object_id) = true;
		  to_keep(object_id) = true;
		  ++kept;
	      }
	    }
	  }
	}

	component_set<L> output;
	if (kept == components.nelements())
	{
	  return components;
	}

	output = components.duplicate();
	output.update_tags(to_keep, component::Ignored);

	return output;
      }

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECTS_WITH_HOLES_HH
