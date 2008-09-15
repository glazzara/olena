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

#ifndef MLN_WIN_CUBOID3D_HH
# define MLN_WIN_CUBOID3D_HH

/// \file mln/win/cuboid3d.hh
/// \brief Definition of the mln::win::cuboid3d window.

# include <mln/core/internal/window_base.hh>
# include <mln/core/internal/dpoints_base.hh>
# include <mln/core/alias/dpoint3d.hh>


namespace mln
{

  namespace win
  {
 
    /** \brief Cuboid defined on the 3-D square grid.

	A cuboid3d is a 3-D window with cuboid (also known as
	rectangular prism or rectangular parallelepiped) shape.  It is
	centered and symmetric.

        For instance: \n

            o o o o o o o\n
           o o o o o o o\n
          o o o o o o o\n
         o o o o o o o\n
        o o o o o o \on

            o o o o o o o\n
           o o o o o o o\n
          o o o x o o o\n
         o o o o o o o\n
        o o o o o o \on

            o o o o o o o\n
           o o o o o o o\n
          o o o o o o o\n
         o o o o o o o\n
        o o o o o o \on

        is defined with depth = 3, height = 5 and width = 7.

        Reference:
          http://en.wikipedia.org/wiki/Cuboid
    */
    struct cuboid3d : public internal::window_base< dpoint3d, cuboid3d >,
		      public internal::dpoints_base_< dpoint3d, cuboid3d >
    {
      /// \brief Constructor.
      ///
      /// \param[in] depth  The depth of the cuboid3d.
      /// \param[in] height The height of the cuboid3d.
      /// \param[in] width  The width of the cuboid3d.
      ///
      /// \pre Argument \a depth, \a height and \a width must be odd.
      cuboid3d(unsigned depth, unsigned height, unsigned width);


      /// Properties of the window.
      /// \{
      /// \brief Test if the window is centered.
      /// \return \c true (always).
      bool is_centered() const;
      /// \brief Test if the window is symmetric.
      /// \return \c true (always).
      bool is_symmetric() const;
      /// \}

      /// Accessors.
      /// \{
      /// \brief Return the depth of the cuboid.
      unsigned depth() const;
      /// \brief Return the height of the cuboid.
      unsigned height() const;
      /// \brief Return the width of the cuboid.
      unsigned width() const;
      /// \}

      /// \brief Return the volume of the cuboid.
      unsigned volume() const;

      /// \brief Give the maximum coordinate gap between the center of
      /// the window and a point of the window.
      unsigned delta() const;

      /// Apply a central symmetry to the target window.
      cuboid3d& sym();

    protected:
      /// The depth of the cuboid (expressed as a number of slices).
      unsigned depth_;
      /// The height of the cuboid (expressed as a number of rows).
      unsigned height_;
      /// The width of the cuboid (expressed as a number of columns).
      unsigned width_;
    };


    /// \brief Print a cuboid window \a win into the output stream \a ostr.
    ///
    /// \param[in,out] ostr An output stream.
    /// \param[in] win A cuboid window.
    ///
    /// \return The modified output stream \a ostr.
    ///
    /// \relates mln::win::cuboid3d
    std::ostream& operator<<(std::ostream& ostr, const cuboid3d& win);

 

# ifndef MLN_INCLUDE_ONLY

    inline
    cuboid3d::cuboid3d(unsigned depth, unsigned height, unsigned width)
      : depth_(depth),
	height_(height),
	width_(width)
    {
      mln_precondition(height % 2 == 1 && width % 2 == 1);
      const int dsli = depth / 2;
      const int drow = height / 2;
      const int dcol = width / 2;
      for (int sli = -dsli; sli <= dsli; ++sli)
	for (int row = -drow; row <= drow; ++row)
	  for (int col = -dcol; col <= dcol; ++col)
	    insert(dpoint3d(sli, row, col));
    }

    inline
    bool
    cuboid3d::is_centered() const
    {
      return true;
    }

    inline
    bool
    cuboid3d::is_symmetric() const
    {
      return true;
    }

    inline
    unsigned
    cuboid3d::depth() const
    {
      return depth_;
    }

    inline
    unsigned
    cuboid3d::height() const
    {
      return height_;
    }

    inline
    unsigned
    cuboid3d::width() const
    {
      return width_;
    }

    inline
    unsigned
    cuboid3d::volume() const
    {
      return depth_ * width_ * height_;
    }

    inline
    unsigned
    cuboid3d::delta() const
    {
      if (depth_ > height_)
	if (depth_ > width_)
	  // height_ < depth_ && width_ < depth_.
	  return depth_ / 2;
	else
	  // height_ < depth_ <= width_.
	  return width_ / 2;
      else
	if (height_ > width_)
	  // depth_ <= height_ && width_ <= height_.
	  return height_ / 2;
	else
	  // depth_ <= height_ <= width_.
	  return width_ / 2;	  
    }

    inline
    cuboid3d&
    cuboid3d::sym()
    {
      return *this;
    }

    inline
    std::ostream&
    operator<<(std::ostream& ostr, const cuboid3d& win)
    {
      ostr << "[cuboid3d: width=" << win.depth()
	   << ", depth=" << win.width()
	   << ", height=" << win.height() << ']';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln


#endif // ! MLN_WIN_CUBOID3D_HH
