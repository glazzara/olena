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

#ifndef MLN_CORE_CONCEPT_FASTEST_IMAGE_HH
# define MLN_CORE_CONCEPT_FASTEST_IMAGE_HH

/*! \file mln/core/concept/fastest_image.hh
 * \brief Definition of the concept of mln::Fastest_Image.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/generalized_point.hh>
# include <mln/core/trait/qlf_value.hh>


namespace mln
{

  /*! \brief Base class for implementation of fastest image classes.
   *
   * \see mln::doc::Fastest_Image for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Fastest_Image : public Image<E>
  {
    /*
      unsigned border();

      int offset(const dpoint& dp) const; // FIXME: std::ptr_diff_t?
      point point_at_offset(unsigned o) const;

      mln_qlf_value(E)* buffer();
      const value* buffer() const;

      rvalue operator[](unsigned o) const;
      lvalue operator[](unsigned o);

      std::size_t ncells() const;
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
    unsigned
    offset_at(const Generalized_Point<P>& p) const;

  };


# ifndef MLN_INCLUDE_ONLY

  // FIXME: Move in internal/image/impl...

  template <typename E>
  template <typename P>
  unsigned // FIXME: std::size_t?
  Fastest_Image<E>::offset_at(const Generalized_Point<P>& p_) const
  {
    // FIXME: check that P is mln_point(E)
    const E* this_ = exact(this);
    const P& p = internal::force_exact<P>(p_);
    mln_precondition(this_->has_data());
    mln_precondition(this_->owns_(p));

    unsigned o = & this_->operator()(p) - this_->buffer();
    mln_postcondition(p == this_->point_at_offset(o));
    return o;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_FASTEST_IMAGE_HH
