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

#ifndef MLN_CORE_MESH_P_PITER_HH
# define MLN_CORE_MESH_P_PITER_HH

# include <mln/core/internal/point_iterator_base.hh>
# include <mln/core/mesh_p.hh>

/*! \file mln/core/mesh_psite.hh
 *
 * \brief Definition of point iterator on graph-based point set.
 */

namespace mln
{

  template<typename P> class mesh_p_piter_;

  template<typename P>
  class mesh_p_piter_ : public internal::point_iterator_base_< P, mesh_p_piter_<P> >
  {
    typedef mesh_p_piter_<P> self_;
    typedef internal::point_iterator_base_< P, self_ > super_;

  public:
    
    // Make definitions from super class available.
    enum { dim = super_::dim };

    mesh_p_piter_(const mesh_p<P>& s);

    /// Give a hook to the point address.
    const P* pointer_() const;

    /// Read-only access to the \p i-th coordinate.
    mln_coord(P) operator[](unsigned i) const;

    /// Test if the iterator is valid.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Convert the iterator into a point.
    operator P() const;

  protected:
    const std::vector<P>& loc_;
    unsigned i_;
    P p_;
  };

# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  mesh_p_piter_<P>::mesh_p_piter_(const mesh_p<P>& s)
    : loc_(s.loc_)
  {
    invalidate();
  }


  template<typename P>
  const P*
  mesh_p_piter_<P>::pointer_() const
  {
    return & p_;
  }


  template<typename P>
  mln_coord(P)
  mesh_p_piter_<P>::operator[](unsigned i) const
  {
    return p_[i];
  }


  template<typename P>
  bool
  mesh_p_piter_<P>::is_valid() const
  {
    return i_ != loc_.size();
  }


  template<typename P>
  void
  mesh_p_piter_<P>::invalidate()
  {
    i_ = loc_.size();
  }


  template<typename P>
  void 
  mesh_p_piter_<P>::start()
  {
    i_ = 0;
    if (is_valid())
      p_ = loc_[i_];
  }
  

  template<typename P>
  void 
  mesh_p_piter_<P>::next_()
  {
    ++i_;
    if (is_valid())
      p_ = loc_[i_];
  }
  

  template<typename P>
  mesh_p_piter_<P>::operator P() const
  {
    mln_precondition(is_valid());
    return p_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_MESH_P_PITER_HH
