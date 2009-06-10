// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_GEOM_BBOX_HH
# define MLN_CORE_INTERNAL_GEOM_BBOX_HH

/// \file
///
/// \internal Routine that computes a bounding box from a window.
/// This file is included in mln/core/concept/window.hh and avoid
/// circular dependency since mln/geom/bbox.hh cannot be included in
/// concept files.

# include <mln/accu/bbox.hh>
# include <mln/literal/origin.hh>


namespace mln
{

  namespace internal
  {

    template <typename W>
    box<mln_psite(W)>
    geom_bbox(const W& win);


# ifndef MLN_INCLUDE_ONLY

    template <typename W>
    box<mln_psite(W)>
    geom_bbox(const W& win)
    {
      typedef mln_psite(W) P;
      accu::bbox<P> b;
      P O = literal::origin;
      mln_qiter(W) q(exact(win), O);
      for_all(q)
	b.take(q);
      return b;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_GEOM_BBOX_HH
