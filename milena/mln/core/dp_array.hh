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

#ifndef MLN_CORE_DP_ARRAY_HH
# define MLN_CORE_DP_ARRAY_HH

/*! \file mln/core/dp_array.hh
 *
 * \brief Definition of a point set class based on std::vector.
 */

# include <vector>

# include <mln/core/internal/site_set_base.hh>
# include <mln/accu/bbox.hh>


namespace mln
{

  // Fwd decls.
  template <typename D> struct dp_array_fwd_piter_;
  template <typename D> struct dp_array_bkd_piter_;


  /*! \brief Point set class based on std::vector.
   *
   * This is a multi-set of points.
   *
   * \warning We have some troubles with point set comparison based on
   * a call to npoints().  FIXME: Explain!
   *
   * \todo Make it work with P being a Point_Site.
   */
  template <typename D>
  class dp_array : public internal::dpoints_base_< D, dp_array<D> >
  {
  public:

    /// Dpoint associated type.
    typedef D dpoint;

    /// Point associated type.
    typedef mln_point(D) point;

    /*! \brief Site_Iterator type to browse the points of a generic
     * neighborhood w.r.t. the ordering of delta-points.
     */
    typedef dpoints_fwd_piter<D> fwd_niter;

    /*! \brief Site_Iterator type to browse the points of a generic
     * neighborhood w.r.t. the reverse ordering of delta-points.
     */
    typedef dpoints_bkd_piter<D> bkd_niter;

    /// Constructor.
    dp_array();

    /// Constructor from a vector \p vect.
    dp_array(const std::vector<D>& vect);

    /// Reserve \p n cells.
    void reserve(std::size_t n);

    /// Test is \p p belongs to this point set.
    bool has(const D& dp) const;

    /// Give the number of dpoints.
    std::size_t ndpoints() const;

    /// Give the exact bounding box.
    const box_<point>& bbox() const;

    /// Append a point \p p.
    dp_array<D>& append(const D& dp);

    /// Append an array \p other of points.
    dp_array<D>& append(const dp_array<D>& other);

    /// Clear this set.
    void clear();

    /// Return the corresponding std::vector of points.
    const std::vector<D>& vect() const;

    /// Return the \p i-th point.
    const D& operator[](unsigned i) const;

    /// Hook to data.
    std::vector<D>& hook_();

  protected:

    std::vector<D> vect_;
    mutable accu::bbox<dpoint> bb_;
    mutable bool bb_needs_update_;

    void update_bb_() const;
    // FIXME: Add invariant  bb_.is_valid() <=> npoints() != 0
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  inline
  dp_array<D>::dp_array()
  {
    bb_needs_update_ = false;
  }

  template <typename D>
  inline
  dp_array<D>::dp_array(const std::vector<D>& vect)
    : vect_(vect)
  {
    bb_needs_update_ = true;
  }

  template <typename D>
  inline
  void
  dp_array<D>::reserve(std::size_t n)
  {
    vect_.reserve(n);
  }

  template <typename D>
  inline
  std::vector<D>&
  dp_array<D>::hook_()
  {
    return vect_;
  }

  template <typename D>
  inline
  void
  dp_array<D>::update_bb_() const
  {
    bb_.init();
    for (unsigned i = 0; i < vect_.size(); ++i)
      bb_.take(vect_[i]);
    bb_needs_update_ = false;
  }

  template <typename D>
  inline
  bool
  dp_array<D>::has(const D& dp) const
  {
    for (unsigned i = 0; i < vect_.size(); ++i)
      if (vect_[i] == dp)
	return true;
    return false;
  }

  template <typename D>
  inline
  std::size_t
  dp_array<D>::ndpoints() const
  {
    return vect_.size();
  }

  template <typename D>
  inline
  const box_<mln_point(D)>&
  dp_array<D>::bbox() const
  {
    mln_precondition(ndpoints() != 0);
    if (bb_needs_update_)
      update_bb_();
    return bb_.to_result();
  }

  template <typename D>
  inline
  dp_array<D>&
  dp_array<D>::append(const D& dp)
  {
    vect_.push_back(dp);
    if (! bb_needs_update_)
      bb_needs_update_ = true;
    return *this;
  }

  template <typename D>
  inline
  dp_array<D>&
  dp_array<D>::append(const dp_array<D>& other)
  {
    vect_.insert(vect_.end(),
		 other.vect().begin(), other.vect().end());
    if (! bb_needs_update_)
      bb_needs_update_ = true;
    return *this;
  }

  template <typename D>
  inline
  void
  dp_array<D>::clear()
  {
    bb_.init();
    vect_.clear();
    bb_needs_update_ = false;
  }

  template <typename D>
  inline
  const std::vector<D>&
  dp_array<D>::vect() const
  {
    return vect_;
  }

  template <typename D>
  inline
  const D&
  dp_array<D>::operator[](unsigned i) const
  {
    mln_precondition(i < ndpoints());
    return vect_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_DP_ARRAY_HH
