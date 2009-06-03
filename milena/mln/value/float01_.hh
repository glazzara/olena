// Copyright (C) 2006, 2007, 2008 EPITA Research and Development
// Laboratory (LRDE)
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
# include <mln/value/internal/convert.hh>
# include <mln/value/float01.hh>
# include <mln/trait/value_.hh>



namespace mln
{

  // Fwd decls.
  namespace value {
    class float01;
    template <unsigned n> struct float01_;
  }


  namespace trait
  {

    template <unsigned n>
    struct value_< mln::value::float01_<n> >
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
    template <unsigned n>
    struct float01_

      : public Floating< float01_<n> >,

	public internal::value_like_< float,             // Equivalent. // FIXME: Why not float01?
				      mln_enc(int_u<n>), // Encoding.
				      float,             // Interoperation.
				      float01_<n> >      // Exact.
    {
      /// Constructor without argument.
      float01_();

      /// Constructor from a float.
      float01_(float val);

      /// \{ Constructors/assignments with literals.
      float01_(const mln::literal::zero_t&);
      float01_& operator=(const mln::literal::zero_t&);
      float01_(const mln::literal::one_t&);
      float01_& operator=(const mln::literal::one_t&);
      /// \}

      /// Assigment from a float.
      float01_<n>& operator=(float val);

      /// Access to std type.
      float value() const;

      /// Set value to the \p val th position in the quantized interval.
      void set_ind(unsigned long val);

      /// Conversion to a float.
      operator float() const;

    private:
      typedef mln_enc(int_u<n>) enc_;
    };


    namespace internal
    {

      template <unsigned n>
      struct convert_< float01_<n> >
      {
	static float01_<n> value_at_index(unsigned i)
	{
	  float01_<n> tmp;
	  tmp.set_ind(i);
	  return tmp;
	}

	static unsigned index_of_value(const float01_<n>& v)
	{
	  return v.to_enc();
	}
      };
    }


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
    inline
    float01_<n>::float01_()
    {
    }

    template <unsigned n>
    inline
    float01_<n>::float01_(float val)
    {
      mln_precondition(val >= 0.f);
      mln_precondition(val <= 1.f);
      this->v_ = static_cast<enc_>(val * (float(mln_max(enc_)) - 1.f)); // FIXME
    }

    template <unsigned n>
    inline
    float
    float01_<n>::value() const
    {
      return float(this->v_) / (float(mln_max(enc_)) - 1.f); // FIXME
    }

    template <unsigned n>
    inline
    void
    float01_<n>::set_ind(unsigned long val)
    {
      this->v_ = static_cast<enc_>(val);
    }

    template <unsigned n>
    inline
    float01_<n>::float01_(const mln::literal::zero_t&)
    {
      this->v_ = 0;
    }

    template <unsigned n>
    inline
    float01_<n>&
    float01_<n>::operator=(const mln::literal::zero_t&)
    {
      this->v_ = 0;
      return *this;
    }

    template <unsigned n>
    inline
    float01_<n>::float01_(const mln::literal::one_t&)
    {
      this->v_ = 1;
    }

    template <unsigned n>
    inline
    float01_<n>&
    float01_<n>::operator=(const mln::literal::one_t&)
    {
      this->v_ = 1;
      return *this;
    }

    template <unsigned n>
    inline
    float01_<n>&
    float01_<n>::operator=(float val)
    {
      mln_precondition(val >= 0.f);
      mln_precondition(val <= 1.f);
      this->v_ = static_cast<enc_>(val * (float(mln_max(enc_)) - 1.f)); // FIXME
      return *this;
    }

    template <unsigned n>
    inline
    float01_<n>::operator float() const
    {
      return float(this->v_) / (float(mln_max(enc_)) - 1.f);
    }


    // Operators.

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const float01_<n>& f)
    {
      return ostr << f.value();
    }

    template <unsigned n, unsigned m>
    inline
    bool approx_equal(const float01_<n>& lhs, const float01_<m>& rhs)
    {
      return float01(lhs) == float01(rhs);
    }

    template <unsigned n>
    inline
    bool approx_equal(const float01_<n>& lhs, float f)
    {
      return float01(lhs) == float01_<n>(f);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_FLOAT01__HH
