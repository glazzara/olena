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

#ifndef OLN_CORE_1D_BOX1D_HH
# define OLN_CORE_1D_BOX1D_HH

# include <oln/core/gen/box.hh>
# include <oln/core/1d/point1d.hh>


namespace oln
{

  // Forward declarations
  struct box1d;

  // Super type
  template <>
  struct super_trait_< box1d >
  {
    typedef box1d current;
    typedef internal::box_<box1d> ret;
  };

  // Virtual types
  template <>
  struct vtypes<box1d>
  {
    typedef point1d point;
    typedef box1d box;
  };

  // Class box1d
  class box1d : public internal::box_< box1d >
  {
    typedef box1d current;
    typedef internal::box_< box1d > super;
  public:
    // Note: we can't use stc_using because box1d isn't a templated class
    typedef super::point point;

    box1d();
    box1d(const box1d::from_to_t& dat);
    box1d(const point1d& pmin, const point1d& pmax);
    template <typename D>
    box1d(const internal::initializer_<D>& data);
  };

# ifndef OLN_INCLUDE_ONLY

  box1d::box1d()
  {
  }

  box1d::box1d(const box1d::from_to_t& dat) :
    super(dat)
  {
  }

  box1d::box1d(const point1d& pmin, const point1d& pmax) :
    super(pmin, pmax)
  {
  }

  template <typename D>
  box1d::box1d(const internal::initializer_<D>& data) :
    super(data)
  {
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_1D_BOX1D_HH

