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

#ifndef MLN_MAKE_ATTACHMENT_HH
# define MLN_MAKE_ATTACHMENT_HH

/// \file mln/make/attachment.hh
/// \brief Compute the attachment of a cell to a binary
/// complex-based image.

# include <mln/core/image/complex_image.hh>
# include <mln/make/cell.hh>
# include <mln/topo/is_facet.hh>

namespace mln
{

  namespace make
  {

    /** \brief Compute the attachment of the cell corresponding to the
	facet \a f to the image \a ima.

	\pre \a f is a facet (it does not belong to any face of higher
	     dimension).
	\pre ima is an image of Boolean values.

	\return a set of faces containing the attachment.

	We do not use the fomal definition of the attachment here (see
	couprie.08.pami).  We use the following (equivalent) definition:
	an N-face F in CELL is in the attachment of CELL to IMA if it is
	adjacent to at least an (N-1)-face or an (N+1)-face that does not
	belong to CELL.  */
    template <unsigned D, typename G, typename V>
    p_set< complex_psite<D, G> >
    attachment(const complex_psite<D, G>& f,
	       const complex_image<D, G, V>& ima);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned D, typename G, typename V>
    inline
    p_set< complex_psite<D, G> >
    attachment(const complex_psite<D, G>& f,
	       const complex_image<D, G, V>& ima)
    {
      mln_precondition(topo::is_facet(f));
      mlc_equal(V, bool)::check();

      typedef complex_psite<D, G> psite;
      typedef p_set<psite> faces_t;

      faces_t f_hat = make::cell(f);
      faces_t att_f;

      typedef complex_lower_higher_neighborhood<D, G> adj_nbh_t;
      adj_nbh_t adj_nbh;
      mln_piter(faces_t) g(f_hat);
      mln_niter(adj_nbh_t) n(adj_nbh, g);
      for_all(g)
	for_all(n)
	if (ima(n) && !f_hat.has(n))
	  {
	    att_f.insert(g);
	    break;
	  }
      return att_f;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_ATTACHMENT_HH
