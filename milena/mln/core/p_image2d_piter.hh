// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_P_IMAGE2D_PITER_HH
# define MLN_CORE_P_IMAGE2D_PITER_HH

/// \file mln/core/p_image2d_piter.hh
/// \brief Definition of point iterators on mln::p_image2d.

# include <mln/core/p_image2d.hh>
# include <mln/core/box_piter.hh>


namespace mln
{

  /// \brief Forward iterator on points of a p_array<P>.
  template <typename P>
  struct p_image2d_fwd_piter_
    : public internal::site_iterator_base_< P, p_image2d_fwd_piter_<P> >
  {
    typedef p_image2d_fwd_piter_<P> self_;
    typedef internal::site_iterator_base_< P, self_ > super_;
  public:
    /// The associated psite type.
    typedef P psite;

    /// The associated point type.
    typedef mln_point(P) point;

    enum { dim = super_::dim };

    /// Coordinate associated type.
    template <typename S>
    p_image2d_fwd_piter_(const p_image2d<S>& s);

    /// Reference of the corresponding psite.
    const psite& to_psite() const;

    /// Reference of the corresponding point.
    const point& to_point() const;

    /// Read-only access to the \p i-th coordinate.
    mln_coord(point) operator[](unsigned i) const;

    /// Test if the iterator is valid.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Convert the iterator into a psite.
    operator psite() const;

  protected:
    box_fwd_piter_<P> piter_;
    const image2d<bool> ima_;
  };



  /// \brief Backward iterator on points of a p_array<P>.
  template <typename P>
  struct p_image2d_bkd_piter_
    : public internal::site_iterator_base_< P, p_image2d_bkd_piter_<P> >
  {
    typedef p_image2d_bkd_piter_<P> self_;
    typedef internal::site_iterator_base_< P, self_ > super_;
  public:
    /// The associated psite type.
    typedef P psite;

    /// The associated point type.
    typedef mln_point(P) point;

    enum { dim = super_::dim };

    /// Coordinate associated type.
    template <typename S>
    p_image2d_bkd_piter_(const p_image2d<S>& s);

    /// Reference of the corresponding psite.
    const psite& to_psite() const;

    /// Reference of the corresponding point.
    const point& to_point() const;

    /// Read-only access to the \p i-th coordinate.
    mln_coord(point) operator[](unsigned i) const;

    /// Test if the iterator is valid.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Convert the iterator into a psite.
    operator psite() const;

  protected:
    box_bkd_piter_<P> piter_;
    image2d<bool> ima_;

  };



# ifndef MLN_INCLUDE_ONLY

  /*------------------------.
  | p_image2d_fwd_piter_<P>.  |
  `------------------------*/

  template <typename P>
  template <typename S>
  inline
  p_image2d_fwd_piter_<P>::p_image2d_fwd_piter_(const p_image2d<S>& s)
    : piter_(exact(s).image().domain()),
      ima_(exact(s).image())
  {
    invalidate();
  }

  template <typename P>
  inline
  const P&
  p_image2d_fwd_piter_<P>::to_psite() const
  {
    return piter_.to_psite();
  }

  template <typename P>
  inline
  const mln_point(P)&
  p_image2d_fwd_piter_<P>::to_point() const
  {
    return piter_.to_point();
  }

  template <typename P>
  inline
  mln_coord(mln_point_(P))
  p_image2d_fwd_piter_<P>::operator[](unsigned i) const
  {
    return piter_[i];
  }

  template <typename P>
  inline
  bool
  p_image2d_fwd_piter_<P>::is_valid() const
  {
    return piter_.is_valid();
  }

  template <typename P>
  inline
  void
  p_image2d_fwd_piter_<P>::invalidate()
  {
    return piter_.invalidate();
  }

  template <typename P>
  inline
  void
  p_image2d_fwd_piter_<P>::start()
  {
    piter_.start();
    while(is_valid() && !ima_(piter_))
      piter_.next();
  }

  template <typename P>
  inline
  void
  p_image2d_fwd_piter_<P>::next_()
  {
    piter_.next();
    while(is_valid() && !ima_(piter_))
      piter_.next();
  }

  template <typename P>
  inline
  p_image2d_fwd_piter_<P>::operator P() const
  {
    return P(piter_);
  }


  /*------------------------.
  | p_image2d_bkd_piter_<P>.  |
  `------------------------*/

  template <typename P>
  template <typename S>
  inline
  p_image2d_bkd_piter_<P>::p_image2d_bkd_piter_(const p_image2d<S>& s)
    : piter_(exact(s).image().domain()),
      ima_(exact(s).image())
  {
    invalidate();
  }

  template <typename P>
  inline
  const P&
  p_image2d_bkd_piter_<P>::to_psite() const
  {
    return piter_.to_psite();
  }

  template <typename P>
  inline
  const mln_point(P)&
  p_image2d_bkd_piter_<P>::to_point() const
  {
    return piter_.to_point();
  }

  template <typename P>
  inline
  mln_coord(mln_point_(P))
  p_image2d_bkd_piter_<P>::operator[](unsigned i) const
  {
    mln_precondition(is_valid());
    return piter_[i];
  }

  template <typename P>
  inline
  bool
  p_image2d_bkd_piter_<P>::is_valid() const
  {
    return piter_.is_valid();
  }

  template <typename P>
  inline
  void
  p_image2d_bkd_piter_<P>::invalidate()
  {
    return piter_.invalidate();
  }

  template <typename P>
  inline
  void
  p_image2d_bkd_piter_<P>::start()
  {
    piter_.start();
    while(is_valid() && !ima_(piter_))
      piter_.next();
  }

  template <typename P>
  inline
  void
  p_image2d_bkd_piter_<P>::next_()
  {
    piter_.next();
    while(is_valid() && !ima_(piter_))
      piter_.next();
  }

  template <typename P>
  inline
  p_image2d_bkd_piter_<P>::operator P() const
  {
    mln_precondition(is_valid());
    return P(piter_);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_IMAGE2D_PITER_HH
