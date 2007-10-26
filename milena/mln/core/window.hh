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

#ifndef MLN_CORE_WINDOW_HH
# define MLN_CORE_WINDOW_HH

/*! \file mln/core/window.hh
 *
 * \brief Definition of the generic window class mln::window.
 *
 * \todo Make naming coherent: we have window (without '_') but
 * point_, neighb_, etc.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/concept/point_site.hh>
# include <mln/core/internal/dpoints_base.hh>
# include <mln/core/dpoint.hh>
# include <mln/core/box.hh>

# include <mln/convert/to_dpoint.hh>
# include <mln/geom/sym.hh>
# include <mln/metal/is_a.hh>


namespace mln
{

  // fwd decls
  template <typename D> class dpoints_fwd_piter;
  template <typename D> class dpoints_bkd_piter;
 

  /*! \brief Generic window class.
   *
   * This type of window is just like a set of delta-points.  The
   * parameter is \c D, type of delta-point.
   */
  template <typename D>
  class window : public Window< window<D> >,
		 public internal::dpoints_base_<D, window<D> >
  {
    typedef internal::dpoints_base_<D, window<D> > super_;
  public:

    /*! \brief Point_Iterator type to browse the points of a generic window
     * w.r.t. the ordering of delta-points.
     */
    typedef dpoints_fwd_piter<D> fwd_qiter;

    /*! \brief Point_Iterator type to browse the points of a generic window
     * w.r.t. the reverse ordering of delta-points.
     */
    typedef dpoints_bkd_piter<D> bkd_qiter;


    /*! \brief Constructor without argument.
     *
     * The constructed window is empty. 
     */
    window();


    /*! \brief Test if the window is symmetric.
     */
    bool is_symmetric() const;

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

    /// Apply a central symmetry to the target window.
    window<D>& sym();

  protected:
    
    box_<mln_point(D)> b_;
  };


  // FIXME: Move code at EOF + doc.
  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const window<D>& win)
  {
    // FIXME
    for (unsigned i = 0; i < win.ndpoints(); ++i)
      ostr << win.dp(i);
    return ostr;
  }



# ifndef MLN_INCLUDE_ONLY

  // window<D>

  template <typename D>
  window<D>::window()
  {
    // FIXME HERE: Was: mln::metal::is_a<D, Dpoint>::check();
    mln::metal::is_a<D, Delta_Point_Site>::check();
  }

  template <typename D>
  bool window<D>::is_symmetric() const
  {
    return geom::sym(*this) == *this;
  }

  template <typename D>
  window<D>&
  window<D>::insert(const D& dp)
  {
    mln_precondition(! has(dp));
    this->super_::insert(dp);
    return *this;
  }

  template <typename D>
  window<D>&
  window<D>::insert(const mln_coord(D)& dind)
  {
    D dp(dind);
    mln_precondition(! has(dp));
    return this->insert(dp);
  }

  template <typename D>
  window<D>&
  window<D>::insert(const mln_coord(D)& drow, const mln_coord(D)& dcol)
  {
    D dp(drow, dcol);
    mln_precondition(! has(dp));
    return this->insert(dp);
  }

  template <typename D>
  window<D>&
  window<D>::insert(const mln_coord(D)& dsli, const mln_coord(D)& drow, const mln_coord(D)& dcol)
  {
    D dp(dsli, drow, dcol);
    mln_precondition(! has(dp));
    return this->insert(dp);
  }

  template <typename D>
  window<D>&
  window<D>::sym()
  {
    window<D> tmp;
    const unsigned n = this->ndpoints();
    for (unsigned i = 0; i < n; ++i)
      tmp.insert(- this->dp(i));
    *this = tmp;
    return *this;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpoints_piter.hh>


#endif // ! MLN_CORE_WINDOW_HH
