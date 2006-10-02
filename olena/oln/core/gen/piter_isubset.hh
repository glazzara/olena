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

#ifndef OLN_CORE_GEN_PITER_ISUBSET_HH
# define OLN_CORE_GEN_PITER_ISUBSET_HH

# include <oln/core/abstract/topology.hh>
# include <oln/core/abstract/iterator_on_points.hh>


namespace oln
{


  // Forward declaration.
  template <typename piter_t, typename isubset_t> class piter_isubset_;


  // Super type declaration.
  template <typename piter_t, typename isubset_t>
  struct set_super_type< piter_isubset_<piter_t, isubset_t> >
  {
    typedef piter_isubset_<piter_t, isubset_t> self_t;
    typedef abstract::iterator_on_points<self_t> ret;
  };


  /// Virtual types associated to oln::piter_isubset_<piter_t, isubset_t>.
  template <typename piter_t, typename isubset_t>
  struct vtypes< piter_isubset_<piter_t, isubset_t> >
  {
    typedef oln_type_of(piter_t, point) point_type;
    typedef oln_type_of(piter_t, grid)  grid_type;

    typedef topo_add_isubset<oln_type_of(piter_t, topo), isubset_t> topo_type;
  };



  /// Abstract forward point iterator class.
  template <typename piter_t, typename isubset_t>
  class piter_isubset_ : public abstract::iterator_on_points< piter_isubset_<piter_t, isubset_t> >
  {
    typedef piter_isubset_<piter_t, isubset_t> self_t;
    typedef abstract::iterator_on_points<self_t> super_t;

    typedef oln_type_of(self_t, point)  topo_t;
    typedef oln_type_of(piter_t, point) point_t;

  public:
    
    template <typename T>
    piter_isubset_(const abstract::topology<T>& topo)
      : p_(topo),
	isubset_(topo.exact().isubset())
    {
    }

    void impl_start()
    {
      p_.start();
      while (p_.is_valid() and isubset_(p_) == false)
	p_.next();
    }

    void impl_next()
    {
      do
	p_.next();
      while (p_.is_valid() and isubset_(p_) == false);
    }

    void impl_invalidate()
    {
      p_.invalidate();
    }
    
    bool impl_is_valid() const
    {
      return p_.is_valid();
    }
    
    point_t impl_to_point() const
    {
      return p_.to_point();
    }
    
    const point_t* impl_point_adr() const
    {
      return p_.point_adr();
    }

    const topo_t& topo() const
    {
      topo_t tmp(p_.topo(), isubset_);
      return tmp;
    }

    template <typename new_topo_t>
    struct change_topology_
    {
      typedef typename piter_t::template change_topology_<new_topo_t>::ret new_piter_t;
      typedef piter_isubset_<new_piter_t, isubset_t> ret;
    };

  protected:

    piter_t p_;
    const isubset_t& isubset_;

  }; // end of class oln::piter_isubset_<point>
  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_PITER_ISUBSET_HH
