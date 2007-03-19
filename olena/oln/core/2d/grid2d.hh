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

#ifndef OLN_CORE_2D_GRID2D_HH
# define OLN_CORE_2D_GRID2D_HH

# include <mlc/int.hh>
# include <oln/core/concept/grid.hh>



# define OLN_ENV_2D



namespace oln
{

  /// Instant values.
  oln_decl_instant_value(row);
  oln_decl_instant_value(col);
  oln_decl_instant_value(drow);
  oln_decl_instant_value(dcol);
  oln_decl_instant_value(nrows);
  oln_decl_instant_value(ncols);


  /// \{
  /// Fwd decls.

  template <typename Exact> struct Grid_2D;
  struct grid2d;
  struct grid2d_hex;
  struct grid2d_tri;

  /// \}


  /// \{
  /// Grid_2D.

  template <typename Exact>
  struct vtypes< Grid_2D<Exact> >
  {
    typedef mlc::uint_<2> dim;
  };
  
  template <typename Exact>
  struct Grid_2D : public Grid<Exact>
  {
  protected:
    Grid_2D() {}
  };

  /// \}


  /// \{
  /// Super types.

  template<>
  struct super_trait_< grid2d >
  {
    typedef Grid_2D< grid2d > ret;
  };

  template<>
  struct super_trait_< grid2d_hex >
  {
    typedef Grid_2D< grid2d_hex > ret;
  };

  template<>
  struct super_trait_< grid2d_tri >
  {
    typedef Grid_2D< grid2d_tri > ret;
  };

  /// \}
  



  /// Rectangular grid struct.

  struct grid2d : public Grid_2D< grid2d >
  {};

  typedef grid2d grid2d_rec; // more explicit


  /// Hexagonal grid struct.

  struct grid2d_hex : public Grid_2D< grid2d_hex >
  {};


  /// Triangular grid struct.

  struct grid2d_tri : public Grid_2D< grid2d_tri >
  {};


} // end of namespace oln


#endif // ! OLN_CORE_2D_GRID2D_HH
