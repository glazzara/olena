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
# include <mln/core/routine/duplicate.hh>
# include <mln/data/fill.hh>
# include <mln/level/compare.hh>
# include <mln/data/paste.hh>
# include <mln/level/sort_psites.hh>
# include <mln/literal/zero.hh>
# include <mln/math/max.hh>
# include <mln/math/min.hh>
# include <mln/accu/max.hh>

# include <tests/timer.hh>

# include <mln/pw/value.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include "canvas/self_dual_reconstruction.hh"
# include "reconstructions_on_function.hh"

namespace mln
{

  namespace impl
  {

    template <typename I_, typename J_>
    struct self_dual_reconstruction_t
    {
      // d1 : marker(p) < mask(p) -> dilation
      // d2 : marker(p) > mask(p) -> erosion

      typedef I_ I;
      typedef J_ J;
      typedef mln_site(I) P;
      typedef mln_value(I) V;
      typedef p_array<mln_psite(I)> S;

      self_dual_reconstruction_t (const I& marker, const J& mask, mln_concrete(I)& output)
	: marker(marker),
	  mask(mask),
	  output(output),
	  d1_s(level::sort_psites_decreasing(mask | (pw::value(marker) <= pw::value(mask)))),
	  d2_s(level::sort_psites_increasing(mask | (pw::value(marker) > pw::value(mask))))
      {
      }

      bool is_in_d1(const P& p) { return marker(p) <= mask(p); }
      bool is_in_d2(const P& p) { return marker(p) > mask(p); }



      bool d1_is_active(const P& r, const P& p)
      {
	mln_assertion(is_in_d1(r) && is_in_d1(p));
	return output(r) <= mask(p);
      }

      void d1_merge(const P& r, const P& p)
      {
	mln_assertion(is_in_d1(r) && is_in_d1(p));
	if (output(r) > output(p))
	  output(p) = output(r);
      }


      bool d2_is_active(const P& r, const P& p)
      {
	mln_assertion(is_in_d2(r) && is_in_d2(p));
	return output(r) >= mask(p);
      }

      void d2_merge(const P& r, const P& p)
      {
	mln_assertion(is_in_d2(r) && is_in_d2(p));
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

    mln_precondition(marker.is_valid());
    mln_precondition(mask.is_valid());
    mln_precondition(mask.domain() == marker.domain());

    mln_concrete(I) output;
    initialize(output, marker);

    typedef impl::self_dual_reconstruction_t<I, J> F;
    timer t;
    t.start();
    F f(marker, mask, output);
    std::cout << "Functor construction: " << t << std::endl;

    canvas::morpho::self_dual_reconstruction(nbh, f);

    trace::exiting("morpho::self_dual_reconstruction");
    return output;
  }


  template <typename I, typename J, typename N>
  mln_concrete(I)
  self_dual_reconstruction_ref(const Image<I>& marker_,
			       const Image<J>& mask_,
			       const Neighborhood<N>& nbh_)
  {
    trace::entering("morpho::self_dual_reconstruction_ref");

    const I& marker = exact(marker_);
    const J& mask   = exact(mask_);
    const N& nbh    = exact(nbh_);

    mln_precondition(marker.is_valid());
    mln_precondition(mask.is_valid());
    mln_precondition(mask.domain() == marker.domain());

    mln_concrete(I) output;
    initialize(output, marker);

    mln_concrete(I) by_dilation =
      reconstruction_on_function_by_dilation
      (marker | (pw::value(marker) < pw::value(mask)),
       mask   | (pw::value(marker) < pw::value(mask)),
       nbh);

    io::pgm::save(by_dilation, "sd_by_dilation.pgm");

    mln_concrete(I) by_erosion =
      reconstruction_on_function_by_erosion
      (marker | (pw::value(marker) > pw::value(mask)),
       mask   | (pw::value(marker) > pw::value(mask)),
       nbh);

    io::pgm::save(by_erosion, "sd_by_erosion.pgm");

    mln_piter(I) p(output.domain());
    for_all(p)
    {
      if (marker(p) > mask(p))
	output(p) = by_erosion(p);
      else if (marker(p) < mask(p))
	output(p) = by_dilation(p);
	else
	output(p) = mask(p);
    }

    trace::exiting("morpho::self_dual_reconstruction_ref");
    return output;
  }


} // end of namespace mln.


#endif // ! MLN_SELF_DUAL_RECONSTRUCTION_IMPL_HH
