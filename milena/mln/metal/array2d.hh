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

#ifndef MLN_METAL_ARRAY2D_HH
# define MLN_METAL_ARRAY2D_HH

# include <mln/core/concept/object.hh>

# include <mln/trait/all.hh>
# include <mln/trait/value_.hh>

# include <mln/value/ops.hh>

namespace mln
{

  // Fwd decls.
  namespace metal  {
    template <typename T, unsigned r, unsigned c> struct array2d;
  }
  
  namespace trait
  {
    
    template <typename T, unsigned r, unsigned c>
    struct value_< mln::metal::array2d<T, r, c> >
    {
      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::data        kind;
      
      enum {
        nbits = r * c * mln_nbits(T),
        card  = r * c * mln_card(T)
      };
      typedef mln_value_quant_from_(card)     quant;

      typedef metal::array2d<mln_sum(T),r, c> sum;
    };
    
  } // end of namespace mln::trait

  
  namespace metal
  {
    
    template <typename T, unsigned r, unsigned c>
    struct array2d : public Object< array2d<T, r, c> >
    {

      //
      // Constructors
      //

      array2d();
      array2d(T* ptr);

      // Copy

      array2d(const array2d<T, r, c>& rhs);

      array2d<T, r, c>& operator=(const array2d<T, r, c>& rhs);

      // Operators

      template <class U>
      array2d<T, r, c> operator*(U w);

      template <class U>
      array2d<mln_trait_op_div(T,U), r, c>
      operator/(U w);

      template <typename U>
      array2d<mln_trait_op_plus(T,U), r, c>
      operator+(const array2d<U, r, c>& rhs) const;
      array2d<T, r, c>& operator+=(const array2d<T, r, c>& rhs);

      template <typename U>
      array2d<mln_trait_op_minus(T,U), r, c>
                                   operator-(const array2d<U, r, c>& rhs) const;
      array2d<T, r, c>&
      operator-=(const array2d<T, r, c>& rhs);

      // dynamic accessors:

      T operator()(unsigned row, unsigned col) const {
        mln_precondition(row < r * c);
        return buffer_[col * r + row];
      }
      T& operator()(unsigned row, unsigned col) {
        mln_precondition(row < r * c);
        return buffer_[col * r + row];
      }

      // static accessor

      template<unsigned row, unsigned col>
      T get() const {
        return buffer_[col * r + row];
      }
      template<unsigned row, unsigned col>
      T& get() {
        return buffer_[col * r + row];
      }

      template<unsigned row, unsigned col>
      T get_at() const {
        mln_precondition(col * r + row < r *c);
        return buffer_[col * r + row];
      }
      template<unsigned row, unsigned col>
      T& get_at() {
        mln_precondition(col * r + row < r *c);
        return buffer_[col * r + row];
      }

      enum { length = r * c };
    protected:

      T buffer_[r * c];
    };

  }

  namespace trait
  {

    // For unary traits.

    template < template <class> class Name,
	       unsigned r, unsigned c, typename T >
    struct set_precise_unary_< Name, metal::array2d<T, r, c> >
    {
      typedef mln_trait_unary(Name, T) V;
      typedef metal::array2d<V, r, c> ret;
    };

    // For binary traits.

    template < template <class, class> class Name,
	       unsigned r, unsigned c, typename T,
	       typename U >
    struct set_precise_binary_< Name,
				metal::array2d<T, r, c>, metal::array2d<U, r, c> >
    {
      typedef mln_trait_binary(Name, T, U) V;
      typedef metal::array2d<V, r, c> ret;
    };

    template < unsigned r, unsigned c, typename T,
	       typename U >
    struct set_precise_binary_< op::times,
				metal::array2d<T, r, c>, metal::array2d<U, r, c> >
    {
      typedef mln_sum_x(T,U) ret;
    };

    template < template <class, class> class Name,
	       unsigned r, unsigned c, typename T,
	       typename S >
    struct set_precise_binary_< Name,
				metal::array2d<T, r, c>, mln::value::scalar_<S> >
    {
      typedef mln_trait_binary(Name, T, S) V;
      typedef metal::array2d<V, r, c> ret;
    };

    template < template<class, class> class Name,
	       unsigned r, unsigned c, typename T,
	       typename S >
    struct set_binary_< Name,
			mln::Object, metal::array2d<T, r, c>,
			mln::value::Scalar, S >
    {
      typedef mln_trait_binary(Name, T, S) V;
      typedef metal::array2d<T, r, c> ret;
    };

  } // end of namespace mln::trait


  namespace metal
  {

    //
    // Constructors
    //

    template <typename T, unsigned r, unsigned c>
    array2d<T, r, c>::array2d()
    {
    }

    template <typename T, unsigned r, unsigned c>
    array2d<T, r, c>::array2d(T* ptr)
    {
      for (unsigned i = 0; i < r * c; ++i)
        buffer_[i] = *ptr++;
    }

    // Copy

    template <typename T, unsigned r, unsigned c>
    array2d<T, r, c>::array2d(const array2d<T, r, c>& rhs)
    {
      for (unsigned i = 0; i < r * c; ++i)
        buffer_[i] = rhs[i];
    }
    template <typename T, unsigned r, unsigned c>
    array2d<T, r, c>&
    array2d<T, r, c>::operator=(const array2d<T, r, c>& rhs)
    {
      for (unsigned i = 0; i < r * c; ++i)
        buffer_[i] = rhs[i];
      return *this;
    }

    // Operators

    template <typename T, unsigned r, unsigned c>
    template <class U>
    array2d<T, r, c>
    array2d<T, r, c>::operator*(U w)
    {
      //fixme mln_trait_op_mult<int,U>
      array2d<T, r, c> tmp;
      for (unsigned i = 0; i < r * c; ++i)
        tmp[i] = this->buffer_[i] * w;
      return tmp;
    }

    template <typename T, unsigned r, unsigned c>
    template <class U>
    array2d<mln_trait_op_div(T,U), r, c>
    array2d<T,r, c>::operator/(U w)
    {
      array2d<T, r, c> tmp;
      for (unsigned i = 0; i < r * c; ++i)
        tmp[i] = this->buffer_[i] / w;
      return tmp;
    }

    template <typename T, unsigned r, unsigned c>
    template <typename U>
    array2d<mln_trait_op_plus(T,U), r, c>
    array2d<T,r, c>::operator+(const array2d<U, r, c>& rhs) const
    {
      array2d<T, r, c> tmp;
      for (unsigned i = 0; i < r * c; ++i)
        tmp[i] = this->buffer_[i] + rhs.buffer_[i];
      return tmp;
    }
    template <typename T, unsigned r, unsigned c>
    array2d<T, r, c>&
    array2d<T, r, c>::operator+=(const array2d<T, r, c>& rhs)
    {
      for (unsigned i = 0; i < r * c; ++i)
        this->buffer_[i] += rhs.buffer_[i];
      return *this;
    }

    template <typename T, unsigned r, unsigned c>
    template <typename U>
    array2d<mln_trait_op_minus(T,U), r, c>
    array2d<T,r, c>::operator-(const array2d<U, r, c>& rhs) const
    {
      array2d<T, r, c> tmp;
      for (unsigned i = 0; i < r * c; ++i)
        tmp[i] = this->buffer_[i] - rhs.buffer_[i];
      return tmp;
    }
    template <typename T, unsigned r, unsigned c>
    array2d<T, r, c>&
    array2d<T, r, c>::operator-=(const array2d<T, r, c>& rhs)
    {
      for (unsigned i = 0; i < r * c; ++i)
        this->buffer_[i] -= rhs.buffer_[i];
      return *this;
    }

  } // end of namespace metal

} // end of namespace mln

#endif /* MLN_METAL_ARRAY2D_HH */

