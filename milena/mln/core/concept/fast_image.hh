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

#ifndef MLN_CORE_CONCEPT_FAST_IMAGE_HH
# define MLN_CORE_CONCEPT_FAST_IMAGE_HH

/*! \file mln/core/concept/fast_image.hh
 * \brief Definition of the concept of mln::Fast_Image.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/trait/pixter.hh>


namespace mln
{

  /*! \brief Base class for implementation of fast image classes.
   *
   * \see mln::doc::Fast_Image for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Fast_Image : public Image<E>
  {
    /*
      unsigned border();

      int offset(const dpoint& dp);
      unsigned offset(const point& p);
      point point_at_offset(unsigned o) const;

      const value* buffer() const;
     */

  protected:
    Fast_Image();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Fast_Image<E>::Fast_Image()
  {
    typedef mln_point(E)   point;
    typedef mln_dpoint(E) dpoint;
    typedef mln_value(E)   value;

    typedef mln_fwd_pixter(E) fwd_pixter;
    typedef mln_bkd_pixter(E) bkd_pixter;

    int (E::*m1)(const dpoint&) = & E::offset;
    m1 = 0;
    unsigned (E::*m2)(const point&) = & E::offset;
    m2 = 0;
    point (E::*m3)(unsigned) const = & E::point_at_offset;
    m3 = 0;
    const value* (E::*m4)() const = & E::buffer;
    m4 = 0;
    unsigned (E::*m5)() const = & E::border;
    m5 = 0;

    // FIXME: how to check that qixter are defined when W is unknown!
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_FAST_IMAGE_HH
