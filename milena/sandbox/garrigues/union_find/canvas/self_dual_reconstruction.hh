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

#ifndef MLN_SELF_DUAL_RECONSTRUCTION_HH
# define MLN_SELF_DUAL_RECONSTRUCTION_HH

# include <mln/core/image/image2d.hh>
# include <mln/data/fill.hh>
# include <mln/literal/zero.hh>
# include <mln/convert/to_upper_window.hh>
# include <mln/accu/count.hh>
# include <mln/util/pix.hh>

# include <mln/core/alias/neighb2d.hh>

# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/load.hh>

namespace mln
{

  namespace canvas
  {

    namespace morpho
    {

      template <typename I>
      static inline
      mln_psite(I)
      find_root(I& parent,
		const mln_psite(I)& x)
      {
	if (parent(x) == x)
	  return x;
	else
	  return parent(x) = find_root(parent, parent(x));
      }

      template <typename N, typename F>
      inline
      void
      self_dual_reconstruction_tests(const Neighborhood<N>& nbh_,
				     F& f)
      {
	// Static tests.

	// Types of f required.
	typedef typename F::I I;
	typedef Image<I> I_;
	typedef typename F::S S;

	// Attributs of f required.
	(void) f.d1_s;
	(void) f.d2_s;
	(void) f.mask;
	(void) f.marker;
	(void) f.output;


	// Method of F required
	typedef mln_site(I) P;
	bool (F::*m1)(const P&, const P&) = & F::d1_is_active;
	m1 = 0;
	void (F::*m2)(const P&, const P&) = & F::d1_merge;
	m2 = 0;
	bool (F::*m3)(const P&, const P&) = & F::d2_is_active;
	m3 = 0;
	void (F::*m4)(const P&, const P&) = & F::d2_merge;
	m4 = 0;

	// Dynamic tests.
	mln_precondition(f.mask.domain() == f.output.domain());
	mln_precondition(f.mask.domain() == f.marker.domain());
      }

      template <typename N, typename F>
      void
      self_dual_reconstruction(const Neighborhood<N>& nbh_,
			       F& f)
      {
	trace::entering("canvas::morpho::self_dual_reconstruction");

	// Tests.
	self_dual_reconstruction_tests(nbh_, f);

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
	  // Body of D1.
	  {
	    mln_fwd_piter(S) p(f.d1_s);
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	    {
	      // Make set.
	      parent(p) = p;

	      for_all(n) if (f.mask.domain().has(n) && deja_vu(n))
	      {
		mln_assertion(f.is_in_d1(n));
		//do_union(n, p);
		P r = find_root(parent, n);
		if (r != p)
		  if (f.d1_is_active(r, p))
		  {
		    parent(r) = p;
		    f.d1_merge(r, p);
		  }
		  else
		    f.d1_escape(p);
	      }

	      deja_vu(p) = true;
	    }
	  }

	  mln::data::fill(deja_vu, false);

	  // Body of D2.
	  {
	    mln_fwd_piter(S) p(f.d2_s);
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	    {
	      // Make set.
	      parent(p) = p;

	      for_all(n) if (f.mask.domain().has(n) && deja_vu(n))
	      {
		mln_assertion(f.is_in_d2(n));
		//do_union(n, p);
		P r = find_root(parent, n);
		if (r != p)
		  if (f.d2_is_active(r, p))
		  {
		    parent(r) = p;
		    f.d2_merge(r, p);
		  }
		  else
		    f.d2_escape(p);
	      }

	      deja_vu(p) = true;
	    }
	  }
	}

	// second pass
	{
	  {
	    mln_bkd_piter(S) p(f.d1_s);
	    for_all(p)
	    {
	      if (parent(p) == p) // if p is a root.
	      {
		if (f.output(p) == f.d1_escape_value)
		  f.output(p) = f.mask(p);
	      }
	      else
		f.output(p) = f.output(parent(p));

	    }
	  }
	  {
	    mln_bkd_piter(S) p(f.d2_s);
	    for_all(p)
	    {
	      if (parent(p) == p) // if p is a root.
	      {
		if (f.output(p) == f.d2_escape_value)
		  f.output(p) = f.mask(p);
	      }
	      else
 		f.output(p) = f.output(parent(p));

	    }
	  }
	}

	trace::exiting("canvas::morpho::self_dual_reconstruction");
      }


    } // end of namespace mln::canvas::morpho

  } // end of namespace mln::canvas

} // end of namespace mln.

#endif // ! MLN_SELF_DUAL_RECONSTRUCTION_HH
