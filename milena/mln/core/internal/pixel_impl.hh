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

#ifndef MLN_CORE_INTERNAL_PIXEL_IMPL_HH
# define MLN_CORE_INTERNAL_PIXEL_IMPL_HH

/*! \file mln/core/internal/pixel_impl.hh
 *
 * \brief Define a couple of implementation classes to provide methods
 * to classes of generalized pixels.
 */

# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief Implementation class to equip generalized pixel
     *  classes based on mutable images.
     *
     * \internal
     */
    template <typename I, typename E>
    class pixel_impl_
    {
    public:

      /// Image value type.
      typedef mln_value(I) value;

      /// Image lvalue type.
      typedef mln_lvalue(I) lvalue;

      /// Image rvalue type.
      typedef mln_rvalue(I) rvalue;

      /// Qualified value type.
      typedef value value_;


      /// pixel iterator value.
      lvalue operator*();

      /// Get the pixel iterator value.
      rvalue operator*() const;


      /// Address of the current iterator value/pixel.
      value** address_() const;

    protected:

      /// Image associated to the iterator
      I& image_;

      /// Current pixel / value
      value* value_ptr_;

      /// Constructor
      pixel_impl_(I& image);

    private:
      bool is_valid_() const;
    };


    /*! \brief Implementation class to equip generalized pixel
     *  classes based on constant images.
     *
     * \internal
     */
    template <typename I, typename E>
    class pixel_impl_< const I, E >
    {
    public:

      /// Image value type.
      typedef mln_value(I) value;

      /// Image rvalue type.
      typedef mln_rvalue(I) rvalue;

      /// Qualified value type.
      typedef const value value_;


      /// Get the pixel iterator value.
      rvalue operator*() const;


      /// Address of the current iterator value/pixel.
      const value** address_() const;

    protected:

      /// Image associated to the iterator
      const I& image_;

      /// Current pixel / value
      const value* value_ptr_;

      /// Constructor
      pixel_impl_(const I& image);

    private:
      bool is_valid_() const;
    };


#ifndef MLN_INCLUDE_ONLY

    // pixel_impl_<I, E>

    template <typename I, typename E>
    bool
    pixel_impl_<I, E>::is_valid_() const
    {
      return value_ptr_ != 0 && internal::force_exact<E>(*this).is_valid();
    }

    template <typename I, typename E>
    pixel_impl_<I, E>::pixel_impl_(I& image) :
      image_(image),
      value_ptr_(0)
    {
    }

    template <typename I, typename E>
    mln_lvalue(I)
    pixel_impl_<I, E>::operator*()
    {
      mln_precondition(is_valid_());
      return *value_ptr_;
    }

    template <typename I, typename E>
    mln_rvalue(I)
    pixel_impl_<I, E>::operator*() const
    {
      mln_precondition(is_valid_());
      return *value_ptr_;
    }

    template <typename I, typename E>
    mln_value(I) **
    pixel_impl_<I, E>::address_() const
    {
      mln_precondition(is_valid_());
      return (value**)(& this->value_ptr_);
    }


    // pixel_impl_<const I, E>

    template <typename I, typename E>
    bool
    pixel_impl_<const I, E>::is_valid_() const
    {
      return value_ptr_ != 0 && internal::force_exact<E>(*this).is_valid();
    }

    template <typename I, typename E>
    pixel_impl_<const I, E>::pixel_impl_(const I& image) :
      image_(image),
      value_ptr_(0)
    {
    }

    template <typename I, typename E>
    mln_rvalue(I)
    pixel_impl_<const I, E>::operator*() const
    {
      mln_precondition(is_valid_());
      return *value_ptr_;
    }

    template <typename I, typename E>
    const mln_value(I) **
    pixel_impl_<const I, E>::address_() const
    {
      mln_precondition(is_valid_());
      return (const value**)(& this->value_ptr_);
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PIXEL_IMPL_HH
