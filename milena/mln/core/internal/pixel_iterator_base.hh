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

/*! \file mln/core/internal/fast_iterator_base.hh
 *
 * \brief Base class for Fast_Iterator concept implementation.
 *        Usefull for code factorisation
 */

# include <mln/core/concept/pixel_iterator.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief pixel_iterator_base_ class
     */
    template <typename Image, typename Exact>
    class pixel_iterator_base_ : public Pixel_Iterator<Exact>
    {
    public:
      /// Image type.
      typedef Image ima;
      /// Image value type.
      typedef mln_value(Image) value;
      /// Image lvalue type.
      typedef mln_lvalue(Image) lvalue;
      /// Image rvalue type.
      typedef mln_rvalue(Image) rvalue;
      /// Image psite type.
      typedef mln_psite(Image) psite;

      /// Get the image associated to the current pixel iterator.
      const ima& image() const;
      /// pixel iterator value.
      lvalue operator* ();
      /// Get the pixel iterator value.
      rvalue operator* () const;
      /// Address of the current iterator value/pixel.
      const value* address() const;
      /// Address of the current iterator value/pixel.
      value* address();

      /// psite associated to the iterator.
      const psite& site() const;
      /// psite associated to the iterator.
      psite& site();

    protected:
      /// Current pixel value
      value* current_value_;
      /// Image associated to the iterator
      ima&   ima_;
      /// Psite of the pixel
      psite  p_;

      pixel_iterator_base_(ima& source);
    };

#ifndef MLN_INCLUDE_ONLY

    template <typename Image, typename Exact>
    pixel_iterator_base_<Image, Exact>::pixel_iterator_base_(ima& source) :
      ima_(source)
    {
    }

    template <typename Image, typename Exact>
    const typename pixel_iterator_base_<Image, Exact>::ima&
    pixel_iterator_base_<Image, Exact>::image() const
    {
      return ima_;
    }

    template <typename Image, typename Exact>
    typename pixel_iterator_base_<Image, Exact>::lvalue
    pixel_iterator_base_<Image, Exact>::operator* ()
    {
      assert(exact(this)->is_valid());
      return *current_value_;
    }

    template <typename Image, typename Exact>
    typename pixel_iterator_base_<Image, Exact>::rvalue
    pixel_iterator_base_<Image, Exact>::operator* () const
    {
      assert(exact(this)->is_valid());
      return *current_value_;
    }

    template <typename Image, typename Exact>
    const typename pixel_iterator_base_<Image, Exact>::value*
    pixel_iterator_base_<Image, Exact>::address() const
    {
      assert(exact(this)->is_valid());
      return current_value_;
    }

    template <typename Image, typename Exact>
    typename pixel_iterator_base_<Image, Exact>::value*
    pixel_iterator_base_<Image, Exact>::address()
    {
      assert(exact(this)->is_valid());
      return current_value_;
    }

    template <typename Image, typename Exact>
    const typename pixel_iterator_base_<Image, Exact>::psite&
    pixel_iterator_base_<Image, Exact>::site() const
    {
      //FIXME: update psite
      return p_;
    }

    template <typename Image, typename Exact>
    typename pixel_iterator_base_<Image, Exact>::psite&
    pixel_iterator_base_<Image, Exact>::site()
    {
      //FIXME: update psite
      return p_;
    }


// FIXME: Dead code
//     /*! \brief pixel_iterator_base_ class
//      */
//     template <typename Exact, typename Image>
//     class pixel_iterator_base_ : public Pixel_Iterator<Exact>
//     {
//     public:
//       /// Image pixel value type.
//       typedef mln_value(Image) value;
//       /// Image pixel rvalue type.
//       typedef mln_value(Image)& rvalue;
//       /// Image pixel lvalue type
//       typedef mln_value(Image) lvalue;

//       // Go to the beginning of the image.
//       void start();
//       // Go on the next element.
//       void next_();
//       // Invalidate the iterator.
//       void invalidate();
//       // Is the iterator referencing a correct position in the image?
//       bool is_valid() const;


//       // Return the current pixel.
//       rvalue operator* ();
//       lvalue operator* () const;

//     protected:
//       // beginning of the image
//       value* start_;
//       // end of the image
//       value* eoi_;
//       // current position in the image
//       value* current_;
//       // End of Factoriasable

//       pixel_iterator_base_();
//     };

// #ifndef MLN_INCLUDE_ONLY

//     template <typename Exact, typename Image>
//     pixel_iterator_base_<Exact, Image>::pixel_iterator_base_()
//     {
//     }

//     template <typename Exact, typename Image>
//     void pixel_iterator_base_<Exact, Image>::start()
//     {
//       current_ = start_;
//     }

//     template <typename Exact, typename Image>
//     void pixel_iterator_base_<Exact, Image>::next_()
//     {
//       ++current_;
//     }

//     template <typename Exact, typename Image>
//     void pixel_iterator_base_<Exact, Image>::invalidate()
//     {
//       current_ = eoi_;
//     }

//     template <typename Exact, typename Image>
//     bool pixel_iterator_base_<Exact, Image>::is_valid() const
//     {
//       return (current_ != eoi_);
//     }

//     template <typename Exact, typename Image>
//     typename pixel_iterator_base_<Exact, Image>::rvalue
//     pixel_iterator_base_<Exact, Image>::operator*()
//     {
//       return *current_;
//     }


//     template <typename Exact, typename Image>
//     typename pixel_iterator_base_<Exact, Image>::lvalue
//     pixel_iterator_base_<Exact, Image>::operator*() const
//     {
//       return *current_;
//     }

#endif // ! MLN_INCLUDE_ONLY
  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PIXEL_ITERATOR_BASE_HH
