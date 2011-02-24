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

#ifndef MLN_MAKE_DETACHMENT_HH
# define MLN_MAKE_DETACHMENT_HH

/// \file
/// \brief Compute the detachment of a cell w.r.t. from a binary
/// complex-based image.

# include <mln/core/image/complex_image.hh>
# include <mln/make/cell.hh>

namespace mln
{

  namespace make
  {

    /** \brief Compute the detachment of the cell corresponding to the
	face \a f from the image \a ima.

	\param ima  The input image from which the face is to be
		    detached.
	\param f    The psite corresponding to the face to detach.
	\param nbh  An adjacency relationship between faces
		    (should return the set of (n-1)- and (n+1)-faces
		    adjacent to an n-face).

	\return     A set of faces containing the detachment.

	\pre \a ima is an image of Boolean values.

	We do not use the fomal definition of the detachment here (see
	couprie.08.pami).  We use the following (equivalent) definition:
	an N-face F in CELL is not in the detachment of CELL from IMA if
	it is adjacent to at least an (N-1)-face or an (N+1)-face that
	does not belong to CELL.

	Moreover, the term detachment does not correspond to the
	complex resulting from the collapsing of CELL onto IMA, but
	the part that is removed, i.e., the detached part CELL -
	ATTACHMENT.  It would be wise to rename this routine to
	something else, e.g., `detached'.  */
    template <typename I, typename N>
    p_set<mln_psite(I)>
    detachment(const Image<I>& ima, const mln_psite(I)& f,
	       const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N>
    inline
    p_set<mln_psite(I)>
    detachment(const Image<I>& ima_, const mln_psite(I)& f,
	       const Neighborhood<N>& nbh_)
    {
      mlc_equal(mln_value(I), bool)::check();

      I ima = exact(ima_);
      N nbh = exact(nbh_);

      typedef p_set<mln_psite(I)> faces_t;

      faces_t f_hat = make::cell(f);
      // Initialize DETACH_F to F_HAT.
      faces_t detach_f = f_hat;

      mln_piter(faces_t) g(f_hat);
      mln_niter(N) n(nbh, g);
      for_all(g)
	for_all(n)
	if (ima(n) && !f_hat.has(n))
	  {
	    detach_f.remove(g);
	    break;
	  }
      return detach_f;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_DETACHMENT_HH
