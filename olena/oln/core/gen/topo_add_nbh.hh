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

#ifndef OLN_CORE_GEN_TOPO_ADD_NBH_HH
# define OLN_CORE_GEN_TOPO_ADD_NBH_HH

# include <oln/core/internal/topology_morpher.hh>



namespace oln
{


  // Forward declarations.
  template <typename topo, typename nbh> class topo_add_nbh;


  // Super type declaration.
  template <typename topo, typename nbh>
  struct set_super_type< topo_add_nbh<topo, nbh> >
  {
    typedef topo_add_nbh<topo, nbh> self_t;
    typedef internal::topology_morpher<topo, self_t> ret;
  };


  /// Virtual types associated to oln::bbox_<point>.
  template <typename topo, typename nbh>
  struct vtypes< topo_add_nbh<topo, nbh> >
  {
    typedef nbh neighborhood_type;
  };

  template <typename topo, typename nbh>
  struct single_vtype< topo_add_nbh<topo, nbh>, typedef_::delegated_type >
  {
    typedef topo ret;
  };


  /// Bounding box topology based on a point class.
  template <typename topo_t, typename nbh_t>
  class topo_add_nbh : public internal::topology_morpher<topo_t, topo_add_nbh<topo_t, nbh_t> >
  {
  public:

    topo_add_nbh();

    topo_add_nbh(const topo_t& topo, const nbh_t& nbh);
    const nbh_t& impl_neighborhood() const;

    const topo_t& delegate() const;

    operator topo_t() const;

  protected:

    topo_t topo_;
    nbh_t nbh_;
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename topo_t, typename nbh_t>
  topo_add_nbh<topo_t, nbh_t>::topo_add_nbh()
  {
  }

  template <typename topo_t, typename nbh_t>
  topo_add_nbh<topo_t, nbh_t>::topo_add_nbh(const topo_t& topo, const nbh_t& nbh)
    : topo_(topo),
      nbh_(nbh)
  {
  }

  template <typename topo_t, typename nbh_t>
  const nbh_t& topo_add_nbh<topo_t, nbh_t>::impl_neighborhood() const
  {
    return nbh_;
  }

  template <typename topo_t, typename nbh_t>
  const topo_t& topo_add_nbh<topo_t, nbh_t>::delegate() const
  {
    return topo_;
  }

  template <typename topo_t, typename nbh_t>
  topo_add_nbh<topo_t, nbh_t>::operator topo_t() const
  {
    return topo_;
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_TOPO_ADD_NBH_HH
