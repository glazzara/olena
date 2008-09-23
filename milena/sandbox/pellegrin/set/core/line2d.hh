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

#ifndef MLN_CORE_SITE_SET_LINE2D_HH
# define MLN_CORE_SITE_SET_LINE2D_HH

/*! \file mln/core/site_set/p_line2d.hh
 *
 * \brief Definition of a point set class based on std::vector.
 */

# include <vector>

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/p_array_piter.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/math/all.hh>
# include <trait/point_set.hh>


namespace mln
{

  // Fwd decl.
  template <typename P> struct line2d;

  namespace trait
  {

    template <>
    struct point_set_<line2d> : public default_point_set_<line2d>
    {
      typedef trait::point_set::arity::unique   arity;
      typedef trait::point_set::has_speed::fast has_speed;
    };

  }

  /*! \brief 2D line point set class.
   */
  class line2d : public internal::point_set_base_< point2d, line2d >
  {
  public:

    /// Forward Site_Iterator associated type.
    typedef p_array_fwd_piter_<point2d> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_array_bkd_piter_<point2d> bkd_piter;


    /// Constructor from point \p beg to point \p end.
    line2d(const point2d& beg, const point2d& end);


    /// Test is \p p belongs to this point set.
    bool has(const point2d& p) const;

    /// Give the number of points.
    std::size_t nsites() const;

    /// Give the exact bounding box.
    const box_<point2d>& bbox() const;

    /// Return the corresponding std::vector of points.
    const std::vector<point2d>& vect() const;

    /// Return the \p i-th point.
    const point2d& operator[](unsigned i) const;

  protected:

    point2d beg_, end_;
    std::vector<point2d> vect_;
    box2d bb_;

    void compute_();
  };



# ifndef MLN_INCLUDE_ONLY

  inline
  line2d::line2d(const point2d& beg, const point2d& end)
    : beg_(beg),
      end_(end)
  {
    compute_();
  }

  inline
  void
  line2d::compute_()
  {
    // vect_
    dpoint2d dp = end_ - beg_;
    int
      srow = math::sign(dp.row()), drow = math::abs(dp.row()), ddrow = 2 * drow,
      scol = math::sign(dp.col()), dcol = math::abs(dp.col()), ddcol = 2 * dcol,
      row = beg_.row(),
      col = beg_.col();
    if ( dcol > drow )
      {
	int e = ddrow - dcol;
	for (int i = 0; i < dcol; ++i)
	  {
	    vect_.push_back(point2d(row, col));
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
	    vect_.push_back(point2d(row, col));
	    while (e >= 0)
	      {
		col += scol;
		e -= ddrow;
	      }
	    row += srow;
	    e += ddcol;
	  }
      }
    vect_.push_back(point2d(row, col));
    // bb_
    bb_.pmin() = point2d(math::min(beg_.row(), end_.row()),
			       math::min(beg_.col(), end_.col()));
    bb_.pmax() = point2d(math::max(beg_.row(), end_.row()),
			       math::max(beg_.col(), end_.col()));
  }

  inline
  bool
  line2d::has(const point2d& p) const
  {
    if (! bb_.has(p))
      return false;
    // FIXME: Optimize!
    for (unsigned i = 0; i < vect_.size(); ++i)
      if (vect_[i] == p)
	return true;
    return false;
  }

  inline
  std::size_t
  line2d::nsites() const
  {
    return vect_.size();
  }

  inline
  const box2d&
  line2d::bbox() const
  {
    return bb_;
  }

  inline
  const std::vector<point2d>&
  line2d::vect() const
  {
    return vect_;
  }

  inline
  const point2d&
  line2d::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());
    return vect_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_LINE2D_HH
