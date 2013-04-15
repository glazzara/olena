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

#ifndef MLN_CORE_INTERNAL_PIXEL_ITERATOR_BASE_HH
# define MLN_CORE_INTERNAL_PIXEL_ITERATOR_BASE_HH

/// \file
///
/// Base classes factoring code for pixel iterator classes.

# include <mln/core/concept/pixel_iterator.hh>
# include <mln/core/internal/pixel_impl.hh>
# include <mln/core/trait/qlf_value.hh>


namespace mln
{

  namespace internal
  {

    /*---------------------------------------.
    | internal::pixel_iterator_base_<I, E>.  |
    `---------------------------------------*/

    /*!
      \internal
      \brief A base class for pixel iterators.
    */
    template <typename I, typename E>
    class pixel_iterator_base_ : public Pixel_Iterator<E>,
				 public internal::pixel_impl_<I, E>
    {
      typedef internal::pixel_impl_<I, E> super_;

    protected:
      /// Constructor.
      pixel_iterator_base_(I& image);

    protected:

      /// Beginning of the image.
      mln_qlf_value(I)* boi_;

      /// End of the image (past-the-end).
      mln_qlf_value(I)* eoi_;

      /// Default impl is no-op.
      void start_();
    };


    /*-----------------------------------------------.
    | internal::forward_pixel_iterator_base_<I, E>.  |
    `-----------------------------------------------*/

    /*!
      \internal
      \brief A base class for forward pixel iterators.
    */
    template <typename I, typename E>
    class forward_pixel_iterator_base_ : public pixel_iterator_base_<I, E>
    {
      typedef pixel_iterator_base_<I, E> super_;

    public:

      /// Manipulation
      /// \{
      /// Start an iteration.
      void start();
      /// Invalidate the iterator.
      void invalidate();
      /// Test if the iterator is valid.
      bool is_valid() const;
      /// \}

    protected:

      /// Constructor.
      forward_pixel_iterator_base_(I& image);
    };


    /*------------------------------------------------.
    | internal::backward_pixel_iterator_base_<I, E>.  |
    `------------------------------------------------*/

    /*!
      \internal
      \brief A base class for backward pixel iterators.
    */
    template <typename I, typename E>
    class backward_pixel_iterator_base_ : public pixel_iterator_base_<I, E>
    {
      typedef pixel_iterator_base_<I, E> super_;

    public:
      /// Manipulation
      /// \{
      /// Start an iteration.
      void start();
      /// Invalidate the iterator.
      void invalidate();
      /// Test if the iterator is valid.
      bool is_valid() const;
      /// \}

    protected:

      /// Constructor.
      backward_pixel_iterator_base_(I& image);
    };



#ifndef MLN_INCLUDE_ONLY


    /*---------------------------------------.
    | internal::pixel_iterator_base_<I, E>.  |
    `---------------------------------------*/

    template <typename I, typename E>
    inline
    pixel_iterator_base_<I, E>::pixel_iterator_base_(I& image)
      : super_(image)
    {
      mln_precondition(image.is_valid());
      I& ima = this->image_;
      boi_ = & ima( ima.domain().pmin() ) - 1;
      eoi_ = & ima( ima.domain().pmax() ) + 1;
      exact(*this).invalidate();
    }

    template <typename I, typename E>
    inline
    void
    pixel_iterator_base_<I, E>::start_()
    {
      // Default impl is no-op.
    }


    /*-----------------------------------------------.
    | internal::forward_pixel_iterator_base_<I, E>.  |
    `-----------------------------------------------*/

    template <typename I, typename E>
    inline
    forward_pixel_iterator_base_<I, E>::forward_pixel_iterator_base_(I& image)
      : super_(image)
    {
    }

    template <typename I, typename E>
    inline
    void
    forward_pixel_iterator_base_<I, E>::start()
    {
      this->value_ptr_ = this->boi_ + 1;
      exact(this)->start_();
    }

    template <typename I, typename E>
    inline
    void
    forward_pixel_iterator_base_<I, E>::invalidate()
    {
      this->value_ptr_ = this->eoi_;
    }

    template <typename I, typename E>
    inline
    bool
    forward_pixel_iterator_base_<I, E>::is_valid() const
    {
      return this->value_ptr_ != this->eoi_;
    }


    /*------------------------------------------------.
    | internal::backward_pixel_iterator_base_<I, E>.  |
    `------------------------------------------------*/

    template <typename I, typename E>
    inline
    backward_pixel_iterator_base_<I, E>::backward_pixel_iterator_base_(I& image)
      : super_(image)
    {
    }

    template <typename I, typename E>
    inline
    void
    backward_pixel_iterator_base_<I, E>::start()
    {
      this->value_ptr_ = this->eoi_ - 1;
      exact(this)->start_();
    }

    template <typename I, typename E>
    inline
    void
    backward_pixel_iterator_base_<I, E>::invalidate()
    {
      this->value_ptr_ = this->boi_;
    }

    template <typename I, typename E>
    inline
    bool
    backward_pixel_iterator_base_<I, E>::is_valid() const
    {
      return this->value_ptr_ != this->boi_;
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PIXEL_ITERATOR_BASE_HH
