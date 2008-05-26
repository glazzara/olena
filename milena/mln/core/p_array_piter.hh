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

#ifndef MLN_CORE_P_ARRAY_PITER_HH
# define MLN_CORE_P_ARRAY_PITER_HH

/// \file mln/core/p_array_piter.hh
/// \brief Definition of point iterators on mln::p_array.

# include <mln/core/p_array.hh>
# include <mln/core/internal/site_iterator_base.hh>


namespace mln
{

  /// \brief Forward iterator on points of a p_array<P>.
  template <typename P>
  class p_array_fwd_piter_
    :
    public internal::site_iterator_base_< p_array_psite<P>,
					  p_array_fwd_piter_<P> >
  {
    typedef p_array_fwd_piter_<P> self;
    typedef internal::site_iterator_base_<p_array_psite<P>, self> super;

  public:

    /// Constructor with no argument.
    p_array_fwd_piter_();

    /// Constructor.
    p_array_fwd_piter_(const p_array<P>& arr);

    /// Change of site set target.
    void change_target(const p_array<P>& arr);

    /// Test if the iterator is valid.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Return the subject.
    const p_array_psite<P>& unproxy() const;

    // As a Site_Proxy:
    typedef typename super::site site;
    const site& to_site() const;

  protected:

    p_array_psite<P> p_;
  };



//   /// \brief Backward iterator on points of a p_array<P>.
//   template <typename P>
//   struct p_array_bkd_piter_
//     : public internal::point_iterator_base_< P, p_array_bkd_piter_<P> >
//   {
//     typedef p_array_bkd_piter_<P> self_;
//     typedef internal::point_iterator_base_< P, self_ > super_;
//   public:
//     /// The associated psite type.
//     typedef P psite;

//     /// The associated point type.
//     typedef mln_point(P) point;

//     enum { dim = super_::dim };

//     /// Coordinate associated type.
//     template <typename S>
//     p_array_bkd_piter_(const Site_Set<S>& s);

//     /// Reference of the corresponding psite.
//     const psite& to_psite() const;

//     /// Reference of the corresponding point.
//     const point& to_point() const;

//     /// Read-only access to the \p i-th coordinate.
//     mln_coord(point) operator[](unsigned i) const;

//     /// Test if the iterator is valid.
//     bool is_valid() const;

//     /// Invalidate the iterator.
//     void invalidate();

//     /// Start an iteration.
//     void start();

//     /// Go to the next point.
//     void next_();

//     /// Convert the iterator into a psite.
//     operator psite() const;

//   protected:
//     const std::vector<P>& vect_;
//     /* FIXME: See the comment on p_array_fwd_piter_<P>::i_ above.  We
//        could turn this `int' into an `unsigned'.  Then,
//        - setting the value of i_ to -1 (== UINT_MAX) in invalidate(),
//        - and having valid() test whether i_ is strictly smaller than
//          vect_.size()
//        should work in both iterators (fwd and bkd).  */
//     int i_;
//     psite p_;
//   };



# ifndef MLN_INCLUDE_ONLY

  /*------------------------.
  | p_array_fwd_piter_<P>.  |
  `------------------------*/

  template <typename P>
  inline
  p_array_fwd_piter_<P>::p_array_fwd_piter_()
  {
  }

  template <typename P>
  inline
  p_array_fwd_piter_<P>::p_array_fwd_piter_(const p_array<P>& arr)
  {
    change_target(arr);
  }

  template <typename P>
  inline
  void
  p_array_fwd_piter_<P>::change_target(const p_array<P>& arr)
  {
    p_.change_target(arr);
    invalidate();
  }

  template <typename P>
  inline
  bool
  p_array_fwd_piter_<P>::is_valid() const
  {
    return p_.target() != 0 && p_.index() < int(p_.target()->nsites());
  }

  template <typename P>
  inline
  void
  p_array_fwd_piter_<P>::invalidate()
  {
    if (p_.target() != 0)
      p_.index() = p_.target()->nsites();
  }

  template <typename P>
  inline
  void
  p_array_fwd_piter_<P>::start()
  {
    mln_precondition(p_.target() != 0);
    p_.index() = 0;
  }

  template <typename P>
  inline
  void
  p_array_fwd_piter_<P>::next_()
  {
    ++p_.index();
  }

  template <typename P>
  inline
  const p_array_psite<P>&
  p_array_fwd_piter_<P>::unproxy() const
  {
    return p_;
  }

  template <typename P>
  inline
  const typename p_array_fwd_piter_<P>::site&
  p_array_fwd_piter_<P>::to_site() const
  {
    mln_precondition(p_.target() != 0);
    return p_.to_site();
  }

  /*------------------------.
  | p_array_bkd_piter_<P>.  |
  `------------------------*/

//   template <typename P>
//   template <typename S>
//   inline
//   p_array_bkd_piter_<P>::p_array_bkd_piter_(const Site_Set<S>& s)
//     : vect_(exact(s).vect())
//   {
//     invalidate();
//   }

//   template <typename P>
//   inline
//   const P&
//   p_array_bkd_piter_<P>::to_psite() const
//   {
//     return p_;
//   }

//   template <typename P>
//   inline
//   const mln_point(P)&
//   p_array_bkd_piter_<P>::to_point() const
//   {
//     return p_.to_point();
//   }

//   template <typename P>
//   inline
//   mln_coord(mln_point_(P))
//   p_array_bkd_piter_<P>::operator[](unsigned i) const
//   {
//     mln_precondition(i < dim);
//     mln_precondition(is_valid());
//     return p_.to_point()[i];
//   }

//   template <typename P>
//   inline
//   bool
//   p_array_bkd_piter_<P>::is_valid() const
//   {
//     return i_ >= 0;
//   }

//   template <typename P>
//   inline
//   void
//   p_array_bkd_piter_<P>::invalidate()
//   {
//     i_ = -1;
//   }

//   template <typename P>
//   inline
//   void
//   p_array_bkd_piter_<P>::start()
//   {
//     i_ = vect_.size() - 1;
//     if (is_valid())
//       p_ = vect_[i_];
//   }

//   template <typename P>
//   inline
//   void
//   p_array_bkd_piter_<P>::next_()
//   {
//     --i_;
//     if (is_valid())
//       p_ = vect_[i_];
//   }

//   template <typename P>
//   inline
//   p_array_bkd_piter_<P>::operator P() const
//   {
//     mln_precondition(is_valid());
//     return p_;
//   }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_ARRAY_PITER_HH
