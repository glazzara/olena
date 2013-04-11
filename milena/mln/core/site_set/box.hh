// Copyright (C) 2007, 2008, 2009, 2010, 2011, 2013 EPITA Research and
// Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_SITE_SET_BOX_HH
# define MLN_CORE_SITE_SET_BOX_HH

/// \file
///
/// This file defines a generic box class.
///
/// \todo Test if the safety code in box::box() is not too slow.

# include <mln/core/concept/box.hh>
# include <mln/core/internal/box_impl.hh>
# include <mln/core/point.hh>
# include <mln/literal/origin.hh>


namespace mln
{

  // Fwd decls.
  template <typename P> class box;
  template <typename P> class box_fwd_piter_;
  template <typename P> class box_bkd_piter_;


  namespace trait
  {

    template <typename P>
    struct site_set_< box<P> >
    {
      typedef trait::site_set::nsites::known   nsites;
      typedef trait::site_set::bbox::straight  bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };

    template <typename P>
    struct set_precise_unary_< op::ord, box<P> >
    {
      typedef set_precise_unary_< op::ord, box<P> > ret; // Itself.
      bool strict(const box<P>& lhs, const box<P>& rhs) const;
    };

  } // end of namespace mln::trait


  /// \brief Generic box class: site set containing points of a
  /// regular grid.
  ///
  /// Parameter \c P is the corresponding type of point.
  ///
  /// \ingroup modsitesetbasic
  //
  template <typename P>
  class box : public Box< box<P> >,
              public internal::box_impl_< P::dim, mln_coord(P), box<P> >,
              private mlc_is_unqualif(P)::check_t
  {
  public:
    /// Dimension.
    enum { dim = P::dim };

    /// Element associated type.
    typedef P element;

    /// Psite associated type.
    typedef P psite;

    /// Site associated type.
    typedef P site;

    /// Forward Site_Iterator associated type.
    typedef box_fwd_piter_<P> fwd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;

    /// Backward Site_Iterator associated type.
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
    box();

    /// Constructor of a box going from \p pmin to \p pmax.
    box(const site& pmin, const site& pmax);

    /// \{ Constructors with different numbers of arguments
    /// (sizes) w.r.t. the dimension.
    explicit box(mln_coord(P) ninds);
    box(mln_coord(P) nrows, mln_coord(P) ncols);
    box(mln_coord(P) nslis, mln_coord(P) nrows, mln_coord(P) ncols);
    /// \}

    /*! \brief Test if \p p belongs to the box.
     *
     * \param[in] p A point site.
     */
    bool has(const P& p) const;

    /// Enlarge the box with a border \p b.
    void enlarge(unsigned b);

    /// Enlarge the box with a border \p b for dimension \p dim.
    void enlarge(unsigned dim, unsigned b);

    /// Give a larger box.
    box<P> to_larger(unsigned b) const;

    /// Return the approximated central site of this box.
    P pcenter() const;

    /// Test that the box owns valid data, i.e., is initialized and
    /// with pmin being 'less-than' pmax.
    bool is_valid() const;

    /// Crop this bbox in order to fit in the reference box \p b.
    void crop_wrt(const box<P>& b);

    /// Merge inplace with another box.
    void merge(const box<P>& b);

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

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
   * \relates mln::box<P>
   */
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const box<P>& b);


  /// Return the minimum box including box \p a and box \p b
  /// \relates mln::box<P>
  template <typename P>
  inline
  box<P>
  larger_than(const box<P> a, const box<P> b);



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  bool
  box<P>::is_valid() const
  {
    // Validity is: for all i, pmin_[i] <= pmax_[i].
    // Nota bene: a one-point box is valid.
    return util::ord_weak(pmin_, pmax_);
  }

  template <typename P>
  inline
  void
  box<P>::crop_wrt(const box<P>& ref)
  {
    if (pmin_.col() < ref.pmin().col())
      pmin_.col() = ref.pmin().col();
    if (pmin_.row() < ref.pmin().row())
      pmin_.row() = ref.pmin().row();

    if (pmax_.col() > ref.pmax().col())
      pmax_.col() = ref.pmax().col();
    if (pmax_.row() > ref.pmax().row())
      pmax_.row() = ref.pmax().row();
  }

  template <typename P>
  inline
  void
  box<P>::merge(const box<P>& b)
  {
    mln_precondition(is_valid());
    if (! b.is_valid())
    {
      // no-op
      return;
    }

    for (unsigned i = 0; i < P::dim; ++i)
    {
      if (b.pmin()[i] < pmin_[i])
	pmin_[i] = b.pmin()[i];
      if (b.pmax()[i] > pmax_[i])
	pmax_[i] = b.pmax()[i];
    }
  }

  template <typename P>
  inline
  P
  box<P>::pmin() const
  {
    mln_precondition(is_valid());
    return pmin_;
  }

  template <typename P>
  inline
  P&
  box<P>::pmin()
  {
    return pmin_;
  }

  template <typename P>
  inline
  P
  box<P>::pmax() const
  {
    mln_precondition(is_valid());
    return pmax_;
  }

  template <typename P>
  inline
  P&
  box<P>::pmax()
  {
    return pmax_;
  }

  template <typename P>
  inline
  box<P>::box()
    : pmin_(P::plus_infty()),
      pmax_(P::minus_infty())
  {
    // FIXME: The code above can be slow; think about removing it...
  }

  template <typename P>
  inline
  box<P>::box(const site& pmin, const site& pmax)
    : pmin_(pmin),
      pmax_(pmax)
  {
    mln_precondition(is_valid());
  }

  template <typename P>
  inline
  box<P>::box(mln_coord(P) ninds)
  {
    metal::bool_<(dim == 1)>::check();
    pmin_ = literal::origin;
    pmax_ = P(ninds - 1);
  }

  template <typename P>
  inline
  box<P>::box(mln_coord(P) nrows, mln_coord(P) ncols)
  {
    metal::bool_<(dim == 2)>::check();
    mln_precondition(nrows != 0 && ncols != 0);

    pmin_ = literal::origin;
    pmax_ = P(--nrows, --ncols);
    mln_postcondition(is_valid());
  }

  template <typename P>
  inline
  box<P>::box(mln_coord(P) nslis, mln_coord(P) nrows, mln_coord(P) ncols)
  {
    metal::bool_<(dim == 3)>::check();
    pmin_ = literal::origin;
    pmax_ = P(nslis - 1, nrows - 1, ncols - 1);
    mln_postcondition(is_valid());
  }

  template <typename P>
  inline
  bool
  box<P>::has(const P& p) const
  {
    mln_precondition(is_valid());
    for (unsigned i = 0; i < P::dim; ++i)
      if (p[i] < pmin_[i] || p[i] > pmax_[i])
	return false;
    return true;
  }

  template <typename P>
  inline
  void
  box<P>::enlarge(unsigned b)
  {
    mln_precondition(is_valid());
    for (unsigned i = 0; i < P::dim; ++i)
    {
      pmin_[i] = static_cast<mln_coord(P)>(pmin_[i] - b);
      pmax_[i] = static_cast<mln_coord(P)>(pmax_[i] + b);
    }
    mln_postcondition(is_valid());
  }

  template <typename P>
  inline
  void
  box<P>::enlarge(unsigned dim, unsigned b)
  {
    mln_precondition(is_valid());
    pmin_[dim] = static_cast<mln_coord(P)>(pmin_[dim] - b);
    pmax_[dim] = static_cast<mln_coord(P)>(pmax_[dim] + b);
    mln_postcondition(is_valid());
  }

  template <typename P>
  inline
  box<P>
  larger_than(const box<P> a, const box<P> b)
  {
    P pmin,pmax;

    for (unsigned i = 0; i < P::dim; i++)
      {
        pmin[i] = (a.pmin()[i] < b.pmin()[i]) ? a.pmin()[i] : b.pmin()[i];
        pmax[i] = (a.pmax()[i] > b.pmax()[i]) ? a.pmax()[i] : b.pmax()[i];
      }

    return box<P>(pmin, pmax);
  }

  template <typename P>
  inline
  box<P>
  box<P>::to_larger(unsigned b) const
  {
    mln_precondition(is_valid());
    box<P> tmp(*this);

    for (unsigned i = 0; i < P::dim; ++i)
    {
      tmp.pmin_[i] = static_cast<mln_coord(P)>(tmp.pmin_[i] - b);
      tmp.pmax_[i] = static_cast<mln_coord(P)>(tmp.pmax_[i] + b);
    }
    mln_postcondition(tmp.is_valid());
    return tmp;
  }

  template <typename P>
  inline
  P
  box<P>::pcenter() const
  {
    mln_precondition(is_valid());
    P center;
    for (unsigned i = 0; i < P::dim; ++i)
      center[i] = static_cast<mln_coord(P)>(pmin_[i] + ((pmax_[i] - pmin_[i]) / 2));
    return center;
  }

  template <typename P>
  inline
  std::size_t
  box<P>::memory_size() const
  {
    return sizeof(*this);
  }

  template <typename P>
  inline
  std::ostream& operator<<(std::ostream& ostr, const box<P>& b)
  {
    mln_precondition(b.is_valid());
    return ostr << "[" << b.pmin() << ".." << b.pmax() << ']';
  }

  namespace trait
  {

    template <typename P>
    inline
    bool
    set_precise_unary_< op::ord, box<P> >::strict(const box<P>& lhs, const box<P>& rhs) const
    {
      // Lexicographical over "pmin then pmax".
      return util::ord_lexi_strict(lhs.pmin(), lhs.pmax(),
				   rhs.pmin(), rhs.pmax());
    }

  } // end of namespace mln::trait

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/site_set/box_piter.hh>


#endif // ! MLN_CORE_SITE_SET_BOX_HH
