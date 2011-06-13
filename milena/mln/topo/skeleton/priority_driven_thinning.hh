// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_TOPO_SKELETON_PRIORITY_DRIVEN_THINNING_HH
# define MLN_TOPO_SKELETON_PRIORITY_DRIVEN_THINNING_HH

/// \file
/// \brief Computing a skeleton by using priority-driven thinning on a
/// binary image.
///
/// Careful: The meaning of the `constraint' is inversed with respect
/// to the definitions used in
///
///   Gilles Bertrand and Michel Couprie: Transformations topologiques
///   discrètes.  In David Coeurjolly, Annick Montanvert and Jean-Marc
///   Chassery, eds.: Géométrie discrète et images numériques.  Hermes
///   Sciences Publications (2007), pages 187--209.

# include <mln/core/routine/duplicate.hh>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/site_set/p_priority.hh>

# include <mln/topo/no_constraint.hh>

# include <mln/data/fill.hh>


namespace mln
{

  namespace topo
  {

    namespace skeleton
    {

      /** \brief Skeleton by Priority-Driven Thinning.

	  A generic implementation of the computation of a skeleton
	  using a priority-driven thinning on a binary image.

	  \param input      The input image.
	  \param nbh        The adjacency relation between triangles.
	  \param is_simple  The predicate on the simplicity of points
			    (sites).  This functor must provide a method
			    <tt>void set_image(const Image<I>&)</tt>.
	  \param detach     A function used to detach a cell from \a input.
			    This functor must provide a method
			    <tt>void set_image(const Image<I>&)</tt>.
	  \param priority   A priority function expressed as an image.
	  \param constraint A constraint on point (site); if it
			    returns \c false for a point, this point
			    will not be removed.

	  Keywords: skeletons, simple points.  */
      template <typename I, typename N, typename F, typename G, typename J,
		typename H>
      mln_concrete(I)
      priority_driven_thinning(const Image<I>& input,
			       const Neighborhood<N>& nbh,
			       Function_v2b<F>& is_simple,
			       G& detach,
			       const Image<J>& priority,
			       const Function_v2b<H>& constraint);


      /** \brief Skeleton by Priority-Driven Thinning with no constraint.

	  A generic implementation of the computation of a skeleton
	  using a priority-driven thinning on a binary image.

	  \param input      The input image.
	  \param nbh        The adjacency relation between triangles.
	  \param is_simple  The predicate on the simplicity of points
			    (sites).  This functor must provide a method
			    <tt>void set_image(const Image<I>&)</tt>.
	  \param detach     A function used to detach a cell from \a input.
			    This functor must provide a method
			    <tt>void set_image(const Image<I>&)</tt>.
	  \param priority   A priority function expressed as an image.

	  Keywords: skeletons, simple points.  */
      template <typename I, typename N, typename F, typename G, typename J>
      mln_concrete(I)
      priority_driven_thinning(const Image<I>& input,
			       const Neighborhood<N>& nbh,
			       Function_v2b<F>& is_simple,
			       G& detach,
			       const Image<J>& priority);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename N, typename F, typename G, typename J,
		typename H>
      inline
      mln_concrete(I)
      priority_driven_thinning(const Image<I>& input_,
			       const Neighborhood<N>& nbh_,
			       Function_v2b<F>& is_simple_,
			       G& detach,
			       const Image<J>& priority_,
			       const Function_v2b<H>& constraint_)
      {
	trace::entering("topo::skeleton::priority_driven_thinning");

	const I& input = exact(input_);
	const N& nbh = exact(nbh_);
	F& is_simple = exact(is_simple_);
	const J& priority = exact(priority_);
	const H& constraint = exact(constraint_);

	mln_concrete(I) output = duplicate(input);
	// Attach the work image to IS_SIMPLE and DETACH.
	is_simple.set_image(output);
	detach.set_image(output);

	// Priority queue.
	typedef mln_psite(I) psite;
	typedef p_queue_fast<psite> queue_t;
	typedef p_priority<mln_value(J), queue_t> priority_queue_t;
	priority_queue_t queue;
	// Image showing whether a site has been inserted into the queue.
	mln_ch_value(I, bool) in_queue;
	initialize(in_queue, input);
	data::fill(in_queue, false);

	// Populate QUEUE with candidate simple points.
	mln_piter(I) p(output.domain());
	for_all(p)
	{
	  if (output(p) && constraint(p) && is_simple(p))
	    {
	      queue.push(priority(p), p);
	      in_queue(p) = true;
	    }
	}

	while (!queue.is_empty())
	  {
	    psite p = queue.pop_front();
 	    in_queue(p) = false;
	    if (output(p) && constraint(p) && is_simple(p))
	      {
		detach(p);
		mln_niter(N) n(nbh, p);
		for_all(n)
		{
		  if (output.domain().has(n)
		      && output(n) && constraint(n) && is_simple(n)
		      && !in_queue(n))
		    {
		      queue.push(priority(n), n);
		      in_queue(n) = true;
		    }
		}
	      }
	  }

	trace::exiting("topo::skeleton::priority_driven_thinning");
	return output;
      }


      template <typename I, typename N, typename F, typename G, typename J>
      inline
      mln_concrete(I)
      priority_driven_thinning(const Image<I>& input,
			       const Neighborhood<N>& nbh,
			       Function_v2b<F>& is_simple,
			       G& detach,
			       const Image<J>& priority)
      {
	// mln::topo::no_constraint is a dummy functor always
	// returning `true'.
	no_constraint constraint;
	return priority_driven_thinning(input, nbh, is_simple, detach,
					priority, constraint);
      }

# endif // MLN_INCLUDE_ONLY

    } // end of namespace mln::topo::skeleton

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_SKELETON_PRIORITY_DRIVEN_THINNING_HH
