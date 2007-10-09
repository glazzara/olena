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

#ifndef MLN_CORE_WIN_RECTANGLE2D_HH
# define MLN_CORE_WIN_RECTANGLE2D_HH

/*! \file mln/win/rectangle2d.hh
 *
 * \brief Definition of the mln::win::rectangle2d window.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/internal/dpoints_base.hh>
# include <mln/core/dpoint2d.hh>
# include <mln/core/dpoints_piter.hh>


namespace mln
{

  namespace win
  {
 
    /*! \brief Rectangular window defined on the 2D square grid.
     *
     * A rectangle2d is a 2D window with rectangular shape.  It is
     * centered and symmetrical.
     *
     * For instance: \n
     *  o o o o o \n
     *  o o x o o \n
     *  o o o o o \n
     * is defined with height = 3 and width = 5.
     */
    struct rectangle2d : public Window< rectangle2d >,
			 public internal::dpoints_base_< dpoint2d, rectangle2d >
    {
      /// Point associated type.
      typedef point2d point;

      /// Dpoint associated type.
      typedef dpoint2d dpoint;

      /*! \brief Point_Iterator type to browse a rectangle such as: "for each row
       * (increasing), for each column (increasing)."
       */
      typedef dpoints_fwd_piter<dpoint2d> fwd_qiter;

      /*! \brief Point_Iterator type to browse a rectangle such as: "for each row
       * (decreasing), for each column (decreasing)."
       */
      typedef dpoints_bkd_piter<dpoint2d> bkd_qiter;


      /*! \brief Constructor.
       *
       * \param[in] height sic
       * \param[in] width sic
       *
       * \pre Height and width are odd.
       */
      rectangle2d(unsigned height, unsigned width);


      /*! \brief Test if the window is centered.
       *
       * \return True.
       */
      bool is_centered() const;

      /*! \brief Test if the window is symmetric.
       *
       * \return true.
       */
      bool is_symmetric() const;

      /*! \brief Give the rectangle height.
       */
      unsigned height() const;

      /*! \brief Give the rectangle width.
       */
      unsigned width() const;

      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta() const;

      /// Apply a central symmetry to the target window.
      rectangle2d& sym();

    protected:
      unsigned height_, width_;
    };


    /*! \brief Print a rectangle window \p win into the output stream \p
     *  ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] win A rectangle window.
     *
     * \return The modified output stream \p ostr.
     *
     * \relates mln::win::rectangle2d
     */
    std::ostream& operator<<(std::ostream& ostr, const rectangle2d& win);

 

# ifndef MLN_INCLUDE_ONLY

    rectangle2d::rectangle2d(unsigned height, unsigned width)
      : height_(height),
	width_(width)
    {
      mln_precondition(height % 2 == 1 && width % 2 == 1);
      const int drow = height / 2, dcol = width / 2;
      for (int row = - drow; row <= drow; ++row)
	for (int col = - dcol; col <= dcol; ++col)
	  insert(make::dpoint2d(row, col));
    }

    bool rectangle2d::is_centered() const
    {
      return true;
    }

    bool rectangle2d::is_symmetric() const
    {
      return true;
    }

    unsigned rectangle2d::height() const
    {
      return height_;
    }

    unsigned rectangle2d::width() const
    {
      return width_;
    }

    unsigned rectangle2d::delta() const
    {
      return width_ > height_ ? width_ / 2 : height_ / 2;
    }

    rectangle2d& rectangle2d::sym()
    {
      return *this;
    }

    std::ostream& operator<<(std::ostream& ostr, const rectangle2d& win)
    {
      ostr << "[rectangle2d: width=" << win.width() << ", height=" << win.height() << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



// when rectangle2d is involved, one surely also wants:
# include <mln/win/hline2d.hh>
# include <mln/win/vline2d.hh>


#endif // ! MLN_CORE_WIN_RECTANGLE2D_HH
