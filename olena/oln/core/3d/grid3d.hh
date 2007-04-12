// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_3D_GRID3D_HH
# define OLN_CORE_3D_GRID3D_HH

# include <mlc/int.hh>
# include <oln/core/2d/grid2d.hh> // So import 2d instant values.



# define OLN_ENV_3D



namespace oln
{

  /// Instant values.
  oln_decl_instant_value(sli);
  oln_decl_instant_value(dsli);
  oln_decl_instant_value(nslis);


  /// \{
  /// Fwd decls.

  template <typename Exact> struct Grid_3D;
  struct grid3d;

  /// \}


  /// \{
  /// Grid_3D.

  template <typename Exact>
  struct vtypes< Grid_3D<Exact> >
  {
    typedef mlc::uint_<3> dim;
  };
  
  template <typename Exact>
  struct Grid_3D : public Grid<Exact>
  {
  protected:
    Grid_3D() {}
  };

  /// \}


  // Super type.

  template<>
  struct super_trait_< grid3d >
  {
    typedef Grid_3D< grid3d > ret;
  };


  /// Cubic grid struct.

  struct grid3d : public Grid_3D< grid3d >
  {
  };

} // end of namespace oln


#endif // ! OLN_CORE_3D_GRID3D_HH
