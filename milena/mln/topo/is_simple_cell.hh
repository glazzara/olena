// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

# include <mln/make/attachment.hh>

namespace mln
{

  namespace topo
  {

    /** \brief A predicate for the simplicity of a point based on the
	collapse property of the attachment.

	The functor does not actually take a cell as input, but a face
	that is expected to be a D-facet.  */
    template <typename I>
    class is_simple_cell : public mln::Function_v2b< is_simple_cell<I> >
    {
    public:
      /// Dimension of the image (and therefore of the complex).
      static const unsigned D = I::dim;
      /// Geometry of the image.
      typedef mln_geom(I) G;
      /// Psite type.
      typedef mln::complex_psite<D, G> psite;

      /// Result type of the functor.
      typedef bool result;

      is_simple_cell();
      is_simple_cell(const mln::Image<I>& ima);

      /* FIXME: Rename as init() or something like this?  See how other
	 functors are written.  */
      /// Set the underlying image.
      void set_image(const mln::Image<I>& ima);

      /// Based on the algorithm A2 from couprie.08.pami.
      bool operator()(const psite& p) const;

    private:
      const I* ima_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    is_simple_cell<I>::is_simple_cell()
      : ima_(0)
    {
    }

    template <typename I>
    inline
    is_simple_cell<I>::is_simple_cell(const mln::Image<I>& ima)
      : ima_(mln::exact(&ima))
    {
    }

    template <typename I>
    inline
    void
    is_simple_cell<I>::set_image(const mln::Image<I>& ima)
    {
      ima_ = mln::exact(&ima);
    }

    template <typename I>
    inline
    bool
    is_simple_cell<I>::operator()(const is_simple_cell<I>::psite& p) const
    {
      mln_precondition(ima_);

      typedef p_set<psite> faces_t;

      // Compute the attachment of the cell corresponding to P to he
      // domain of *IMA_.
      faces_t att = make::attachment(p, *ima_);

      // A cell with an empty attachment is not simple.
      /* FIXME: Why does p_set not provide an empty() predicate?  */
      if (att.nsites() == 0)
	return false;

      /* FIXME: This part could be moved to its own function/method
	 (`is_collapsible').  Moreover, the code could be split: looking
	 up for a free pair (g, h) could be performed in another
	 routine.  */
      // Try to collapse the attachment (to a single point).
      {
	typedef complex_lower_neighborhood<D, G> lower_adj_nbh_t;
	typedef complex_higher_neighborhood<D, G> higher_adj_nbh_t;
	lower_adj_nbh_t lower_adj_nbh;
	higher_adj_nbh_t higher_adj_nbh;
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
	      if (static_cast<psite>(g).n() > 0)
		{
		  // Check whether G is a facet within ATT.
		  bool g_is_facet = true;
		  mln_niter(higher_adj_nbh_t) f(higher_adj_nbh, g);
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
		  mln_niter(lower_adj_nbh_t) h(lower_adj_nbh, g);
		  for_all(h)
		  {
		    bool h_strictly_in_g = true;
		    if (att.has(h))
		      {
			mln_niter(higher_adj_nbh_t) i(higher_adj_nbh, h);
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
