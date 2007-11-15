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
// License.  
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_MESH_P_HH
# define MLN_CORE_MESH_P_HH

# include <mln/core/concept/point_site.hh>
# include <mln/core/internal/point_set_base.hh>
# include <mln/accu/bbox.hh>
# include <mln/util/graph.hh>
# include <mln/core/mesh_psite.hh>
# include <mln/core/mesh_p_piter.hh>

/*! \file mln/core/mesh_p.hh
 *
 * \brief Definition of an point set based on graph.
 */

namespace mln
{

  template<typename P> class mesh_p_piter_;

  template<typename P>
  struct mesh_p : public internal::point_set_base_< P, mesh_p<P> >
  {
    mesh_p (util::graph<void>& gr,
	    std::vector<P>& loc);

    /// Point_Site associated type.
    typedef mesh_psite<P> psite;
    
    /// Forward Point_Iterator associated type.
    typedef mesh_p_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef mesh_p_piter_<P> bkd_piter; // FIXME

    std::size_t npoints() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    bool has(const psite& p) const;
                                                
    util::graph<void>	gr_;
    std::vector<P>	loc_;
    box_<P> bb_;
  };

# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  mesh_p<P>::mesh_p (util::graph<void>& gr,
		     std::vector<P>& loc)
    : gr_ (gr),
      loc_ (loc)
  {
    accu::bbox<P> a;
    for (unsigned i = 0; i < loc.size(); ++i)
      a.take(loc[i]);
    bb_ = a.to_result();
  }

  template<typename P>
  std::size_t
  mesh_p<P>::npoints() const
  {
    return this->gr_.nb_node_;
  }
  
  template<typename P>
  const box_<P>&
  mesh_p<P>::bbox() const
  {
    return bb_;
  }

  template<typename P>
  bool
  mesh_p<P>::has(const psite& p) const
  {
    for (unsigned i = 0; i < loc_.size(); ++i)
      if (loc_[i] == p)
	return true;
    return false;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_CORE_MESH_P_HH
