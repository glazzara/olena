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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_RD_HH
# define SCRIBO_PRIMITIVE_INTERNAL_RD_HH

/// \file
///
/// \brief Tolerant constrained reconstruction algorithm.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/border/equalize.hh>
#include <mln/border/fill.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/util/timer.hh>


namespace scribo
{

  namespace primitive
  {

    namespace internal
    {

      using namespace mln;

      /// \brief Tolerant constrained reconstruction algorithm.
      template <typename I, typename J>
      mln_concrete(I)
      rd(const Image<I>& ima, const Image<J>& constraint);


# ifndef MLN_INCLUDE_ONLY

      template <typename Par>
      inline
      unsigned
      Rd_find_root_(Par& parent, unsigned x)
      {
	if (parent.element(x) == x)
	  return x;
	else
	  return parent.element(x) = Rd_find_root_(parent, parent.element(x));
      }

      template <typename I, typename J, typename N>
      inline
      mln_concrete(I)
      Rd(const Image<I>& f_, const Image<J>& g_, const Neighborhood<N>& nbh_)
      {
	mln_trace("Rd");

	const I& f = exact(f_);
	const J& g = exact(g_);
	const N& nbh = exact(nbh_);

	typedef mln_psite(I) P;
	typedef mln_value(I) V;

	// Auxiliary data.
	mln_ch_value(I, bool)     deja_vu;
	mln_ch_value(I, unsigned) parent;
	mln_concrete(I)           output;

	// Initialization.
	{
	  border::equalize(f, g, nbh.delta());
	  mln::border::fill(g, false);

	  initialize(output, f);
	  mln::data::fill(output, false);

	  initialize(parent, f);
	}

	mln::util::array<int> dp = negative_offsets_wrt(f, nbh);
	const unsigned n_nbhs = dp.nelements();

	// First pass.
	{
	  mln_fwd_pixter(const I) pxl_g(g);
	  for_all(pxl_g)
	  {
	    if (pxl_g.val() == false)
	      continue;

	    unsigned p = pxl_g.offset();

	    // Make-Set.
	    parent.element(p) = p;
	    // Init.
	    output.element(p) = f.element(p);

	    for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      unsigned n = p + dp[j];
	      if (g.element(n) == true)
	      {
		mln_invariant(f.domain().has(f.point_at_offset(n)));
		// Do-Union.
		unsigned r = Rd_find_root_(parent, n);
		if (r != p)
		{
		  // Set-Parent:
		  parent.element(r) = p;
		  // Merge:
		  output.element(p) = output.element(p) || output.element(r);
		}
	      }
	    }
	  }
	}

	// Second pass.
	{
	  mln_bkd_pixter(const I) pxl_g(g);
	  for_all(pxl_g)
	  {
	    if (pxl_g.val() == false)
	      continue;
	    unsigned p = pxl_g.offset();
	    output.element(p) = output.element(parent.element(p));
	  }
	}

	return output;
      }




      template <typename I, typename J>
      mln_concrete(I)
      rd(const Image<I>& ima, const Image<J>& constraint)
      {
	mln_trace("scribo::primitive::internal::rd");

	mln_precondition(exact(ima).is_valid());
	mln_precondition(exact(constraint).is_valid());

	using namespace mln;

	// FIXME: not generic.
	neighb2d nbh = c4();

	mln_concrete(I) output = Rd(ima, constraint, nbh);

	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::internal

  } // end of namespace scribo::primitive

} // end of namespace scribo




#endif // ! SCRIBO_PRIMITIVE_INTERNAL_RD_HH
