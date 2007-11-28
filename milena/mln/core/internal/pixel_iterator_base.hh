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

#ifndef MLN_CORE_INTERNAL_PIXEL_ITERATOR_BASE_HH
# define MLN_CORE_INTERNAL_PIXEL_ITERATOR_BASE_HH

/*! \file mln/core/internal/pixel_iterator_base.hh
 *
 * \brief Base class to factor code for pixel iterator classes.
 */

# include <mln/core/concept/pixel_iterator.hh>
# include <mln/core/internal/pixel_impl.hh>
# include <mln/core/trait/qlf_value.hh>


namespace mln
{

  namespace internal
  {


    /*! \brief A base class for pixel iterators. 
     *
     */
    template <typename I, typename E>
    class pixel_iterator_base_ : public Pixel_Iterator<E>,
				 public internal::pixel_impl_<I, E>
    {
      typedef internal::pixel_impl_<I, E> super_;
 
    public:

      /// Start an iteration.
      void start();

      /// Invalidate the iterator.
      void invalidate();

      /// Test if the iterator is valid.
      bool is_valid() const;

    protected:

      /// Beginning of the image.
      mln_qlf_value(I)* boi_;

      /// End of the image (past-the-end).
      mln_qlf_value(I)* eoi_;

      /// Constructor.
      pixel_iterator_base_(I& image);
    };


#ifndef MLN_INCLUDE_ONLY

    template <typename I, typename E>
    inline
    pixel_iterator_base_<I, E>::pixel_iterator_base_(I& image)
      : super_(image)
    {
      mln_precondition(image.has_data());
      I& ima = this->image_;
      boi_ = & ima( ima.domain().pmin() ) - 1;
      eoi_ = & ima( ima.domain().pmax() ) + 1;
      invalidate();
    }

    // FIXME: Remove cause dangerous when bkd!!!
    template <typename I, typename E>
    inline
    void
    pixel_iterator_base_<I, E>::start()
    {
      this->value_ptr_ = boi_ + 1;
    }

    template <typename I, typename E>
    inline
    void
    pixel_iterator_base_<I, E>::invalidate()
    {
      this->value_ptr_ = eoi_;
    }

    // FIXME: Remove casue not optimal!!!
    template <typename I, typename E>
    inline
    bool
    pixel_iterator_base_<I, E>::is_valid() const
    {
      return this->value_ptr_ != eoi_ && this->value_ptr_ != boi_;
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PIXEL_ITERATOR_BASE_HH
