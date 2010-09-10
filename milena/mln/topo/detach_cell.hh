// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_DETACH_CELL_HH
# define MLN_TOPO_DETACH_CELL_HH

/// \file
/// \brief Detaching a cell from a binary (probably complex-based) image.

# include <mln/core/site_set/p_set.hh>
# include <mln/core/image/complex_image.hh>
# include <mln/make/detachment.hh>
# include <mln/topo/is_facet.hh>

namespace mln
{

  namespace topo
  {

    /** A functor detaching a cell from a binary (probably
	complex-based) image.

	\tparam I  The type of the image.
	\tparam N  An neighborhood type corresponding to (directly)
		   adjacent faces (should return the set of (n-1)- and
		   (n+1)-faces adjacent to an n-face).  */
    template <typename I, typename N>
    class detach_cell
    {
    public:
      /// Constructors.
      /// \{
      /** Construct an mln::topo::detach_cell from a neighborhood.

	  \param nbh  An adjacency relationship between faces
		      (should return the set of (n-1)- and (n+1)-faces
		      adjacent to an n-face).  */
      detach_cell(const Neighborhood<N>& nbh);

      /** Construct an mln::topo::detach_cell from an image and a
	  neighborhood.

	  \pre \a ima is an image of Boolean values.

	  \param ima  The input image from which the facet is to be
		    detached.
	  \param nbh  An adjacency relationship between faces
		      (should return the set of (n-1)- and (n+1)-faces
		      adjacent to an n-face).  */
      detach_cell(mln::Image<I>& ima, const Neighborhood<N>& nbh);
      /// \}

      /* FIXME: Rename as init() or something like this?  See how other
	 functors are written.  */
      /** Set the underlying image.

	  \pre \a ima is an image of Boolean values.  */
      void set_image(mln::Image<I>& ima);

      /** Detach the cell corresponding to \a f from \a ima.

	  \pre \a f is a facet (it does not belong to any face of
	       higher dimension).

	  \param f    The psite corresponding to the facet to detach.
	  \param nbh  An adjacency relationship between faces
		      (should return the set of (n-1)- and (n+1)-faces
		      adjacent to an n-face).  */
      void
      operator()(const mln_psite(I)& f);

    private:
      I* ima_;
      const N& nbh_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N>
    inline
    detach_cell<I, N>::detach_cell(const Neighborhood<N>& nbh)
      : ima_(0), nbh_(exact(nbh))
    {
    }

    template <typename I, typename N>
    inline
    detach_cell<I, N>::detach_cell(mln::Image<I>& ima,
				   const Neighborhood<N>& nbh)
      : ima_(exact(&ima)), nbh_(exact(nbh))
    {
      mlc_equal(mln_value(I), bool)::check();
    }

    template <typename I, typename N>
    inline
    void
    detach_cell<I, N>::set_image(mln::Image<I>& ima)
    {
      mlc_equal(mln_value(I), bool)::check();
      ima_ = exact(&ima);
    }

    template <typename I, typename N>
    inline
    void
    detach_cell<I, N>::operator()(const mln_psite(I)& f)
    {
      mln_precondition(ima_);
      // FIXME: The current implementation of topo::is_facet is too
      // naive: it does not take the values of the image into account.
      mln_precondition(topo::is_facet(f));

      typedef p_set<mln_psite(I)> faces_t;

      // Compute the detachment of P from *IMA_.
      faces_t detach = make::detachment(*ima_, f, nbh_);
      // Detach all its faces from *IMA_.
#  if 0
      // FIXME: Does not work yet?  Check again.
      data::fill(*ima_ | detach, false);
#  endif
      mln_piter(faces_t) p(detach);
      for_all(p)
	(*ima_)(p) = false;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_DETACH_CELL_HH
