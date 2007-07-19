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

#ifndef MLN_CORE_INTERNAL_LINEARY_PIXEL_ITERATOR_BASE_HH
# define MLN_CORE_INTERNAL_LINEARY_PIXEL_ITERATOR_BASE_HH

/*! \file mln/core/internal/lineary_pixel_iterator_base.hh
 *
 * \brief Code factorization for lineary pixel iterator classes.
 */

# include <mln/core/internal/pixel_iterator_base.hh>

namespace mln
{

  namespace internal
  {

    /*! \brief lineary_pixel_iterator_base_ class
     *
     */
    template <typename I, typename E>
    class  lineary_pixel_iterator_base_ : public pixel_iterator_base_<I, E>
    {
      typedef pixel_iterator_base_<I, E> super;
    public:
      /// Image pixel value type.
      typedef mln_value(I) value;

      /// Go to the beginning of the image.
      void start();
      /// Invalidate the iterator.
      void invalidate();
      /// Is the iterator referencing a correct position in the image?
      bool is_valid() const;


    protected:
      /// beginning of the image
      value* start_;
      /// end of the image
      value* eoi_;

      /// Constructor
      lineary_pixel_iterator_base_(I& image);
    };

#ifndef MLN_INCLUDE_ONLY

    template <typename I, typename E>
    lineary_pixel_iterator_base_<I, E>::lineary_pixel_iterator_base_(I& image)
       : super(image)
    {
    }

    template <typename I, typename E>
    void
    lineary_pixel_iterator_base_<I, E>::start()
    {
      this->value_ptr_ = start_;
    }

    template <typename I, typename E>
    void
    lineary_pixel_iterator_base_<I, E>::invalidate()
    {
      this->value_ptr_ = eoi_;
    }

    template <typename I, typename E>
    bool
    lineary_pixel_iterator_base_<I, E>::is_valid() const
    {
      return (this->value_ptr_ != eoi_);
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_LINEARY_PIXEL_ITERATOR_BASE_HH
