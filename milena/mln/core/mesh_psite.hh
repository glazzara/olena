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

#ifndef MLN_MESH_PSITE_HH
# define MLN_MESH_PSITE_HH

/*! \file mln/core/mesh_psite.hh
 *
 * \brief Definition of a graph-based point site.
 *
 * \todo Clean-up!
 */


namespace mln
{

  // Fwd decl.
  template<typename P> class mesh_p;


  // FIXME: Doc!
  template<typename P>
  struct mesh_psite : public Point_Site< mesh_psite<P> >
  {
    typedef mln_mesh(P) mesh;
    enum { dim = P::dim };
    typedef P point;
    typedef mln_dpoint(P) dpoint;
    typedef mln_coord(P) coord;

    mesh_psite(unsigned i, mesh_p<P>* m_ptr);

    operator P() const;

    const point& to_point() const;

    coord operator[](unsigned i) const;

    unsigned i_;

    mesh_p<P>* m_ptr_;
  };

# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  mesh_psite<P>::mesh_psite(unsigned i, mesh_p<P>* m_ptr)
    : i_(i) ,
      m_ptr_(m_ptr)
  {
  }

  template<typename P>
  mesh_psite<P>::operator P() const
  {
    return m_ptr_->loc_[i_];
  }

  template<typename P>
  const P&
  mesh_psite<P>::to_point() const
  {
    return m_ptr_->loc_[i_];
  }

  template<typename P>
  mln_coord(P)
  mesh_psite<P>::operator[](unsigned i) const
  {
    return to_point()[i];
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of mln

#endif // MLN_MESH_PSITE_HH
