// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_SELF_DUAL_RECONSTRUCTION_IMPL_HH
# define MLN_SELF_DUAL_RECONSTRUCTION_IMPL_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/routine/clone.hh>
# include <mln/level/fill.hh>
# include <mln/level/compare.hh>
# include <mln/level/paste.hh>
# include <mln/level/sort_psites.hh>
# include <mln/literal/zero.hh>
# include <mln/math/max.hh>
# include <mln/math/min.hh>
# include <mln/accu/max.hh>

# include <mln/pw/value.hh>
# include <mln/core/image/image_if.hh>

# include "canvas/self_dual_reconstruction.hh"

namespace mln
{

  namespace impl
  {

    template <typename I_, typename J_>
    struct self_dual_reconstruction_t
    {
      // d1 : marker(p) < mask(p)
      // d2 : marker(p) > mask(p)

      typedef I_ I;
      typedef J_ J;
      typedef mln_site(I) P;
      typedef p_array<mln_psite(I)> S;

      self_dual_reconstruction_t (const I& marker, const J& mask, mln_concrete(I)& output)
	: marker(marker),
	  mask(mask),
	  output(output)
	  // 	  d1_s(level::sort_psites_increasing(mask | (pw::value(marker) > pw::value(mask)))),
	  // 	  d2_s(level::sort_psites_decreasing(mask | (pw::value(marker) > pw::value(mask))))
      {
	S d1, d2;
	mln_piter(I) p(mask.domain());
	for_all(p)
	{
	  if (is_in_d1(p))
	    d1_s.append(p);
	  else
	    if (is_in_d2(p))
	      d2_s.append(p);
	}

      }

      bool is_in_d1(const P& p) { return marker(p) < mask(p); }
      bool d1_is_active(const P& r, const P& p) { return output(r) <= mask(p); }
      void d1_merge(const P& r, const P& p)
      {
	if (output(r) > output(p))
	  output(p) = output(r);
      }

      bool is_in_d2(const P& p) { return marker(p) > mask(p); }
      bool d2_is_active(const P& r, const P& p) { return output(r) <= mask(p); }
      void d2_merge(const P& r, const P& p)
      {
	if (output(r) < output(p))
	  output(p) = output(r);
      }

      const I& marker; // F
      const J& mask; // G
      mln_concrete(I)& output; // O

      S d1_s, d2_s;
    };

  } // end of namespace mln::impl.

  template <typename I, typename J, typename N>
  mln_concrete(I)
  self_dual_reconstruction(const Image<I>& marker_,
			   const Image<J>& mask_,
			   const Neighborhood<N>& nbh_)
  {
    trace::entering("morpho::self_dual_reconstruction");

    const I& marker = exact(marker_);
    const J& mask   = exact(mask_);
    const N& nbh    = exact(nbh_);

    mln_precondition(marker.has_data());
    mln_precondition(mask.has_data());
    mln_precondition(mask.domain() == marker.domain());

    mln_concrete(I) output;
    initialize(output, marker);

    typedef impl::self_dual_reconstruction_t<I, J> F;
    F f(marker, mask, output);
    canvas::morpho::self_dual_reconstruction(nbh, f);

    trace::exiting("morpho::self_dual_reconstruction");
    return output;
  }

} // end of namespace mln.


#endif // ! MLN_SELF_DUAL_RECONSTRUCTION_IMPL_HH
