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

#ifndef OLN_CORE_GEN_TOPO_ADD_ISUBSET_HH
# define OLN_CORE_GEN_TOPO_ADD_ISUBSET_HH

# include <oln/core/internal/topology_morpher.hh>



namespace oln
{


  // Forward declarations.
  template <typename topo, typename isubset> class topo_add_isubset;


  // Super type declaration.
  template <typename topo, typename isubset>
  struct set_super_type< topo_add_isubset<topo, isubset> >
  {
    typedef topo_add_isubset<topo, isubset> self_t;
    typedef internal::topology_morpher<topo, self_t> ret;
  };


  /// Virtual types associated to oln::bbox_<point>.
  template <typename topo, typename isubset>
  struct vtypes< topo_add_isubset<topo, isubset> >
  {
    typedef isubset isubset_type;
  };


  /// Bounding box topology based on a point class.
  template <typename topo_t, typename isubset_t>
  class topo_add_isubset
    : public internal::topology_morpher<topo_t, topo_add_isubset<topo_t, isubset_t> >
  {
    typedef topo_add_isubset<topo_t, isubset_t> self_t;
    typedef oln_type_of(self_t, point) point_t;

  public:

    topo_add_isubset()
    {
    }

    topo_add_isubset(const topo_t& topo, const isubset_t& isubset)
      : topo_(topo),
	isubset_(isubset)
    {
    }

    bool impl_has(const point_t& p) const
    {
      return topo_.has(p) and isubset_(p) == true;
    }
    
    bool impl_has_large(const point_t& p) const
    {
      return topo_.has(p) and isubset_(p) == true;
    }

    const topo_t& delegate() const
    {
      return topo_;
    }

    const isubset_t& isubset() const
    {
      return isubset_;
    }

    operator topo_t() const
    {
      return topo_;
    }

  protected:

    topo_t topo_;
    isubset_t isubset_;
  };


} // end of namespace oln


#endif // ! OLN_CORE_GEN_TOPO_ADD_ISUBSET_HH
