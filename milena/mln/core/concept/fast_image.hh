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
# include <mln/core/concept/generalized_point.hh>
# include <mln/core/trait/qlf_value.hh>


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

      int offset(const dpoint& dp) const;
      point point_at_offset(unsigned o) const;

      mln_qlf_value(E)* buffer();
      const value* buffer() const;

      rvalue operator[](unsigned o) const;
      lvalue operator[](unsigned o);
     */


    /*! \brief Give the offset of the point \p p.
     *
     * \param[in] p A generalized point.
     *
     * \warning This method is final.
     *
     * \pre The image has to be initialized and to own the point \p p.
     * \post p == point_at_offset(result)
     */
    template <typename P>
    unsigned offset(const Generalized_Point<P>& p) const;

  protected:
    Fast_Image();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  template <typename P>
  unsigned
  Fast_Image<E>::offset(const Generalized_Point<P>& p_) const
  {
    // FIXME: check that P is mln_point(E)
    const E& this_ = exact(this);
    const P& p = internal::force_exact<P>(p_);
    mln_precondition(this_->has_data());
    mln_precondition(this_->owns_(p));

    unsigned o = & this_->operator()(p) - this_->buffer();
    mln_postcondition(p == this_->point_at_offset(o));
    return o;
  }

  template <typename E>
  Fast_Image<E>::Fast_Image()
  {
    typedef mln_point(E)   point;
    typedef mln_dpoint(E) dpoint;

    typedef mln_fwd_pixter(E) fwd_pixter;
    typedef mln_bkd_pixter(E) bkd_pixter;

    int (E::*m1)(const dpoint&) const = & E::offset;
    m1 = 0;
    point (E::*m2)(unsigned) const = & E::point_at_offset;
    m2 = 0;
    unsigned (E::*m3)() const = & E::border;
    m3 = 0;

    typedef mln_value(E) value;

    mln_qlf_value(E)* (E::*m4)() = & E::buffer;
    m4 = 0;
    const value* (E::*m5)() const = & E::buffer;
    m5 = 0;

    typedef mln_rvalue(E) rvalue;
    typedef mln_lvalue(E) lvalue;

    rvalue (E::*m6)(unsigned) const = & E::operator[];
    m6 = 0;
    lvalue (E::*m7)(unsigned) = & E::operator[];
    m7 = 0;

    // FIXME: how to check that qixter are defined when W is unknown!
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_FAST_IMAGE_HH
