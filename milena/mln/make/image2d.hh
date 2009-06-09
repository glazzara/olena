// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_IMAGE2D_HH
# define MLN_MAKE_IMAGE2D_HH

/// \file
///
/// Routine to create a 2D image from a 1D array.
///
/// \todo Think about removing make::image2d since it is redundant
/// with convert::to and convert::from_to.

# include <mln/core/image/image2d.hh>


namespace mln
{

  namespace make
  {

    /// Create an image2d from an 2D array of values.
    ///
    /// \param[in] values 2D array.
    ///
    /// \return A 2D image.
    ///
    template <typename V, unsigned S>
    mln::image2d<V>
    image2d(V (&values)[S]);



# ifndef MLN_INCLUDE_ONLY

    template <typename V, unsigned S>
    mln::image2d<V>
    image2d(V (&values)[S])
    {
      mlc_bool(S != 0)::check();
      enum { s = mlc_sqrt_int(S) };
      metal::bool_<(s * s == S)>::check();
      mln::image2d<V> tmp;
      convert::from_to(values, tmp);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_IMAGE2D_HH
