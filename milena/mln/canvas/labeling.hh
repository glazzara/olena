// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CANVAS_LABELING_HH
# define MLN_CANVAS_LABELING_HH

/*! \file mln/canvas/labeling.hh
 *
 * \brief Connected component labeling of the object part in a binary
 * image.
 */

# include <mln/core/concept/image.hh>
# include <mln/level/fill.hh>
# include <mln/convert/to_window.hh> // FIXME: to_upper_window


namespace mln
{

  namespace canvas
  {

    // General version.

    template <typename F>
    struct labeling
    {
      // Functor.
      F& f;

      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::L L;
      typedef typename F::S S;

      // Local type.
      typedef mln_psite(I) point;

      // Auxiliary data.
      mln_ch_value(I, bool)  deja_vu;
      mln_ch_value(I, point) parent;

      // Output.
      mln_ch_value(I, L) output;
      L nlabels;
      bool status;

      // Ctor.
      labeling(F& f)
	: f(f)
      {
	trace::entering("canvas::labeling");

	init();
	f.init(); // Client initialization.
	pass_1();
	pass_2();

	trace::exiting("canvas::labeling");
      }


      void init()
      {
	initialize(deja_vu, f.input);
	mln::level::fill(deja_vu, false);
	initialize(parent, f.input);
	initialize(output, f.input);
	mln::level::fill(output, 0); // FIXME: Use literal::zero. 
	nlabels = 0;
      }

      void pass_1()
      {
	mln_fwd_piter(S) p(f.s);
	mln_niter(N) n(f.nbh, p);
	for_all(p) if (f.handles(p))
	  {
	    make_set(p);
	    for_all(n)
	      if (f.input.has(n) && deja_vu(n))
		if (f.equiv(n, p))
		  do_union(n, p);
		else
		  f.do_no_union(n, p);
	    deja_vu(p) = true;
	  }
      }

      void pass_2()
      {
	mln_bkd_piter(S) p(f.s);
	for_all(p) if (f.handles(p))
	  {
	    if (is_root(p))
	      {
		if (f.labels(p))
		  {
		    if (nlabels == mln_max(L))
		      {
			status = false;
			return;
		      }
		    output(p) = ++nlabels;
		  }
	      }
	    else
	      output(p) = output(parent(p));
	  }
	status = true;
      }


      // Auxiliary methods.
      
      void make_set(const point& p)
      {
	parent(p) = p;
	f.init_attr(p);
      }

      bool is_root(const point& p) const
      {
	return parent(p) == p;
      }

      point find_root(const point& x)
      {
	if (parent(x) == x)
	  return x;
	else
	  return parent(x) = find_root(parent(x));
      }

      void do_union(const point& n, const point& p)
      {
	point r = find_root(n);
	if (r != p)
	  {
	    parent(r) = p;
	    f.merge_attr(r, p);
	  }
      }

    };




    template <typename F>
    struct labeling_fastest
    {
      // Functor.
      F& f;
      
      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::L L;
      
      // Auxiliary data.
      mln_ch_value(I, unsigned) parent;

      // Output.
      mln_ch_value(I, L) output;
      L nlabels;
      bool status;

      // Ctor.
      labeling_fastest(F& f)
	: f(f)
      {
	trace::entering("canvas::labeling_fastest");

	init();
	f.init(); // Client initialization.
	pass_1();
	pass_2();

	trace::exiting("canvas::labeling_fastest");
      }

      
      void init()
      {
	initialize(parent, f.input);
	for (unsigned p = 0; p < parent.ncells(); ++p)
	  parent[p] = p; // make_set
	initialize(output, f.input);
	mln::level::fill(output, 0); // FIXME: Use literal::zero. 
	nlabels = 0;
      }

      void pass_1()
      {
	mln_bkd_pixter(const I) p(f.input);

	typedef window<mln_dpoint(I)> W;
	W win = mln::convert::to_upper_window(f.nbh);
	mln_qixter(const I, W) n(p, win);
	  
	for_all(p) if (f.handles(p))
	  {
	    f.init_attr(p);
	    for_all(n)
	      if (f.equiv(n, p))
		do_union(n, p);
	      else
		f.do_no_union(n, p);
	  }
      }
      
      void pass_2()
      {
	mln_fwd_pixter(const I) p(f.input);

	for_all(p) if (f.handles(p))
	  {
	    if (is_root(p))
	      {
		if (f.labels(p))
		  {
		    if (nlabels == mln_max(L))
		      {
			status = false;
			return;
		      }
		    output[p] = ++nlabels;
		  }
	      }
	    else
	      output[p] = output[parent[p]];
	  }
	status = true;
      }

      // Auxiliary methods.

      bool is_root(unsigned p) const
      {
	return parent[p] == p;
      }
    
      unsigned find_root(unsigned x)
      {
	if (parent[x] == x)
	  return x;
	else
	  return parent[x] = find_root(parent[x]);
      }
    
      void do_union(unsigned n, unsigned p)
      {
	unsigned r = find_root(n);
	if (r != p)
	  {
	    parent[r] = p;
	    f.merge_attr(r, p);
	  }
      }
      
    };


  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_LABELING_HH
