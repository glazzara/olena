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

# include <mln/level/fill.hh>
# include <mln/level/sort_points.hh>

# include <mln/convert/to_window.hh>

namespace mln
{

  namespace canvas
  {

    // Fast version (version 2).

    template <typename F>
    struct labeling_fast_try2
    {
      F& f;

      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::O O;
      typedef typename F::S S;

      // aux:
      mln_ch_value(O, unsigned) parent;

      labeling(F& f)
	: f(f),
	  parent(f.output.domain())
      {
	run();
      }

      void run()
      {
	// init
	{
	  f.nlabels = 0;
	  f.init();
	}
	// first pass
	{
 	  mln_fwd_pixter(const I) p(f.input); // p is a pixel

	  W win = convert::to_window(f.nbh);
	  mln_qixter(const I, W) q(f.input, win, p);

	  for_all(p) // if (f.handles(p))
	    {
	      make_set(p);
	      for_all(q)
		if (f.equiv(n, p))
		  do_union(n, p);
		else
		  f.do_no_union(n, p);
	    }
	}

// 	// second pass
// 	{
// 	  mln_bkd_piter(S) p(f.s);
// 	  for_all(p) if (f.handles(p))
// 	    {
// 	      if (is_root(p))
// 		{
// 		  if (f.labels(p))
// 		    {
// 		      if (f.nlabels == mln_max(mln_value(O)))
// 			{
// 			  f.status = false;
// 			  return;
// 			}
// 		      f.output(p) = ++f.nlabels;
// 		    }
// 		}
// 	      else
// 		f.output(p) = f.output(parent[p]);
// 	    }
// 	  f.status = true;
// 	}

      } // end of run()

      void make_set(const unsigned& p)
      {
	parent[p] = p;
	f.init_attr(p);
      }

      bool is_root(const unsigned& p) const
      {
	return parent[p] == p;
      }

      point find_root(const unsigned& x)
      {
	if (parent[x] == x)
	  return x;
	else
	  return parent[x] = find_root(parent[x]);
      }

      void do_union(const unsigned& n, const unsigned& p)
      {
	point r = find_root(n);
	if (r != p)
	  {
	    parent[r] = p;
	    f.merge_attr(r, p);
	  }
      }

    };









    // General version.

    template <typename F>
    struct labeling
    {
      F& f;

      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::O O;
      typedef typename F::S S;
      typedef mln_point(I) point;

      // aux:
      mln_ch_value(O, bool)  deja_vu;
      mln_ch_value(O, point) parent;

      labeling(F& f)
	: f(f),
	  deja_vu(f.output.domain()),
	  parent(f.output.domain())
      {
	run();
      }

      void run()
      {
	// init
	{
	  f.nlabels = 0;
	  mln::level::fill(deja_vu, false);
	  f.init();
	}
	// first pass
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

	// second pass
	{
	  mln_bkd_piter(S) p(f.s);
	  for_all(p) if (f.handles(p))
	    {
	      if (is_root(p))
		{
		  if (f.labels(p))
		    {
		      if (f.nlabels == mln_max(mln_value(O)))
			{
			  f.status = false;
			  return;
			}
		      f.output(p) = ++f.nlabels;
		    }
		}
	      else
		f.output(p) = f.output(parent(p));
	    }
	  f.status = true;
	}

      } // end of run()

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

















    // FIXME: Fast version.

    // FIXME (ADD)
    //
    template <typename F>
    struct labeling_fast
    {
      F& f;

      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::O O;
      typedef typename F::S S;
      typedef mln_point(I) point;

      labeling_fast(F& f)
	: f(f)
      {
	run();
      }

      void run()
      {
	// init
	{
	  f.nlabels = 0;
	  f.init();
	}
	// first pass
	{
 	  typedef mln_dpoint(N) DP;
 	  typedef mln_point(N) P;
	  typedef window<DP> W;

 	  mln_fwd_pixter(const I)	pix_input(f.input);
	  W				win = convert::to_window(f.nbh);

  	  mln_qiter(W)	q_input(win, P::zero);

	  for_all(pix_input)
	    {
// 	    if (f.handles(pix_input))
// 	      {
	        const point p = pix_input;
    		make_set(p);
 		for_all(q_input)
 		  {
//		    const point q;
// 		    if (f.equiv(q, p))
// 		      do_union(q, p);
// 		    else
// 		      f.do_no_union(q, p);
 		  }
// 	      }
	    }
	}

// 	// second pass
// 	{
// 	  mln_bkd_piter(S) p(f.s);
// 	  for_all(p) if (f.handles(p))
// 	    {
// 	      if (is_root(p))
// 		{
// 		  if (f.labels(p))
// 		    {
// 		      if (f.nlabels == mln_max(mln_value(O)))
// 			{
// 			  f.status = false;
// 			  return;
// 			}
// 		      f.output(p) = ++f.nlabels;
// 		    }
// 		}
// 	      else
// 		f.output(p) = f.output(parent(p));
// 	    }
// 	  f.status = true;
// 	}

      } // end of run()

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
    //
    // END FIXME (ADD)
    

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_LABELING_HH
