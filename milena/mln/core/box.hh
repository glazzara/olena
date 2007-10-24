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

#ifndef MLN_CORE_BOX_HH
# define MLN_CORE_BOX_HH

/*! \file mln/core/box.hh
 *
 * \brief This file defines a generic box class.
 */

# include <mln/core/concept/box.hh>
# include <mln/core/internal/box_impl.hh>
# include <mln/core/point.hh>
# include <mln/literal/origin.hh>


namespace mln
{

  // fwd decl
  template <typename P> struct box_fwd_piter_;
  template <typename P> struct box_bkd_piter_;


  /*! \brief Generic box class.
   *
   * Parameter \c P is the corresponding type of point.
   */
  template <typename P>
  struct box_ : public Box< box_<P> >,
		public internal::box_impl_< P::dim, mln_coord(P), box_<P> >
  {
    /// Dimension.
    enum { dim = P::dim };

    /// Mesh associated type.
    typedef mln_mesh(P) mesh;

    /// Point_Site associated type.
    typedef P psite;

    /// Point associated type.
    typedef P point;

    /// Dpoint associated type.
    typedef mln_dpoint(P) dpoint;

    /// Forward Point_Iterator associated type.
    typedef box_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef box_bkd_piter_<P> bkd_piter;

    /// Minimum point.
    P  pmin() const;

    /// Reference to the minimum point.
    P& pmin();

    /// Maximum point.
    P  pmax() const;

    /// Reference to the maximum point.
    P& pmax();

    /// Constructor without argument.
    box_();

    /// Constructor of a box going from \p pmin to \p pmax.
    box_(const point& pmin, const point& pmax);

    /// \{ Constructors with different numbers of arguments
    /// (sizes) w.r.t. the dimension.
    explicit box_(mln_coord(P) ninds);
    box_(mln_coord(P) nrows, mln_coord(P) ncols);
    box_(mln_coord(P) nslis, mln_coord(P) nrows, mln_coord(P) ncols);
    /// \}

    /*! \brief Test if \p p belongs to the box.
     *
     * \param[in] p A point site.
     */
    bool has(const P& p) const;

    /// Enlarge the box with a border \p b.
    void enlarge(unsigned b);

    /// Give a larger box.
    box_<P> to_larger(unsigned b) const;

  protected:
    P pmin_, pmax_;
  };


  /*! \brief Print a generic box \p b into the output stream \p ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] b A generic box.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::box_
   */
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const box_<P>& b);


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  P
  box_<P>::pmin() const
  {
    return pmin_;
  }

  template <typename P>
  P&
  box_<P>::pmin()
  {
    return pmin_;
  }

  template <typename P>
  P
  box_<P>::pmax() const
  {
    return pmax_;
  }

  template <typename P>
  P&
  box_<P>::pmax()
  {
    return pmax_;
  }

  template <typename P>
  box_<P>::box_()
  {
  }

  template <typename P>
  box_<P>::box_(const point& pmin, const point& pmax)
    : pmin_(pmin),
      pmax_(pmax)
  {

  }

  template <typename P>
  box_<P>::box_(mln_coord(P) ninds)
  {
    metal::bool_<(dim == 1)>::check();
    pmin_ = literal::origin;
    pmax_ = P(ninds - 1);
  }

  template <typename P>
  box_<P>::box_(mln_coord(P) nrows, mln_coord(P) ncols)
  {
    metal::bool_<(dim == 2)>::check();
    pmin_ = literal::origin;
    pmax_ = P(nrows - 1, ncols - 1);
  }

  template <typename P>
  box_<P>::box_(mln_coord(P) nslis, mln_coord(P) nrows, mln_coord(P) ncols)
  {
    metal::bool_<(dim == 3)>::check();
    pmin_ = literal::origin;
    pmax_ = P(nslis - 1, nrows - 1, ncols - 1);
  }

  template <typename P>
  bool
  box_<P>::has(const P& p) const
  {
    for (unsigned i = 0; i < P::dim; ++i)
      if (p[i] < pmin_[i] || p[i] > pmax_[i])
	return false;
    return true;
  }

  template <typename P>
  void
  box_<P>::enlarge(unsigned b)
  {
    for (unsigned i = 0; i < P::dim; ++i)
    {
      pmin_[i] -= b;
      pmax_[i] += b;
    }
  }


  template <typename P>
  box_<P>
  box_<P>::to_larger(unsigned b) const
  {
    box_<P> tmp(*this);

    for (unsigned i = 0; i < P::dim; ++i)
    {
      tmp.pmin_[i] -= b;
      tmp.pmax_[i] += b;
    }
    return tmp;
  }

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const box_<P>& b)
  {
    return ostr << "[" << b.pmin() << ".." << b.pmax() << ']';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/box_piter.hh>


#endif // ! MLN_CORE_BOX_HH
