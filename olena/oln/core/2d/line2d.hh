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

#ifndef OLN_CORE_2D_LINE2D_HH
# define OLN_CORE_2D_LINE2D_HH

# include <oln/core/2d/box2d.hh>
# include <oln/core/internal/pset_adaptor.hh>
# include <oln/core/gen/pvec.hh>


namespace oln
{

  // FIXME: Move it!
  namespace util
  {
    template <typename T>
    int sign(const T& t)
    {
      if (t != 0)
	return t > 0 ? 1 : -1;
      else
	return 0;
    }

  } // end of namespace oln::util



  // Fwd decl.
  class line2d;


  // Super type.
  template <>
  struct super_trait_< line2d >
  {
    typedef internal::pset_adaptor_<line2d> ret;
  };


  // Virtual types.
  template <>
  struct vtypes< line2d >
  {
    typedef pvec_<point2d> adapted;
  };

  class line2d : public internal::pset_adaptor_<line2d>
  {
  public:

    line2d(const point2d& begin, const point2d& end);
    const point2d& begin_point() const;
    const point2d& end_point() const;

    const pvec_<point2d>& pvec() const;
    const std::vector<point2d>& con() const { return this->ps_.con(); }

  protected:
    point2d beg_, end_;
    void compute_(); // FIXME: we also need a semi_open_line (without the end point)...
  };
  

# ifndef OLN_INCLUDE_ONLY

  line2d::line2d(const point2d& begin,
		   const point2d& end)
    : beg_(begin),
      end_(end)
  {
    this->compute_();
  }

  const point2d&
  line2d::begin_point() const
  {
    return this->beg_;
  }

  const point2d&
  line2d::end_point() const
  {
    return this->end_;
  }

  void
  line2d::compute_()
  {
    dpoint2d dp = this->end_ - this->beg_;
    int
      srow = util::sign(dp.row()), drow = std::abs(dp.row()), ddrow = 2 * drow,
      scol = util::sign(dp.col()), dcol = std::abs(dp.col()), ddcol = 2 * dcol,
      row = this->beg_.row(),
      col = this->beg_.row();

    if ( dcol > drow )
      {
	int e = ddrow - dcol;
	for (int i = 0; i < dcol; ++i)
	  {
	    this->ps_.append(point2d(row, col));
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
	    this->ps_.append(point2d(row, col));
	    while (e >= 0)
	      {
		col += scol;
		e -= ddrow;
	      }
	    row += srow;
	    e += ddcol;
	  }
      }
    this->ps_.append(point2d(row, col));
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln

#endif // ! OLN_CORE_2D_LINE2D_HH
