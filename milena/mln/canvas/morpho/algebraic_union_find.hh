// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CANVAS_MORPHO_ALGEBRAIC_UNION_FIND_HH
# define MLN_CANVAS_MORPHO_ALGEBRAIC_UNION_FIND_HH

/*! \file mln/canvas/morpho/algebraic_union_find.hh
 *
 * \brief FIXME: Doc!
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/level/fill.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace canvas
  {

    namespace morpho
    {

      // General version.

      template <typename F>
      struct algebraic_union_find
      {
	F& f;

	typedef typename F::I I;
	typedef typename F::N N;
	typedef typename F::O O;
	typedef typename F::S S;
	typedef typename F::A A;
	typedef mln_psite(I) psite;

	// aux:
	mln_ch_value(O, bool)  deja_vu;
	mln_ch_value(O, psite) parent;
	mln_ch_value(O, A)     data;

	algebraic_union_find(F& f)
	  : f(f)
	{
	  run();
	}

	void run()
	{
	  // init
	  {
	    initialize(deja_vu, f.input);
	    mln::level::fill(deja_vu, false);
	    initialize(parent, f.input);
	    initialize(data, f.input);
	    f.init();
	  }

	  // first pass
	  {
	    mln_fwd_piter(S) p(f.s);
	    mln_niter(N) n(f.nbh, p);
	    for_all(p)
	      {
		make_set(p);
		for_all(n)
		  if (f.input.domain().has(n) && deja_vu(n))
		    do_union(n, p);
		deja_vu(p) = true;
	      }
	  }

	  // second pass
	  {
	    mln_bkd_piter(S) p(f.s);
	    for_all(p)
	      if (is_root(p))
		f.output(p) = f.input(p);
	      else
		f.output(p) = f.output(parent(p));
	  }

	  /*
	    Change 2nd pass into:
	       for_all(p) if (not is_root(p)) f.output(p) = f.output(parent(p));
	    and add in init:
	       mln::level::assign(f.output, f.input);
	   */

	} // end of run()

	void make_set(const psite& p)
	{
	  parent(p) = p;
	  /* FIXME: What if the value_type of DATA (i.e., A) were not
	     based on a accu::count_<mln::pix>?  Currently, nothing
	     enforces this, but the code below expects this line to be
	     valid:

               data(p).take_as_init(make::pix(f.input, p))

	     which probably restricts the kind of input images.

	     If we want to be more generic, the initialization should
	     read something like:

               init_data(p);

	     i.e., the functor for the initialization of data should
	     be passed as an argument to the canvas' ctor.

	     Of course, we might want to restrict attributes to the
	     accumulator accu::count_<mln::pix> (which is perfectly
	     acceptable), but then this class should statically check
	     the conformance of the template parameter A to this
	     constraint.  */
	  data(p).take_as_init(make::pix(f.input, p)); // FIXME: algebraic so p!
	}

	bool is_root(const psite& p) const
	{
	  return parent(p) == p;
	}

	psite find_root(const psite& x)
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent(x));
	}

	bool equiv(const psite& r, const psite& p)
	{
	  // Either a flat zone or the component of r is still growing.
	  return f.input(r) == f.input(p) || f.is_active(data(r));
	}

	void do_union(const psite& n, const psite& p)
	{
	  psite r = find_root(n);
	  if (r != p)
	    {
	      if (equiv(r, p))
		{
		  /* FIXME: Same remark as above concerning the
		     initialization of data(p); instead of

		       data(p).take(data(r));
		     
		     we should (or could) have

		       unite_data(p, r);

		     so as to keep the generic aspect of this canvas
		     (as long as the set of acceptable types for the
		     template parameter A is not bound).  */
		  data(p).take(data(r));
		  parent(r) = p;
		}
	      else
		f.inactivate(data(p));
	    }
	}

      };


      // FIXME: Fast version.


    } // end of namespace mln::canvas::morpho

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_MORPHO_ALGEBRAIC_UNION_FIND_HH
