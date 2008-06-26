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

#ifndef MLN_CORE_P_SET_HH
# define MLN_CORE_P_SET_HH

/// \file mln/core/p_set.hh
/// \brief Definition of a point set class based on std::set, that can
/// behave also as a vector.

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/accu/bbox.hh>
# include <mln/core/p_array.hh>


namespace mln
{

  /// \brief Point set class based on std::set.
  ///
  /// This is a mathematical set of points (not a multi-set).  The
  /// parameter \p P shall be a Point type or a Point_Site.
  ///
  /// FIXME: If \p is a Point_Site, but not a Point, the method
  /// mln::p_set<P>::bbox will not compile (see below).  (Will be fixed
  /// with the merge with branch cleanup-2008.)
  template <typename P>
  class p_set : public internal::point_set_base_< P, p_set<P> >,
		private internal::set_of_<P>
  {
    typedef internal::set_of_<P> super_;

  public:
    /* FIXME: Should be removed as soon as branch cleanup-2008 is
       merged with the trunk.  */
    typedef mln_point(P) point;

    /* FIXME: mln::p_set uses the same iterators as mln::p_array,
       because mln::p_set also have the interface of an array.  We
       might want to keep this, but we should change the name of the
       iterator, either by using an alias (typedef), of by giving them
       their own type (which could share code with
       mln::p_array_{fwd,bkd}_piter_, of course).  */

    /// Forward Point_Iterator associated type.
    typedef p_array_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef p_array_bkd_piter_<P> bkd_piter;

    /// Constructor.
    p_set();

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Return the corresponding std::vector of points.
    using super_::vect;

    /// Give the number of points.
    std::size_t npoints() const;

    /// Insert a point \p p.
    p_set<P>& insert(const P& p);

    /// Remove a point \p p.
    p_set<P>& remove(const P& p);

    /// Return the \p i-th point.
    const P& operator[](unsigned i) const;

    /// Clear this set.
    void clear();

    /* FIXME: Won't work if P is not (implicitly) convertible to
       mln_point(P).  */
    /// Give the exact bounding box.
    const box_<point>& bbox() const;

  protected:
    mutable accu::bbox<point> bb_;
    mutable bool bb_needs_update_;

    void update_bb_() const;
    // FIXME: Add invariant  bb_.is_valid() <=> npoints() != 0
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_set<P>::p_set() 
    : bb_needs_update_(false)
  {
  }

  template <typename P>
  inline
  void
  p_set<P>::update_bb_() const
  {
    bb_.init();
    for (typename std::set<P>::const_iterator i = this->s_.begin();
	 i != this->s_.end(); ++i)
      bb_.take(*i);
    bb_needs_update_ = false;
  }

  template <typename P>
  inline
  bool
  p_set<P>::has(const P& p) const
  {
    return this->super_::has(p);
  }

  template <typename P>
  inline
  std::size_t
  p_set<P>::npoints() const
  {
    return this->super_::nelements();
  }

  template <typename P>
  inline
  p_set<P>&
  p_set<P>::insert(const P& p)
  {
    this->super_::insert(p);
    if (! bb_needs_update_)
      bb_needs_update_ = true;
    return *this;
  }

  template <typename P>
  inline
  p_set<P>&
  p_set<P>::remove(const P& p)
  {
    this->super_::remove(p);
    if (! bb_needs_update_)
      bb_needs_update_ = true;
    return *this;
  }

  template <typename P>
  inline
  const P&
  p_set<P>::operator[](unsigned i) const
  {
    mln_precondition(i < npoints());
    return this->super_::element(i);
  }

  template <typename P>
  inline
  void
  p_set<P>::clear()
  {
    this->super_::clear();
    bb_.init();
    bb_needs_update_ = false;
  }

  template <typename P>
  inline
  const box_<mln_point(P)>&
  p_set<P>::bbox() const
  {
    mln_precondition(npoints() != 0);
    if (bb_needs_update_)
      update_bb_();
    return bb_.to_result();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_SET_HH
