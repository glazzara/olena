// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_FILTER_PARAGRAPHS_BBOX_OVERLAP_HH
# define SCRIBO_FILTER_PARAGRAPHS_BBOX_OVERLAP_HH

/// \file
///
/// Remove invalid paragraphs.
/// \fixme Share same test canvas as text::merging.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/data/transform.hh>
# include <mln/util/array.hh>

# include <scribo/core/paragraph_set.hh>

#include <mln/labeling/colorize.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// Remove invalid paragraphs.
    ///
    /// \param[in] paragraphs    A paragraph set.
    ///
    /// \return A paragraph set with invalid paragraphs tag set to
    /// Paragraph::Ignored.
    template <typename L>
    paragraph_set<L>
    paragraphs_bbox_overlap(const paragraph_set<L>& parset);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename L>
      struct order_paragraphs_id
      {
	order_paragraphs_id(const scribo::paragraph_set<L>& parset)
	  : parset_(parset)
	{
	}

	bool operator()(const scribo::paragraph_id_t& l1,
			const scribo::paragraph_id_t& l2) const
	{
	  const unsigned l1_nsites = parset_(l1).bbox().nsites();
	  const unsigned l2_nsites = parset_(l2).bbox().nsites();

	  if (l1_nsites == l2_nsites)
	    return l1 > l2;
	  return l1_nsites > l2_nsites;
	}

	scribo::paragraph_set<L> parset_;
      };

    } // end of namespace scribo::filter::internal


    template <typename L>
    paragraph_set<L>
    paragraphs_bbox_overlap(const paragraph_set<L>& parset)
    {
      trace::entering("scribo::filter::paragraphs_bbox_overlap");

      mln_precondition(parset.is_valid());

      mln_ch_value(L, paragraph_id_t) billboard;
      initialize(billboard, parset.lines().components().labeled_image());
      data::fill(billboard, 0);

      mln::util::array<bool> not_to_ignore(parset.nelements() + 1, true);
      not_to_ignore(0) = false;

      paragraph_set<L> output = parset.duplicate();

      mln::util::array<paragraph_id_t> candidate;
      candidate.reserve(parset.nelements());
      for_all_paragraphs(cur_id, parset)
	if (parset(cur_id).is_valid())
	  candidate.append(cur_id);

      std::sort(candidate.hook_std_vector_().begin(),
		candidate.hook_std_vector_().end(),
		internal::order_paragraphs_id<L>(parset));

      for_all_elements(e, candidate)
      {
	paragraph_id_t cur_id = candidate(e);

	const box2d& b_ = parset(cur_id).bbox();

	if (parset(cur_id).nlines() > 3)
	{
	  mln::draw::box_plain(billboard, b_, cur_id);
	  continue;
	}

	const unsigned mc = billboard.at_(b_.pcenter().row(), b_.pcenter().col());

	// Box is mostly in the background => do nothing.
	if (mc == 0)
	{
	  mln::draw::box_plain(billboard, b_, cur_id);
	  continue;
	}
	else // Bbox center is inside another box. Check if we can
	     // merge the current box with it.
	{
	  // Consider other potential overlapping bboxes.
	  const unsigned tl = billboard(b_.pmin());
	  const unsigned tr = billboard.at_(b_.pmin().row(), b_.pmax().col());
	  const unsigned bl = billboard.at_(b_.pmax().row(), b_.pmin().col());
	  const unsigned br = billboard(b_.pmax());

	  typedef std::set<unsigned> set_t;
	  set_t labels;
	  labels.insert(tl);
	  labels.insert(tr);
	  labels.insert(mc);
	  labels.insert(bl);
	  labels.insert(br);

	  // FIXME: check that there are at least 3 points (including
	  // the center) in another paragraph.

	  // The potential merged bbox is already ignored or the
	  // current bbox overlaps with several bboxes.
	  // => Ignore current bbox .
	  //
	  if (!not_to_ignore(mc)
	      || (labels.size() > 1 && labels.find(0) == labels.end()))
	  {
	    mln::draw::box_plain(billboard, b_, cur_id); // Really?
	    not_to_ignore(cur_id) = false;
	    continue;
	  }

	  for (set_t::const_iterator it = labels.begin();
	       it != labels.end(); ++it)
	    if (*it)
	    {
	      mln_assertion(*it != mc);

	      box2d b2 = output(*it).bbox();
	      box2d b_i = scribo::util::box_intersection(b_, b2);
	      volatile float
		b_ratio = b_i.nsites() / (float)b_.nsites();

	      // If the bbox is widely included in another box.
	      if (b_ratio > 0.8)
	      {
		output(mc).fast_merge(output(cur_id));
		mln::draw::box_plain(billboard, parset(mc).bbox(), mc);
	      }
	      else
		mln::draw::box_plain(billboard, parset(cur_id).bbox(), cur_id);
	      break;
	    }

	}
      }

      // 	  if (not_to_ignore(*it))
      // 	  {
      // 	    box2d b2 = output(*it).bbox();
      // 	    box2d b_i = scribo::util::box_intersection(b_, b2);

      // 	    volatile float
      // 	      b_ratio = b_i.nsites() / (float)b_.nsites(),
      // 	      b2_ratio = b_i.nsites() / (float)b2.nsites();

      // 	    if (b2_ratio == 1)
      // 	    {
      // 	      // Merge paragraphs and redraw the new bbox.
      // 	      output(cur_id).fast_merge(output(*it));
      // 	      mln::draw::box_plain(billboard, output(cur_id).bbox(), cur_id);
      // 	    }
      // 	    else if (b_ratio == 1)
      // 	    {
      // 	      // Merge paragraphs and redraw the new bbox.
      // 	      output(*it).fast_merge(output(cur_id));
      // 	      mln::draw::box_plain(billboard, output(*it).bbox(), *it);
      // 	    }
      // 	    else if ((b_ratio > 0.4 || b2_ratio > 0.9))
      // 	    {
      // 	      // si b_ est inclus dans une boite dont le nombre de
      // 	      // comp > 4 => invalid juste b_ sinon => invalid b_ et
      // 	      // b2
      // 	      not_to_ignore(cur_id) = false;

      // 	      if (parset(*it).nlines() < 4)
      // 		not_to_ignore(*it) = false;
      // 	    }
      // 	  }

      // 	mln::draw::box_plain(billboard, b_, cur_id);
      // }

      output.invalidate(not_to_ignore);

      for_all_paragraphs(p, output)
      	if (output(p).is_valid())
      	  output(p).force_stats_update();

      trace::exiting("scribo::filter::paragraphs_bbox_overlap");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_PARAGRAPHS_BBOX_OVERLAP_HH
