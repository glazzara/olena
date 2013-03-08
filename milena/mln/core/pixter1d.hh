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

#ifndef MLN_CORE_PIXTER1D_HH
# define MLN_CORE_PIXTER1D_HH

/// \file
/// \brief Pixel iterators on a 1-D image with border.

# include <mln/core/internal/pixel_iterator_base.hh>
# include <mln/core/alias/point1d.hh>
# include <mln/geom/size1d.hh>

namespace mln
{

  /*------------------.
  | fwd_pixter1d<I>.  |
  `------------------*/

  /*!
    \internal
    \brief Forward pixel iterator on a 1-D image with border.
  */
  template <typename I>
  class fwd_pixter1d :
    public internal::forward_pixel_iterator_base_< I, fwd_pixter1d<I> >
  {
    typedef internal::forward_pixel_iterator_base_< I, fwd_pixter1d<I> > super_;

  public:
    /// Image type.
    typedef I image;

    /// \brief Constructor.
    /// \param[in] image The image this pixel iterator is bound to.
    fwd_pixter1d(I& image);

    /// Go to the next pixel.
    void next_();
  };


  /*------------------.
  | bkd_pixter1d<I>.  |
  `------------------*/

  /*!
    \internal
    \brief Backward pixel iterator on a 1-D image with border.
  */
  template <typename I>
  class bkd_pixter1d :
    public internal::backward_pixel_iterator_base_< I, bkd_pixter1d<I> >
  {
    typedef internal::backward_pixel_iterator_base_< I, bkd_pixter1d<I> > super_;

  public:
    /// Image type.
    typedef I image;

    /// \brief Constructor.
    /// \param[in] image The image this pixel iterator is bound to.
    bkd_pixter1d(I& image);

    /// Go to the next pixel.
    void next_();
  };



#ifndef MLN_INCLUDE_ONLY

  /*------------------.
  | fwd_pixter1d<I>.  |
  `------------------*/

  template <typename I>
  inline
  fwd_pixter1d<I>::fwd_pixter1d(I& image)
    : super_(image)
  {
    mln_precondition(image.is_valid());
  }

  template <typename I>
  inline
  void
  fwd_pixter1d<I>::next_()
  {
    ++this->value_ptr_;
  }


  /*------------------.
  | bkd_pixter1d<I>.  |
  `------------------*/

  template <typename I>
  inline
  bkd_pixter1d<I>::bkd_pixter1d(I& image)
    : super_(image)
  {
    mln_precondition(image.is_valid());
  }

  template <typename I>
  inline
  void
  bkd_pixter1d<I>::next_()
  {
    --this->value_ptr_;
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_PIXTER1D_HH
