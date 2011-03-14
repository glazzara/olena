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

#ifndef MLN_TOPO_DETACH_PAIR_HH
# define MLN_TOPO_DETACH_PAIR_HH

/// \file
/// \brief Detaching a simple pair from a binary (probably
/// complex-based) image (elementary collapse operation).

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/topo/is_simple_pair.hh>


namespace mln
{

  namespace topo
  {

    /** \ brief A functor detaching a simple pair from a binary
	(probably complex-based) image (elementary collapse
	operation).

	\tparam I   The type of the image.
	\tparam NL  The neighborhood type returning the set of
		    (n-1)-faces adjacent to a an n-face.
	\tparam NH  The neighborhood type returning the set of
		    (n+1)-faces adjacent to a an n-face.  */
    template <typename I, typename NL, typename NH>
    class detach_pair
    {
    public:
      /// Constructors.
      /// \{
      /** Construct an mln::topo::detach_pair from a couple of
	  neighborhoods.

	  \param lower_adj_nbh   An adjacency relationship between faces
			         returning the set of (n-1)-faces
			         adjacent to a given n-face.
	  \param higher_adj_nbh  An adjacency relationship between faces
			         returning the set of (n+1)-faces
			         adjacent to a given n-face.  */
      detach_pair(const Neighborhood<NL>& lower_adj_nbh,
		  const Neighborhood<NH>& higher_adj_nbh);

      /** Construct an mln::topo::detach_pair from an image and a
	  couple of neighborhoods.

	  \pre \a ima is an image of Boolean values.

	  \param ima             The input image from which the pair is
		                 to be detached.
	  \param lower_adj_nbh   An adjacency relationship between faces
			         returning the set of (n-1)-faces
			         adjacent to a given n-face.
	  \param higher_adj_nbh  An adjacency relationship between faces
			         returning the set of (n+1)-faces
			         adjacent to a given n-face.  */
      detach_pair(mln::Image<I>& ima,
		  const Neighborhood<NL>& lower_adj_nbh,
		  const Neighborhood<NH>& higher_adj_nbh);
      /// \}

      /* FIXME: Rename as init() or something like this?  See how other
	 functors are written.  */
      /// Set the underlying image.
      void set_image(mln::Image<I>& ima);

      /** Detach the pair (\a f, \a q) from \a ima_.

	  \param f    An n-facet of \a ima
	  \param g    An (n-1)-face ajacent to \a f.  */
      void operator()(const mln_psite(I)& f, const mln_psite(I)& g);

      /** Detach a simple pair from \a ima_ containing the face \a f.

	  \param f    An n-facet of \a ima.  */
      void operator()(const mln_psite(I)& f);

    private:
      I* ima_;
      const NL& lower_adj_nbh_;
      const NH& higher_adj_nbh_;
      /// Simplicity citerion.
      is_simple_pair<I, NL, NH> is_simple_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename NL, typename NH>
    inline
    detach_pair<I, NL, NH>::detach_pair(const Neighborhood<NL>& lower_adj_nbh,
					const Neighborhood<NH>& higher_adj_nbh)
      : ima_(0),
	lower_adj_nbh_(exact(lower_adj_nbh)),
	higher_adj_nbh_(exact(higher_adj_nbh)),
	is_simple_(lower_adj_nbh, higher_adj_nbh)
    {
      mlc_equal(mln_value(I), bool)::check();
    }

    template <typename I, typename NL, typename NH>
    inline
    detach_pair<I, NL, NH>::detach_pair(mln::Image<I>& ima,
					const Neighborhood<NL>& lower_adj_nbh,
					const Neighborhood<NH>& higher_adj_nbh)
      : ima_(exact(&ima)),
	lower_adj_nbh_(exact(lower_adj_nbh)),
	higher_adj_nbh_(exact(higher_adj_nbh)),
	is_simple_(ima, lower_adj_nbh, higher_adj_nbh)
    {
      mlc_equal(mln_value(I), bool)::check();
    }

    template <typename I, typename NL, typename NH>
    inline
    void
    detach_pair<I, NL, NH>::set_image(mln::Image<I>& ima)
    {
      ima_ = exact(&ima);
      is_simple_.set_image(ima);
    }

    template <typename I, typename NL, typename NH>
    inline
    void
    detach_pair<I, NL, NH>::operator()(const mln_psite(I)& f,
				       const mln_psite(I)& g)
    {
      mln_precondition(ima_);
      mln_precondition(is_simple_(f, g));

      // Shortcut.
      // FIXME: Introduce an `ima()' accessor instead?
      I& ima = *ima_;

      ima(f) = false;
      ima(g) = false;
    }

    // FIXME: What about moving this operator() into its own functor?
    template <typename I, typename NL, typename NH>
    inline
    void
    detach_pair<I, NL, NH>::operator()(const mln_psite(I)& f)
    {
      mln_niter(NL) g(lower_adj_nbh_, f);
      for_all(g)
      {
	if (is_simple_(f, g))
	  // Delegate the detach operation to the binary operator().
	  return (*this)(f, g);
      }
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_DETACH_PAIR_HH
