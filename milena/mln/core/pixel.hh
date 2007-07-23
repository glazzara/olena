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

#ifndef MLN_CORE_PIXEL_HH
# define MLN_CORE_PIXEL_HH

/*! \file mln/core/pixel.hh
 *
 * \brief Definition of the generic pixel class mln::pixel.
 */

# include <mln/core/concept/generalized_pixel.hh>
# include <mln/core/internal/pixel_impl.hh>


namespace mln
{


  /*! \brief Generic pixel class.
   *
   * The parameter is \c I the type of the image it belongs to.
   */
  template <typename I>
  struct pixel : public Object< pixel<I> >,
		 public Generalized_Pixel< pixel<I> >,
		 internal::pixel_impl_< I, pixel<I> >
  {
    /// Constructor.
    pixel(I& image);

    /// Constructor.
    pixel(I& image, const mln_point(I)& p);

    /// Change the pixel to the one at point \p p.
    void change_to(const mln_point(I)& p);

    /// Test if this pixel is valid.
    bool is_valid() const;

  private:
    typedef internal::pixel_impl_<I, pixel<I> > super;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  pixel<I>::pixel(I& image)
    : super(image)
  {
    this->value_ptr_ = 0;
  }

  template <typename I>
  pixel<I>::pixel(I& image, const mln_point(I)& p)
    : super(image)
  {
    mln_precondition(this->image_.owns_(p));
    change_to(p);
  }

  template <typename I>
  void
  pixel<I>::change_to(const mln_point(I)& p)
  {
    mln_precondition(this->image_.owns_(p));
    this->value_ptr_ = & this->image_(p);
  }

  template <typename I>
  bool
  pixel<I>::is_valid() const
  {
    I& ima = this->image_;
    if (this->value_ptr_ == 0 || ! ima.has_data())
      return false;
    int o = this->value_ptr_ - ima.buffer();
    mln_point(I) p = ima.point_at_offset(o);
    return ima.owns_(p);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_PIXEL_HH
