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

#ifndef MLN_CORE_P_ARRAY_HH
# define MLN_CORE_P_ARRAY_HH

/*! \file mln/core/p_array.hh
 *
 * \brief Definition of a point set class based on std::vector.
 */

# include <vector>

# include <mln/core/internal/point_set_base.hh>
# include <mln/accu/bbox.hh>
# include <trait/point_set.hh>


namespace mln
{

  // Fwd decls.
  template <typename P> struct p_array;
  template <typename P> struct p_array_fwd_piter_;
  template <typename P> struct p_array_bkd_piter_;

  namespace trait
  {

    template <typename P>
    struct point_set_< p_array<P> > : public default_point_set_< p_array<P> >
    {
      typedef trait::point_set::arity::multiple arity;
      typedef trait::point_set::has_speed::fast has_speed;
    };

  }

  /*! \brief Point set class based on std::vector.
   *
   * This is a multi-set of points.
   *
   * \warning We have some troubles with point set comparison based on
   * a call to npoints().  FIXME: Explain!
   *
   * \todo Make it work with P being a Point_Site.
   */
  template <typename P>
  class p_array : public internal::point_set_base_< P, p_array<P> >
  {
  public:

    /// Forward Site_Iterator associated type.
    typedef p_array_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_array_bkd_piter_<P> bkd_piter;

    /// Constructor.
    p_array();

    /// Constructor from a vector \p vect.
    p_array(const std::vector<P>& vect);

    /// Reserve \p n cells.
    void reserve(std::size_t n);

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Give the number of points.
    std::size_t npoints() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    /// Append a point \p p.
    p_array<P>& append(const P& p);

    /// Clear this set.
    void clear();

    /// Return the corresponding std::vector of points.
    const std::vector<P>& vect() const;

    /// Return the \p i-th point.
    const P& operator[](unsigned i) const;

    /// Hook to data.
    std::vector<P>& hook_();

  protected:

    std::vector<P> vect_;
    mutable accu::bbox<P> bb_;
    mutable bool bb_needs_update_;

    void update_bb_() const;
    // FIXME: Add invariant  bb_.is_valid() <=> npoints() != 0
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_array<P>::p_array()
  {
    bb_needs_update_ = false;
  }

  template <typename P>
  inline
  p_array<P>::p_array(const std::vector<P>& vect)
    : vect_(vect)
  {
    bb_needs_update_ = true;
  }

  template <typename P>
  inline
  void
  p_array<P>::reserve(std::size_t n)
  {
    vect_.reserve(n);
  }

  template <typename P>
  inline
  std::vector<P>&
  p_array<P>::hook_()
  {
    return vect_;
  }

  template <typename P>
  inline
  void
  p_array<P>::update_bb_() const
  {
    bb_.init();
    for (unsigned i = 0; i < vect_.size(); ++i)
      bb_.take(vect_[i]);
    bb_needs_update_ = false;
  }

  template <typename P>
  inline
  bool
  p_array<P>::has(const P& p) const
  {
    for (unsigned i = 0; i < vect_.size(); ++i)
      if (vect_[i] == p)
	return true;
    return false;
  }

  template <typename P>
  inline
  std::size_t
  p_array<P>::npoints() const
  {
    return vect_.size();
  }

  template <typename P>
  inline
  const box_<P>&
  p_array<P>::bbox() const
  {
    mln_precondition(npoints() != 0);
    if (bb_needs_update_)
      update_bb_();
    return bb_.to_result();
  }

  template <typename P>
  inline
  p_array<P>&
  p_array<P>::append(const P& p)
  {
    vect_.push_back(p);
    if (! bb_needs_update_)
      bb_needs_update_ = true;
    return *this;
  }

  template <typename P>
  inline
  void
  p_array<P>::clear()
  {
    vect_.clear();
    bb_needs_update_ = false;
  }

  template <typename P>
  inline
  const std::vector<P>&
  p_array<P>::vect() const
  {
    return vect_;
  }

  template <typename P>
  inline
  const P&
  p_array<P>::operator[](unsigned i) const
  {
    mln_precondition(i < npoints());
    return vect_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/p_array_piter.hh>


#endif // ! MLN_CORE_P_ARRAY_HH
