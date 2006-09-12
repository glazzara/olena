// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
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

#ifndef OLENA_CORE_3D_GRID3D_HH
# define OLENA_CORE_3D_GRID3D_HH

# include <mlc/int.hh>
# include <oln/core/abstract/grid.hh>


namespace oln
{

  // Forward declarations.
  struct grid3d;
  struct point3d;
  struct dpoint3d;
  struct size3d;
  struct coord_t;

  // Super type.
  stc_set_super(grid3d, abstract::grid<grid3d>);

  /// Virtual types associated to oln::grid3d.
  template <>
  struct vtypes<grid3d>
  {
    typedef point3d       point_type;
    typedef dpoint3d      dpoint_type;
    typedef size3d        size_type;
    typedef coord_t       coord_type;
    typedef mlc::uint_<3> dimvalue_type;
  };

  /// A 3-dimension rectangular grid.
  struct grid3d : public stc_get_supers(grid3d)
  {
  protected:
    grid3d() {}
  };

} // end of namespace oln



#endif // ! OLENA_CORE_3D_GRID3D_HH
