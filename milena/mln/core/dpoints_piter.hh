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

#ifndef MLN_CORE_DPOINTS_PITER_HH
# define MLN_CORE_DPOINTS_PITER_HH

/*! \file mln/core/dpoints_piter.hh
 *
 * \brief Definition of mln::dpoints_fwd_piter and mln::dpoints_bkd_piter.
 */

# include <mln/core/concept/point_iterator.hh>
# include <mln/core/concept/generalized_point.hh>


namespace mln
{

  /*! \brief A generic forward iterator on points of windows and of
   *  neighborhoods.
   *
   * The parameter \c D is the type of delta-points.
   */
  template <typename D>
  class dpoints_fwd_piter : public Point_Iterator< dpoints_fwd_piter<D> >
  {
  public:

    /// Space dimension.
    enum { dim = D::dim };

    /// Dpoint associated type.
    typedef D           dpoint;

    /// Point associated type.
    typedef mln_point(D) point;

    /// Point_Site associated type.
    typedef point        psite;

    /// Coordinate associated type.
    typedef mln_coord(D) coord;

    /*! \brief Constructor.
     *
     * \param[in] dps   Object that can provide an array of delta-points.
     * \param[in] p_ref Center point to iterate around.
     */
    template <typename Dps, typename Pref>
    dpoints_fwd_piter(const Dps& dps, // FIXME: explicitly set_of_<D>?
		      const Generalized_Point<Pref>& p_ref);

    /// Convertion to point.
    operator mln_point(D) () const;

    /// Address of the point this iterator designates.
    const point* pointer_() const;

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Give the i-th coordinate.
    coord operator[](unsigned i) const;

    /// The point around which this iterator moves.
    const point& center_point() const;

    /// Force this iterator to update its location to take into
    /// account that its center point may have moved. 
    void update();

  private:
    const std::vector<D>& dps_;
    const point& p_ref_; // reference point (or "center point")

    unsigned i_;
    point p_; // location of this iterator; p_ makes this iterator be
	      // itself a potential center point (Cf. the pointer_() method).
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  template <typename Dps, typename Pref>
  dpoints_fwd_piter<D>::dpoints_fwd_piter(const Dps& dps,
					  const Generalized_Point<Pref>& p_ref)
    : dps_(exact(dps).vec()),
      p_ref_(* internal::force_exact<Pref>(p_ref).pointer_())
  {
    invalidate();
  }

  template <typename D>
  dpoints_fwd_piter<D>::operator mln_point(D) () const
  {
    mln_precondition(is_valid());
    return p_;
  }

  template <typename D>
  const mln_point(D)*
  dpoints_fwd_piter<D>::pointer_() const
  {
    return & p_;
  }

  template <typename D>
  bool
  dpoints_fwd_piter<D>::is_valid() const
  {
    return i_ != dps_.size();
  }

  template <typename D>
  void
  dpoints_fwd_piter<D>::invalidate()
  {
    i_ = dps_.size();
  }

  template <typename D>
  void
  dpoints_fwd_piter<D>::start()
  {
    i_ = 0;
    update();
  }

  template <typename D>
  void
  dpoints_fwd_piter<D>::next_()
  {
    ++i_;
    update();
  }

  template <typename D>
  const mln_point(D)&
  dpoints_fwd_piter<D>::center_point() const
  {
    return p_ref_;
  }

  template <typename D>
  void
  dpoints_fwd_piter<D>::update()
  {
    if (is_valid())
      p_ = p_ref_ + dps_[i_];
  }

  template <typename D>
  mln_coord(D)
  dpoints_fwd_piter<D>::operator[](unsigned i) const
  {
    mln_precondition(is_valid());

    // below we test that no update is required
    // meaning that p_ref_ has not moved or that
    // the user has explicitly called update()
    mln_precondition(p_ref_ + dps_[i_] == p_);
    // FIXME: Explain this issue in the class documentation...

    return p_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_DPOINTS_PITER_HH
