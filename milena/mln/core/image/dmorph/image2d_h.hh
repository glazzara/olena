// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_DMORPH_IMAGE2D_H_HH
# define MLN_CORE_IMAGE_DMORPH_IMAGE2D_H_HH

/// \file
///
/// Definition of an alias for a 2d image based on an hexagonal
/// mesh.

# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/hexa.hh>

# include <mln/border/thickness.hh>


# include <mln/debug/println.hh>

namespace mln
{

  /// 2d image based on an hexagonal mesh.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename V>
  struct image2d_h
    : public hexa< image2d<V> >
  {
    //typedef hexa< image2d<V> > super_;

    /// Point site type
    typedef point2d_h psite;

    /// Constructor with the numbers of rows and columns
    /// border thickness.
    ///
    /// image2d_h(3,6) will build this hexa image :
    ///
    ///        1   3   5
    ///      0   2   4
    ///    --------------
    ///   0| x   x   x
    ///    |
    ///   2|   x   x   x
    ///    |
    ///   4| x   x   x

    image2d_h(int nrows, int ncols, unsigned bdr = border::thickness);

    //using super_::init_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename V>
  inline
  image2d_h<V>::image2d_h(int nrows, int ncols, unsigned bdr)
  {
    // numbers of cols can't be odd.
    mln_assertion(ncols % 2 == 0);

    image2d<V> ima(nrows,
		   ncols / 2,
		   bdr);

    this->init_(ima);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_IMAGE2D_H_HH
