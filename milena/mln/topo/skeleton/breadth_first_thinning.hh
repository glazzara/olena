// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_SKELETON_BREADTH_FIRST_THINNING_HH
# define MLN_TOPO_SKELETON_BREADTH_FIRST_THINNING_HH

/// \file mln/topo/skeleton/breadth_first_thinning.hh
/// \brief Computing a skeleton by using breadth-first thinning on a
/// binary image.

# include <algorithm>

# include <mln/core/routine/duplicate.hh>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/core/site_set/p_set.hh>

# include <mln/fun/p2b/tautology.hh>

namespace mln
{

  namespace topo
  {

    namespace skeleton
    {

      /** \brief Skeleton by Breadth-First Thinning.

	  A generic implementation of the computation of a skeleton
	  using a breadth-first thinning on a binary.

          \param input      The input image.
          \param nbh        The adjacency relation between triangles.
          \param is_simple  The predicate on the simplicity of points
                            (sites).  This functor must provide a method
                            <tt>void set_image(const Image<I>&)</tt>.
	  \param detach     A function used to detach a cell from \a input.
          \param constraint A constraint on point (site); if it
       	                    returns \c false for a point, this point
                            will not be removed.  */
      template <typename I, typename N, typename F, typename G, typename H>
      mln_concrete(I)
      breadth_first_thinning(const Image<I>& input,
			     const Neighborhood<N>& nbh,
			     Function_p2b<F>& is_simple,
			     G detach,
			     const Function_p2b<H>& constraint =
			       fun::p2b::tautology());


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename N, typename F, typename G, typename H>
      inline
      mln_concrete(I)
      breadth_first_thinning(const Image<I>& input_,
			     const Neighborhood<N>& nbh_,
			     Function_p2b<F>& is_simple_,
			     G detach,
			     const Function_p2b<H>& constraint_)
      {
	const I& input = exact(input_);
	const N& nbh = exact(nbh_);
	F& is_simple = exact(is_simple_);
	const H& constraint = exact(constraint_);

	I output = duplicate(input);
	// Attach the work image to IS_SIMPLE.
	is_simple.set_image(output);

	typedef mln_psite(I) psite;
	typedef p_set<psite> set_t;
	set_t set;
	// Populate SET with candidate simple points.
	mln_piter(I) p_(output.domain());
	for_all(p_)
	{
	  /* CONSTRAINTS and IS_SIMPLE are site-to-boolean (p2b)
	     predicate functors; passing an iterator as argument might
	     not be possible (C++ cannot resolve template routines if
	     an implicit conversion of the argument is needed).  Help
	     the compiler and pass an actual, explicit psite.  */
	  psite p = p_;
	  if (output(p) && constraint(p) && is_simple(p))
	    set.insert(p);
	}

	while (!set.is_empty())
	  {
	    set_t next_set;

	    /* FIXME: Using the following code does not work (it does
	       compiles, but does not behave like the code using a
	       hand-made loop).  There must be a bug somewhere in
	       p_set or p_indexed_psite. */
# if 0
	    mln_piter(set_t) ps(set);
	    for_all(ps);
	    {
	      // Same remark as above.
	      psite p = p_;
# endif
	    for (unsigned i = 0; i < set.nsites(); ++i)
	      {
		psite p = set[i];

		/* FIXME: We compute the cell and attachment of P twice:
		   during the call to is_simple() and within detach().
		   How could we reuse this elegantly, without breaking
		   the genericity of the skeleton algorithm?  */
		if (constraint(p) && is_simple(p))
		  {
		    detach(p, output);
		    mln_niter(N) n(nbh, p);
		    for_all(n)
		      if (output.domain().has(n)
			  && output(n) && constraint(p) && is_simple(n))
			next_set.insert(n);
		  }
	      }
	    set.clear();
	    std::swap(set, next_set);
	  }
	return output;
      }

# endif // MLN_INCLUDE_ONLY

    } // end of namespace mln::topo::skeleton

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_SKELETON_BREADTH_FIRST_THINNING_HH
