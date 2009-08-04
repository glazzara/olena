// Copyright (C) 2006, 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

// includes
#include <iostream>

#include <mln/core/concept/object.hh>
#include <mln/trait/all.hh>
#include <mln/trait/value_.hh>
#include <mln/value/ops.hh>
#include <mln/value/scalar.hh>
#include <mln/debug/format.hh>

#include <mln/math/min.hh>
#include <mln/math/max.hh>
#include <mln/math/abs.hh>

#include <mln/literal/zero.hh>
#include <mln/literal/one.hh>

#include <mln/trace/entering.hh>
#include <mln/trace/exiting.hh>

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
	T numerator_;
	T denominator_;

      public:
	const T& numerator(const T& numerator)      {this->numerator_ = numerator; return numerator;}
	const T& denominator(const T& denominator)  {this->denominator_ = denominator; return denominator;}

	const T& numerator() const                  {return numerator_;}
	const T& denominator() const                {return denominator_;}
      };
    } // end of namespace mln::algebra::internal

    template<typename T>
    class frac : public internal::frac_base_<T>
    {
      typedef internal::frac_base_<T> super_;

    private:
      const T gcd_(const T& a, const T& b) const;

    public:
      // Constructor without argument ==> very dangerous one in this context
      frac();

      // Constructor to convert scalar in fraction (transtyping)
      frac(const T& scalar);

      // Constructor with literal zero
      frac(const literal::zero_t& zero);
      frac& operator=(const literal::zero_t& zero);

      // Constructor with literal one
      frac(const literal::one_t& one);
      frac& operator=(const literal::one_t& one);

      // Constructor setting the internal values
      frac(const T& numerator_, const T& denominator_);

      // Constructor of copy
      frac(const frac<T>& rhs);

      // Constructor of copy
      template <typename F>
      frac(const frac<F>& rhs);

      // Assignement operator
      frac& operator=(const frac<T>& rhs);

      // Second assignement operator
      template<typename F>
      frac& operator=(const frac<F>& rhs);

      // Greatest common divisor (Euclidian Algorithm)
      const T gcd(const T& a, const T& b) const;

      const frac<T>& irreducible();

      // Return the quotient
      float eval() const;
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


    // - frac
    template <typename T>
    struct set_precise_unary_<op::uminus, algebra::frac<T> >
    {
      typedef mln_trait_op_uminus(T) V;
      typedef algebra::frac<V>       ret;
    };
    
    // frac + s
    template <typename T, typename S>
    struct set_precise_binary_<op::plus, algebra::frac<T>, mln::value::scalar_<S> >
    {
      typedef mln_trait_op_plus(T, S) V;
      typedef algebra::frac<V>        ret;
    };

    
    // frac - s
    template <typename T, typename S>
    struct set_precise_binary_<op::minus, algebra::frac<T>, mln::value::scalar_<S> >
    {
      typedef mln_trait_op_minus(T, S) V;
      typedef algebra::frac<V>         ret;
    };
    

    // frac * s
    template <typename T, typename S>
    struct set_precise_binary_<op::times, algebra::frac<T>, mln::value::scalar_<S> >
    {
      typedef mln_trait_op_times(T, S) V;
      typedef algebra::frac<V>         ret;
    };
    
    
    // frac / s
    template <typename T, typename S>
    struct set_precise_binary_<op::div, algebra::frac<T>, mln::value::scalar_<S> >
    {
      typedef mln_trait_op_div(T, S) V;
      typedef algebra::frac<V>       ret;
    };

    
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
      //typedef mln_sum_poduct(T, F) ret;
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


  }// end of namespace mln::trait

  namespace algebra
  {
    // frac == frac
    template <typename T, typename F>
    bool
    operator==(const frac<T>& lhs, const frac<F>& rhs);

    // <<
    template <typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const frac<T>& rhs);
    
    // >>
    template <typename T>
    std::istream&
    operator<<(std::istream& istr, const frac<T>& rhs);

    // - frac
    template <typename T>
    frac<mln_trait_op_uminus(T)>
    operator-(const frac<T>& rhs);
    
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
    template <typename T>
    inline
    frac<T>::frac() : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac()");
      
      this->numerator(0);
      this->denominator(1);

      trace::exiting("mln::algebra::frac<T>::frac()");
    }

    template <typename T>
    frac<T>::frac(const T& scalar) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac(const S& scalar)");
      
      this->numerator(scalar);
      this->denominator(1);

      trace::exiting("mln::algebra::frac<T>::frac(const S& scalar)");
    }
    
    template <typename T>
    inline
    frac<T>::frac(const T& numerator, const T& denominator) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac(const T& numerator, const T& denominator)");

      this->numerator(numerator);
      this->denominator(denominator);

      irreducible();

      trace::exiting("mln::algebra::frac<T>::frac(const T& numerator, const T& denominator)");
    }

    template <typename T>
    inline
    frac<T>::frac(const literal::zero_t& zero) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac(const literal::zero_t& zero)");

      this->numerator(zero);
      this->denominator(1);

      trace::exiting("mln::algebra::frac<T>::frac(const literal::zero_t& zero)");
    }

    template <typename T>
    inline
    frac<T>& frac<T>::operator=(const literal::zero_t& zero)
    {
      trace::entering("frac<T>& mln::algebra::frac<T>::operator=(const literal::zero_t& zero)");

      this->numerator(zero);
      this->denominator(1);

      trace::exiting("frac<T>& mln::algebra::frac<T>::operator=(const literal::zero_t& zero)");
      return *this;
    }



    template <typename T>
    inline
    frac<T>::frac(const literal::one_t& one) : super_()
    {
      trace::entering("frac<T>::frac(const literal::one_t& one)");

      this->numerator(one);
      this->denominator(1);

      trace::exiting("frac<T>::frac(const literal::one_t& one)");
    }

    template <typename T>
    inline
    frac<T>& frac<T>::operator=(const literal::one_t& one)
    {
      trace::entering("frac<T>& mln::algebra::frac<T>::operator=(const literal::one_t& one)");

      this->numerator(one);
      this->denominator(1);

      trace::exiting("frac<T>& mln::algebra::frac<T>::operator=(const literal::one_t& one)");
      return *this;
    }

    template <typename T>
    inline
    frac<T>::frac(const frac<T>& rhs) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac(const frac<T>& rhs)");

      this->numerator(rhs.numerator());
      this->denominator(rhs.denominator());

      irreducible();

      trace::exiting("mln::algebra::frac<T>::frac(const frac<T>& rhs)");
    }

    template <typename T>
    inline
    frac<T>& frac<T>::operator=(const frac<T>& rhs)
    {
      trace::entering("frac<T>& mln::algebra::frac<T>::operator=(const frac<T>& rhs)");

      numerator(rhs.numerator());
      denominator(rhs.denominator());
      
     irreducible();
     trace::exiting("frac<T>& mln::algebra::frac<T>::operator=(const frac<T>& rhs)");

      return *this;
    }

    template <typename T>
    template <typename F>
    inline
    frac<T>::frac(const frac<F>& rhs) : super_()
    {
      trace::entering("mln::algebra::frac<T>::frac(const frac<F>& rhs)");
      mlc_converts_to(F, T)::check();

      numerator(static_cast<T>(rhs.numerator()));
      denominator(static_cast<T>(rhs.denominator()));

      irreducible();

      trace::exiting("mln::algebra::frac<T>::frac(const frac<F>& rhs)");
    }

    template <typename T>
    template <typename F>
    inline
    frac<T>& frac<T>::operator=(const frac<F>& rhs)
    {
      trace::entering("frac<T>& mln::algebra::frac<T>::operator=(const frac<F>& rhs)");
      mlc_converts_to(F, T)::check();

      numerator(static_cast<T>(rhs.numerator()));
      denominator(static_cast<T>(rhs.denominator()));
      
     irreducible();
     trace::exiting("frac<T>& mln::algebra::frac<T>::operator=(const frac<F>& rhs)");

      return *this;
    }

    // Greatest common divisor (Euclidian algorithm)
    template <typename T>
    inline
    const T frac<T>::gcd(const T& a, const T& b) const
    {
      trace::entering("const T mln::algebra::frac<T>::gcd(const T& a, const T& b)");

      T result;
      T abs_a = math::abs(a);
      T abs_b = math::abs(b);

      if ((0 == abs_a) || (0 == abs_b))
	result = 1;
      else
	result = gcd_(math::max(abs_a, abs_b), math::min(abs_a, abs_b));

      trace::exiting("const T mln::algebra::frac<T>::gcd(const T& a, const T& b)");
      return result;
    }

    template <typename T>
    inline
    const T frac<T>::gcd_(const T& a, const T& b) const
    {
      trace::entering("const T mln::algebra::frac<T>::cgd_(const T& a, const T& b)");
      mln_assertion(a > 0);
      mln_assertion(b > 0);
      mln_assertion(a >= b);

      T rest = a%b;

      if (0 == rest) 
	rest = b;
      else
	rest = gcd(b, rest);

      trace::exiting("const T mln::algebra::frac<T>::cgd_(const T& a, const T& b)");
      return rest;
    }

    template <typename T>
    inline
    const frac<T>& frac<T>::irreducible()
    {
      trace::entering("frac<T>& mln::algebra::frac<T>::irreducible()");

      const T& numerator   = this->numerator();
      const T& denominator = this->denominator();
      T        divisor     = this->gcd(numerator, denominator);

      this->numerator(numerator/divisor);
      this->denominator(denominator/divisor);
      
      trace::exiting("frac<T>& mln::algebra::frac<T>::irreducible()");
      return *this;
    }
    
    template <typename T>
    inline
    float frac<T>::eval() const
    {
      trace::entering("frac<T>& mln::algebra::frac<T>::eval()");

      const T& numerator   = super_::numerator();
      const T& denominator = super_::denominator();

      float float_num      = static_cast<float>(numerator);
      float float_denom    = static_cast<float>(denominator);
      float result         = float_num / float_denom; 
      
      trace::exiting("frac<T>& mln::algebra::frac<T>::eval()");
      return  result ;
    }
    
    template <typename T, typename F>
    inline
    bool operator==(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("bool mln::algebra::operator==(const frac<T>& lhs, const frac<F>& rhs)");

      frac<T> lhsi(lhs);
      frac<T> rhsi(rhs);

      lhsi.irreducible();
      rhsi.irreducible();

      bool  same_numerator   = lhsi.numerator() == rhsi.numerator();
      bool  same_denominator = rhsi.denominator() == rhsi.denominator();
      bool  result           = same_numerator && same_denominator;

      trace::exiting("bool mln::algebra::operator==(const frac<T>& lhs, const frac<F>& rhs)");
      return result; 
    }
    
    template <typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const frac<T>& rhs)
    {
      trace::entering("std::ostream& mln::algebra::operator<<(std::ostream& ostr, const frac<T>& rhs)");

      ostr << '(' << debug::format(rhs.numerator()) << '/' << debug::format(rhs.denominator()) << ')';

      trace::exiting("std::ostream& mln::algebra::operator<<(std::ostream& ostr, const frac<T>& rhs)");
      return ostr;
    }

    template <typename T>
    std::istream&
    operator>>(std::istream& istr, const frac<T>& rhs)
    {
      trace::entering("std::istream& mln::algebra::operator>>(std::istream& istr, const frac<T>& rhs)");

      istr >> rhs.numerator();
      istr >> rhs.denominator();

      trace::exiting("std::istream& mln::algebra::operator>>(std::istream& istr, const frac<T>& rhs)");
      return istr;
    }
  
    // - frac
    template <typename T>
    frac<mln_trait_op_uminus(T)>
    operator-(const frac<T>& lhs)
    {
      trace::entering("frac<mln_trait_op_uminus(T)> mln::algebra::operator-(const frac<T>& lhs)");
      typedef mln_trait_op_uminus(T) R;
      frac<R> tmp = lhs;
      
      tmp.numerator(-lhs.numerator());
      tmp.denominator(lhs.denominator());
      
      tmp.irreducible();
      
      trace::exiting("frac<mln_trait_op_uminus(T)> mln::algebra::operator-(const frac<T>& lhs)");
      return tmp;
    }
    
    // frac + s
    template <typename T, typename S>
    frac<mln_trait_op_plus(T,S)>
    operator+(const frac<T>& lhs, const value::scalar_<S>& s)
    {
      trace::entering("frac<mln_trait_op_plus(T,S)> mln::algebra::operator+(const frac<T>& lhs, value::scalar<S>& s)");
      typedef mln_trait_op_plus(T,S) R;
      frac<R> tmp = lhs;
      
      tmp.numerator(lhs.numerator() + s.to_equiv() * lhs.denominator());
      tmp.denominator(lhs.denominator());

      tmp.irreducible();
      
      trace::exiting("frac<mln_trait_op_plus(T,S)> mln::algebra::operator+(const frac<T>& lhs, value::scalar<S>& s)");
      return tmp;
    }
    
    // frac - s
    template <typename T, typename S>
    frac<mln_trait_op_minus(T,S)>
    operator-(const frac<T>& lhs, const value::scalar_<S>& s)
    {
      trace::entering("frac<mln_trait_op_minus(T,S)> mln::algebra::operator-(const frac<T>& lhs, value::scalar<S>& s)");
      typedef mln_trait_op_minus(T,S) R;
      frac<R> tmp = lhs;
      
      tmp.numerator(lhs.numerator() - s.to_equiv() * lhs.denominator());
      tmp.denominator(lhs.denominator());

      tmp.irreducible();
      
      trace::exiting("frac<mln_trait_op_minus(T,S)> mln::algebra::operator-(const frac<T>& lhs, value::scalar<S>& s)");
      return tmp;
    }

    // frac * s
    template <typename T, typename S>
    frac<mln_trait_op_times(T,S)>
    operator*(const frac<T>& lhs, const value::scalar_<S>& s)
    {
      trace::entering("frac<mln_trait_op_times(T,S)> mln::algebra::operator*(const frac<T>& lhs, value::scalar<S>& s)");
      typedef mln_trait_op_times(T,S) R;
      frac<R> tmp = lhs;
      
      std::cout << tmp << std::endl;

      tmp.numerator(lhs.numerator() * s.to_equiv());
      tmp.denominator(lhs.denominator());
      
      std::cout << tmp << std::endl;

      tmp.irreducible();

      std::cout << tmp << std::endl;
      
      trace::exiting("frac<mln_trait_op_times(T,S)> mln::algebra::operator*(const frac<T>& lhs, value::scalar<S>& s)");
      return tmp;
    }
  
    // frac / s
    template <typename T, typename S>
    frac<mln_trait_op_div(T,S)>
    operator/(const frac<T>& lhs, const value::scalar_<S>& s)
    {
      trace::entering("frac<mln_trait_op_div(T,S)> mln::algebra::operator/(const frac<T>& lhs, value::scalar<S>& s)");
      typedef mln_trait_op_div(T,S) R;
      frac<R> tmp = lhs;
      
      std::cout << tmp << std::endl;
      
      tmp.numerator(lhs.numerator());
      tmp.denominator(lhs.denominator() * s.to_equiv());
      
      std::cout << tmp << std::endl;

      tmp.irreducible();

      std::cout << tmp << std::endl;
      
      trace::exiting("frac<mln_trait_op_div(T,S)> mln::algebra::operator/(const frac<T>& lhs, value::scalar<S>& s)");
      return tmp;
    }
    
    // frac + frac
    template <typename T, typename F>
    frac<mln_trait_op_plus(T,F)>
    operator+(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("frac<mln_trait_op_plus(T,F)> mln::algebra::operator+(const frac<T>& lhs, const frac<F>& rhs)");
      typedef mln_trait_op_plus(T,F) R;
      frac<R> tmp;      
      
      tmp.numerator(lhs.numerator()*rhs.denominator() + lhs.denominator()*rhs.numerator());
      tmp.denominator(lhs.denominator()*rhs.denominator());

      tmp.irreducible();
      
      trace::exiting("frac<mln_trait_op_plus(T,F)> mln::algebra::operator+(const frac<T>& lhs, const frac<F>& rhs)");
      return tmp;
    }
    
    // frac - frac
    template <typename T, typename F>
    frac<mln_trait_op_minus(T,F)>
    operator-(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("frac<mln_trait_op_minus(T,F)> mln::algebra::operator-(const frac<T>& lhs, const frac<F>& rhs)");
      typedef mln_trait_op_minus(T,F) R;
      frac<R> tmp;
      
      tmp.numerator(lhs.numerator()*rhs.denominator() - lhs.denominator()*rhs.numerator());
      tmp.denominator(lhs.denominator()*rhs.denominator());
      
      tmp.irreducible();
      
      trace::exiting("frac<mln_trait_op_minus(T,F)> mln::algebra::operator-(const frac<T>& lhs, const frac<F>& rhs)");
      return tmp;
    }
    
    // frac * frac
    template <typename T, typename F>
    frac<mln_trait_op_times(T,F)>
    operator*(const frac<T>& lhs, const frac<F>& rhs)
    {
      trace::entering("frac<mln_trait_op_times(T,F)> mln::algebra::operator*(const frac<T>& lhs, const frac<F>& rhs)");
      typedef mln_trait_op_times(T,F) R;
      frac<R> tmp;
      
      tmp.numerator(lhs.numerator()*rhs.numerator());
      tmp.denominator(lhs.denominator()*rhs.denominator());
      
      tmp.irreducible();
      
      trace::exiting("frac<mln_trait_op_times(T,F)> mln::algebra::operator*(const frac<T>& lhs, const frac<F>& rhs)");
      return tmp;
    }

    // frac / frac
    template <typename T, typename F>
    frac<mln_trait_op_div(T,F)>
    operator/(const frac<T>& lhs, frac<F>& rhs)
    {
      trace::entering("frac<mln_trait_op_div(T,F)> mln::algebra::operator/(const frac<T>& lhs, const frac<F>& rhs)");
      typedef mln_trait_op_div(T,F) R;
      frac<R> tmp;
      
      tmp.numerator(lhs.numerator()*rhs.denominator());
      tmp.denominator(lhs.denominator()*rhs.numerator());
      
      tmp.irreducible();
      
      trace::exiting("frac<mln_trait_op_div(T,F)> mln::algebra::operator/(const frac<T>& lhs, const frac<F>& rhs)");
      return tmp;
    }
    
  } // end of namespace mln::algebra
  
# endif // MLN_INCLUDE_ONLY
  
} // end of namespace mln

#endif // ! MLN_ALGEBRA_FRAC_HH


