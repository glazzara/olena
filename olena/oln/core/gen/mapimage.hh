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

#ifndef OLENA_CORE_GEN_MAPIMAGE_HH
# define OLENA_CORE_GEN_MAPIMAGE_HH

# include <oln/core/image_entry.hh>
# include <oln/core/gen/topo_bbox.hh>
# include <map>


namespace oln
{

  // Forward declaration.
  template <typename point_t, typename value_t> class mapimage;


  /// Virtual types associated to oln::mapimage<>.
  template <typename point_t, typename value_t>
  struct vtypes< mapimage<point_t, value_t> >
  {
    typedef topo_bbox_<point_t> topo_type;
    typedef oln_type_of(point_t, grid) grid_type;

    typedef point_t point_type;
    
    typedef fwd_piter_bbox_<topo_type> fwd_piter_type;
    typedef bkd_piter_bbox_<topo_type> bkd_piter_type;
    
    typedef value_t value_type;

    typedef mlc::true_ is_mutable_type;
    typedef value_t lvalue_type;
    
    typedef void real_type; // FIXME
  };


  /// Super type declaration.
  template <typename point_t, typename value_t>
  struct set_super_type< mapimage<point_t, value_t> >
  {
    typedef mapimage<point_t, value_t> self_t;
    typedef image_entry<self_t> ret;
  };


  /// General 2D image class.
  template <typename point_t, typename value_t>
  class mapimage : public image_entry< mapimage<point_t, value_t> >
  {
    typedef mapimage<point_t, value_t> self_t;
    typedef oln_type_of(self_t, topo) topo_t;

  public:

    /// Ctor.
    mapimage(const value_t& val)
      : val_(val)
    {
    }

    const topo_t& impl_topo() const
    {
      return topo_;
    }

    value_t impl_op_read(const point_t& p) const
    {
      if (not has(p))
	return val_;
      return data_[p];
    }

    value_t& impl_op_readwrite(const point_t& p)
    {
      topo_.bbox().take(p);
      return data_[p];
    }

    bool impl_has(const point_t& p) const
    {
      return data_.find(p) != data_.end();
    }

  private:
    
    value_t val_;
    mutable topo_t topo_;
    mutable std::map<point_t, value_t> data_;
  };


} // end of namespace oln


#endif // ! OLENA_CORE_GEN_MAPIMAGE_HH
