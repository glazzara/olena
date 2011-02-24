// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_CELL_HH
# define MLN_MAKE_CELL_HH

/// \file
/// \brief Computing the set of faces of the cell.

# include <mln/core/site_set/p_set.hh>
# include <mln/core/site_set/complex_psite.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>

namespace mln
{

  namespace make
  {

    /** Compute the set of faces of the cell corresponding to the
	face \a f.

	\return An mln::p_set of sites (faces) containing the
	attachment.  */
    template <unsigned D, typename G>
    p_set< complex_psite<D, G> >
    cell(const complex_psite<D, G>& f);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned D, typename G>
    inline
    p_set< complex_psite<D, G> >
    cell(const complex_psite<D, G>& f)
    {
      typedef complex_psite<D, G> psite;
      typedef p_set<psite> faces_t;

      // Compute the cell F^HAT.
      faces_t f_hat;
      /* FIXME: We need a cell-iterator here
	 (see https://trac.lrde.org/olena/ticket/162).  */
      typedef complex_m_face_neighborhood<D, G> m_faces_nbh_t;
      m_faces_nbh_t m_faces_nbh;
      mln_niter(m_faces_nbh_t) g(m_faces_nbh, f);
      for (unsigned m = 0; m < f.n(); ++m)
	{
	  g.iter().set_m(m);
	  for_all(g)
	  {
	    f_hat.insert(g);
	  }
	}
      f_hat.insert(f);
      return f_hat;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_CELL_HH
