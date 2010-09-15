// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_DETACH_POINT_HH
# define MLN_TOPO_DETACH_POINT_HH

/// \file
/// \brief Detaching a point from a binary image.

// FIXME: Not generic.  Swap arguments and use Image<I> and
// mln_psite(I) as types.

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/point2d.hh>

namespace mln
{

  namespace topo
  {
    
    /// \brief Detach a point from a binary image.
    inline
    void
    detach_point(const mln::point2d& p, mln::image2d<bool>& ima);


# ifndef MLN_INCLUDE_ONLY

    inline
    void
    detach_point(const mln::point2d& p, mln::image2d<bool>& ima)
    {
      ima(p) = false;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_DETACH_POINT_HH
