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

#ifndef MLN_RECONSTRUCTIONS_ON_SET_HH
# define MLN_RECONSTRUCTIONS_ON_SET_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/data/fill.hh>
# include <mln/data/paste.hh>
# include <mln/data/compare.hh>

# include "canvas/reconstruction_on_set.hh"

namespace mln
{

  namespace impl
  {

    struct reconstruction_on_set_by_dilation_t
    {
      typedef image2d<bool> I;
      typedef mln_site_(I) P;

      reconstruction_on_set_by_dilation_t (const I& marker, const I& mask, I& output)
	: marker(marker), mask(mask), output(output)
      {}

      bool is_in_D(P p) { return mask(p); }
      void set_default_output() { data::paste(marker, output); }
      void init(P p) { (void) p; }
      void merge(P r, P p) { output(p) = output(p) || output(r); }
      void visit_ext_border(P n, P p) {	(void) n; (void) p; }

      const I& marker; // F
      const I& mask; // G
      I& output; // O
    };


    struct reconstruction_on_set_by_dilation_alt_t
    {
      typedef image2d<bool> I;
      typedef mln_site_(I) P;

      reconstruction_on_set_by_dilation_alt_t (const I& marker, const I& mask, I& output)
	: marker(marker), mask(mask), output(output)
      {}

      bool is_in_D(P p) { return mask(p) && !marker(p); }
      void set_default_output() { data::paste(marker, output); }
      void init(P p) { output(p) = false; }
      void merge(P r, P p) { output(p) = output(p) || output(r); }
      void visit_ext_border(P n, P p) {	if (marker(n)) output(p) = true; }

      const I& marker; // F
      const I& mask; // G
      I& output; // O
    };


    struct reconstruction_on_set_by_erosion_t
    {
      typedef image2d<bool> I;
      typedef mln_site_(I) P;

      reconstruction_on_set_by_erosion_t (const I& marker, const I& mask, I& output)
	: marker(marker), mask(mask), output(output)
      {}

      bool is_in_D(P p) { return !mask(p); }
      void set_default_output() { data::paste(marker, output); }
      void init(P p) { (void) p; }
      void merge(P r, P p) { output(p) = output(p) && output(r); }
      void visit_ext_border(P n, P p) { (void) n; (void) p; }

      const I& marker; // F
      const I& mask; // G
      I& output; // O
    };

    struct reconstruction_on_set_by_erosion_alt_t
    {
      typedef image2d<bool> I;
      typedef mln_site_(I) P;

      reconstruction_on_set_by_erosion_alt_t (const I& marker, const I& mask, I& output)
	: marker(marker), mask(mask), output(output)
      {}

      bool is_in_D(P p) { return !mask(p) && marker(p); }
      void set_default_output() { data::paste(mask, output); }
      void init(P p) {  output(p) = true; }
      void merge(P r, P p) { output(p) = output(p) && output(r); }
      void visit_ext_border(P n, P p) {	if (!marker(n)) output(p) = false; }

      const I& marker; // F
      const I& mask; // G
      I& output; // O
    };

  } // end of namespace mln::impl


  image2d<bool>
  reconstruction_on_set_by_dilation(const image2d<bool>& marker,
				    const image2d<bool>& mask,
				    const neighb2d& nbh)
  {
    trace::entering("morpho::reconstruction_on_set_by_dilation");

    mln_precondition(exact(marker).is_valid());
    mln_precondition(exact(mask).is_valid());
    mln_precondition(exact(marker) <= exact(mask));
    image2d<bool> output;
    initialize(output, marker);

    typedef impl::reconstruction_on_set_by_dilation_t F;
    F f(marker, mask, output);
    canvas::morpho::reconstruction_on_set(nbh, f);

    trace::exiting("morpho::reconstruction_on_set_by_dilation");
    return output;
  }


  image2d<bool>
  reconstruction_on_set_by_dilation_alt(const image2d<bool>& marker,
					const image2d<bool>& mask,
					const neighb2d& nbh)
  {
    trace::entering("morpho::reconstruction_on_set_by_dilation_alt");

    mln_precondition(exact(marker).is_valid());
    mln_precondition(exact(mask).is_valid());
    mln_precondition(exact(marker) <= exact(mask));

    image2d<bool> output;
    initialize(output, marker);

    typedef impl::reconstruction_on_set_by_dilation_alt_t F;
    F f(marker, mask, output);
    canvas::morpho::reconstruction_on_set(nbh, f);

    trace::exiting("morpho::reconstruction_on_set_by_dilation_alt");
    return output;
  }


  image2d<bool>
  reconstruction_on_set_by_erosion(const image2d<bool>& marker,
				   const image2d<bool>& mask,
				   const neighb2d& nbh)
  {
    trace::entering("morpho::reconstruction_on_set_by_erosion");

    mln_precondition(exact(marker).is_valid());
    mln_precondition(exact(mask).is_valid());
    mln_precondition(exact(marker) >= exact(mask));

    image2d<bool> output;
    initialize(output, marker);

    typedef impl::reconstruction_on_set_by_erosion_t F;
    F f(marker, mask, output);
    canvas::morpho::reconstruction_on_set(nbh, f);

    trace::exiting("morpho::reconstruction_on_set_by_erosion");
    return output;
  }


  image2d<bool>
  reconstruction_on_set_by_erosion_alt(const image2d<bool>& marker,
				       const image2d<bool>& mask,
				       const neighb2d& nbh)
  {
    trace::entering("morpho::reconstruction_on_set_by_erosion_alt");

    mln_precondition(exact(marker).is_valid());
    mln_precondition(exact(mask).is_valid());
    mln_precondition(exact(marker) >= exact(mask));

    image2d<bool> output;
    initialize(output, marker);

    typedef impl::reconstruction_on_set_by_erosion_alt_t F;
    F f(marker, mask, output);
    canvas::morpho::reconstruction_on_set(nbh, f);

    trace::exiting("morpho::reconstruction_on_set_by_erosion_alt");
    return output;
  }

} // end of namespace mln.


#endif // ! MLN_RECONSTRUCTIONS_ON_SET_HH
