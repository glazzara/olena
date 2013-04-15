// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_TABLE_INTERNAL_REPAIR_LINES_HH
# define SCRIBO_TABLE_INTERNAL_REPAIR_LINES_HH

/// \file
///
/// \brief Repair lines which have small discontinuities.

# include <vector>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/core/image/dmorph/extension_val.hh>

# include <mln/pw/all.hh>

# include <mln/literal/zero.hh>

# include <mln/core/site_set/box.hh>

# include <mln/core/routine/extend.hh>

# include <mln/data/fill.hh>

# include <mln/util/couple.hh>
# include <mln/util/array.hh>
# include <mln/util/ord.hh>

# include <mln/win/line.hh>

# include <mln/labeling/colorize.hh>

# include <mln/value/rgb8.hh>
# include <mln/value/label_16.hh>

# include <mln/core/var.hh>

# include <scribo/make/debug_filename.hh>

# include <scribo/core/central_sites.hh>
# include <scribo/core/macros.hh>
# include <scribo/table/internal/repair_lines.hh>


namespace scribo
{

  namespace table
  {

    namespace internal
    {

      using namespace mln;

      /// \brief Repair lines which have small discontinuities.
      /// FIXME: buggy. Sometimes few lines move or shrink!
      ///
      /// \param[in] input		A binary image.
      /// \param[in,out] tableboxes	Vertical or horizontal lines.
      /// \param[in] max_discontinuity	The maximum discontinuity length which
      ///				can be repaired.
      //
      template <unsigned axis, typename I>
      void
      repair_lines(const Image<I>& input,
		   mln::util::array<box<mln_site(I)> >& tableboxes,
		   unsigned max_discontinuity);


# ifndef MLN_INCLUDE_ONLY


      template <unsigned axis, typename I>
      void
      repair_lines(const Image<I>& input_,
		   mln::util::array<box<mln_site(I)> >& tableboxes,
		   unsigned max_discontinuity)
      {
	mln_trace("scribo::table::internal::repair_lines");

	const I& input = exact(input_);
	mln_precondition(input.is_valid());

	typedef mln_site(I) P;
	typedef win::line<mln_grid(P), axis, mln_coord(P)> line_t;

	// Initialization
	typedef mln_ch_value(I,value::label_16) L;
	L l(input.domain());
	data::fill(l, literal::zero);
	for_all_comps(i, tableboxes)
	{
	  mln::util::couple<P,P> cp = central_sites(tableboxes[i], axis);
	  l(cp.first()) = i;
	  l(cp.second()) = i;
	}

	// Repair
	extension_val<L> l_ext(l, literal::zero);

	mln::util::array<box<P> > result;
	std::vector<bool> to_keep(tableboxes.nelements(), true);

	//FIXME: use mln_VAR?
	typedef
	  extension_ima<
	      const image_if<L,
	         mln::fun::neq_v2b_expr_<pw::value_<L>,
				       pw::cst_<literal::zero_t> > >, const L> tbb_ima_t;
	tbb_ima_t tbb_ima = extend(l | (pw::value(l) != pw::cst(literal::zero)), l);

	//FIXME: use a half window, just the bottom of the vertical line.
	line_t vl(max_discontinuity);
	mln_piter(tbb_ima_t) p(tbb_ima.domain());
	mln_qiter(line_t) q(vl, p);
	for_all(p)
	{
	  mln::util::couple<P,P> cp_p = central_sites(tableboxes[l_ext(p)], axis);
	  for_all(q)
	  if (l_ext(q) != literal::zero && l_ext(q) != l_ext(p))
	  {
	    if (mln::util::ord_strict(tableboxes[l_ext(p)].pmax(),
				 tableboxes[l_ext(q)].pmax()))
	    {
	      tableboxes[l_ext(p)].pmax() = tableboxes[l_ext(q)].pmax();
	      to_keep[l_ext(q)] = false;
	    }

	    if (mln::util::ord_strict(tableboxes[l_ext(q)].pmin(),
				 tableboxes[l_ext(p)].pmin()))
	    {
	      tableboxes[l_ext(p)].pmin() = tableboxes[l_ext(q)].pmin();
	      to_keep[l_ext(q)] = false;
	    }

	    if (!to_keep[l_ext(q)])
	    {
	      mln::util::couple<P,P> cp_q = central_sites(tableboxes[l_ext(q)], axis);
	      l_ext(cp_q.first()) = literal::zero;
	      l_ext(cp_q.second()) = literal::zero;

	      unsigned p_i = l_ext(p);
	      l_ext(cp_p.first()) = literal::zero;
	      l_ext(cp_p.second()) = literal::zero;

	      mln::util::couple<P,P> new_cp_p = central_sites(tableboxes[p_i], axis);
	      l_ext(new_cp_p.first()) = p_i;
	      l_ext(new_cp_p.second()) = p_i;
	    }
	  }
	}


	// Remove merged boxes.
	for_all_elements(i, tableboxes)
	  if (to_keep[i])
	    result.append(tableboxes[i]);

	tableboxes = result;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::table::internal

  } // end of namespace scribo::table

} // end of namespace scribo


#endif // ! SCRIBO_TABLE_INTERNAL_REPAIR_LINES_HH
