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

#ifndef SCRIBO_FILTER_OBJECTS_WITH_HOLES_HH
# define SCRIBO_FILTER_OBJECTS_WITH_HOLES_HH

/// \file
///
/// \brief Remove objects having a minimum number of holes.

# include <sstream>

# include <mln/core/concept/image.hh>
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
# include <scribo/core/object_image.hh>
# include <scribo/filter/internal/compute.hh>

# include <mln/data/fill.hh>
# include <mln/data/paste.hh>

# include <mln/util/timer.hh>

# include <mln/value/label_16.hh>
# include <mln/core/var.hh>


#include <mln/debug/filename.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /*! \brief Remove objects having a minimum number of holes.



     */
    template <typename L>
    object_image(L)
    objects_with_holes(const object_image(L)& objects,
		       unsigned min_holes_count,
		       unsigned min_size);


    template <typename L>
    inline
    object_image(L)
    objects_with_two_holes(const object_image(L)& objects,
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
      compute_bboxes_image(const object_image(L)& objects)
      {
	typedef mln_psite(L) P;
	typedef mln_dpsite(P) D;

	extension::adjust_fill(objects, 1, 0);

	mln_concrete(L) output;
	initialize(output, objects);
	data::fill(output, 0);

	for_all_components(i, objects.bboxes())
	{
	  mln_box(L) b = objects.bbox(i);
	  b.enlarge(1);

	  unsigned
	    nrows = b.pmax().row() - b.pmin().row() + 1,
	    ncols = b.pmax().col() - b.pmin().col() + 1,
	    row_offset = objects.labeled_image_().delta_index(D(+1, -ncols));

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
    object_image(L)
    objects_with_holes(const object_image(L)& objects,
		       unsigned min_holes_count,
		       unsigned min_size)
    {
      trace::entering("scribo::filter::objects_with_holes");

      typedef object_image(L) O;
      neighb2d nbh = c4();

      image2d<unsigned> parent, card;
      L bboxes_ima;

      util::array<util::set<unsigned> > bg_comps(
	static_cast<unsigned>(objects.nlabels()) + 1);

      fun::i2v::array<bool>
	to_keep(static_cast<unsigned>(objects.nlabels()) + 1,
		false);

      const L& lbl = objects.labeled_image_();

      std::cout << "objects.nlabels = " << objects.nlabels() << std::endl;

      util::timer timer_;
      timer_.start();

      // init
      {
	extension::adjust_fill(objects, nbh, mln_max(mln_value(L)));
	initialize(parent, objects);
	data::fill(parent, 0u);

	initialize(card, objects);
	data::fill(card, 1);


	// FIXME: Improve.
	util::timer t2;
	t2.start();
	bboxes_ima = internal::compute_bboxes_image(objects);
	float t2_ = t2;
	std::cout << "compute bboxes image " << t2_ << std::endl;

	to_keep(0) = true;
      }
      float t_ = timer_;
      std::cout << "init = " << t_ << std::endl;

      // 1st pass
      timer_.restart();
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
      t_ = timer_;
      std::cout << "1st pass = " << t_ << std::endl;

      // FIXME: Iterate over another label when a label is marked as
      // "to be kept".

      // 2nd pass
      timer_.restart();
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

	float t_ = timer_;
	std::cout << "2nd pass = " << t_ << std::endl;

 	std::cout << "kept = " << kept << std::endl;
//  	debug::println(parent);
//  	std::cout << bg_comps << std::endl;
//  	std::cout << to_keep << std::endl;

	timer_.restart();
	object_image(L) output;

	if (kept == objects.nlabels())
	{
	  trace::exiting("scribo::filter::objects_with_holes");
	  return objects;
	}

	output.init_from_(objects);
	output.relabel(to_keep);
	t_ = timer_;
	std::cout << "init output = " << t_ << std::endl;

	trace::exiting("scribo::filter::objects_with_holes");
	return output;
      }


    }


    template <typename L>
    inline
    object_image(L)
    objects_with_two_holes(const object_image(L)& objects,
			   unsigned min_size)
    {
      trace::entering("scribo::filter::objects_with_holes");

      std::cout << objects.nlabels() << std::endl;

      typedef object_image(L) O;
      neighb2d nbh = c8();

      image2d<unsigned> parent, card;
      L bboxes_ima;

      util::array<unsigned> bg_comps(
	static_cast<unsigned>(objects.nlabels()) + 1, 0);
      util::array<bool> bg_comps_done(
	static_cast<unsigned>(objects.nlabels()) + 1, false);

      fun::i2v::array<bool>
	to_keep(static_cast<unsigned>(objects.nlabels()) + 1,
		false);

      const L& lbl = objects.labeled_image_();

      // init
      {
	extension::fill(objects, mln_max(mln_value(L)));
//	extension::adjust_fill(objects, nbh, mln_max(mln_value(L)));
	initialize(parent, objects);
	data::fill(parent, 0u);

	initialize(card, objects);
	data::fill(card, 1);
	border::fill(card, 1);

	bboxes_ima = internal::compute_bboxes_image(objects);

	to_keep(0) = true;
      }

      // 1st pass
      std::cout << "1st pass" << std::endl;
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
      std::cout << "2nd pass" << std::endl;
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
	    if (!bg_comps_done(object_id))
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

	object_image(L) output;
	if (kept == objects.nlabels())
	{
	  trace::exiting("scribo::filter::objects_with_holes");
	  return objects;
	}

	output.init_from_(objects);
	output.relabel(to_keep);

	trace::exiting("scribo::filter::objects_with_holes");
	return output;
      }

    }



//     template <typename L>
//     inline
//     object_image(L)
//     objects_with_holes(const object_image(L)& objects,
// 		       unsigned min_holes_count)
//     {
//       trace::entering("scribo::filter::objects_with_holes");

//       mln_precondition(objects.is_valid());


//       L bboxes_ima;
//       initialize(bboxes_ima, objects);
//       data::fill(bboxes_ima, literal::zero);

//       for_all_components(i, objects.bboxes())
// 	mln::draw::box(bboxes_ima, objects.bbox(i), i);

//       util::array<util::set<mln_value(L)> > first_bg_comp(
// 	static_cast<unsigned>(objects.nlabels()) + 1);

//       fun::i2v::array<bool>
// 	to_keep(static_cast<unsigned>(objects.nlabels()) + 1,
// 		false);
//       to_keep(0) = true;

//       mln_value(L) nbglabels;
//       L bg_lbl = labeling::background(objects, c8(), nbglabels);

//       unsigned kept;
//       mln_piter(L) p(bboxes_ima.domain());
//       for_all(p)
//       {
// 	if (bboxes_ima(p) == literal::zero)
// 	  continue;

// 	if (bg_lbl(p) != 0)
// 	  if (! first_bg_comp(bboxes_ima(p)).has(bg_lbl(p)))
// 	    if (first_bg_comp(bboxes_ima(p)).nelements() < min_holes_count - 1)
// 	      first_bg_comp(bboxes_ima(p)).insert(bg_lbl(p));
// 	    else
// 	    {
// 	      to_keep(bboxes_ima(p)) == true;
// 	      ++kept;
// 	    }
//       }

//       object_image(L) output;
//       if (kept == objects.nlabels())
// 	output = objects;
//       else
// 	output = internal::compute(objects, to_keep);

//       trace::exiting("scribo::filter::objects_with_holes");
//       return output;
//     }


    template <typename L>
    inline
    object_image(L)
    objects_with_holes_slow(const object_image(L)& objects,
			    unsigned min_holes_count)
    {
      trace::entering("scribo::filter::objects_with_holes");

      mln_precondition(objects.is_valid());

      fun::i2v::array<bool>
	to_keep(static_cast<unsigned>(objects.nlabels()) + 1,
		true);

      bool to_remove = false;
      for_all_components(i, objects.bboxes())
      {
	mln_domain(L) b = objects.bbox(i);
	b.enlarge(1);

	mln_ch_value(L, bool) tmp(b);
	data::fill(tmp, true);
 	data::fill((tmp | ((objects | objects.bbox(i)) | (pw::value(objects) == pw::cst(i))).domain()).rw(), false);

	typedef accu::math::count<mln_value(L)> accu_t;
	mln_value(L) nlabels;
	util::array<unsigned> counts
	  = labeling::blobs_and_compute(tmp,
					c8(), nlabels,
					accu_t()).second();
	unsigned nholes = 0;
	for_all_components(j, counts)
	  if (counts(j) > 4u)
	    ++nholes;

	if (nholes < min_holes_count)
	{
	  to_keep(i) = false;
	  to_remove = true;
	}
      }

      object_image(L) output;
      if (! to_remove)
	output = objects;
      else
	output = internal::compute(objects, to_keep);

      trace::exiting("scribo::filter::objects_with_holes");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECTS_WITH_HOLES_HH
