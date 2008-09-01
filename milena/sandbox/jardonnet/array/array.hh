// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_METAL_ARRAY1D_HH
# define MLN_METAL_ARRAY1D_HH

# include <mln/trait/all.hh>

namespace mln
{

  namespace metal
  {

    template <typename T, unsigned Size>
    struct array1d
    {

      //
      // Constructors
      //

      array1d();
      array1d(T* ptr);

      // Copy

      array1d(const array1d<T, Size>& rhs);
      array1d<T, Size>& operator=(const array1d<T, Size>& rhs);

      // Operators

      template <class U>
      array1d<T, Size> operator*(U w);

      template <class U>
      array1d<mln_trait_op_div(T,U), Size>
      operator/(U w);

      template <typename U>
      array1d<mln_trait_op_plus(T,U), Size>
      operator+(const array1d<U, Size>& rhs) const;
      array1d<T, Size>& operator+=(const array1d<T, Size>& rhs);

      template <typename U>
      array1d<mln_trait_op_minus(T,U), Size>
      operator-(const array1d<U, Size>& rhs) const;
      array1d<T, Size>&
      operator-=(const array1d<T, Size>& rhs);

    protected:

      T buffer_[Size];
    };

    //
    // Constructors
    //

    template <typename T, unsigned Size>
    array1d<T,Size>::array1d()
    {
    }

    template <typename T, unsigned Size>
    array1d<T,Size>::array1d(T* ptr)
    {
      for (unsigned i = 0; i < Size; ++i)
        buffer_[i] = *ptr++;
    }

    // Copy

    template <typename T, unsigned Size>
    array1d<T,Size>::array1d(const array1d<T, Size>& rhs)
    {
      for (unsigned i = 0; i < Size; ++i)
        buffer_[i] = rhs[i];
    }
    template <typename T, unsigned Size>
    array1d<T, Size>&
    array1d<T,Size>::operator=(const array1d<T, Size>& rhs)
    {
      for (unsigned i = 0; i < Size; ++i)
        buffer_[i] = rhs[i];
      return *this;
    }

    // Operators

    template <typename T, unsigned Size>
    template <class U>
    array1d<T, Size>
    array1d<T,Size>::operator*(U w)
    {
      //fixme mln_trait_op_mult<int,U>
      array1d<T, Size> tmp;
      for (unsigned i = 0; i < Size; ++i)
        tmp[i] = this->buffer_[i] * w;
      return tmp;
    }

    template <typename T, unsigned Size>
    template <class U>
    array1d<mln_trait_op_div(T,U), Size>
    array1d<T,Size>::operator/(U w)
    {
      array1d<T, Size> tmp;
      for (unsigned i = 0; i < Size; ++i)
        tmp[i] = this->buffer_[i] / w;
      return tmp;
    }

    template <typename T, unsigned Size>
    template <typename U>
    array1d<mln_trait_op_plus(T,U), Size>
    array1d<T,Size>::operator+(const array1d<U, Size>& rhs) const
    {
      array1d<T, Size> tmp;
      for (unsigned i = 0; i < Size; ++i)
        tmp[i] = this->buffer_[i] + rhs.buffer_[i];
      return tmp;
    }
    template <typename T, unsigned Size>
    array1d<T, Size>&
    array1d<T,Size>::operator+=(const array1d<T, Size>& rhs)
    {
      for (unsigned i = 0; i < Size; ++i)
        this->buffer_[i] += rhs.buffer_[i];
      return *this;
    }

    template <typename T, unsigned Size>
    template <typename U>
    array1d<mln_trait_op_minus(T,U), Size>
    array1d<T,Size>::operator-(const array1d<U, Size>& rhs) const
    {
      array1d<T, Size> tmp;
      for (unsigned i = 0; i < Size; ++i)
        tmp[i] = this->buffer_[i] - rhs.buffer_[i];
      return tmp;
    }
    template <typename T, unsigned Size>
    array1d<T, Size>&
    array1d<T,Size>::operator-=(const array1d<T, Size>& rhs)
    {
      for (unsigned i = 0; i < Size; ++i)
        this->buffer_[i] -= rhs.buffer_[i];
      return *this;
    }

  } // end of namespace metal

} // end of namespace mln

#endif /* MLN_METAL_ARRAY1D_HH */

