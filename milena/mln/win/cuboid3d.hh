// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_WIN_CUBOID3D_HH
# define MLN_WIN_CUBOID3D_HH

/// \file
///
/// Definition of the mln::win::cuboid3d window.

# include <mln/core/internal/classical_window_base.hh>
# include <mln/core/alias/dpoint3d.hh>


namespace mln
{

  mln_internal_add_classical_window_trait(cuboid3d);


  namespace win
  {

    /** \brief Cuboid defined on the 3-D square grid.

	A cuboid3d is a 3-D window with cuboid (also known as
	rectangular prism or rectangular parallelepiped) shape.  It is
	centered and symmetric.

        For instance:
	\verbatim
            o o o o o o o
           o o o o o o o
          o o o o o o o
         o o o o o o o
        o o o o o o o

            o o o o o o o
           o o o o o o o
          o o o x o o o
         o o o o o o o
        o o o o o o o

            o o o o o o o
           o o o o o o o
          o o o o o o o
         o o o o o o o
        o o o o o o o
	\endverbatim

        is defined with depth = 3, height = 5 and width = 7.

        Reference:
          http://en.wikipedia.org/wiki/Cuboid

	\ingroup modwin3d
    */
    struct cuboid3d : public internal::classical_window_base< dpoint3d, cuboid3d >
    {
      /// \brief Constructor.
      ///
      /// \param[in] depth  The depth of the cuboid3d.
      /// \param[in] height The height of the cuboid3d.
      /// \param[in] width  The width of the cuboid3d.
      ///
      /// \pre Argument \a depth, \a height and \a width must be odd.
      cuboid3d(unsigned depth, unsigned height, unsigned width);


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
      unsigned delta_() const;

      void print_(std::ostream& ostr) const;

    protected:
      /// The depth of the cuboid (expressed as a number of slices).
      unsigned depth_;
      /// The height of the cuboid (expressed as a number of rows).
      unsigned height_;
      /// The width of the cuboid (expressed as a number of columns).
      unsigned width_;
    };



# ifndef MLN_INCLUDE_ONLY

    inline
    cuboid3d::cuboid3d(unsigned depth, unsigned height, unsigned width)
      : depth_(depth),
	height_(height),
	width_(width)
    {
      mln_precondition(height % 2 == 1 && width % 2 == 1);
      const def::coord
	dsli = static_cast<def::coord>(depth / 2),
	drow = static_cast<def::coord>(height / 2),
	dcol = static_cast<def::coord>(width / 2),
	minus_dsli = static_cast<def::coord>(- dsli),
	minus_drow = static_cast<def::coord>(- drow),
	minus_dcol = static_cast<def::coord>(- dcol);
      for (def::coord sli = minus_dsli; sli <= dsli; ++sli)
	for (def::coord row = minus_drow; row <= drow; ++row)
	  for (def::coord col = minus_dcol; col <= dcol; ++col)
	    insert(dpoint3d(sli, row, col));
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
    cuboid3d::delta_() const
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
    void
    cuboid3d::print_(std::ostream& ostr) const
    {
      ostr << "[cuboid3d: width=" << depth_
	   << ", depth=" << width_
	   << ", height=" << height_ << ']';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln


#endif // ! MLN_WIN_CUBOID3D_HH
