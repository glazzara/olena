// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_FLOAT01__HH
# define MLN_VALUE_FLOAT01__HH

/*! \file mln/value/float01_.hh
 *
 * \brief FIXME.
 */

# include <iostream>
# include <mln/core/contract.hh>
# include <mln/metal/math/pow.hh>
# include <mln/metal/bexpr.hh>

# include <mln/value/int_u.hh>
# include <mln/value/concept/floating.hh>
# include <mln/value/float01.hh>
# include <mln/value/props.hh>



namespace mln
{

  namespace value
  {

    /// Fwd decl.
    class float01;


    /// General float01_ class on n bits.
    template <unsigned n>
    struct float01_

      : public Floating< float01_<n> >,

	public internal::value_like_< int_u<n>,          // Equivalent. // FIXME: Why not float01?
				      mln_enc(int_u<n>), // Encoding.
				      float,             // Interoperation.
				      float01_<n> >      // Exact.
    {
      /// Ctor.
      float01_();

      /// Ctor.
      float01_(float val);

      /// Access to std type.
      float value() const;

      /// Set value to the \p val th position in the quantized interval.
      void set_ind(unsigned long val);

      /// Float convertion.
      operator float() const;

      /// Float assigment.
      float01_<n>& operator=(float val);
    };


    namespace internal
    {

      template <unsigned n>
      struct convert_< float01_<n> >
      {
	static float01_<n> value_at_index(std::size_t i)
	{
	  float01_<n> tmp;
	  tmp.set_ind(i);
	  return tmp;
	}

	static std::size_t index_of_value(const float01_<n>& v)
	{
	  return v.to_enc();
	}
      };
    }

    template <unsigned n>
    struct props< float01_<n> >
    {
      static const std::size_t card_ = metal::math::pow_int<2, n>::value;
      static const float min() { return 0.f; }
      static const float max() { return 1.f; }
      static const unsigned nbits = n;
      typedef trait::value::kind::data kind;
      typedef float sum;
      typedef float interop;
    };


    /// Op<<.
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const float01_<n>& f);


    template <unsigned n, unsigned m>
    bool approx_equal(const float01_<n>& lhs, const float01_<m>& rhs);

    template <unsigned n>
    bool approx_equal(const float01_<n>& lhs, const float f);



# ifndef MLN_INCLUDE_ONLY

    // Float01_<n>.

    template <unsigned n>
    float01_<n>::float01_()
    {
    }

    template <unsigned n>
    float01_<n>::float01_(float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->v_ = int(val * (mln_card_(float01_<n>) - 1)); // FIXME
    }

    template <unsigned n>
    float
    float01_<n>::value() const
    {
      return float(this->v_) / (mln_card_(float01_<n>) - 1); // FIXME
    }

    template <unsigned n>
    void
    float01_<n>::set_ind(unsigned long val)
    {
      this->v_ = val;
    }

    template <unsigned n>
    float01_<n>&
    float01_<n>::operator=(float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->v_ = int(val * (mln_card_(float01_<n>) - 1)); // FIXME
      return *this;
    }

    template <unsigned n>
    float01_<n>::operator float() const
    {
      return float(this->v_) / (mln_card_(float01_<n>) - 1);
    }


    // Operators.

    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const float01_<n>& f)
    {
      return ostr << f.value();
    }

    template <unsigned n, unsigned m>
    bool approx_equal(const float01_<n>& lhs, const float01_<m>& rhs)
    {
      return float01(lhs) == float01(rhs);
    }

    template <unsigned n>
    bool approx_equal(const float01_<n>& lhs, float f)
    {
      return float01(lhs) == float01_<n>(f);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_FLOAT01__HH
