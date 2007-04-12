// Copyright (C) 2001, 2002, 2003, 2004, 2006, 2007 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_2D_DPOINT2D_HH
# define OLN_CORE_2D_DPOINT2D_HH

# include <oln/core/2d/grid2d.hh>
# include <oln/core/internal/dpoint_base.hh>
# include <oln/core/internal/point_impl.hh>


namespace oln
{

  // Fwd decls.
  struct  point2d;
  struct dpoint2d;


  // Super type.
  template<>
  struct super_trait_< dpoint2d >
  {
    typedef internal::dpoint_base_< dpoint2d > ret;
  };


  // Virtual types.
  template <>
  struct vtypes< dpoint2d >
  {
    typedef grid2d   grid;
    typedef int      coord;
    typedef point2d  point;
  };


  /// Usual 2D dpoint class.
  class dpoint2d : public internal::dpoint_base_< dpoint2d >,
		   public internal::point_impl_< 2, dpoint2d >
  {
    typedef internal::dpoint_base_< dpoint2d > super;
  public:

    stc_using_(coord);

    /// Ctors.
    dpoint2d();
    dpoint2d(int row, int col);
  };



# ifndef OLN_INCLUDE_ONLY

  dpoint2d::dpoint2d()
  {
  }

  dpoint2d::dpoint2d(int row, int col)
  {
    this->row() = row;
    this->col() = col;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


# include <oln/core/2d/point2d.hh>


#endif // ! OLN_CORE_2D_DPOINT2D_HH
