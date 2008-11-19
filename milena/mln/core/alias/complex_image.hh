// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ALIAS_COMPLEX_IMAGE_HH
# define MLN_CORE_ALIAS_COMPLEX_IMAGE_HH

/// \file mln/core/alias/complex_image.hh
/// \brief Definition of mln::complex_image aliases.

# include <mln/core/image/complex_image.hh>
# include <mln/core/alias/complex_geometry.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>


namespace mln
{

  /// \brief Type alias for a binary image based on a 2-complex, where
  /// 0-faces are located at floating-point 3-dimensional points.
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, bool>
  bin_2complex_image3df;

  /// \brief Type alias for an 8-bit grey-level image based on a
  /// 2-complex, where 0-faces are located at floating-point
  /// 3-dimensional points.
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, mln::value::int_u8>
  int_u8_2complex_image3df;

  /// \brief Type alias for a grey-level image based on a 2-complex,
  /// where 0-faces are located at floating-point 3-dimensional
  /// points.
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, unsigned>
  unsigned_2complex_image3df;

  /// \brief Type alias for a floating-point image based on a
  /// 2-complex, where 0-faces are located at floating-point
  /// 3-dimensional points.
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, float>
  float_2complex_image3df;

  /// \brief Type alias for a (3x8-bit) RGB image based on a
  /// 2-complex, where 0-faces are located at floating-point
  /// 3-dimensional points.
  typedef
  mln::complex_image<2, mln::space_2complex_geometry, mln::value::rgb8>
  rgb8_2complex_image3df;

} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_COMPLEX_IMAGE_HH
