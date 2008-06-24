// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_NEIGHBORHOOD_HH
# define MLN_CORE_NEIGHBORHOOD_HH

/*! \file mln/core/neighborhood.hh
 *
 * \brief Definition of the generic neighborhood class mln::neighborhood.
 *
 * \todo Make naming coherent: we have neighborhood (without '_') but
 * point_, neighb_, etc.
 *
 * \todo Code other comparisons (< and <=).
 *
 * \todo Add static checks in insert methods.
 */

# include <mln/core/internal/neighborhood_base.hh>
# include <mln/core/window.hh>
# include <mln/literal/zero.hh>


namespace mln
{

  // Fwd decls.
  template <typename V> class dpsites_fwd_piter;
  template <typename V> class dpsites_bkd_piter;


  /*! \brief Generic neighborhood class.
   *
   * This type of neighborhood is just like a set of delta-points.  The
   * parameter is \c D, type of delta-point.
   */
  template <typename D>
  class neighborhood : public internal::neighborhood_base< D, neighborhood<D> >
  {
  public:

    /*! \brief Constructor without argument.
     *
     * The constructed neighborhood is empty. 
     */
    neighborhood();

    /*! \brief Constructor from a window.
     *
     * \pre The window has to be symmetrical and not centered.
     */
    neighborhood(const mln::window<D>& win);

    /*! \brief Site_Iterator type to browse the points of a basic neighborhood
     * w.r.t. the ordering of delta-points.
     */
    typedef dpsites_fwd_piter< neighborhood<D> > fwd_niter;
      
    /*! \brief Site_Iterator type to browse the points of a basic neighborhood
     * w.r.t. the reverse ordering of delta-points.
     */
    typedef dpsites_bkd_piter< neighborhood<D> > bkd_niter;

    /*! \brief Site_Iterator type to browse the points of a basic neighborhood
     * whatever the ordering of delta-points.
     */
    typedef fwd_niter niter;


    /// Give the neighborhood size, i.e., the number of delta-sites.
    unsigned size() const;

    /*! \brief Give the maximum coordinate gap between the neighborhood
      center and a neighborhood point.
    */
    unsigned delta() const;


    /// Insert a delta-point \p dp.
    neighborhood<D>& insert(const D& dp);

    /// \{ Insertion of a delta-point with different numbers of
    /// arguments (coordinates) w.r.t. the dimension.
    neighborhood<D>& insert(const mln_coord(D)& dind); // For 1D.
      
    neighborhood<D>& insert(const mln_coord(D)& drow,
			    const mln_coord(D)& dcol); // For 2D.
      
    neighborhood<D>& insert(const mln_coord(D)& dsli,
			    const mln_coord(D)& drow,
			    const mln_coord(D)& dcol); // For 3D.
    /// \}



    typedef window<D> window;

    mln::window<D> to_window() const
    {
      mln::window<D> tmp = win_;
      D zero = literal::zero;
      tmp.insert(zero);
      return tmp;
    }

    /// Give the vector of delta-points.
    const std::vector<D>& std_vector() const;

    /// Hook to the set of D.
    const util::set<D>& dps_hook_() const;

  private:

    mln::window<D> win_;
  };


  // FIXME: Doc!
  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const neighborhood<D>& nbh);



  /*! \brief Equality comparison between neighborhoods \p lhs and \p rhs.
   *
   * \relates mln::neighborhood<D>
   */
  template <typename D>
  bool operator==(const neighborhood<D>& lhs, const neighborhood<D>& rhs);



# ifndef MLN_INCLUDE_ONLY

  // neighborhood<D>

  template <typename D>
  inline
  neighborhood<D>::neighborhood()
  {
    // FIXME HERE: Was: mln::metal::is_a<D, Dpoint>::check();
    // mln::metal::is_a<D, Delta_Point_Site>::check();
  }

  template <typename D>
  inline
  neighborhood<D>::neighborhood(const mln::window<D>& win)
  {
    mln_precondition(win.is_symmetric() == true);
    mln_precondition(win.is_centered() == false);
    win_ = win;
  }

  template <typename D>
  inline
  unsigned
  neighborhood<D>::delta() const
  {
    mln_precondition(size() != 0);
    return win_.delta();
  }

  template <typename D>
  inline
  unsigned
  neighborhood<D>::size() const
  {
    return win_.size();
  }

  template <typename D>
  inline
  const std::vector<D>&
  neighborhood<D>::std_vector() const
  {
    return win_.std_vector();
  }

  template <typename D>
  inline
  neighborhood<D>&
  neighborhood<D>::insert(const D& dp)
  {
    win_.insert( dp);
    win_.insert(-dp);
    return *this;
  }

  template <typename D>
  inline
  neighborhood<D>&
  neighborhood<D>::insert(const mln_coord(D)& dind)
  {
    mlc_bool(D::dim == 1)::check();
    D dp(dind);
    return insert(dp);
  }

  template <typename D>
  inline
  neighborhood<D>&
  neighborhood<D>::insert(const mln_coord(D)& drow,
			  const mln_coord(D)& dcol)
  {
    mlc_bool(D::dim == 2)::check();
    D dp(drow, dcol);
    return insert(dp);
  }

  template <typename D>
  inline
  neighborhood<D>&
  neighborhood<D>::insert(const mln_coord(D)& dsli,
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
  neighborhood<D>::dps_hook_() const
  {
    return win_.dps_hook_();
  }

  // Operators.

  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const neighborhood<D>& nbh)
  {
    return ostr << nbh.dps_hook_();
  }

  template <typename D>
  bool operator==(const neighborhood<D>& lhs, const neighborhood<D>& rhs)
  {
    return lhs.dps_hook_() == rhs.dps_hook_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_NEIGHBORHOOD_HH
