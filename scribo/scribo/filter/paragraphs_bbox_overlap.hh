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
    paragraphs_bbox_overlap(const paragraph_set<L>& paragraphs);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename L>
      struct order_paragraphs_id
      {
	order_paragraphs_id(const scribo::paragraph_set<L>& paragraphs)
	  : paragraphs_(paragraphs)
	{
	}

	bool operator()(const scribo::paragraph_id_t& l1,
			const scribo::paragraph_id_t& l2) const
	{
	  const unsigned l1_nsites = paragraphs_(l1).bbox().nsites();
	  const unsigned l2_nsites = paragraphs_(l2).bbox().nsites();

	  if (l1_nsites == l2_nsites)
	    return l1 > l2;
	  return l1_nsites > l2_nsites;
	}

	scribo::paragraph_set<L> paragraphs_;
      };

    } // end of namespace scribo::filter::internal


    template <typename L>
    paragraph_set<L>
    paragraphs_bbox_overlap(const paragraph_set<L>& paragraphs)
    {
      trace::entering("scribo::filter::paragraphs_bbox_overlap");

      mln_precondition(paragraphs.is_valid());

      L billboard;
      initialize(billboard, paragraphs.lines().components().labeled_image());
      data::fill(billboard, 0);

      mln::util::array<bool> not_to_ignore(paragraphs.nelements() + 1, true);
      not_to_ignore(0) = false;

      for_all_paragraphs(cur_id, paragraphs)
      {
	const box2d& b_ = paragraphs(cur_id).bbox();

	if (paragraphs(cur_id).nlines() > 1)
	{
	  mln::draw::box_plain(billboard, b_, cur_id);
	  continue;
	}

	const unsigned tl = billboard(b_.pmin());
	const unsigned tr = billboard.at_(b_.pmin().row(), b_.pmax().col());
	const unsigned ml = billboard.at_(b_.pcenter().row(), b_.pmin().col());
	const unsigned mc = billboard.at_(b_.pcenter().row(), b_.pcenter().col());
	const unsigned mr = billboard.at_(b_.pcenter().row(), b_.pmax().col());
	const unsigned bl = billboard.at_(b_.pmax().row(), b_.pmin().col());
	const unsigned br = billboard(b_.pmax());

	typedef std::set<unsigned> set_t;
	set_t labels;
	labels.insert(tl);
	labels.insert(tl);
	labels.insert(tr);
	labels.insert(ml);
	labels.insert(mc);
	labels.insert(mr);
	labels.insert(bl);
	labels.insert(br);

	for (set_t::const_iterator it = labels.begin();
	     it != labels.end();
	     ++it)
	  if (not_to_ignore(*it))
	  {
	    box2d b2 = paragraphs(*it).bbox();
	    box2d b_i = scribo::util::box_intersection(b_, b2);

	    // si b_ est inclus dans une boite donc le nombre de comp > 1 => invalid juste b_
	    // sinon => invalid b_ et b2
	    if ((b_i.nsites() / (float)b_.nsites() > 0.4
		 || (b_i.nsites() / (float)b2.nsites()) > 0.9))
	    {
	      not_to_ignore(cur_id) = false;

	      if (paragraphs(*it).nlines() < 4)
		not_to_ignore(*it) = false;
	    }
	  }

	mln::draw::box_plain(billboard, b_, cur_id);
      }

      paragraph_set<L> output = paragraphs.duplicate();
      output.invalidate(not_to_ignore);

      trace::exiting("scribo::filter::paragraphs_bbox_overlap");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_PARAGRAPHS_BBOX_OVERLAP_HH
