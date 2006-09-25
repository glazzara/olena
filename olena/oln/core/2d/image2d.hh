// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and
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

#ifndef OLENA_CORE_2D_IMAGE2D_HH
# define OLENA_CORE_2D_IMAGE2D_HH

# include <oln/core/image_entry.hh>


namespace oln
{

  // Forward declaration.
  template <typename T> class image2d;


  /// Virtual types associated to oln::image2d<T>.
  template <typename T>
  struct vtypes< image2d<T> >
  {
    typedef topo_lbbox_<point2d> topo_type;
    typedef grid2d grid_type;

    typedef point2d point_type;
    
    typedef fwd_piter_bbox_<topo_type> fwd_piter_type;
    typedef bkd_piter_bbox_<topo_type> bkd_piter_type;
    
    typedef T value_type;
    
    typedef image2d<T> real_type;
  };


  /// Super type declaration.
  template <typename T>
  struct set_super_type< image2d<T> >
  {
    typedef image2d<T> self_t;
    typedef image_entry<self_t> ret;
  };


  /// General 2D image class.
  template <typename T>
  class image2d : public image_entry< image2d<T> >
  {
    typedef image2d<T> self_t;

  public:

    /// Ctor.
    image2d(unsigned nrows, unsigned ncols, unsigned border = 2)
      : topo_( bbox2d( point2d(0,0), point2d(nrows-1, ncols-1)), border )
    {
    }

    const topo2d& impl_topo() const
    {
      return topo_;
    }

    T impl_op_read(const point2d& p) const
    {
      return (T)(void*)this;
//       static T val_;
//       return ++val_;
    }

    bool impl_has(const point2d& p) const
    {
      return true;
    }

  private:

    topo2d topo_;
  };


} // end of namespace oln


#endif // ! OLENA_CORE_2D_IMAGE2D_HH
