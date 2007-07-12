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

#ifndef MLN_CORE_INTERNAL_FAST_ITERATOR_BASE_HH
# define MLN_CORE_INTERNAL_FAST_ITERATOR_BASE_HH

/*! \file mln/core/internal/fast_iterator_base.hh
 *
 * \brief Base class for Fast_Iterator concept implementation.
 *        Usefull for code factorisation
 */

# include <mln/core/concept/fast_iterator.hh>


namespace mln
{

  namespace internal
  {


    /*! \brief fast_iterator_base_ class
     */
    template <typename Exact, typename Image>
    class fast_iterator_base_ : public Fast_Iterator<Exact>
    {
    public:
      /// Image pixel value type.
      typedef mln_value(Image) value;
      /// Image pixel rvalue type.
      typedef mln_value(Image)& rvalue;
      /// Image pixel lvalue type
      typedef mln_value(Image) lvalue;

      // Go to the beginning of the image.
      void start();
      // Go on the next element.
      void next_();
      // Invalidate the iterator.
      void invalidate();
      // Is the iterator referencing a correct position in the image?
      bool is_valid() const;


      // Return the current pixel.
      rvalue operator* ();
      lvalue operator* () const;

    protected:
      // beginning of the image
      value* start_;
      // end of the image
      value* eoi_;
      // current position in the image
      value* current_;
      // End of Factoriasable

      fast_iterator_base_();
    };

#ifndef MLN_INCLUDE_ONLY

    template <typename Exact, typename Image>
    fast_iterator_base_<Exact, Image>::fast_iterator_base_()
    {
    }

    template <typename Exact, typename Image>
    void fast_iterator_base_<Exact, Image>::start()
    {
      current_ = start_;
    }

    template <typename Exact, typename Image>
    void fast_iterator_base_<Exact, Image>::next_()
    {
      ++current_;
    }

    template <typename Exact, typename Image>
    void fast_iterator_base_<Exact, Image>::invalidate()
    {
      current_ = eoi_;
    }

    template <typename Exact, typename Image>
    bool fast_iterator_base_<Exact, Image>::is_valid() const
    {
      return (current_ != eoi_);
    }

    template <typename Exact, typename Image>
    typename fast_iterator_base_<Exact, Image>::rvalue
    fast_iterator_base_<Exact, Image>::operator*()
    {
      return *current_;
    }


    template <typename Exact, typename Image>
    typename fast_iterator_base_<Exact, Image>::lvalue
    fast_iterator_base_<Exact, Image>::operator*() const
    {
      return *current_;
    }

#endif // ! MLN_INCLUDE_ONLY
  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_FAST_ITERATOR_BASE_HH
