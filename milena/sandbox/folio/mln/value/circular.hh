// Copyright (C) 2006, 2007, 2008 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_VALUE_CIRCULAR_HH
# define MLN_VALUE_CIRCULAR_HH

/*! \file mln/value/circular.hh
 *
 * \brief FIXME.
 */

# include <iostream>
# include <math.h>
# include <mln/core/contract.hh>
# include <mln/metal/math/pow.hh>
# include <mln/metal/bexpr.hh>

# include <mln/value/int_u.hh>
# include <mln/value/concept/floating.hh>
# include <mln/value/internal/convert.hh>
# include <mln/trait/value_.hh>



namespace mln
{

  // Fwd decls.
  namespace value {
    template <unsigned n, int inf, int sup> struct circular;
  }


  namespace trait
  {

    template <unsigned n, int inf, int sup>
    struct value_< mln::value::circular<n, inf, sup> >
    {
      enum constants_ {
	dim = 1,
	nbits = n,
	card  = mln_value_card_from_(nbits)
      };

      typedef trait::value::nature::floating nature;
      typedef trait::value::kind::data       kind;
      typedef mln_value_quant_from_(card)    quant;

      static float min() { return 0.f; }
      static float max() { return 1.f; }
      static float epsilon() { return 0.f; }

      typedef float comp;

      typedef float sum;
    };

  } // end of namespace trait


  namespace value
  {

    /// General class for the interval [0,1] of |R made discrete (quantized with n bits).
    template <unsigned n, int inf, int sup>
    struct circular

      : public Floating< circular<n, inf, sup> >,

	public internal::value_like_< float,             // Equivalent. // FIXME: Why not circular?
				      mln_enc(int_u<n>), // Encoding.
				      float,             // Interoperation.
				      circular<n, inf, sup> >      // Exact.
    {
      /// Constructor without argument.
      circular();

      /// Constructor from a float.
      circular(float val);

      /// Assigment from a float.
      circular<n, inf, sup>& operator=(float val);

      /// Access to std type.
      float value() const;

      /// Set value to the \p val th position in the quantized interval.
      void set_ind(unsigned long val);

      /// Conversion to a float.
      operator float() const;

    private:
      typedef mln_enc(int_u<n>) enc_;

      float
      circle(const float i) const;

      float float01encode(const float i) const;
      float float01decode(const float i) const;

    };


    namespace internal
    {

      template <unsigned n, int inf, int sup>
      struct convert_< circular<n, inf, sup> >
      {
	static circular<n, inf, sup> value_at_index(unsigned i)
	{
	  circular<n, inf, sup> tmp;
	  tmp.set_ind(i);
	  return tmp;
	}

	static unsigned index_of_value(const circular<n, inf, sup>& v)
	{
	  return v.to_enc();
	}
      };
    }


    /// Op<<.
    template <unsigned n, int inf, int sup>
    std::ostream& operator<<(std::ostream& ostr, const circular<n, inf, sup>& f);

    // template <unsigned n, int inf, int sup, unsigned m, int inf2, int sup2>
    // bool approx_equal(const circular<n, inf, sup>& lhs, const circular<m, inf2, sup2>& rhs);

    template <unsigned n, int inf, int sup>
    bool approx_equal(const circular<n, inf, sup>& lhs, const float f);



# ifndef MLN_INCLUDE_ONLY

    // Circular<n, inf, sup>.

    template <unsigned n, int inf, int sup>
    inline
    circular<n, inf, sup>::circular()
    {
    }

    template <unsigned n, int inf, int sup>
    inline
    float
    circular<n, inf, sup>::float01encode(const float i) const
    {
      return circle((i - inf) / (sup - inf));
    }

    template <unsigned n, int inf, int sup>
    inline
    float
    circular<n, inf, sup>::float01decode(const float i) const
    {
      return i * (sup - inf) + inf;
    }

    template <unsigned n, int inf, int sup>
    inline
    circular<n, inf, sup>::circular(float val)
    {
      this->v_ = static_cast<enc_>(float01encode(val) * (float(mln_max(enc_)) - 1.f)); // FIXME
    }

    template <unsigned n, int inf, int sup>
    inline
    float
    circular<n, inf, sup>::circle(const float i) const
    {
      return (int(floor(i)) - inf) % (sup - inf) + inf + (i - floor(i));
    }

    template <unsigned n, int inf, int sup>
    inline
    float
    circular<n, inf, sup>::value() const
    {
      return float(float01decode(this->v_)) / (float(mln_max(enc_)) - 1.f); // FIXME
    }

    template <unsigned n, int inf, int sup>
    inline
    void
    circular<n, inf, sup>::set_ind(unsigned long val)
    {
      this->v_ = static_cast<enc_>(val);
    }

    template <unsigned n, int inf, int sup>
    inline
    circular<n, inf, sup>&
    circular<n, inf, sup>::operator=(float val)
    {
      this->v_ = static_cast<enc_>(float01encode(val) * (float(mln_max(enc_)) - 1.f)); // FIXME
      return *this;
    }

    template <unsigned n, int inf, int sup>
    inline
    circular<n, inf, sup>::operator float() const
    {
      return float(float01decode(this->v_)) / (float(mln_max(enc_)) - 1.f);
    }


    // Operators.

    template <unsigned n, int inf, int sup>
    inline
    std::ostream& operator<<(std::ostream& ostr, const circular<n, inf, sup>& f)
    {
      return ostr << f.value();
    }

    // template <unsigned n, int inf, int sup, unsigned m, int inf2, int sup2>
    // inline
    // bool approx_equal(const circular<n, inf, sup>& lhs, const circular<m, inf2, sup2>& rhs)
    // {
    //   return circular<n, inf, sup>(lhs) == circular<>(rhs);
    // }

    template <unsigned n, int inf, int sup>
    inline
    bool approx_equal(const circular<n, inf, sup>& lhs, float f)
    {
      return circular<n, inf, sup>(lhs) == circular<n, inf, sup>(f);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_CIRCULAR_HH
