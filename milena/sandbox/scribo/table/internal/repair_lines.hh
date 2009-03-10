// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef SCRIBO_TABLE_INTERNAL_REPAIR_LINES_HH
# define SCRIBO_TABLE_INTERNAL_REPAIR_LINES_HH

/// \file scribo/table/internal/repair_lines.hh
///
/// Repair lines which have small discontinuities.

# include <vector>

# include <mln/core/concept/image.hh>
# include <mln/core/image/extension_val.hh>
# include <mln/core/site_set/box.hh>
# include <mln/data/fill.hh>
# include <mln/util/couple.hh>
# include <mln/util/array.hh>
# include <mln/win/line.hh>
# include <mln/pw/all.hh>

# include <scribo/core/central_sites.hh>
# include <scribo/table/internal/repair_lines.hh>


namespace scribo
{

  namespace table
  {

    namespace internal
    {


# ifndef MLN_INCLUDE_ONLY

      /// Repair lines which have small discontinuities.
      /// FIXME: buggy. Sometimes few lines move or shrink!
      template <unsigned axis, typename I>
      void
      repair_lines(const Image<I>& input_,
		   util::array<box<mln_site(I)> >& tableboxes)
      {
	trace::entering("scribo::table::internal::repair_lines");

	const I& input = exact(input_);
	mln_precondition(input.is_valid());

	typedef mln_site(I) P;
	typedef win::line<mln_grid(P), axis, mln_coord(P)> line_t;

	// Initialization
	mln_ch_value(I,unsigned) l(input.domain());
	data::fill(l, literal::zero);
	for_all_components(i, tableboxes)
	{
	  util::couple<P,P> cp = central_sites(tableboxes[i], axis);
	  l(cp.first()) = i;
	  l(cp.second()) = i;
	}

	// Repair
	extension_val<mln_ch_value(I,unsigned)> l_ext(l, literal::zero);

	util::array<box<P> > result;
	std::vector<bool> to_keep(tableboxes.nelements(), true);

	mln_VAR(tbb_ima, extend(l | pw::value(l) != literal::zero, l));
	//FIXME: use a half window, just the bottom of the vertical line.
	line_t vl(settings.repair_max_dist);
	mln_piter(tbb_ima_t) p(tbb_ima.domain());
	mln_qiter(line_t) q(vl, p);
	for_all(p)
	  for_all(q)
	  if (l_ext(q) != literal::zero && l_ext(q) != l_ext(p))
	  {
	    to_keep[l_ext(q)] = false;

	    tableboxes[l_ext(p)].pmax() = tableboxes[l_ext(q)].pmax();

	    util::couple<P,P> cp = central_sites(tableboxes[l_ext(q)], axis);
	    l_ext(cp.first()) = l_ext(p);
	    l_ext(cp.second()) = l_ext(p);
	  }


	// Remove merged boxes.
	for_all_elements(i, tableboxes)
	  if (to_keep[i])
	    result.append(tableboxes[i]);

	tableboxes = result;
	trace::exiting("scribo::table::internal::repair_lines");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::table::internal

  } // end of namespace scribo::table

} // end of namespace scribo


#endif // ! SCRIBO_TABLE_INTERNAL_REPAIR_LINES_HH
