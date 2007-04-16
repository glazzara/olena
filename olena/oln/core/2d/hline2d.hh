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

#ifndef OLN_CORE_2D_HLINE2D_HH
# define OLN_CORE_2D_HLINE2D_HH

# include <oln/core/internal/symmetrical_window.hh>
# include <oln/core/2d/dpoint2d.hh>


namespace oln
{

  // Fwd decl.
  class hline2d;


# define super internal::symmetrical_window_<hline2d>

  // Super type.
  template <>
  struct super_trait_< hline2d >
  {
    typedef super ret;
  };


  // Virtual types.
  template <>
  struct vtypes< hline2d >
  {
    typedef point2d point;
  };


  /// 2D horizontal line window.

  class hline2d : public super
  {
  public:
    
    hline2d(unsigned half_length);

  private:
    void init_(int dcol_min, int dcol_max);
    void impl_take(); // safety; w/o impl, it prevents from calling take(dp).

  }; // end of class oln::hline2d



# ifndef OLN_INCLUDE_ONLY

  hline2d::hline2d(unsigned half_length)
  {
    precondition(half_length > 0);
    this->init_(- half_length, + half_length);
  }

  void
  hline2d::init_(int dcol_min, int dcol_max)
  {
    for (int dcol = dcol_min; dcol <= dcol_max; ++dcol)
      this->super::impl_take(dpoint2d(0, dcol));
  }

# endif // ! OLN_INCLUDE_ONLY

# undef super

} // end of namespace oln


#endif // ! OLN_CORE_2D_HLINE2D_HH
