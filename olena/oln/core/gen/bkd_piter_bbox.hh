// Copyright (C) 2001, 2003, 2004, 2005, 2006 EPITA Research and
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

#ifndef OLN_CORE_GEN_BKD_PITER_BBOX_HH
# define OLN_CORE_GEN_BKD_PITER_BBOX_HH

# include <oln/core/abstract/topology.hh>
# include <oln/core/internal/bbox_bkd_piter.hh>


namespace oln
{


  // Forward declaration.
  template <typename topo> class bkd_piter_bbox_;


  // Super type declaration.
  template <typename topo>
  struct set_super_type< bkd_piter_bbox_<topo> >
  {
    typedef bkd_piter_bbox_<topo> self_t;
    typedef internal::bbox_bkd_piter<self_t> ret;
  };


  /// Virtual types associated to oln::bkd_piter_bbox_<topo>.
  template <typename topo>
  struct vtypes< bkd_piter_bbox_<topo> >
  {
    typedef oln_type_of(topo, point) point_type;
    typedef oln_type_of(topo, grid) grid_type;
  };



  /// Abstract forward point iterator class.
  template <typename topo_t>
  class bkd_piter_bbox_ : public internal::bbox_bkd_piter< bkd_piter_bbox_<topo_t> >,
			  private mlc::assert_< mlc_is_a(topo_t, abstract::topology) >
  {
    typedef bkd_piter_bbox_<topo_t> self_t;
    typedef internal::bbox_bkd_piter<self_t> super_t;

  public:
    
    template <typename T>
    bkd_piter_bbox_(const abstract::topology<T>& topo)
      : super_t(topo.exact().bbox()),
        topo_(topo.exact())
    {
    }
    
    const topo_t& topo() const
    {
      return topo_;
    }

    void print(std::ostream& ostr) const
    {
      ostr << "{ p=" << super_t::p_
	   << " }";
    }

    friend
    std::ostream& operator<<(std::ostream& ostr, const bkd_piter_bbox_<topo_t>& t)
    {
      t.print(ostr);
      return ostr;
    }

    template <typename new_topo_t>
    struct change_topology_
    {
      typedef bkd_piter_bbox_<new_topo_t> ret;
    };

  protected:

    const topo_t& topo_;

  }; // end of class oln::bkd_piter_bbox_<point>
  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_BKD_PITER_BBOX_HH
