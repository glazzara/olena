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

#ifndef MLN_CANVAS_LABELING_HH
# define MLN_CANVAS_LABELING_HH

/// \file mln/canvas/labeling.hh
///
/// Connected component labeling of the object part in a binary image.
///
/// \todo Make the fastest version work.

# include <mln/core/concept/image.hh>
# include <mln/level/fill.hh>
# include <mln/literal/zero.hh>
# include <mln/convert/to_upper_window.hh>


namespace mln
{

  namespace canvas
  {

    // General version.
    template <typename I, typename N, typename F, typename L>
    mln_ch_value(I, L)
    labeling(const Image<I>& input, const Neighborhood<N>& nbh,
	     F& functor, L& nlabels);


# ifndef MLN_INCLUDE_ONLY


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

    template <typename I, typename N, typename F, typename L>
    mln_ch_value(I, L)
      labeling(const Image<I>& input_, const Neighborhood<N>& nbh_,
	       F& f, L& nlabels)
    {
      trace::entering("canvas::labeling");

      // FIXME: Test?!

      const I& input = exact(input_);
      const N& nbh   = exact(nbh_);

      typedef typename F::S S;

      // Local type.
      typedef mln_psite(I) P;

      // Auxiliary data.
      mln_ch_value(I, bool) deja_vu;
      mln_ch_value(I, P)    parent;

      // Output.
      mln_ch_value(I, L) output;
      bool status;

      // Initialization.

      {
	initialize(deja_vu, input);
	mln::level::fill(deja_vu, false);

	initialize(parent, input);

	initialize(output, input);
	mln::level::fill(output, L(literal::zero));
	nlabels = 0;

	f.init(); // Client initialization.
      }

      // First Pass.

      {
	mln_fwd_piter(S) p(f.s);
	mln_niter(N) n(nbh, p);
	for_all(p) if (f.handles(p))
	{

	  // Make-Set.
	  {
	    parent(p) = p;
 	    f.init_attr(p);
	  }

	  for_all(n)
	    if (input.domain().has(n) && deja_vu(n))
	    {
	      if (f.equiv(n, p))
	      {
		// Do-Union.
		P r = find_root(parent, n);
		if (r != p)
		{
		  parent(r) = p;
 		  f.merge_attr(r, p);
		}
	      }
	      else
		f.do_no_union(n, p);
	    }
	  deja_vu(p) = true;
	}
      }

      // Second Pass. pass_2();
      {
	mln_bkd_piter(S) p(f.s);
	for_all(p) if (f.handles(p))
	{
	  if (parent(p) == p) // if p is root
	  {
	    if (f.labels(p))
	    {
	      if (nlabels == mln_max(L))
	      {
		status = false;
		return output;
	      }
	      output(p) = ++nlabels;
	    }
	  }
	  else
	    output(p) = output(parent(p));
	}
	status = true;

      }

      trace::exiting("canvas::labeling");
      return output;
    }





    // -----------------------------------------------------------
    // Old code below.


    /*

    // Fastest version.

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
      labeling_fastest(F& f);

      void init();

      void pass_1();

      void pass_2();

      // Auxiliary methods.

      bool is_root(unsigned p) const;

      unsigned find_root(unsigned x);

      void do_union(unsigned n, unsigned p);

    };

    template <typename F>
    labeling_fastest<F>::labeling_fastest(F& f)
      : f(f)
    {
      trace::entering("canvas::labeling_fastest");

      init();
      f.init(); // Client initialization.
      pass_1();
      pass_2();

      trace::exiting("canvas::labeling_fastest");
    }

    template <typename F>
    void
    labeling_fastest<F>::init()
    {
      initialize(parent, f.input);
      for (unsigned p = 0; p < parent.nelements(); ++p)
	parent.element(p) = p; // make_set
      initialize(output, f.input);
      mln::level::fill(output, 0); // FIXME: Use literal::zero.
      nlabels = 0;
    }

    template <typename F>
    void
    labeling_fastest<F>::pass_1()
    {
      mln_bkd_pixter(const I) p(f.input);

      typedef window<mln_dpsite(I)> W;
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

    template <typename F>
    void
    labeling_fastest<F>::pass_2()
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
		  output(p) = ++nlabels;
		}
	    }
	  else
	    output(p) = output(parent.element(p));
	}
      status = true;
    }

    template <typename F>
    bool
    labeling_fastest<F>::is_root(unsigned p) const
    {
      return parent.element(p) == p;
    }

    template <typename F>
    unsigned
    labeling_fastest<F>::find_root(unsigned x)
    {
      if (parent.element(x) == x)
	return x;
      else
	return parent.element(x) = find_root(parent.element(x));
    }

    template <typename F>
    void
    labeling_fastest<F>::do_union(unsigned n, unsigned p)
    {
      unsigned r = find_root(n);
      if (r != p)
	{
	  parent.element(r) = p;
	  f.merge_attr(r, p);
	}
    }

*/

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_LABELING_HH
