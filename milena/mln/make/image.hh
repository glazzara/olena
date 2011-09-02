// Copyright (C) 2007, 2008, 2009, 2010, 2011 EPITA Research and
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

#ifndef MLN_MAKE_IMAGE_HH
# define MLN_MAKE_IMAGE_HH

/// \file
///
/// \brief Routine to create an image from values given as a C-array.
///
/// \todo I guess that we should modify V into mln_unconst(V); test
/// with C arrays being "const int[]" and run g++-2.95...

# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/opt/at.hh>



namespace mln
{

  namespace make
  {

    /// Create an image1d from an 1D array of values.
    ///
    /// \param[in] values 1D array.
    ///
    /// \return A 1D image.
    ///
    template <typename V, unsigned L>
    mln::image1d<V> image(V (&values)[L]);


    /// Create an image2d from an 2D array of values.
    ///
    /// \param[in] values 2D array.
    ///
    /// \return A 2D image.
    ///
    template <typename V, unsigned R, unsigned C>
    mln::image2d<V> image(V (&values)[R][C]);


    /// Create an image3d from an 3D array of values.
    ///
    /// \param[in] values 3D array.
    ///
    /// \return A 3D image.
    ///
    template <typename V, unsigned S, unsigned R, unsigned C>
    mln::image3d<V> image(V (&values)[S][R][C]);



# ifndef MLN_INCLUDE_ONLY

    template <typename V, unsigned L>
    mln::image1d<V>
    image(V (&values)[L])
    {
      mlc_bool(L != 0)::check();
      mln::image1d<V> tmp(L);
      const def::coord ninds = static_cast<def::coord>(L);
      for (def::coord ind = 0; ind < ninds; ++ind)
	tmp(point1d(ind)) = values[ind];
      return tmp;
    }

    template <typename V, unsigned R, unsigned C>
    mln::image2d<V>
    image(V (&values)[R][C])
    {
      mlc_bool(R != 0 && C != 0)::check();
      mln::image2d<V> tmp(R, C);
      const def::coord
	nrows = static_cast<def::coord>(R),
	ncols = static_cast<def::coord>(C);
      for (def::coord row = 0; row < nrows; ++row)
	for (def::coord col = 0; col < ncols; ++col)
	  opt::at(tmp, row, col) = values[row][col];
      return tmp;
    }

    template <typename V, unsigned S, unsigned R, unsigned C>
    mln::image3d<V>
    image(V (&values)[S][R][C])
    {
      mlc_bool(S != 0 && R != 0 && C != 0)::check();
      mln::image3d<V> tmp(S, R, C);
      const def::coord
	nslis = static_cast<def::coord>(S),
	nrows = static_cast<def::coord>(R),
	ncols = static_cast<def::coord>(C);
      for (def::coord sli = 0; sli < nslis; ++sli)
	for (def::coord row = 0; row < nrows; ++row)
	  for (def::coord col = 0; col < ncols; ++col)
	    opt::at(tmp, sli, row, col) = values[sli][row][col];
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_IMAGE_HH
