// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_IS_SIMPLE_PAIR_HH
# define MLN_TOPO_IS_SIMPLE_PAIR_HH

/// \file
/// \brief Testing whether a pair of faces is simple (or whether a
/// face belongs to a simple pair)

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/topo/is_facet.hh>


namespace mln
{

  namespace topo
  {

    /** \brief A predicate for the simplicity of a pair (or a face
	that might belong to a simple pair).

	The functor may either take a pair composed of an n-face F and
	an (n-1)-face G (where F is a facet), or just a single facet
	F.  In this latter case, it tries to find an (n-1)-face G so
	that (F, G) form a simple pair.

	\tparam I   The type of the image.
	\tparam NL  The neighborhood type returning the set of
		    (n-1)-faces adjacent to a an n-face.
	\tparam NH  The neighborhood type returning the set of
		    (n+1)-faces adjacent to a an n-face.  */
    template <typename I, typename NL, typename NH>
    class is_simple_pair
      : public mln::Function_v2b< is_simple_pair<I, NL, NH> >
    {
    public:
      /// Result type of the functor.
      typedef bool result;

      /// Constructors.
      /// \{
      /** Construct an mln::topo::is_simple_pair from a couple of
	  neighborhoods.

	  \param lower_adj_nbh   An adjacency relationship between faces
			         returning the set of (n-1)-faces
			         adjacent to a given n-face.
	  \param higher_adj_nbh  An adjacency relationship between faces
			         returning the set of (n+1)-faces
			         adjacent to a given n-face.  */
      is_simple_pair(const Neighborhood<NL>& lower_adj_nbh,
		     const Neighborhood<NH>& higher_adj_nbh);

      /** Construct an mln::topo::detach_pair from an image and a
	  couple of neighborhoods.

	  \pre \a ima is an image of Boolean values.

	  \param ima             The input image the pair belongs to.
	  \param lower_adj_nbh   An adjacency relationship between faces
			         returning the set of (n-1)-faces
			         adjacent to a given n-face.
	  \param higher_adj_nbh  An adjacency relationship between faces
			         returning the set of (n+1)-faces
			         adjacent to a given n-face.  */
      is_simple_pair(const mln::Image<I>& ima,
		     const Neighborhood<NL>& lower_adj_nbh,
		     const Neighborhood<NH>& higher_adj_nbh);
      /// \}

      /* FIXME: Rename as init() or something like this?  See how other
	 functors are written.  */
      /// Set the underlying image.
      void set_image(const mln::Image<I>& ima);

      /** \brief Test whether the pair (\a f, \a g) is simple.

	  If \a f is not a facet, return false.
	  If \a g is not a (n-1)-face adjacent to the n-face \a f,
	  return false.  */
      bool operator()(const mln_psite(I)& f, const mln_psite(I)& g) const;

      /** \brief Test whether \a f is part of a simple pair.

	  If \a f is not a facet, return false.  */
      bool operator()(const mln_psite(I)& f) const;

    private:
      const I* ima_;
      const NL& lower_adj_nbh_;
      const NH& higher_adj_nbh_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename NL, typename NH>
    inline
    is_simple_pair<I, NL, NH>::is_simple_pair(const Neighborhood<NL>& lower_adj_nbh,
					      const Neighborhood<NH>& higher_adj_nbh)
      : ima_(0),
	lower_adj_nbh_(exact(lower_adj_nbh)),
	higher_adj_nbh_(exact(higher_adj_nbh))
    {
      mlc_equal(mln_value(I), bool)::check();
    }

    template <typename I, typename NL, typename NH>
    inline
    is_simple_pair<I, NL, NH>::is_simple_pair(const mln::Image<I>& ima,
					      const Neighborhood<NL>& lower_adj_nbh,
					      const Neighborhood<NH>& higher_adj_nbh)
      : ima_(exact(&ima)),
	lower_adj_nbh_(exact(lower_adj_nbh)),
	higher_adj_nbh_(exact(higher_adj_nbh))
    {
      mlc_equal(mln_value(I), bool)::check();
    }

    template <typename I, typename NL, typename NH>
    inline
    void
    is_simple_pair<I, NL, NH>::set_image(const mln::Image<I>& ima)
    {
      ima_ = exact(&ima);
    }

    template <typename I, typename NL, typename NH>
    inline
    bool
    is_simple_pair<I, NL, NH>::operator()(const mln_psite(I)& f,
					  const mln_psite(I)& g) const
    {
      mln_precondition(ima_);
      // Shortcut.
      // FIXME: Introduce an `ima()' accessor instead?
      const I& ima = *ima_;

      // (F, G) cannot be a simple pair unless they are part of
      // objects in IMA.
      if (!ima(f) || !ima(g))
	return false;

      // F cannot be part of a simple pair unless it is a facet.
      if (!is_facet(ima, f, higher_adj_nbh_))
	return false;

      /* FIXME: It would be nice if we could skip this check when G is
	 passed by the unary operator(), since this one already
	 ensuress that F and G are adjacent.  */
      // Ensure the (n-1)-face G is adjacent to the n-face F.
      {
	bool f_g_adjacent = false;
	mln_niter(NH) h(higher_adj_nbh_, g);
	for_all(h)
	  if (h == f)
	    {
	      f_g_adjacent = true;
	      break;
	    }
	if (!f_g_adjacent)
	  return false;
      }

      // Check whether G is stricly included in F.
      mln_niter(NH) h(higher_adj_nbh_, g);
      for_all(h)
	if (h != f && ima.has(h) && ima(h))
	  return false;
      return true;
    }

    // FIXME: What about moving this operator() into its own functor
    // called e.g. mln::topo::is_in_simple_pair?
    template <typename I, typename NL, typename NH>
    inline
    bool
    is_simple_pair<I, NL, NH>::operator()(const mln_psite(I)& f) const
    {
      mln_niter(NL) g(lower_adj_nbh_, f);
      for_all(g)
      {
	// Delegate the computation to the binary operator().
	if ((*this)(f, g))
	  // (F, G) is a simple pair.
	  return true;
      }
      return false;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_SIMPLE_PAIR_HH
