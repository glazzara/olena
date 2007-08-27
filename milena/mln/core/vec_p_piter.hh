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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_VEC_P_PITER_HH
# define MLN_CORE_VEC_P_PITER_HH

/*! \file mln/core/vec_p_piter.hh
 *
 * \brief Definition of point iterators on mln::vec_p.
 */

# include <mln/core/vec_p.hh>


namespace mln
{

  /*! \brief Forward iterator on points of a vec_p<P>.
   *
   */
  template <typename P>
  struct vec_p_fwd_piter_ : public Point_Iterator< vec_p_fwd_piter_<P> >
  {
    enum { dim = P::dim };

    /// Point_Site associated type.
    typedef P psite;

    /// Point associated type.
    typedef P point;

    /// Dpoint associated type.
    typedef mln_dpoint(P) dpoint;

    /// Coordinate associated type.
    typedef mln_coord(P) coord;

    /// Coordinate associated type.
    template <typename S>
    vec_p_fwd_piter_(const Point_Set<S>& s);

    /// Give a hook to the point address.
    const P* pointer_() const;

    /// Read-only access to the \p i-th coordinate.
    coord operator[](unsigned i) const;

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
    const std::vector<P>& vect_;
    unsigned i_;
    P p_;
  };



  // FIXME:
  template <typename P>
  struct vec_p_bkd_piter_ : internal::fixme
  {};



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  template <typename S>
  vec_p_fwd_piter_<P>::vec_p_fwd_piter_(const Point_Set<S>& s)
    : vect_(exact(s).vect())
  {
    invalidate();
  }

  template <typename P>
  const P*
  vec_p_fwd_piter_<P>::pointer_() const
  {
    return & p_;
  }

  template <typename P>
  mln_coord(P)
  vec_p_fwd_piter_<P>::operator[](unsigned i) const
  {
    mln_precondition(i < dim);
    mln_precondition(is_valid());
    return p_[i];
  }

  template <typename P>
  bool
  vec_p_fwd_piter_<P>::is_valid() const
  {
    return i_ < vect_.size();
  }

  template <typename P>
  void
  vec_p_fwd_piter_<P>::invalidate()
  {
    i_ = vect_.size();
  }

  template <typename P>
  void
  vec_p_fwd_piter_<P>::start()
  {
    i_ = 0;
    if (is_valid())
      p_ = vect_[i_];
  }

  template <typename P>
  void
  vec_p_fwd_piter_<P>::next_()
  {
    ++i_;
    p_ = vect_[i_];
  }

  template <typename P>
  vec_p_fwd_piter_<P>::operator P() const
  {
    mln_precondition(is_valid());
    return p_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_VEC_P_PITER_HH
