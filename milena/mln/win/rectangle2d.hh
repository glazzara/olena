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

#ifndef MLN_WIN_RECTANGLE2D_HH
# define MLN_WIN_RECTANGLE2D_HH

/*! \file mln/win/rectangle2d.hh
 *
 * \brief Definition of the mln::win::rectangle2d window.
 *
 * \todo Reactivate includes at EOF.
 */

# include <mln/core/internal/window_base.hh>
# include <mln/core/internal/dpoints_base.hh>
# include <mln/core/alias/dpoint2d.hh>


namespace mln
{

  namespace win
  {

    /*! \brief Rectangular window defined on the 2D square grid.
     *
     * A rectangle2d is a 2D window with rectangular shape.  It is
     * centered and symmetric.
     *
     * For instance: \n
     *  o o o o o \n
     *  o o x o o \n
     *  o o o o o \n
     * is defined with height = 3 and width = 5.
     */
    struct rectangle2d : public internal::window_base< dpoint2d, rectangle2d >,
			 public internal::dpoints_base_< dpoint2d, rectangle2d >
    {
      typedef internal::dpoints_base_< dpoint2d, rectangle2d > super_;

      typedef super_::site site;
      typedef super_::psite psite;
      typedef super_::dpsite dpsite;

      /*! \brief Constructor.
       *
       * \param[in] height Height of the rectangle2d.
       * \param[in] width Width of the rectangle2d.
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

      /// Give the rectangle height.
      unsigned height() const;

      /// Give the rectangle width.
      unsigned width() const;

      /// Give the rectangle area.
      unsigned area() const;

      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta() const;

      /// Apply a central symmetry to the target window; a no-op here.
      void sym();


      /// Give the std vector of delta-points.
      const std::vector<dpoint2d>& std_vector() const;

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

    inline
    rectangle2d::rectangle2d(unsigned height, unsigned width)
      : height_(height),
	width_(width)
    {
      mln_precondition(height % 2 == 1 && width % 2 == 1);
      const int drow = height / 2, dcol = width / 2;
      for (int row = - drow; row <= drow; ++row)
	for (int col = - dcol; col <= dcol; ++col)
	  this->insert(dpoint2d(row, col));
    }

    inline
    bool rectangle2d::is_centered() const
    {
      return true;
    }

    inline
    bool rectangle2d::is_symmetric() const
    {
      return true;
    }

    inline
    unsigned rectangle2d::height() const
    {
      return height_;
    }

    inline
    unsigned rectangle2d::width() const
    {
      return width_;
    }

    inline
    unsigned rectangle2d::area() const
    {
      return width_ * height_;
    }

    inline
    unsigned rectangle2d::delta() const
    {
      return width_ > height_ ? width_ / 2 : height_ / 2;
    }

    inline
    void
    rectangle2d::sym()
    {
      // No-op.
    }

    inline
    const std::vector<dpoint2d>&
    rectangle2d::std_vector() const
    {
      return win_.std_vector();
    }

    inline
    std::ostream& operator<<(std::ostream& ostr, const rectangle2d& win)
    {
      ostr << "[rectangle2d: width=" << win.width() << ", height=" << win.height() << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



// When rectangle2d is involved, one surely also wants:
# include <mln/win/hline2d.hh>
# include <mln/win/vline2d.hh>


#endif // ! MLN_WIN_RECTANGLE2D_HH
