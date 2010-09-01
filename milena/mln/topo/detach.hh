// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_DETACH_HH
# define MLN_TOPO_DETACH_HH

/// \file
/// \brief Detaching a cell from a binary complex-based image.

# include <mln/core/site_set/p_set.hh>
# include <mln/core/image/complex_image.hh>
# include <mln/make/detachment.hh>
# include <mln/topo/is_facet.hh>

namespace mln
{

  namespace topo
  {

    /** Detach the cell corresponding to \a f from \a ima.

	\pre \a f is a facet (it does not belong to any face of higher
	      dimension).
	\pre \a ima is an image of Boolean values.  */
    template <unsigned D, typename G>
    void
    detach(const complex_psite<D, G>& f, complex_image<D, G, bool>& ima);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned D, typename G>
    inline
    void
    detach(const complex_psite<D, G>& f, complex_image<D, G, bool>& ima)
    {
      mln_precondition(topo::is_facet(f));

      typedef complex_psite<D, G> psite;
      typedef p_set<psite> faces_t;

      // Compute the detachment of P from IMA.
      faces_t detach = make::detachment(f, ima);
      // Detach all its faces from IMA.
#  if 0
      // FIXME: Does not work yet?  Check again.
      data::fill(ima | detach, false);
#  endif
      mln_piter(faces_t) p(detach);
      for_all(p)
	ima(p) = false;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_DETACH_HH
