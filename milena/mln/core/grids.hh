// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_GRIDS_HH
# define MLN_CORE_GRIDS_HH

/// \file
///
/// \brief Definition of some grid classes.

# include <mln/core/concept/regular_grid.hh>
# include <mln/core/def/coord.hh>
# include <mln/metal/bool.hh>


namespace mln
{

  /// Namespace of grids definitions.
  namespace grid
  {

    struct tick : public Regular_Grid< tick >
    {
      typedef metal::true_ aligned;
      enum { dim = 1 };
    };

    struct square : public Regular_Grid< square >
    {
      typedef metal::true_ aligned;
      enum { dim = 2 };
    };

    struct hexa : public Regular_Grid< hexa >
    {
      typedef metal::false_ aligned;
      enum { dim = 2 };
    };

    struct cube : public Regular_Grid< cube >
    {
      typedef metal::true_ aligned;
      enum { dim = 3 };
    };

  } // end of namespace mln::grid


  // Function: dim -> regular grid.

  template <unsigned dim> struct regular_grid_from_dim_;

  template <> struct regular_grid_from_dim_<1> { typedef grid::tick   ret; };
  template <> struct regular_grid_from_dim_<2> { typedef grid::square ret; };
  template <> struct regular_grid_from_dim_<3> { typedef grid::cube   ret; };

} // end of namespace mln


# define mln_regular_grid_from_dim(N) typename mln::regular_grid_from_dim_< N >::ret


#endif // ! MLN_CORE_GRIDS_HH
