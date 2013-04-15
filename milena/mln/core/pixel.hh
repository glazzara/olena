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

#ifndef MLN_CORE_PIXEL_HH
# define MLN_CORE_PIXEL_HH

/// \file
///
/// Definition of the generic pixel class mln::pixel.
///
/// \todo Answer these questions:
///  - why do we have 2 classes (mln::pixel and mln::util::pix)?
///  - what about keeping only one of both?

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
    pixel(I& image, const mln_psite(I)& p);

    /// Change the pixel to the one at point \p p.
    void change_to(const mln_psite(I)& p);

    /// Test if this pixel is valid.
    bool is_valid() const;

  private:
    typedef internal::pixel_impl_<I, pixel<I> > super;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  inline
  pixel<I>::pixel(I& image)
    : super(image)
  {
  }

  template <typename I>
  inline
  pixel<I>::pixel(I& image, const mln_psite(I)& p)
    : super(image)
  {
    mln_precondition(this->image_.has(p));
    change_to(p);
  }

  template <typename I>
  inline
  void
  pixel<I>::change_to(const mln_psite(I)& p)
  {
    mln_precondition(this->image_.has(p));
    this->value_ptr_ = & this->image_(p);
  }

  template <typename I>
  inline
  bool
  pixel<I>::is_valid() const
  {
    if (this->value_ptr_ == 0 || ! this->image_.is_valid())
      return false;
    int o = this->value_ptr_ - this->image_.buffer();
    mln_psite(I) p = this->image_.point_at_offset(o);
    return this->image_.has(p);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/make/pixel.hh>


#endif // ! MLN_CORE_PIXEL_HH
