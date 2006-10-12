// Copyright (C) 2001, 2002, 2003, 2004, 2006 EPITA Research and
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

#ifndef OLN_CORE_3D_DPOINT3D_HH
# define OLN_CORE_3D_DPOINT3D_HH

# include <mlc/int.hh>
# include <oln/core/3d/point3d.hh>
# include <oln/core/internal/dpoint_nd.hh>
// For oln::dpoint3d and oln::dpoint3df.
# include <oln/core/3d/aliases.hh>


namespace oln
{


  /// Super type.
  template<typename C>
  struct set_super_type< dpoint3d_<C> >
  {
    typedef internal::dpoint_nd< dpoint3d_<C> > ret;
  };


  /// Virtual types associated to oln::dpoint3d_<C>.
  template <typename C>
  struct vtypes< dpoint3d_<C> >
  {
    typedef grid3d        grid_type;
    typedef point3d       point_type;
    typedef C             coord_type;
    typedef mlc::uint_<3> dim_type;

    typedef mlc::uint_<0> slice_comp_type;
    typedef mlc::uint_<1> row_comp_type;
    typedef mlc::uint_<2> col_comp_type;
  };


  /// General 3D dpoint class.
  template <typename C>
  class dpoint3d_
    : public stc_get_supers(dpoint3d_<C>)
  {
    typedef dpoint3d_<C>                self_t;
    typedef stc_get_super(dpoint3d_<C>) super_t;

    using super_t::v_;

  public:

    /// Ctor.
    dpoint3d_();

    /// Ctor.
    dpoint3d_(const xtd::vec<3,C>& v);

    /// Ctor.
    dpoint3d_(C slice, C row, C col);

    C  slice() const;
    C& slice();

    C  row()   const;
    C& row();

    C  col()   const;
    C& col();
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename C>
  dpoint3d_<C>::dpoint3d_()
  {
  }

  template <typename C>
  dpoint3d_<C>::dpoint3d_(const xtd::vec<3,C>& v)
    : super_t(v)
  {
  }

  template <typename C>
  dpoint3d_<C>::dpoint3d_(C slice, C row, C col)
    : super_t(xtd::mk_vec(slice, row, col))
  {
  }

  template <typename C>
  C  dpoint3d_<C>::slice() const { return v_[0]; }
  template <typename C>
  C& dpoint3d_<C>::slice()       { return v_[0]; }

  template <typename C>
  C  dpoint3d_<C>::row()   const { return v_[1]; }
  template <typename C>
  C& dpoint3d_<C>::row()         { return v_[1]; }

  template <typename C>
  C  dpoint3d_<C>::col()   const { return v_[2]; }
  template <typename C>
  C& dpoint3d_<C>::col()         { return v_[2]; }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_3D_DPOINT3D_HH
