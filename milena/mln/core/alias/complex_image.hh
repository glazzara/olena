// Copyright (C) 2008, 2009, 2011, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_ALIAS_COMPLEX_IMAGE_HH
# define MLN_CORE_ALIAS_COMPLEX_IMAGE_HH

/// \file
/// \brief Definition of mln::complex_image aliases.
///
/// As C++ does not allow template typedefs (yet), using shortcuts for
/// very long image names is handy.

# include <mln/core/image/complex_image.hh>
# include <mln/core/alias/complex_geometry.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>


namespace mln
{
  /*------------------------------.
  | 2-d plane 1-complex aliases.  |
  `------------------------------*/

  /*!
    \class bin_1complex_image2d
    \headerfile <>

    \brief Type alias for a binary image based on a
    1-complex, where 0-faces are located at discrete (integer)
    2-dimensional points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<1, mln::discrete_plane_1complex_geometry, bool>
  bin_1complex_image2d;
  /// \endcond

  /*!
    \class int_u8_1complex_image2d
    \headerfile <>

    \brief Type alias for an 8-bit gray-level image based on a
    1-complex, where 0-faces are located at discrete (integer)
    2-dimensional points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<1, mln::discrete_plane_1complex_geometry,
		     mln::value::int_u8>
  int_u8_1complex_image2d;
  /// \endcond


  /*------------------------------.
  | 2-d plane 2-complex aliases.  |
  `------------------------------*/

  /*!
    \class bin_2complex_image2d
    \headerfile <>

    \brief Type alias for a binary image based on a
    2-complex, where 0-faces are located at discrete (integer)
    2-dimensional points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<2, mln::discrete_plane_2complex_geometry, bool>
  bin_2complex_image2d;
  /// \endcond

  /*!
    \class int_u8_2complex_image2d
    \headerfile <>

    \brief Type alias for an 8-bit gray-level image based on a
    2-complex, where 0-faces are located at discrete (integer)
    2-dimensional points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<2, mln::discrete_plane_2complex_geometry,
		     mln::value::int_u8>
  int_u8_2complex_image2d;
  /// \endcond


  /*------------------------------.
  | 3-d space 2-complex aliases.  |
  `------------------------------*/

  /*!
    \class bin_2complex_image3df
    \headerfile <>

    \brief Type alias for a binary image based on a 2-complex, where
    0-faces are located at floating-point 3-dimensional points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, bool>
  bin_2complex_image3df;
  /// \endcond

  /*!
    \class int_u8_2complex_image3df
    \headerfile <>

    \brief Type alias for an 8-bit gray-level image based on a
    2-complex, where 0-faces are located at floating-point
    3-dimensional points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, mln::value::int_u8>
  int_u8_2complex_image3df;
  /// \endcond

  /*!
    \class unsigned_2complex_image3df
    \headerfile <>

    \brief Type alias for a gray-level image based on a 2-complex,
    where 0-faces are located at floating-point 3-dimensional
    points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, unsigned>
  unsigned_2complex_image3df;
  /// \endcond

  /*!
    \class float_2complex_image3df
    \headerfile <>

    \brief Type alias for a floating-point image based on a
    2-complex, where 0-faces are located at floating-point
    3-dimensional points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, float>
  float_2complex_image3df;
  /// \endcond

  /*!
    \class rgb8_2complex_image3df
    \headerfile <>

    \brief Type alias for a (3x8-bit) RGB image based on a
    2-complex, where 0-faces are located at floating-point
    3-dimensional points.

    \ingroup modimageconcrete
  */
  /// \cond ALIAS
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, mln::value::rgb8>
  rgb8_2complex_image3df;
  /// \endcond

} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_COMPLEX_IMAGE_HH
