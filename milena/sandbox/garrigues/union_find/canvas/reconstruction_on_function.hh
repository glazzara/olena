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

#ifndef MLN_RECONSTRUCTION_ON_FUNCTION_HH
# define MLN_RECONSTRUCTION_ON_FUNCTION_HH

# include <mln/core/image/image2d.hh>
# include <mln/data/fill.hh>
# include <mln/literal/zero.hh>
# include <mln/convert/to_upper_window.hh>
# include <mln/accu/math/count.hh>
# include <mln/util/pix.hh>

# include <mln/core/alias/neighb2d.hh>

# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/load.hh>

# include <canvas/find_root.hh>

namespace mln
{

  namespace canvas
  {

    namespace morpho
    {

      template <typename N, typename F>
      inline
      void
      reconstruction_on_function_tests(const Neighborhood<N>& nbh_,
				       F& f)
      {
	// Static tests.

	// Types of f required.
	typedef typename F::I I;
	typedef Image<I> I_;
	typedef typename F::S S;

	// Attributs of f required.
	(void) f.s;
	(void) f.mask;
	(void) f.marker;
	(void) f.output;


	// Method of F required
	typedef mln_site(I) P;
	bool (F::*m1)(const P&, const P&) = & F::is_active;
	m1 = 0;
	void (F::*m2)(const P&, const P&) = & F::merge;
	m2 = 0;

	// Dynamic tests.
	mln_precondition(f.mask.domain() == f.output.domain());
	mln_precondition(f.mask.domain() == f.marker.domain());
      }

      template <typename N, typename F>
      void
      reconstruction_on_function(const Neighborhood<N>& nbh_,
				 F& f)
      {
	trace::entering("canvas::morpho::reconstruction_on_function");

	// Tests.
	reconstruction_on_function_tests(nbh_, f);

	const N& nbh = exact(nbh_);

	// Local type.
	typedef typename F::I I;
	typedef typename F::S S;
	typedef mln_site(I) P;
	typedef mln_value(I) V;

	// Auxiliary data.
	mln_ch_value(I, bool)  deja_vu;
	mln_ch_value(I, P)     parent;

	// init
	{
	  initialize(deja_vu, f.mask);
	  initialize(parent, f.mask);

	  mln::data::fill(deja_vu, false);
	  data::fill(f.output, f.marker);
	}

	// first pass
	{
	  mln_fwd_piter(S) p(f.s);
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	  {
	    // Make set.
	    parent(p) = p;

	    for_all(n) if (f.mask.domain().has(n) && deja_vu(n))
	    {
	      //do_union(n, p);
	      P r = find_root(parent, n);
	      if (r != p)
		if (f.is_active(r, p))
		{
		  parent(r) = p;
		  f.merge(r, p);
		}
		else
		  f.output(p) = f.mask(p);
	    }
	    deja_vu(p) = true;
	  }
	}

	// second pass
	{
	  mln_bkd_piter(S) p(f.s);
	  for_all(p)
	  {
 	    if (parent(p) != p) // if p is not a root.
  	      f.output(p) = f.output(parent(p));

	  }
	}

	trace::exiting("canvas::morpho::reconstruction_on_function");
      }

    } // end of namespace mln::canvas::morpho

  } // end of namespace mln::canvas

} // end of namespace mln.

#endif // ! MLN_RECONSTRUCTION_ON_FUNCTION_HH
