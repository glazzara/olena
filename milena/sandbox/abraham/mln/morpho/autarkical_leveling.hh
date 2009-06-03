// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MORPHO_AUTARKICAL_LEVELING_HH
# define MLN_MORPHO_AUTARKICAL_LEVELING_HH

/// \file mln/morpho/autarkical_leveling.hh
///
/// Morphological autarkical leveling filter.
///
/// \todo The overloads are hidden and I don't know why!

# include <mln/morpho/includes.hh>
# include <mln/morpho/general.hh>
# include <mln/norm/l2.hh>
# include <mln/data/paste.hh>

namespace mln
{

  namespace morpho
  {

    /// Morphological autarkical_leveling.
    template <typename I, typename W>
    mln_concrete(I)
    autarkical_leveling(const Image<I>& input, const Image<I>& marker, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // On set with centered window (overloads).

      template <typename I, typename W>
      mln_concrete(I)
      autarkical_leveling(const Image<I>& input_, const Image<I>& marker_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::general_on_set_centered__autarkical_leveling");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const I& marker = exact(marker_);
	const W& win = exact(win_);

	O output;
	O ref;
	output = duplicate(input);
	ref = duplicate(input);

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	mln_qiter(W) r(win, p);
	bool stable = false;
	while (!stable)
	{
	  stable = true;
	  for_all(p)
	  {
	    mln_psite(W) v;
	    double min_dist = std::numeric_limits<double>::infinity();
	    bool same_side = true;

	    for_all(q) if (ref.domain().has(q))
	    {
	      for_all(r) if (ref.domain().has(r) && q!=r)
		if ((marker(q) - ref(p)) * (marker(r) - ref(p)) < 0)
		{
		  same_side = false;
		  break;
		}

	      if (!same_side)
		break;

	      double dist = norm::l2(ref(p) - marker(q));
	      if (dist < min_dist)
	      {
		min_dist = dist;
		v = q;
	      }
	    }
	    if (same_side)
	    {
	      if (output(p) != marker(v))
		stable = false;
	      output(p) = marker(v);
	    }
	    else
	    {
	      if (output(p) != ref(p))
		stable = false;
	      output(p) = ref(p);
	    }
	  }
	  data::paste(output, ref);
	}

	trace::exiting("morpho::impl::general_on_set_centered__autarkical_leveling");
	return output;
      }


    } // end of namespace morpho::impl


    template <typename I, typename W>
    inline
    mln_concrete(I)
    autarkical_leveling(const Image<I>& input, const Image<I>& marker, const Window<W>& win)
    {
      trace::entering("morpho::autarkical_leveling");
      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = impl::autarkical_leveling(input, marker, win);

      trace::exiting("morpho::autarkical_leveling");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_AUTARKICAL_LEVELING_HH
