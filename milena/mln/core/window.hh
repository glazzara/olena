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

#ifndef MLN_CORE_WINDOW_HH
# define MLN_CORE_WINDOW_HH

/*! \file mln/core/window.hh
 *
 * \brief Definition of the generic window class mln::window.
 *
 * \todo Make naming coherent: we have window (without '_') but
 * point_, neighb_, etc.
 *
 * \todo Code other comparisons (< and <=).
 *
 * \todo Add static checks in insert methods.
 */

# include <mln/core/internal/window_base.hh>

# include <mln/metal/is_a.hh>
# include <mln/util/set.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/norm/linfty.hh>


namespace mln
{

  // Fwd decls.
  template <typename V> class dpsites_fwd_piter;
  template <typename V> class dpsites_bkd_piter;


  /*! \brief Generic window class.
   *
   * This type of window is just like a set of delta-points.  The
   * parameter is \c D, type of delta-point.
   */
  template <typename D>
  class window : public internal::window_base< D, window<D> >
  {
  public:

    /*! \brief Constructor without argument.
     *
     * The constructed window is empty. 
     */
    window();

    /*! \brief Test if the window is centered.
     *
     * \return True if the delta-point 0 belongs to the window.
     */
    bool is_centered() const;

    /*! \brief Test if the window is symmetric.
     */
    bool is_symmetric() const;

    /*! Apply a central symmetry to the target window.
     */
    void sym();


    /*! \brief Site_Iterator type to browse the points of a basic window
     * w.r.t. the ordering of delta-points.
     */
    typedef dpsites_fwd_piter< window<D> > fwd_qiter;
      
    /*! \brief Site_Iterator type to browse the points of a basic window
     * w.r.t. the reverse ordering of delta-points.
     */
    typedef dpsites_bkd_piter< window<D> > bkd_qiter;

    /*! \brief Site_Iterator type to browse the points of a basic window
     * whatever the ordering of delta-points.
     */
    typedef fwd_qiter qiter;


    /// Give the window size, i.e., the number of delta-sites.
    unsigned size() const;

    /*! \brief Test if the window is empty (null size; no delta-point).
     */
    bool is_empty() const;

    /*! \brief Give the maximum coordinate gap between the window
      center and a window point.
    */
    unsigned delta() const;

    // Give the \p i-th delta-point.
    const D& dp(unsigned i) const;



    /// Insert a delta-point \p dp.
    window<D>& insert(const D& dp);

    /// \{ Insertion of a delta-point with different numbers of
    /// arguments (coordinates) w.r.t. the dimension.
    window<D>& insert(const mln_coord(D)& dind); // For 1D.
      
    window<D>& insert(const mln_coord(D)& drow,
		      const mln_coord(D)& dcol); // For 2D.
      
    window<D>& insert(const mln_coord(D)& dsli,
		      const mln_coord(D)& drow,
		      const mln_coord(D)& dcol); // For 3D.
    /// \}

    /// Give the vector of delta-points.
    const std::vector<D>& std_vector() const;

    /// Hook to the set of D.
    const util::set<D>& dps_hook_() const;

  private:

    util::set<D> dps_;
  };


  // FIXME: Doc!
  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const window<D>& win);



  /*! \brief Equality comparison between windows \p lhs and \p rhs.
   *
   * \relates mln::window<D>
   */
  template <typename D>
  bool operator==(const window<D>& lhs, const window<D>& rhs);



# ifndef MLN_INCLUDE_ONLY

  // window<D>

  template <typename D>
  inline
  window<D>::window()
  {
    // FIXME HERE: Was: mln::metal::is_a<D, Dpoint>::check();
    // mln::metal::is_a<D, Delta_Point_Site>::check();
  }

  template <typename D>
  inline
  bool
  window<D>::is_symmetric() const
  {
    window<D> cpy = *this;
    return cpy.sym() == *this;
  }

  template <typename D>
  inline
  bool
  window<D>::is_centered() const
  {
    static const D origin = all_to(0);
    return this->dps_.has(origin); // FIXME: Use literal::origin.
  }

  template <typename D>
  inline
  void
  window<D>::sym()
  {
    window<D> tmp;
    const unsigned n = size();
    for (unsigned i = 0; i < n; ++i)
      tmp.insert(- this->dp(i));
    *this = tmp;
  }


  template <typename D>
  inline
  bool
  window<D>::is_empty() const
  {
    return dps_.is_empty();
  }

  template <typename D>
  inline
  unsigned
  window<D>::delta() const
  {
    // FIXME: Is-it correct?
    unsigned d = 0;
    const unsigned n = size();
    for (unsigned i = 0; i < n; ++i)
      {
	unsigned dd = norm::linfty(dp(i).to_vec());
	if (dd > d)
	  d = dd;
      }
    return d;
  }

  template <typename D>
  inline
  unsigned
  window<D>::size() const
  {
    return dps_.nelements();
  }

  template <typename D>
  inline
  const D&
  window<D>::dp(unsigned i) const
  {
    mln_precondition(i < size());
    return dps_[i];
  }

  template <typename D>
  inline
  const std::vector<D>&
  window<D>::std_vector() const
  {
    return dps_.vect();
  }

  template <typename D>
  inline
  window<D>&
  window<D>::insert(const D& dp)
  {
    dps_.insert(dp);
    return *this;
  }

  template <typename D>
  inline
  window<D>&
  window<D>::insert(const mln_coord(D)& dind)
  {
    mlc_bool(D::dim == 1)::check();
    D dp(dind);
    return insert(dp);
  }

  template <typename D>
  inline
  window<D>&
  window<D>::insert(const mln_coord(D)& drow,
		    const mln_coord(D)& dcol)
  {
    mlc_bool(D::dim == 2)::check();
    D dp(drow, dcol);
    return insert(dp);
  }

  template <typename D>
  inline
  window<D>&
  window<D>::insert(const mln_coord(D)& dsli,
		    const mln_coord(D)& drow,
		    const mln_coord(D)& dcol)
  {
    mlc_bool(D::dim == 3)::check();
    D dp(dsli, drow, dcol);
    return insert(dp);
  }

  template <typename D>
  inline
  const util::set<D>&
  window<D>::dps_hook_() const
  {
    return dps_;
  }

  // Operators.

  template <typename D>
  std::ostream&
  operator<<(std::ostream& ostr, const window<D>& win)
  {
    return ostr << win.dps_hook_();
  }

  template <typename D>
  bool
  operator==(const window<D>& lhs, const window<D>& rhs)
  {
    return lhs.dps_hook_() == rhs.dps_hook_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpsites_piter.hh>


#endif // ! MLN_CORE_WINDOW_HH
