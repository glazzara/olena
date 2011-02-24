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

#ifndef MLN_TOPO_IS_SIMPLE_CELL_HH
# define MLN_TOPO_IS_SIMPLE_CELL_HH

/// \file
/// \brief Testing whether a facet is a simple cell.

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>

# include <mln/core/site_set/p_set.hh>
# include <mln/core/site_set/complex_psite.hh>
# include <mln/core/site_set/p_complex_piter.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>

# include <mln/topo/is_facet.hh>
# include <mln/make/attachment.hh>


namespace mln
{

  namespace topo
  {

    /* FIXME: Maybe we could add traits to deduce N from NL and NH (or
       the converse).  Anyway, the part of the code where neighborhood
       are used should be moved into another functor
       (`is_collapsible', see below), thus pushing away this
       issue.  */

    /** \brief A predicate for the simplicity of a point based on the
	collapse property of the attachment.

	The functor does not actually take a cell as input, but a face
	that is expected to be a D-facet.

	\tparam I   The type of the image.
	\tparam N   The neighborhood type returning the set of
		    (n-1)- and (n+1)-faces adjacent to a an n-face.
	\tparam NL  The neighborhood type returning the set of
		    (n-1)-faces adjacent to a an n-face.
	\tparam NH  The neighborhood type returning the set of
		    (n+1)-faces adjacent to a an n-face.  */
    template <typename I, typename N, typename NL, typename NH>
    class is_simple_cell : public mln::Function_v2b< is_simple_cell<I, N, NL, NH> >
    {
    public:
      /// Result type of the functor.
      typedef bool result;

      is_simple_cell();
      is_simple_cell(const mln::Image<I>& ima);

      /* FIXME: Rename as init() or something like this?  See how other
	 functors are written.  */
      /// Set the underlying image.
      void set_image(const mln::Image<I>& ima);

      /** \brief Test whether a face (expected to be facet) is a
	  simple cell.

	  If \a p is not a facet, return false.

	  Based on the algorithm A2 from couprie.08.pami.  */
      /* FIXME: We probably broke the compatiblity with g++ 3.3, as it
	 seems this compiler does not like an indirect type like the
	 one of the following operator's argument.  Check and possibly
	 improve this.  */
      bool operator()(const mln_psite(I)& p) const;

    private:
      const I* ima_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename NL, typename NH>
    inline
    is_simple_cell<I, N, NL, NH>::is_simple_cell()
      : ima_(0)
    {
    }

    template <typename I, typename N, typename NL, typename NH>
    inline
    is_simple_cell<I, N, NL, NH>::is_simple_cell(const mln::Image<I>& ima)
      : ima_(exact(&ima))
    {
    }

    template <typename I, typename N, typename NL, typename NH>
    inline
    void
    is_simple_cell<I, N, NL, NH>::set_image(const mln::Image<I>& ima)
    {
      ima_ = exact(&ima);
    }

    template <typename I, typename N, typename NL, typename NH>
    inline
    bool
    is_simple_cell<I, N, NL, NH>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(ima_);
      // FIXME: Introduce `const I& ima = *ima_;' and use it instead of
      // `ima_'.  Or introduce an `ima()' accessor?

      // The cell corresponding to P cannot be simple unless P is
      // facet.
      {
	NH higher_adj_nbh;
	if (!is_facet(*ima_, p, higher_adj_nbh))
	  return false;
      }

      typedef p_set<mln_psite(I)> faces_t;

      // Compute the attachment of the cell corresponding to P to the
      // domain of *IMA_.
      N adj_nbh;
      faces_t att = make::attachment(*ima_, p, adj_nbh);

      // A cell with an empty attachment is not simple.
      /* FIXME: Why p_set does not provide an empty() predicate?  */
      if (att.nsites() == 0)
	return false;

      /* FIXME: This part could be moved to its own function/method
	 (`is_collapsible').  Moreover, the code could be split: looking
	 up for a free pair (g, h) could be performed in another
	 routine.  */
      // Try to collapse the attachment (to a single point).
      {
	NL lower_adj_nbh;
	NH higher_adj_nbh;
	while (att.nsites() > 1)
	  {

	    bool simple_pair_collapsed = false;
	    /* FIXME: The selection of G and H is probably suboptimal
	       (i.e., selecting G has a face of highest avalaible
	       dimension in CELL is probably smarter).  */
	    mln_piter(faces_t) g(att);
	    for_all(g)
	      /* G cannot have dimension 0, since we later look for an
		 adjacent face H of lower dimension.  */
	      if (static_cast<mln_psite(I)>(g).n() > 0)
		{
		  // Check whether G is a facet within ATT.
		  bool g_is_facet = true;
		  mln_niter(NH) f(higher_adj_nbh, g);
		  for_all(f)
		    if (att.has(f))
		      {
			g_is_facet = false;
			break;
		      }
		  if (!g_is_facet)
		    break;

		  // Look for a face H stricly included in G.
		  bool gh_is_simple_pair = false;
		  mln_niter(NL) h(lower_adj_nbh, g);
		  for_all(h)
		  {
		    bool h_strictly_in_g = true;
		    if (att.has(h))
		      {
			mln_niter(NH) i(higher_adj_nbh, h);
			for_all(i)
			  if (i != g && att.has(i))
			    {
			      h_strictly_in_g = false;
			      break;
			    }
		      }
		    if (h_strictly_in_g)
		      {
			gh_is_simple_pair = true;
			att.remove(g);
			att.remove(h);
			mln_invariant(att.nsites() > 0);
			break;
		      }
		  } // for_all(h)

		  // If a free pair (G, H) has been found and removed,
		  // restart the free pair look up from the beginning.
		  if (gh_is_simple_pair)
		    {
		      simple_pair_collapsed = true;
		      break;
		    }
		} // for_all(g)

	    if (!simple_pair_collapsed)
	      // If no free pair (G, H) was found, then the attachment is
	      // not collapsible.
	      return false;

	  } // while (att.nsites() > 1)

	mln_postcondition(att.nsites() == 1);
	mln_postcondition(att[0].n() == 0);
	// If the attachment is collapsible to a 0-face, then the cell
	// corresponding to the (face) P is simple.
	return true;
      }
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_SIMPLE_CELL_HH
