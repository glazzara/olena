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

#ifndef OLN_CORE_2D_VLINE2D_HH
# define OLN_CORE_2D_VLINE2D_HH

# include <oln/core/internal/window.hh>
# include <oln/core/2d/dpoint2d.hh>


namespace oln
{

  // Fwd decl.
  class vline2d;


  // Super type.
  template <>
  struct super_trait_< vline2d >
  {
    typedef internal::window_<vline2d> ret;
  };


  // Virtual types.
  template <>
  struct vtypes< vline2d >
  {
    typedef point2d point;
  };


  /// 2D vertical line window.

  class vline2d : public internal::window_< vline2d >
  {
  public:
    
    vline2d(unsigned half_length);
    vline2d(int drow_min, int drow_max);

  private:
    void init_(int drow_min, int drow_max);
    void take(); // safety; w/o impl, it provides from calling super::take(dp).

  }; // end of class oln::vline2d



# ifndef OLN_INCLUDE_ONLY

  vline2d::vline2d(unsigned half_length)
  {
    precondition(half_length > 0);
    this->init_(- half_length, half_length);
  }

  vline2d::vline2d(int drow_min, int drow_max)
  {
    precondition(drow_max > drow_min);
    this->init_(drow_min, drow_max);
  }

  void
  vline2d::init_(int drow_min, int drow_max)
  {
    for (int drow = drow_min; drow <= drow_max; ++drow)
      this->internal::window_<vline2d>::take(dpoint2d(drow, 0));
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_2D_VLINE2D_HH
