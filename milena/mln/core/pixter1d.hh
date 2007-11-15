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

#ifndef MLN_CORE_PIXTER1D_HH
# define MLN_CORE_PIXTER1D_HH

/*! \file mln/core/pixter1d.hh
 *
 * \brief Pixel iterator class on a image 1d with border.
 */

# include <mln/core/internal/pixel_iterator_base.hh>
# include <mln/core/point1d.hh>
# include <mln/geom/size1d.hh>



namespace mln
{

  template <typename I>
  class fwd_pixter1d : public internal::pixel_iterator_base_< I, fwd_pixter1d<I> >
  {
    typedef internal::pixel_iterator_base_< I, fwd_pixter1d<I> > super_;

  public:

    /// Image type.
    typedef I image;

    /*! \brief Constructor.
     *
     * \param[in] image Image to iterate over its pixels.
     */
    fwd_pixter1d(I& image);

    /// Go to the next pixel.
    void next_();

  };


  // FIXME: bkd_pixter1d


#ifndef MLN_INCLUDE_ONLY

  template <typename I>
  fwd_pixter1d<I>::fwd_pixter1d(I& image) :
    super_(image)
  {
    mln_precondition(image.has_data());
  }

  template <typename I>
  void
  fwd_pixter1d<I>::next_()
  {
    ++this->value_ptr_;
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_PIXTER1D_HH
