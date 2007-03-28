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

#ifndef OLN_CORE_2D_POINT2D_HH
# define OLN_CORE_2D_POINT2D_HH

# include <oln/core/2d/grid2d.hh>
# include <oln/core/internal/point2d.hh>


namespace oln
{


  class point2d;
  class dpoint2d;


  /// Super type.
  template<>
  struct super_trait_< point2d >
  {
    typedef internal::point2d_< point2d > ret;
  };


  /// Virtual types.
  template <>
  struct vtypes< point2d >
  {
    typedef grid2d   grid;
    typedef int      coord;
    typedef dpoint2d dpoint;
  };


  /// Usual 2D point class.
  class point2d : public internal::point2d_< point2d >
  {
    typedef internal::initializer_<
      internal::pair< internal::row_t<int>, internal::col_t<int> >
    > row_col_t;
  public:

    /// Ctors.
    point2d();
    point2d(int row, int col); // FIXME: Remove!
    point2d(const row_col_t& dat);
  };



# ifndef OLN_INCLUDE_ONLY

  point2d::point2d()
  {
  }

  point2d::point2d(int row, int col) // FIXME: Remove!
  {
    this->row() = row;
    this->col() = col;
  }

  point2d::point2d(const point2d::row_col_t& dat)
  {
    this->row() = dat->first.value;
    this->col() = dat->second.value;
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


// point2d goes with dpoint2d so:
# include <oln/core/2d/dpoint2d.hh>


#endif // ! OLN_CORE_2D_POINT2D_HH
