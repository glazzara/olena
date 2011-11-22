// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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

#ifndef MLN_CORE_INTERNAL_PIXEL_IMPL_HH
# define MLN_CORE_INTERNAL_PIXEL_IMPL_HH

/// \file
///
/// Define a couple of implementation classes to provide methods
/// to classes of generalized pixels.
///
/// \todo Clean-up code.

# include <mln/core/concept/image.hh>
# include <mln/core/internal/force_exact.hh>
# include <mln/util/pix.hh>



namespace mln
{

  namespace internal
  {

    // We indeed have to handle the couple of cases when I is fastest
    // or is not.  Justification: mln::pixel derives from pixel_impl_
    // and is a general purpose pixel class; it can be used on any
    // image whatever it is a fastest one or not.

    template <bool is_fastest, typename I, typename E>
    struct pixel_impl_base_;


    template <typename I, typename E>
    struct pixel_impl_base_< false, I, E > // I is not fastest.
    {
      typedef mlc_if(mlc_is_const(I), const mln_value(I), mln_value(I)) value_ptr_t;

      pixel_impl_base_(I& image, value_ptr_t* value_ptr)
	: image_(image),
	  value_ptr_(value_ptr)
      {
      }

    protected:

      /// Image associated to the iterator
      I& image_;

      /// Current pixel / value
      value_ptr_t* value_ptr_;
    };


    template <typename I, typename E>
    struct pixel_impl_base_< true, I, E > // I is fastest => extra interface.
    {
      typedef mlc_if(mlc_is_const(I), const mln_value(I), mln_value(I)) value_ptr_t;
      typedef mlc_unconst(I) unconst_image_t;

      pixel_impl_base_(I& image, value_ptr_t* value_ptr)
	: image_(image),
	  value_ptr_(value_ptr)
      {
      }

      unsigned offset() const
      {
	return value_ptr_ - image_.buffer();
      }

      operator util::pix<unconst_image_t>() const
      {
	util::pix<unconst_image_t> tmp(image_, image_.point_at_index(offset()));
	return tmp;
      }

    protected:

      /// Image associated to the iterator.
      I& image_;

      /// Current pixel / value.
      value_ptr_t* value_ptr_;
    };


    /// Implementation class to equip generalized pixel
    /// classes based on mutable images.
    ///
    template <typename I, typename E>
    class pixel_impl_

      : public pixel_impl_base_< mlc_is(mln_trait_image_speed(I),
					trait::image::speed::fastest)::value,
				 I, E >
    {
      typedef pixel_impl_base_< mlc_is(mln_trait_image_speed(I),
						 trait::image::speed::fastest)::value,
					  I, E > super_;

    public:

      /// Image type.
      typedef I image;

      /// Image value type.
      typedef mln_value(I) value;

      /// Image lvalue type.
      typedef mln_lvalue(I) lvalue;

      /// Image rvalue type.
      typedef mln_rvalue(I) rvalue;


      /// pixel iterator value.
      lvalue val();

      /// Get the pixel iterator value.
      rvalue val() const;


      /// Give the pixel image.
      I& ima() const;


      /// Address of the current iterator value/pixel.
      value** address_() const;

    protected:

      /// Image associated to the iterator.
      using super_::image_;

      /// Current pixel / value.
      using super_::value_ptr_;

      /// Constructor
      pixel_impl_(I& image);

    private:
      bool is_valid_() const;
    };


    /// Implementation class to equip generalized pixel
    /// classes based on constant images.
    ///
    template <typename I, typename E>
    class pixel_impl_< const I, E >

      : public pixel_impl_base_< mlc_is(mln_trait_image_speed(I),
					trait::image::speed::fastest)::value,
			         const I, E >
    {
      typedef pixel_impl_base_< mlc_is(mln_trait_image_speed(I),
						 trait::image::speed::fastest)::value,
				const I, E > super_;

    public:

      /// Image type.
      typedef const I image;

      /// Image value type.
      typedef mln_value(I) value;

      /// Image rvalue type.
      typedef mln_rvalue(I) rvalue;


      /// Get the pixel iterator value.
      rvalue val() const;


      /// Give the pixel image.
      const I& ima() const;


      /// Address of the current iterator value/pixel.
      const value** address_() const;


    protected:

      /// Image associated to the iterator.
      using super_::image_;

      /// Current pixel / value.
      using super_::value_ptr_;

      /// Constructor
      pixel_impl_(const I& image);

    private:
      bool is_valid_() const;
    };


#ifndef MLN_INCLUDE_ONLY

    // pixel_impl_<I, E>

    template <typename I, typename E>
    inline
    bool
    pixel_impl_<I, E>::is_valid_() const
    {
      return this->value_ptr_ != 0 && internal::force_exact<E>(*this).is_valid();
    }

    template <typename I, typename E>
    inline
    pixel_impl_<I, E>::pixel_impl_(I& image) :
      super_(image, 0)
    {
    }

    template <typename I, typename E>
    inline
    mln_lvalue(I)
    pixel_impl_<I, E>::val()
    {
      mln_precondition(is_valid_());
      return *this->value_ptr_;
    }

    template <typename I, typename E>
    inline
    mln_rvalue(I)
    pixel_impl_<I, E>::val() const
    {
      mln_precondition(is_valid_());
      return *this->value_ptr_;
    }

    template <typename I, typename E>
    inline
    I&
    pixel_impl_<I, E>::ima() const
    {
      // a const pixel, yet a mutable image
      return const_cast<I&>(this->image_);
    }

    template <typename I, typename E>
    inline
    mln_value(I) **
    pixel_impl_<I, E>::address_() const
    {
      return (value**)(& this->value_ptr_);
    }


    // pixel_impl_<const I, E>

    template <typename I, typename E>
    inline
    bool
    pixel_impl_<const I, E>::is_valid_() const
    {
      return this->value_ptr_ != 0 && internal::force_exact<E>(*this).is_valid();
    }

    template <typename I, typename E>
    inline
    pixel_impl_<const I, E>::pixel_impl_(const I& image) :
      super_(image, 0)
    {
    }

    template <typename I, typename E>
    inline
    mln_rvalue(I)
    pixel_impl_<const I, E>::val() const
    {
      mln_precondition(is_valid_());
      return *this->value_ptr_;
    }

    template <typename I, typename E>
    inline
    const I&
    pixel_impl_<const I, E>::ima() const
    {
      return this->image_;
    }

    template <typename I, typename E>
    inline
    const mln_value(I) **
    pixel_impl_<const I, E>::address_() const
    {
      return (const value**)(& this->value_ptr_);
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PIXEL_IMPL_HH
