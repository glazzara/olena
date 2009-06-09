// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

# include <mln/core/concept/object.hh>

# include <mln/trait/all.hh>
# include <mln/trait/value_.hh>

# include <mln/value/ops.hh>

namespace mln
{

  // Fwd decls.
  namespace metal  {
    template <typename T, unsigned Size> struct array1d;
  }
  
  namespace trait
  {
    
    template <typename T, unsigned Size>
    struct value_< mln::metal::array1d<T,Size> >
    {
      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::data        kind;
      
      enum {
        nbits = Size * mln_nbits(T),
        card  = Size * mln_card(T)
      };
      typedef mln_value_quant_from_(card)     quant;

      typedef metal::array1d<mln_sum(T),Size> sum;
    };
    
  } // end of namespace mln::trait

  
  namespace metal
  {

    template <typename T, unsigned Size>
    struct array1d : public Object< array1d<T,Size> >
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

      
      // dynamic accessors:

      T operator[](unsigned i) const {
        mln_precondition(i < Size);
        return buffer_[i];
      }
      T& operator[](unsigned i) {
        mln_precondition(i < Size);
        return buffer_[i];
      }

      // static accessor

      template<unsigned i>
      T get() const {
        return buffer_[i];
      }
      template<unsigned i>
      T& get() {
        return buffer_[i];
      }

      enum { length = Size };
    protected:

      T buffer_[Size];
    };

  }

  namespace trait
  {

    // For unary traits.

    template < template <class> class Name,
	       unsigned n, typename T >
    struct set_precise_unary_< Name, metal::array1d<T, n> >
    {
      typedef mln_trait_unary(Name, T) V;
      typedef metal::array1d<V, n> ret;
    };

    // For binary traits.

    template < template <class, class> class Name,
	       unsigned n, typename T,
	       typename U >
    struct set_precise_binary_< Name,
				metal::array1d<T, n>, metal::array1d<U, n> >
    {
      typedef mln_trait_binary(Name, T, U) V;
      typedef metal::array1d<V, n> ret;
    };

    template < unsigned n, typename T,
	       typename U >
    struct set_precise_binary_< op::times,
				metal::array1d<T, n>, metal::array1d<U, n> >
    {
      typedef mln_sum_product(T,U) ret;
    };

    template < template <class, class> class Name,
	       unsigned n, typename T,
	       typename S >
    struct set_precise_binary_< Name,
				metal::array1d<T, n>, mln::value::scalar_<S> >
    {
      typedef mln_trait_binary(Name, T, S) V;
      typedef metal::array1d<V, n> ret;
    };

    template < template<class, class> class Name,
	       unsigned n, typename T,
	       typename S >
    struct set_binary_< Name,
			mln::Object, metal::array1d<T, n>,
			mln::value::Scalar, S >
    {
      typedef mln_trait_binary(Name, T, S) V;
      typedef metal::array1d<T, n> ret;
    };

  } // end of namespace mln::trait


  namespace metal
  {

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

#endif // ! MLN_METAL_ARRAY1D_HH
