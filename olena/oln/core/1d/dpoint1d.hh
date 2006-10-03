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

#ifndef OLN_CORE_1D_DPOINT1D_HH
# define OLN_CORE_1D_DPOINT1D_HH

# include <mlc/int.hh>
# include <oln/core/1d/point1d.hh>
# include <oln/core/internal/dpoint_nd.hh>


namespace oln
{


  /// Super type.
  template<typename C>
  struct set_super_type< dpoint1d_<C> >
  {
    typedef internal::dpoint_nd< dpoint1d_<C> > ret;
  };


  /// Virtual types associated to oln::dpoint1d_<C>.
  template <typename C>
  struct vtypes< dpoint1d_<C> >
  {
    typedef grid1d        grid_type;
    typedef point1d       point_type;
    typedef C             coord_type;
    typedef mlc::uint_<1> dim_type;
  };


  /// General 1D dpoint class.
  template <typename C>
  class dpoint1d_
    : public stc_get_supers(dpoint1d_<C>)
  {
    typedef dpoint1d_<C>                self_t;
    typedef stc_get_super(dpoint1d_<C>) super_t;
    typedef oln_type_of(self_t, coord)  coord_t;

    using super_t::v_;

  public:

    /// Ctor.
    dpoint1d_()
    {
    }

    /// Ctor.
    dpoint1d_(const xtd::vec<1,coord_t>& v)
      : super_t(v)
    {
    }

    /// Ctor.
    dpoint1d_(coord_t index)
      : super_t(xtd::mk_vec(index))
    {
    }

    coord_t  index() const { return v_[0]; }
    coord_t& index()       { return v_[0]; }
  };


} // end of namespace oln


#endif // ! OLN_CORE_1D_DPOINT1D_HH
