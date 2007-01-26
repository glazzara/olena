// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_GEN_TOPO_LBBOX_HH
# define OLN_CORE_GEN_TOPO_LBBOX_HH

# include <oln/core/gen/bbox.hh>
# include <oln/core/topology_entry.hh>

namespace oln
{

  // Forward declaration.
  template <typename point> class topo_lbbox_;


  // Super type declaration.
  template <typename point>
  struct set_super_type< topo_lbbox_<point> >
  {
    typedef topo_lbbox_<point> self_t;
    typedef topology_entry<self_t> ret;
  };


  /// Virtual types associated to oln::topo_lbbox_<point>.
  template <typename point>
  struct vtypes< topo_lbbox_<point> >
  {
    /* Same trick as in the virtual types of oln::topo_bbox_ to force
       the compiler to instantiate bbox_<point>.  */
    typedef typename bbox_<point>::self_type bbox_type;
    typedef point                            point_type;
    typedef mlc::true_                       is_random_accessible_type;
  };


  /// Bounding box topology based on a point class.
  template <typename point>
  class topo_lbbox_ :
    public topology_entry< topo_lbbox_<point> >,
    private mlc::assert_< mlc_is_a(point, abstract::point) >
  {
    typedef topo_lbbox_<point> self_t;
    typedef oln_vtype(self_t, bbox) bbox_t;

  public:

    topo_lbbox_();
    topo_lbbox_(const bbox_t& bb, unsigned border);
    topo_lbbox_(const point& pmin, const point& pmax, unsigned border);

    const bbox_t& impl_bbox() const;

    bool impl_has(const point& p) const;
    bool impl_has_large(const point& p) const;

    const bbox_t& lbbox() const;
    unsigned border () const;

  protected:

    bbox_<point> bb_;
    unsigned border_;
    bbox_<point> lbb_; // FIXME: HERE

  };


# ifndef OLN_INCLUDE_ONLY

  template <typename point>
  topo_lbbox_<point>::topo_lbbox_()
  {
  }

  template <typename point>
  topo_lbbox_<point>::topo_lbbox_(const typename topo_lbbox_<point>::bbox_t& bb,
				  unsigned border)
    : bb_(bb), border_(border)
  {
    typedef oln_vtype(point, dpoint) dpoint_t;
    dpoint_t dp;
    dp.set_all(border);
    lbb_ = bbox_<point>(bb_.pmin() - dp, bb_.pmax() + dp);
  }

  template <typename point>
  topo_lbbox_<point>::topo_lbbox_(const point& pmin, const point& pmax,
				  unsigned border)
    : bb_(pmin, pmax), border_(border)
  {
    typedef oln_vtype(point, dpoint) dpoint_t;
    dpoint_t dp;
    dp.set_all(border);
    lbb_ = bbox_<point>(bb_.pmin() - dp, bb_.pmax() + dp);
  }

  template <typename point>
  const typename topo_lbbox_<point>::bbox_t&
  topo_lbbox_<point>::impl_bbox() const
  {
    return bb_;
  }

  template <typename point>
  bool
  topo_lbbox_<point>::impl_has(const point& p) const
  {
    return bb_.has(p);
  }

  template <typename point>
  bool
  topo_lbbox_<point>::impl_has_large(const point& p) const
  {
    return lbb_.has(p);
  }

  template <typename point>
  const typename topo_lbbox_<point>::bbox_t&
  topo_lbbox_<point>::lbbox() const
  {
    return lbb_;
  }

  template <typename point>
  unsigned
  topo_lbbox_<point>::border () const
  {
    return border_;
  }
  
# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_TOPO_LBBOX_HH
