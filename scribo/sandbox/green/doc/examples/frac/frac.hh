// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ALGEBRA_FRAC_HH
# define MLN_ALGEBRA_FRAC_HH

#include <iostream>

#include <mln/core/concept/object.hh>
#include <mln/trait/all.hh>
#include <mln/trait/value_.hh>
#include <mln/value/ops.hh>
#include <mln/value/scalar.hh>
#include <mln/debug/format.hh>

#include <mln/math/abs.hh>
#include <mln/math/sign.hh>

#include <mln/literal/zero.hh>
#include <mln/literal/one.hh>

#include <mln/trace/entering.hh>
#include <mln/trace/exiting.hh>

#include <mln/value/int_u8.hh>

#include "sign_prod.hh"
#include "gcd.hh"

namespace mln
{
  namespace algebra
  {
    // T is the type of the numerator and the denominator
    // frac is a class for integer quotient computing
    // numerator and denominator must be kind of integer
    template<typename T> class frac;
  }

  namespace literal
  {
    struct zero_t;
    struct one_t;
  }

  // FIXME : frac += scalar ... doesn't compile ...
  // FIXME : Comment s'assurer que le type T ne contient que des non flottants
  // de type entier ... ???
  // FIXME : Comment trouver la fraction equivalente à un flottant
  // FIXME : Comment enregistrer correctement les traits ??
  // FIXME : Comment intégrer frac pour qu'il fonctionne avec la variance ??
  // FIXME : Code experimental

  namespace trait
  {
    template<typename T>
    struct value_< algebra::frac<T> >
    {
      typedef trait::value::nature::floating nature;
      typedef trait::value::kind::data       kind;
      
      enum
	{
	  dim   = 1,
	  nbits = 2 * mln_nbits(T),
	  card  = mln_card(T)
	};

      typedef T                           comp;
      typedef mln_value_quant_from_(card) quant;
      typedef algebra::frac<mln_sum(T)>   sum;
    };

  }

  namespace algebra
  {
    namespace internal
    {
      template<typename T>
      class frac_base_ : public Object< frac<T> >
      {
      private:
	T            numerator_;
	T            denominator_;
	math::sign_t sign_;

      public:
	const T&            numerator()   const {return numerator_;}
	const T&            denominator() const {return denominator_;}
	const math::sign_t& sign()        const {return sign_;}

	float eval() const
	{
	  T     sign        = math::sign_to_coeff<T>(sign_);
	  float numerator   = static_cast<float>(numerator_);
	  float denominator = static_cast<float>(denominator_);
	  float result      = sign * numerator/denominator;

	  return result;
	}

	void irreducible(const T& numerator, const T& denominator)
	{
	  trace::entering("mln::algebra::internal::frac_base_::irreducible");
	  
	  T divisor          = math::gcd(numerator, denominator);
	  this->sign_        = math::sign_prod<T>(numerator, denominator);
	  this->numerator_   = math::abs(numerator) / divisor;
	  this->denominator_ = math::abs(denominator) / divisor;
      
	  trace::exiting("mln::algebra::internal::frac_base_::irreducible");
	}
	
      };
    } // end of namespace mln::algebra::internal

    template<typename T>
    class frac : public internal::frac_base_<T>
    {
      typedef internal::frac_base_<T> super_;

    public:
      // Constructor without argument ==> very dangerous one in this context
      frac();

      // Constructor to convert scalar in fraction (transtyping)
      template <typename S>
      frac(const value::scalar_<S>& scalar);
      frac(const T& scalar);
      //frac(const value::int_u8& scalar);

      // Constructor with literal zero
      frac(const literal::zero_t& zero);

      // Constructor with literal one
      frac(const literal::one_t& one);

      // Constructor setting the internal values
      frac(const T& numerator_, const T& denominator_);

      // Constructor of copy
      frac(const frac<T>& rhs);

      // Constructor of copy
      template <typename F>
      frac(const frac<F>& rhs);

      // Assignement operator
      frac& operator=(const literal::zero_t& zero);
      frac& operator=(const literal::one_t& one);
      frac& operator=(const frac<T>& rhs);

      // Second assignement operator
      template<typename F>
      frac& operator=(const frac<F>& rhs);
    };
  } // end of namespace mln::algebra

  namespace trait
  {
    // For unary traits
    template< template <class> class Name, typename T >
    struct set_precise_unary_< Name, algebra::frac<T> >
    {
      typedef mln_trait_unary(Name, T) V;
      typedef algebra::frac<T>         ret;
    };


    //
    // SCALAR ARITHMETIC
    //

    // frac + s
    template <typename T, typename S>
    struct set_precise_binary_<op::plus, 
			       algebra::frac<T>, 
			       mln::value::scalar_<S> >
    {
      typedef mln_trait_op_plus(T, S) V;
      typedef algebra::frac<V>        ret;
    };
    
    // frac - s
    template <typename T, typename S>
    struct set_precise_binary_<op::minus, 
			       algebra::frac<T>, 
			       mln::value::scalar_<S> >
    {
      typedef mln_trait_op_minus(T, S) V;
      typedef algebra::frac<V>         ret;
    };
    

    // frac * s
    template <typename T, typename S>
    struct set_precise_binary_<op::times, 
			       algebra::frac<T>, 
			       mln::value::scalar_<S> >
    {
      typedef mln_trait_op_times(T, S) V;
      typedef algebra::frac<V>         ret;
    };
    
    
    // frac / s
    template <typename T, typename S>
    struct set_precise_binary_<op::div, 
			       algebra::frac<T>, 
			       mln::value::scalar_<S> >
    {
      typedef mln_trait_op_div(T, S) V;
      typedef algebra::frac<V>       ret;
    };


    //
    // FRAC ARITHMETIC
    //
    
    // frac + frac
    template <typename T, typename F>
    struct set_precise_binary_<op::plus, algebra::frac<T>, algebra::frac<F> >
    {
      typedef mln_trait_op_plus(T, F) V;
      typedef algebra::frac<V>        ret;
    };
    

    // frac - frac
    template <typename T, typename F>
    struct set_precise_binary_<op::minus, algebra::frac<T>, algebra::frac<F> >
    {
      typedef mln_trait_op_minus(T, F) V;
      typedef algebra::frac<V>        ret;
    };
    
    
    // frac * frac
    template <typename T, typename F>
    struct set_precise_binary_<op::times, algebra::frac<T>, algebra::frac<F> >
    {
      typedef mln_trait_op_times(T, F) V;
      typedef algebra::frac<V>         ret;
    };


    // frac / frac
    template <typename T, typename F>
    struct set_precise_binary_<op::div, algebra::frac<T>, algebra::frac<F> >
    {
      typedef mln_trait_op_div(T, F)  V;
      typedef algebra::frac<V>        ret;
    };


    //
    // COMPARAISON
    //

    // frac == frac
    template <typename T, typename F>
    struct set_precise_binary_<op::eq, algebra::frac<T>, algebra::frac<F> >
    {
      typedef bool        ret;
    };

    // frac < frac
    template <typename T, typename F>
    struct set_precise_binary_<op::less, algebra::frac<T>, algebra::frac<F> >
    {
      typedef bool        ret;
    };

  }// end of namespace mln::trait

  namespace algebra
  {
    //
    // COMPARAISON
    //

    template <typename T, typename F>
    math::sign_t compare(const frac<T>& lhs, const frac<F>& rhs);

    // frac == frac
    template <typename T, typename F>
    bool
    operator==(const frac<T>& lhs, const frac<F>& rhs);

    // frac < frac
    template <typename T, typename F>
    bool
    operator<(const frac<T>& lhs, const frac<F>& rhs);

    //
    // INPUT/OUTPUT STREAM
    //

    // <<
    template <typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const frac<T>& rhs);
    
    // >>
    template <typename T>
    std::istream&
    operator<<(std::istream& istr, const frac<T>& rhs);

    //
    // SCALAR ARITHMETIC
    //
    
    // frac + s
    template <typename T, typename S>
    frac<mln_trait_op_plus(T,S)>
    operator+(const frac<T>& lhs, const mln::value::scalar_<S>& s);
    
    // frac - s
    template <typename T, typename S>
    frac<mln_trait_op_minus(T,S)>
    operator-(const frac<T>& lhs, const mln::value::scalar_<S>& s);
    
    // frac * s
    template <typename T, typename S>
    frac<mln_trait_op_times(T,S)>
    operator*(const frac<T>& lhs, const mln::value::scalar_<S>& s);
    
    // frac / s
    template <typename T, typename S>
    frac<mln_trait_op_div(T,S)>
    operator/(const frac<T>& lhs, const mln::value::scalar_<S>& s);
    
    //
    // FRAC ARITHMETIC
    //

    // frac + frac
    template <typename T, typename F>
    frac<mln_trait_op_plus(T,F)>
    operator+(const frac<T>& lhs, const frac<F>& rhs);

    // frac - frac
    template <typename T, typename F>
    frac<mln_trait_op_minus(T,F)>
    operator-(const frac<T>& lhs, const frac<F>& rhs);

    // frac * frac
    template <typename T, typename F>
    frac<mln_trait_op_times(T,F)>
    operator*(const frac<T>& lhs, const frac<F>& rhs);

    // frac / frac
    template <typename T, typename F>
    frac<mln_trait_op_div(T,F)>
    operator/(const frac<T>& lhs, const frac<F>& rhs);

  }// end of namespace mln::algebra


# ifndef MLN_INCLUDE_ONLY

  namespace algebra
  {

    //
    // INSTANTIATION
    //
    template <typename T>
    inline
    frac<T>::frac() : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac");
      
      this->irreducible(0, 1);

      trace::exiting("mln::algebra::frac<T>::frac");
    }

    template <typename T>
    frac<T>::frac(const T& scalar) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac");
      
      this->irreducible(scalar, 1);

      trace::exiting("mln::algebra::frac<T>::frac");
    }

    template <typename T>
    template <typename S>
    frac<T>::frac(const value::scalar_<S>& scalar) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac");
      
      this->irreducible(scalar, 1);

      trace::exiting("mln::algebra::frac<T>::frac");
    }
    
    template <typename T>
    inline
    frac<T>::frac(const T& numerator, const T& denominator) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac");

      this->irreducible(numerator, denominator);

      trace::exiting("mln::algebra::frac<T>::frac");
    }

    template <typename T>
    inline
    frac<T>::frac(const literal::zero_t& zero) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac");

      this->irreducible(zero, 1);

      trace::exiting("mln::algebra::frac<T>::frac");
    }

    template <typename T>
    inline
    frac<T>::frac(const literal::one_t& one) : super_()
    {
      trace::entering("frac<T>::frac");

      this->irreducible(one, 1);

      trace::exiting("frac<T>::frac");
    }

    template <typename T>
    inline
    frac<T>::frac(const frac<T>& rhs) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac");
      
      T rhs_sign  = math::sign_to_coeff<T>(rhs.sign());
      
      this->irreducible(rhs_sign*rhs.numerator(), rhs.denominator());

      trace::exiting("mln::algebra::frac<T>::frac");
    }

    template <typename T>
    template <typename F>
    inline
    frac<T>::frac(const frac<F>& rhs) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac");
      mlc_converts_to(F, T)::check();

      T rhs_sign        = math::sign_to_coeff<T>(rhs.sign());
      T rhs_numerator   = rhs_sign*static_cast<T>(rhs.numerator());
      T rhs_denominator = static_cast<T>(rhs.denominator());

      this->irreducible(rhs_numerator, rhs_denominator);

      trace::exiting("mln::algebra::frac<T>::frac");
    }
    
    //
    // ASSIGNEMENT
    //
    
    template <typename T>
    inline
    frac<T>& frac<T>::operator=(const frac<T>& rhs)
    {
      trace::entering("mln::algebra::frac<T>::operator=");
      
      T rhs_sign  = math::sign_to_coeff<T>(rhs.sign());
      
      this->irreducible(rhs_sign*rhs.numerator(), rhs.denominator());

      trace::exiting("mln::algebra::frac<T>::operator=");

      return *this;
    }

    template <typename T>
    inline
    frac<T>& frac<T>::operator=(const literal::one_t& one)
    {
      trace::entering("mln::algebra::frac<T>::operator=");

      this->irreducible(one, 1);

      trace::exiting("mln::algebra::frac<T>::operator=");
      return *this;
    }

    template <typename T>
    inline
    frac<T>& frac<T>::operator=(const literal::zero_t& zero)
    {
      trace::entering("mln::algebra::frac<T>::operator=");

      this->irreducible(zero, 1);

      trace::exiting("mln::algebra::frac<T>::operator=");
      return *this;
    }

    template <typename T>
    template <typename F>
    inline
    frac<T>& frac<T>::operator=(const frac<F>& rhs)
    {
      trace::entering("mln::algebra::frac<T>::operator=");
      mlc_converts_to(F, T)::check();

      T rhs_sign        = math::sign_to_coeff<T>(rhs.sign());
      T rhs_numerator   = rhs_sign*static_cast<T>(rhs.numerator());
      T rhs_denominator = static_cast<T>(rhs.denominator());

      this->irreducible(rhs_numerator, rhs_denominator);
      
      trace::exiting("mln::algebra::frac<T>::operator=");

      return *this;
    }

    //
    // COMPARAISON
    //
    template <typename T, typename F>
    inline
    math::sign_t compare(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("mln::algebra::compare");
      mlc_converts_to(F, T)::check();

      T            rhs_sign        = math::sign_to_coeff<T>(rhs.sign());
      T            lhs_sign        = math::sign_to_coeff<T>(lhs.sign());
      T            side1           = rhs_sign*lhs.numerator()*rhs.denominator();
      T            side2           = lhs_sign*rhs.numerator()*lhs.denominator();
      math::sign_t result          = math::sign(side1 - side2);

      trace::exiting("mln::algebra::compare");
      return result;
    }

    template <typename T, typename F>
    inline
    bool operator==(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("mln::algebra::operator==");

      bool   result = is_null(compare(lhs, rhs));

      trace::exiting("mln::algebra::operator==");
      return result;
    }

    template <typename T, typename F>
    inline
    bool operator<(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("mln::algebra::operator<");

      bool result = is_negative(compare(lhs, rhs));

      trace::exiting("mln::algebra::operator<");
      return result;
    }

    //
    // INPUT/OUTPUT STREAM
    //

    template <typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const frac<T>& rhs)
    {
      trace::entering("mln::algebra::operator<<");

      ostr << (math::is_negative(rhs.sign())? '-' : '+');
      ostr << '(' << debug::format(rhs.numerator());
      ostr << '/' << debug::format(rhs.denominator()) << ')';

      trace::exiting("mln::algebra::operator<<");
      return ostr;
    }

    template <typename T>
    std::istream&
    operator>>(std::istream& istr, const frac<T>& rhs)
    {
      trace::entering("mln::algebra::operator>>");

      istr >> rhs.numerator();
      istr >> rhs.denominator();

      trace::exiting("mln::algebra::operator>>");
      return istr;
    }
  
    //
    // FRAC ARITHMETIC
    //
    
    // frac + frac
    template <typename T, typename F>
    frac<mln_trait_op_plus(T,F)>
    operator+(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("mln::algebra::operator+");
      typedef mln_trait_op_plus(T,F) R;

      R       rhs_sign        = math::sign_to_coeff<R>(rhs.sign());
      R       lhs_sign        = math::sign_to_coeff<R>(lhs.sign());
      R       side1           = lhs_sign*lhs.numerator()*rhs.denominator();
      R       side2           = rhs_sign*rhs.numerator()*lhs.denominator();
      R       numerator       = side1 + side2;
      R       denominator     = lhs.denominator()*rhs.denominator();
      frac<R> tmp(numerator, denominator);
      
      trace::exiting("mln::algebra::operator+");
      return tmp;
    }
    
    // frac - frac
    template <typename T, typename F>
    frac<mln_trait_op_minus(T,F)>
    operator-(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("mln::algebra::operator-");
      typedef mln_trait_op_minus(T,F) R;

      R       lhs_sign        = math::sign_to_coeff<R>(lhs.sign());
      R       rhs_sign        = math::sign_to_coeff<R>(rhs.sign());
      R       side1           = lhs_sign*lhs.numerator()*rhs.denominator();
      R       side2           = rhs_sign*rhs.numerator()*lhs.denominator();
      R       numerator       = side1 - side2;
      R       denominator     = lhs.denominator()*rhs.denominator();
      frac<R> tmp(numerator, denominator);
      
      trace::exiting("mln::algebra::operator-");
      return tmp;
    }
    
    // frac * frac
    template <typename T, typename F>
    frac<mln_trait_op_times(T,F)>
    operator*(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("mln::algebra::operator*");
      typedef mln_trait_op_times(T,F) R;

      R       lhs_sign    = math::sign_to_coeff<R>(lhs.sign());
      R       rhs_sign    = math::sign_to_coeff<R>(rhs.sign());
      R       numerator   = lhs_sign*lhs.numerator()*rhs.numerator();
      R       denominator = rhs_sign*lhs.denominator()*rhs.denominator();
      frac<R> tmp(numerator, denominator);
      
      trace::exiting("mln::algebra::operator*");
      return tmp;
    }

    // frac / frac
    template <typename T, typename F>
    frac<mln_trait_op_div(T,F)>
    operator/(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("mln::algebra::operator/");
      typedef mln_trait_op_div(T,F) R;

      R       lhs_sign    = math::sign_to_coeff<R>(lhs.sign());
      R       rhs_sign    = math::sign_to_coeff<R>(rhs.sign());
      R       numerator   = lhs_sign*lhs.numerator()*rhs.denominator();
      R       denominator = rhs_sign*lhs.denominator()*rhs.numerator();
      frac<R> tmp(numerator, denominator);
      
      trace::exiting("mln::algebra::operator/");
      return tmp;
    }

    //
    // SCALAR ARITHMETIC
    //

    // frac + s
    template <typename T, typename S>
    frac<mln_trait_op_plus(T,S)>
    operator+(const frac<T>& lhs, const value::scalar_<S>& s)
    {
      trace::entering("mln::algebra::operator+");
      typedef mln_trait_op_plus(T,S) R;

      R       scalar      = static_cast<R>(s.to_equiv());
      R       lhs_sign    = math::sign_to_coeff<R>(lhs.sign());
      R       side1       = lhs_sign * lhs.numerator();
      R       side2       = scalar * lhs.denominator();
      R       numerator   = side1 + side2;
      R       denominator = lhs.denominator();
      frac<R> tmp(numerator, denominator);
      
      trace::exiting("mln::algebra::operator+");
      return tmp;
    }
    
    // frac - s
    template <typename T, typename S>
    frac<mln_trait_op_minus(T,S)>
    operator-(const frac<T>& lhs, const value::scalar_<S>& s)
    {
      trace::entering("mln::algebra::operator-");
      typedef mln_trait_op_minus(T,S) R;

      R       scalar      = static_cast<R>(s.to_equiv());
      R       lhs_sign    = math::sign_to_coeff<R>(lhs.sign());
      R       side1       = lhs_sign * lhs.numerator();
      R       side2       = scalar * lhs.denominator();
      R       numerator   = side1 - side2;
      R       denominator = lhs.denominator();
      frac<R> tmp(numerator, denominator);
      
      trace::exiting("mln::algebra::operator-");
      return tmp;
    }

    // frac * s
    template <typename T, typename S>
    frac<mln_trait_op_times(T,S)>
    operator*(const frac<T>& lhs, const value::scalar_<S>& s)
    {
      trace::entering("mln::algebra::operator*");
      typedef mln_trait_op_times(T,S) R;

      R       scalar      = static_cast<R>(s.to_equiv());
      R       lhs_sign    = math::sign_to_coeff<R>(lhs.sign());
      R       numerator   = lhs_sign * lhs.numerator() * scalar;
      R       denominator = lhs.denominator();
      frac<R> tmp(numerator, denominator);
      
      trace::exiting("mln::algebra::operator*");
      return tmp;
    }
  
    // frac / s
    template <typename T, typename S>
    frac<mln_trait_op_div(T,S)>
    operator/(const frac<T>& lhs, const value::scalar_<S>& s)
    {
      trace::entering("mln::algebra::operator/");
      typedef mln_trait_op_div(T,S) R;

      R       scalar      = static_cast<R>(s.to_equiv());
      R       lhs_sign    = math::sign_to_coeff<R>(lhs.sign());
      R       numerator   = lhs_sign * lhs.numerator();
      R       denominator = lhs.denominator() * scalar;     
      frac<R> tmp(numerator, denominator);
      
      trace::exiting("mln::algebra::operator/");
      return tmp;
    }
    
    
  } // end of namespace mln::algebra
  
# endif // MLN_INCLUDE_ONLY
  
} // end of namespace mln

#endif // ! MLN_ALGEBRA_FRAC_HH


