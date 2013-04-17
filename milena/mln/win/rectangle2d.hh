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

#ifndef MLN_WIN_RECTANGLE2D_HH
# define MLN_WIN_RECTANGLE2D_HH

/// \file
///
/// Definition of the mln::win::rectangle2d window.
///
/// \todo Reactivate includes at EOF.

# include <mln/core/internal/classical_window_base.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/core/def/coord.hh>


namespace mln
{

  mln_internal_add_classical_window_trait(rectangle2d);


  namespace win
  {

    /*! \brief Rectangular window defined on the 2D square grid.
     *
     * A rectangle2d is a 2D window with rectangular shape.  It is
     * centered and symmetric.
     *
     * For instance:
     * \verbatim
     *  o o o o o
     *  o o x o o
     *  o o o o o
     * \endverbatim
     * is defined with height = 3 and width = 5.
     *
     * \ingroup modwin2d
     */
    struct rectangle2d : public internal::classical_window_base< dpoint2d, rectangle2d >
    {
      /*! \brief Constructor.
       *
       * \param[in] height Height of the rectangle2d.
       * \param[in] width Width of the rectangle2d.
       *
       * \pre Height and width are odd.
       */
      rectangle2d(unsigned height, unsigned width);


      /// Give the rectangle height.
      unsigned height() const;

      /// Give the rectangle width.
      unsigned width() const;

      /// Give the rectangle area.
      unsigned area() const;

      /// Give the std vector of delta-points.
      const std::vector<dpoint2d>& std_vector() const;


      /// \cond INTERNAL_API
      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta_() const;

      void print_(std::ostream& ostr) const;
      /// \endcond

    protected:

      unsigned height_, width_;
    };



# ifndef MLN_INCLUDE_ONLY

    inline
    rectangle2d::rectangle2d(unsigned height, unsigned width)
      : height_(height),
	width_(width)
    {
      mln_precondition(height % 2 == 1 && width % 2 == 1);
      const def::coord  drow = (def::coord) (height / 2);
      const def::coord dcol = (def::coord) (width / 2);

      for (def::coord row = (def::coord) -drow; row <= drow; ++row)
	for (def::coord col = (def::coord) -dcol; col <= dcol; ++col)
	  this->insert(dpoint2d(row, col));
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
    unsigned rectangle2d::delta_() const
    {
      return width_ > height_ ? width_ / 2 : height_ / 2;
    }

    inline
    const std::vector<dpoint2d>&
    rectangle2d::std_vector() const
    {
      return win_.std_vector();
    }

    inline
    void
    rectangle2d::print_(std::ostream& ostr) const
    {
      ostr << "[rectangle2d: width=" << width_ << ", height=" << height_ << ']';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



// When rectangle2d is involved, one surely also wants:
# include <mln/win/hline2d.hh>
# include <mln/win/vline2d.hh>


#endif // ! MLN_WIN_RECTANGLE2D_HH
