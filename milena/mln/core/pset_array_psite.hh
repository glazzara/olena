// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_PSET_ARRAY_PSITE_HH_
# define MLN_CORE_PSET_ARRAY_PSITE_HH_

/*! \file mln/core/runs_psite.hh
 *
 * \brief Definition of the psite associated to the pset_array type.
 */

# include <mln/core/concept/point_site.hh>
# include <mln/util/tracked_ptr.hh>


namespace mln
{
  /*! \brief  Psite class associated to pset_array.
  **
  **          Parameter \c Psite is the type of the underlying psite.
  */
  template <typename Psite>
  class pset_array_psite : public Point_Site< pset_array_psite<Psite> >
  {
  public:

    typedef mln_point(Psite) point;
    typedef mln_dpoint(Psite) dpoint;
    typedef mln_coord(point) coord;

    typedef Psite psite;

    typedef mln_mesh(point) mesh;
    enum { dim = point::dim };

    pset_array_psite(const psite& psite, unsigned index);

    operator point () const;

    /// Reference to the corresponding point.
    const point& to_point() const;


    /// Useful method for the access
    unsigned to_index() const;
    unsigned& to_index();
    const psite& to_psite() const;

    /// Give the i-th coordinate of the corresponding point.
    mln_coord(point) operator[](unsigned i) const;

  protected:

    /// Psite which allows us to access to the images
    const psite& psite_;

    /// Position in the pset_array.
    unsigned index_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename Psite>
  pset_array_psite<Psite>::pset_array_psite(const psite& psite,
					    unsigned index) :
    psite_(psite),
    index_(index)
  {
  }

  template <typename Psite>
  inline
  pset_array_psite<Psite>::operator typename pset_array_psite<Psite>::point ()
    const
  {
    return to_point();
  }

  template <typename Psite>
  inline
  const typename pset_array_psite<Psite>::point&
  pset_array_psite<Psite>::to_point() const
  {
    return psite_.to_point();
  }

  template <typename Psite>
  inline
  unsigned
  pset_array_psite<Psite>::to_index() const
  {
    return index_;
  }

  template <typename Psite>
  inline
  unsigned&
  pset_array_psite<Psite>::to_index()
  {
    return index_;
  }

  template <typename Psite>
  inline
  const Psite&
  pset_array_psite<Psite>::to_psite() const
  {
    return psite_;
  }

  template <typename Psite>
  inline
  mln_coord(pset_array_psite<Psite>::point)
  pset_array_psite<Psite>::operator[] (unsigned i) const
  {
    return psite_[i];
  }



# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

# include <mln/core/pset_array.hh>

#endif // ! MLN_CORE_INTERNAL_PSET_ARRAY_PSITE_HH
