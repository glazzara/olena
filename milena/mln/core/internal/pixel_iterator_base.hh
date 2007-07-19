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
 * \brief Base class for Pixel_Iterator concept implementation classes.
 */

# include <mln/core/concept/pixel_iterator.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief pixel_iterator_base_ class
     *
     */
    template <typename I, typename E>
    class pixel_iterator_base_ : public Pixel_Iterator<E>
    {
    public:

      /// Image value type.
      typedef mln_value(I) value;

      /// Image lvalue type.
      typedef mln_lvalue(I) lvalue;

      /// Image rvalue type.
      typedef mln_rvalue(I) rvalue;


      /// pixel iterator value.
      lvalue operator* ();

      /// Get the pixel iterator value.
      rvalue operator* () const;


      /// Address of the current iterator value/pixel.
      value** address() const;


      // FIXME: Inactivated:

//       /// I type.
//       typedef I ima;

//       /// Image psite type.
//       typedef mln_psite(I) psite;

//       /// Get the image associated to the current pixel iterator.
//       const ima& image() const;

//       /// psite associated to the iterator.
//       const psite& site() const;

//       /// psite associated to the iterator.
//       psite& site();

//       /// Address of the current iterator value/pixel.
//       value* address();

    protected:

      /// Image associated to the iterator
      I& image_;

      /// Current pixel / value
      value* value_ptr_;

      // FIXME: Inactivated:

//       /// Psite of the pixel
//       psite  p_;

      pixel_iterator_base_(I& image);
    };

#ifndef MLN_INCLUDE_ONLY

    template <typename I, typename E>
    pixel_iterator_base_<I, E>::pixel_iterator_base_(I& image) :
      image_(image),
      value_ptr_(0)
    {
    }

//     template <typename I, typename E>
//     const typename pixel_iterator_base_<I, E>::ima&
//     pixel_iterator_base_<I, E>::image() const
//     {
//       return ima_;
//     }

    template <typename I, typename E>
    mln_lvalue(I)
    pixel_iterator_base_<I, E>::operator* ()
    {
      mln_precondition(exact(this)->is_valid());
      mln_precondition(value_ptr_ != 0);
      return *value_ptr_;
    }

    template <typename I, typename E>
    mln_rvalue(I)
    pixel_iterator_base_<I, E>::operator* () const
    {
      mln_precondition(exact(this)->is_valid());
      mln_precondition(value_ptr_ != 0);
      return *value_ptr_;
    }

    template <typename I, typename E>
    mln_value(I) **
    pixel_iterator_base_<I, E>::address() const
    {
      mln_precondition(exact(this)->is_valid());
      mln_precondition(value_ptr_ != 0);
      return & (mln_value(I)*)(value_ptr_);
    }

//     template <typename I, typename E>
//     mln_value(I)*
//     pixel_iterator_base_<I, E>::address()
//     {
//       mln_precondition(exact(this)->is_valid());
//       return value_ptr_;
//     }

//     template <typename I, typename E>
//     const typename pixel_iterator_base_<I, E>::psite&
//     pixel_iterator_base_<I, E>::site() const
//     {
//       //FIXME: update psite
//       return p_;
//     }

//     template <typename I, typename E>
//     typename pixel_iterator_base_<I, E>::psite&
//     pixel_iterator_base_<I, E>::site()
//     {
//       //FIXME: update psite
//       return p_;
//     }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PIXEL_ITERATOR_BASE_HH
