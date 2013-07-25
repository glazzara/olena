// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_LINE2D_HH
# define MLN_CORE_SITE_SET_P_LINE2D_HH

/// \file
///
/// Definition of a 2D discrete line of points (based on
/// p_array).

# include <mln/core/site_set/p_array.hh>
# include <mln/core/alias/box2d.hh>

# include <mln/math/sign.hh>
# include <mln/math/abs.hh>
# include <mln/math/min.hh>
# include <mln/math/max.hh>


namespace mln
{

  // Forward declaration.
  class p_line2d;



  namespace trait
  {

    template <>
    struct site_set_< p_line2d >
    {
      typedef trait::site_set::nsites::known   nsites;
      typedef trait::site_set::bbox::straight  bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };

    template <>
    struct set_precise_unary_< op::ord, p_line2d >
    {
      typedef set_precise_unary_< op::ord, p_line2d > ret; // Itself.
      bool strict(const p_line2d& lhs, const p_line2d& rhs) const;
    };

  } // end of namespace trait



  /// \brief 2D discrete line of points.
  /// It is based on p_array.
  ///
  /// \ingroup modsitesetbasic
  //
  class p_line2d : public internal::site_set_base_< point2d, p_line2d >
  {
    typedef p_line2d self_;
  public:

    /// Element associated type.
    typedef point2d element;

    /// Psite associated type.
    typedef p_indexed_psite<self_> psite;

    /// Site_Iterator associated type.
    typedef p_indexed_fwd_piter<self_> piter;

    /// Forward Site_Iterator associated type.
    typedef p_indexed_fwd_piter<self_> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_indexed_bkd_piter<self_> bkd_piter;


    /// Constructor without argument.
    p_line2d();

    /// Constructor from point \p beg to point \p end.
    p_line2d(const point2d& beg, const point2d& end,
	     bool is_end_excluded = false);


    /// Test if \p p belongs to this point set.
    bool has(const psite& p) const;

    /// Test if index \p i belongs to this point set.
    bool has(const util::index& i) const;

    /// Test if this line is valid, i.e., initialized.
    bool is_valid() const;


    /// Give the number of points.
    unsigned nsites() const;

    /// Give the point that begins the line.
    const point2d& begin() const;

    /// Give the point that ends the line.
    const point2d& end() const;

    /// Return the \p i-th point of the line.
    const point2d& operator[](unsigned i) const;


    /// Box (qualified) associated type.
    typedef const box2d& q_box;

    /// Give the exact bounding box.
    const box2d& bbox() const;


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Return the corresponding std::vector of points.
    const std::vector<point2d>& std_vector() const;

    // Return the corresponding algebra::vec.
    algebra::vec<2, float> to_vec() const;

  protected:

    p_array<point2d> arr_;
    box2d bb_;

    void compute_(const point2d& beg, const point2d& end,
		  bool is_end_excluded);
  };



# ifndef MLN_INCLUDE_ONLY

  inline
  p_line2d::p_line2d()
  {
    mln_postcondition(! is_valid());
  }

  inline
  p_line2d::p_line2d(const point2d& beg, const point2d& end,
		 bool is_end_excluded)
  {
    if (is_end_excluded)
      mln_precondition(end != beg);
    compute_(beg, end, is_end_excluded);
    mln_postcondition(is_valid());
  }

  inline
  void
  p_line2d::compute_(const point2d& beg, const point2d& end,
		   bool is_end_excluded)
  {
    if (is_end_excluded)
      mln_precondition(end != beg);

    // Compute arr_.
    dpoint2d dp = end - beg;
    int
      srow = math::sign(dp.row()), drow = math::abs(dp.row()), ddrow = 2 * drow,
      scol = math::sign(dp.col()), dcol = math::abs(dp.col()), ddcol = 2 * dcol,
      row = beg.row(),
      col = beg.col();
    if ( dcol > drow )
      {
	int e = ddrow - dcol;
	for (int i = 0; i < dcol; ++i)
	  {
	    arr_.append(point2d(static_cast<def::coord>(row),
				static_cast<def::coord>(col)));
	    while (e >= 0)
	      {
		row += srow;
		e -= ddcol;
	      }
	    col += scol;
	    e += ddrow;
	  }
      }
    else
      {
	int e = ddcol - drow;
	for (int i = 0; i < drow; ++i)
	  {
	    arr_.append(point2d(static_cast<def::coord>(row),
				static_cast<def::coord>(col)));
	    while (e >= 0)
	      {
		col += scol;
		e -= ddrow;
	      }
	    row += srow;
	    e += ddcol;
	  }
      }
    if (! is_end_excluded)
      arr_.append(point2d(static_cast<def::coord>(row),
			  static_cast<def::coord>(col)));

    // Compute bb_.
    point2d end_ = arr_[arr_.nsites() - 1];
    bb_.pmin() = point2d(math::min(beg.row(), end_.row()),
			 math::min(beg.col(), end_.col()));
    bb_.pmax() = point2d(math::max(beg.row(), end_.row()),
			 math::max(beg.col(), end_.col()));

    mln_postcondition(this->begin() == beg);
    mln_postcondition(is_end_excluded == (this->end() != end));
  }

  inline
  bool
  p_line2d::has(const psite& p) const
  {
    if (! has(p.index()))
      return false;
    mln_invariant(p.to_site() == (*this)[p.index()]);
    return true;
  }

  inline
  bool
  p_line2d::has(const util::index& i) const
  {
    return i >= 0 && unsigned(i) < nsites();
  }

  inline
  bool
  p_line2d::is_valid() const
  {
    mln_invariant(implies(bb_.is_valid(), ! arr_.is_empty()));
    return bb_.is_valid();
  }

  inline
  unsigned
  p_line2d::nsites() const
  {
    return arr_.nsites();
  }

  inline
  const box2d&
  p_line2d::bbox() const
  {
    mln_precondition(is_valid());
    return bb_;
  }

  inline
  const std::vector<point2d>&
  p_line2d::std_vector() const
  {
    return arr_.std_vector();
  }

  // FIXME: make it more generic?
  inline
  algebra::vec<2, float>
  p_line2d::to_vec() const
  {
    algebra::vec<2, float> res;
    res[0] = float(end().row() - begin().row());
    res[1] = float(end().col() - begin().col());
    return res;
  }

  inline
  const point2d&
  p_line2d::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());
    return arr_[i];
  }

  inline
  const point2d&
  p_line2d::begin() const
  {
    mln_precondition(is_valid());
    return arr_[0];
  }

  inline
  const point2d&
  p_line2d::end() const
  {
    mln_precondition(is_valid());
    return arr_[nsites() - 1];
  }

  inline
  std::size_t
  p_line2d::memory_size() const
  {
    return arr_.memory_size() + sizeof(box2d);
  }


  namespace trait
  {

    inline
    bool
    set_precise_unary_< op::ord, p_line2d >::strict(const p_line2d& lhs,
						    const p_line2d& rhs) const
    {
      return util::ord_lexi_strict(lhs.begin(), lhs.end(),
				   rhs.begin(), rhs.end());
    }

  } // end of namespace mln::trait

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_LINE2D_HH
