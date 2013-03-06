// Copyright (C) 2006, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_ALGEBRA_MAT_HH
# define MLN_ALGEBRA_MAT_HH

/// \file
///
/// \brief Definition of a generic matrix class.

# include <iostream>

# include <mln/core/concept/object.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/contract.hh>
# include <mln/trait/all.hh>
# include <mln/trait/value_.hh>
# include <mln/algebra/vec.hh>
# include <mln/literal/identity.hh>


// FIXME: Document.



namespace mln
{


  // Forward declaration.
  namespace algebra {
    template <unsigned n, unsigned m, typename T> class mat;
  }


  namespace trait
  {

    template <unsigned n, unsigned m, typename T>
    struct value_< algebra::mat<n,m,T> >
    {
      typedef trait::value::nature::matrix nature;
      typedef trait::value::kind::data     kind;

      enum {
	nbits = n * m * mln_nbits(T),
	card  = n * m * mln_card(T)
      };
      typedef mln_value_quant_from_(card)  quant;

      typedef algebra::mat<n, m, mln_sum(T)> sum;
    };

  } // end of namespace mln::trait



  namespace algebra
  {


    template <unsigned n, unsigned m, typename T>
    class mat : public Object< mat<n,m,T> >
    {
    public:

      typedef T coord;
      enum { N = n,
	     M = m,
             dim = n * m };

      static const mat<n,m,T> Id;

      mat();

      mat(const literal::zero_t&);
      mat(const literal::one_t&);
      mat(const literal::identity_t&);

      template <typename U>
      mat(const mat<n,m,U>& rhs);

      /// Constructor; coordinates are set by function \p f.
      template <typename F>
      mat(const Function_v2v<F>& f);

      template <typename U>
      mat& operator=(const mat<n,m,U>& rhs);

      const T& operator()(unsigned i, unsigned j) const;

      T& operator()(unsigned i, unsigned j);

      void set_all(const T& val);

      unsigned size() const;

      static mat identity();

      /// Return the transpose of the matrix.
      mat<m,n,T> t() const;

      /// Return the inverse of the matrix.
      /// Only compile on square matrix.
      mat<n,m,T> _1() const;

    private:

      T data_[n][m];

      void set_id_();
    };


  } // end of namespace algebra



  namespace trait
  {

    // - mat

    template < unsigned n, unsigned m, typename T >
    struct set_precise_unary_< op::uminus,
			       algebra::mat<n,m,T> >
    {
      typedef algebra::mat<n, m, mln_trait_op_uminus(T)> ret;
    };

    // mat + mat

    template < unsigned n, unsigned m, typename T, typename U >
    struct set_precise_binary_< op::plus,
				algebra::mat<n,m,T>, algebra::mat<n,m,U> >
    {
      typedef algebra::mat<n, m, mln_trait_op_plus(T, U)> ret;
    };

    // mat - mat

    template < unsigned n, unsigned m, typename T, typename U >
    struct set_precise_binary_< op::minus,
				algebra::mat<n,m,T>, algebra::mat<n,m,U> >
    {
      typedef algebra::mat<n, m, mln_trait_op_minus(T, U)> ret;
    };

    // mat * mat

    template < unsigned n, unsigned o, typename T,
	       unsigned m, typename U >
    struct set_precise_binary_< op::times,
				algebra::mat<n,o,T>, algebra::mat<o,m,U> >
    {
      typedef algebra::mat<n, m, mln_sum_product(T, U)> ret;
    };

    template < unsigned o, typename T,
	       typename U >
    struct set_precise_binary_< op::times,
				algebra::mat<1,o,T>, algebra::mat<o,1,U> >
    {
      typedef mln_sum_product(T, U) ret;
    };

    // mat * vec

    template < unsigned n, unsigned m, typename T,
	       typename U >
    struct set_precise_binary_< op::times,
				algebra::mat<n,m,T>, algebra::vec<m,U> >
    {
      typedef algebra::vec<n, mln_sum_product(T, U)> ret;
    };

    template < unsigned m, typename T,
	       typename U >
    struct set_precise_binary_< op::times,
				algebra::mat<1,m,T>, algebra::vec<m,U> >
    {
      typedef mln_sum_product(T, U) ret; // a scalar
    };

    // vec * mat

    template < unsigned m, typename T,
	       typename U >
    struct set_precise_binary_< op::times,
				algebra::vec<m,T>, algebra::mat<1,m,U> >
    {
      typedef algebra::mat<m, m, mln_trait_op_times(T, U)> ret;
    };

    // mat * s

    template < unsigned n, unsigned m, typename T,
	       typename S >
    struct set_precise_binary_< op::times,
				algebra::mat<n,m,T>, mln::value::scalar_<S> >
    {
      typedef algebra::mat<n, m, mln_trait_op_times(T, S)> ret;
    };

    //     template < template<class, class> class Name,
    // 	       unsigned n, unsigned m, typename T,
    // 	       typename S >
    //     struct set_binary_< Name,
    // 			mln::Object, algebra::mat<n,m,T>,
    // 			mln::value::Scalar, S >
    //     {
    //       typedef algebra::mat<n, m, mln_trait_binary(Name, T, S)> ret;
    //     };

    // mat / s

    template < unsigned n, unsigned m, typename T,
	       typename S >
    struct set_precise_binary_< op::div,
				algebra::mat<n,m,T>, mln::value::scalar_<S> >
    {
      typedef algebra::mat<n, m, mln_trait_op_div(T, S)> ret;
    };

  } // end of namespace mln::trait



  namespace algebra
  {

    // ==

    template <unsigned n, unsigned m, typename T, typename U>
    bool
    operator==(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // - mat

    template <unsigned n, unsigned m, typename T>
    mat<n, m, mln_trait_op_uminus(T)>
    operator-(const mat<n,m,T>& lhs);

    // mat + mat

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n, m, mln_trait_op_plus(T,U)>
    operator+(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // mat - mat

    template <unsigned n, unsigned m, typename T, typename U>
    mat<n, m, mln_trait_op_minus(T,U)>
    operator-(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs);

    // mat * mat

    template <unsigned n, unsigned o, typename T,
	      unsigned m, typename U>
    mat<n, m, mln_sum_product(T,U)>
    operator*(const mat<n,o,T>& lhs, const mat<o,m,U>& rhs);

    template <unsigned o, typename T,
	      typename U>
    mln_sum_product(T,U)
      operator*(const mat<1,o,T>& lhs, const mat<o,1,U>& rhs);

    // mat * vec

    template <unsigned n, unsigned m, typename T,
	      typename U>
    vec<n, mln_sum_product(T,U)>
    operator*(const mat<n,m,T>& lhs, const vec<m,U>& rhs);

    template <unsigned m, typename T,
	      typename U>
    mln_sum_product(T,U) // scalar
      operator*(const mat<1,m,T>& lhs, const vec<m,U>& rhs);

    // vec * mat

    template <unsigned m, typename T, typename U>
    mat<m, m, mln_trait_op_times(T,U)>
    operator*(const vec<m,T>& lhs, const mat<1,m,U>& rhs);

    // mat * s

    template <unsigned n, unsigned m, typename T,
	      typename S>
    mat<n, m, mln_trait_op_times(T,S)>
    operator*(const mat<n,m,T>& lhs, const value::scalar_<S>& s);

    // mat / s

    template <unsigned n, unsigned m, typename T, typename S>
    mat<n, m, mln_trait_op_div(T,S)>
    operator/(const mat<n,m,T>& lhs, const value::scalar_<S>& s);

    // <<

    template <unsigned n, unsigned m, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,T>& v);



    // Trace.

    /// Compute the trace of a matrix \p m
    template<unsigned n, typename T>
    mln_sum(T)
    tr(const mat<n,n,T>& m);


    // Determinant.

    /// Compute the determinant of a matrix \p m
    template<unsigned n, typename T>
    mln_sum_product(T,T)
    det(const mat<n,n,T>& m);



# ifndef MLN_INCLUDE_ONLY


    // vec -> mat

    template <unsigned n, typename T>
    template <typename U>
    inline
    vec<n,T>::operator mat<n,1,U>() const
    {
      mlc_converts_to(T, U)::check();
      mat<n,1,U> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp(i, 0) = data_[i];
      return tmp;
    }


    // mat -> vec

    template <unsigned n, typename T>
    template <typename U>
    inline
    vec<n,T>::vec(const mat<n, 1, U>& rhs)
    {
      mlc_converts_to(T, U)::check();
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs(i, 0);
    }

    template <unsigned n, typename T>
    template <typename U>
    inline
    vec<n,T>&
    vec<n,T>::operator=(const mat<n, 1, U>& rhs)
    {
      mlc_converts_to(T, U)::check();
      for (unsigned i = 0; i < n; ++i)
	data_[i] = rhs(i, 0);
      return *this;
    }



    // Id

    template <unsigned n, unsigned m, typename T>
    const mat<n,m,T>
    mat<n,m,T>::Id = mat<n,m,T>::identity();

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m,T>
    mat<n,m,T>::identity()
    {
      static mat<n,m,T> id_;
      static bool flower = true;
      if (flower)
	{
	  for (unsigned i = 0; i < n; ++i)
	    for (unsigned j = 0; j < m; ++j)
	      id_(i, j) = (i == j);
	  flower = false;
	}
      return id_;
    }

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m,T>::mat()
    {
    }

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m,T>::mat(const literal::zero_t&)
    {
      this->set_all(0);
    }

    template <unsigned n, unsigned m, typename T>
    inline
    void
    mat<n,m,T>::set_id_()
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  if (i == j)
	    data_[i][j] = literal::one;
	  else
	    data_[i][j] = literal::zero;
    }

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m,T>::mat(const literal::one_t&)
    {
      this->set_id_();
    }

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m,T>::mat(const literal::identity_t&)
    {
      this->set_id_();
    }

    template <unsigned n, unsigned m, typename T>
    template <typename U>
    inline
    mat<n,m,T>::mat(const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = static_cast<T>( rhs(i, j) );
    }

    template <unsigned n, unsigned m, typename T>
    template <typename F>
    inline
    mat<n,m,T>::mat(const Function_v2v<F>& f_)
    {
      mlc_converts_to(mln_result(F), T)::check();
      const F& f = exact(f_);
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = static_cast<T>( f(i * n + j) );
    }

    template <unsigned n, unsigned m, typename T>
    template <typename U>
    inline
    mat<n,m,T>&
    mat<n,m,T>::operator=(const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = static_cast<T>( rhs(i, j) );
      return *this;
    }

    template <unsigned n, unsigned m, typename T>
    inline
    const T&
    mat<n,m,T>::operator()(unsigned i, unsigned j) const
    {
      mln_precondition(i < n && j < m);
      return data_[i][j];
    }

    template <unsigned n, unsigned m, typename T>
    inline
    T&
    mat<n,m,T>::operator()(unsigned i, unsigned j)
    {
      mln_precondition(i < n && j < m);
      return data_[i][j];
    }

    template <unsigned n, unsigned m, typename T>
    inline
    void mat<n,m,T>::set_all(const T& val)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  data_[i][j] = val;
    }

    template <unsigned n, unsigned m, typename T>
    inline
    unsigned mat<n,m,T>::size() const
    {
      return n * m;
    }

    template <unsigned n, unsigned m, typename T>
    inline
    mat<m,n,T>
    mat<n,m,T>::t() const
    {
      mat<m,n,T> tmp;
      for (unsigned i = 0; i < n; ++i)
        for (unsigned j = 0; j < m; ++j)
          tmp(j,i) = data_[i][j];
      return tmp;
    }


    namespace internal
    {

      // "Make" routines.

      template <typename T>
      inline
      mat<2,2,T>
      make(const T& t00, const T& t01,
	   const T& t10, const T& t11)
      {
	mat<2,2,T> tmp;
	tmp(0, 0) = t00;  tmp(0, 1) = t01;
	tmp(1, 0) = t10;  tmp(1, 1) = t11;
	return tmp;
      }

      template <typename T>
      inline
      mat<3,3,T>
      make(const T& t00, const T& t01, const T& t02,
	   const T& t10, const T& t11, const T& t12,
	   const T& t20, const T& t21, const T& t22)
      {
	mat<3,3,T> tmp;
	tmp(0, 0) = t00;  tmp(0, 1) = t01;  tmp(0, 2) = t02;
	tmp(1, 0) = t10;  tmp(1, 1) = t11;  tmp(1, 2) = t12;
	tmp(2, 0) = t20;  tmp(2, 1) = t21;  tmp(2, 2) = t22;
	return tmp;
      }


      // Inverse routines.

      template <typename T>
      inline
      mat<2,2,float>
      inverse(const mat<2,2,T>& m)
      {
	float d = det(m);
	mln_precondition(d != 0);
	return make<float>( + m(1,1) / d,  - m(0,1) / d,
			    - m(1,0) / d,  + m(0,0) / d );
      }

      template <typename T>
      inline
      mat<3,3,float>
      inverse(const mat<3,3,T>& m)
      {
	float d = det(m);
	mln_precondition(d != 0);
	return make<float>( det(make(m(1,1), m(1,2),
				     m(2,1), m(2,2))),

			    det(make(m(0,2), m(0,1),
				     m(2,2), m(2,1))),

			    det(make(m(0,1), m(0,2),
				     m(1,1), m(1,2))),


			    det(make(m(1,2), m(1,0),
				     m(2,2), m(2,0))),

			    det(make(m(0,0), m(0,2),
				     m(2,0), m(2,2))),

			    det(make(m(0,2), m(0,0),
				     m(1,2), m(1,0))),

			    det(make(m(1,0), m(1,1),
				     m(2,0), m(2,1))),

			    det(make(m(0,1), m(0,0),
				     m(2,1), m(2,0))),

			    det(make(m(0,0), m(0,1),
				     m(1,0), m(1,1)))
			    ) / d;
      }

      template <typename T>
      inline
      mat<4,4,float>
      inverse(const mat<4,4,T>& m)
      {
	mat<4,4,T> mo;

	// Based on MESA implementation of the GLU library.
	mo(0,0) =  m(1,1) * m(2,2) * m(3,3) - m(1,1) * m(2,3) * m(3,2) -
	           m(2,1) * m(1,2) * m(3,3) + m(2,1) * m(1,3) * m(3,2) +
	           m(3,1) * m(1,2) * m(2,3) - m(3,1) * m(1,3) * m(2,2);

	mo(1,0) = -m(1,0) * m(2,2) * m(3,3) + m(1,0) * m(2,3) * m(3,2) +
	           m(2,0) * m(1,2) * m(3,3) - m(2,0) * m(1,3) * m(3,2) -
	           m(3,0) * m(1,2) * m(2,3) + m(3,0) * m(1,3) * m(2,2);

	mo(2,0) =  m(1,0) * m(2,1) * m(3,3) - m(1,0) * m(2,3) * m(3,1) -
	           m(2,0) * m(1,1) * m(3,3) + m(2,0) * m(1,3) * m(3,1) +
	           m(3,0) * m(1,1) * m(2,3) - m(3,0) * m(1,3) * m(2,1);

	mo(3,0) = -m(1,0) * m(2,1) * m(3,2) + m(1,0) * m(2,2) * m(3,1) +
	           m(2,0) * m(1,1) * m(3,2) - m(2,0) * m(1,2) * m(3,1) -
	           m(3,0) * m(1,1) * m(2,2) + m(3,0) * m(1,2) * m(2,1);

	mo(0,1) = -m(0,1) * m(2,2) * m(3,3) + m(0,1) * m(2,3) * m(3,2) +
	           m(2,1) * m(0,2) * m(3,3) - m(2,1) * m(0,3) * m(3,2) -
	           m(3,1) * m(0,2) * m(2,3) + m(3,1) * m(0,3) * m(2,2);

	mo(1,1) =  m(0,0) * m(2,2) * m(3,3) - m(0,0) * m(2,3) * m(3,2) -
	           m(2,0) * m(0,2) * m(3,3) + m(2,0) * m(0,3) * m(3,2) +
	           m(3,0) * m(0,2) * m(2,3) - m(3,0) * m(0,3) * m(2,2);

	mo(2,1) = -m(0,0) * m(2,1) * m(3,3) + m(0,0) * m(2,3) * m(3,1) +
	           m(2,0) * m(0,1) * m(3,3) - m(2,0) * m(0,3) * m(3,1) -
	           m(3,0) * m(0,1) * m(2,3) + m(3,0) * m(0,3) * m(2,1);

	mo(3,1) =  m(0,0) * m(2,1) * m(3,2) - m(0,0) * m(2,2) * m(3,1) -
	           m(2,0) * m(0,1) * m(3,2) + m(2,0) * m(0,2) * m(3,1) +
	           m(3,0) * m(0,1) * m(2,2) - m(3,0) * m(0,2) * m(2,1);

	mo(0,2) =  m(0,1) * m(1,2) * m(3,3) - m(0,1) * m(1,3) * m(3,2) -
	           m(1,1) * m(0,2) * m(3,3) + m(1,1) * m(0,3) * m(3,2) +
	           m(3,1) * m(0,2) * m(1,3) - m(3,1) * m(0,3) * m(1,2);

	mo(1,2) = -m(0,0) * m(1,2) * m(3,3) + m(0,0) * m(1,3) * m(3,2) +
	           m(1,0) * m(0,2) * m(3,3) - m(1,0) * m(0,3) * m(3,2) -
	           m(3,0) * m(0,2) * m(1,3) + m(3,0) * m(0,3) * m(1,2);

	mo(2,2) =  m(0,0) * m(1,1) * m(3,3) - m(0,0) * m(1,3) * m(3,1) -
	           m(1,0) * m(0,1) * m(3,3) + m(1,0) * m(0,3) * m(3,1) +
	           m(3,0) * m(0,1) * m(1,3) - m(3,0) * m(0,3) * m(1,1);

	mo(3,2) = -m(0,0) * m(1,1) * m(3,2) + m(0,0) * m(1,2) * m(3,1) +
	           m(1,0) * m(0,1) * m(3,2) - m(1,0) * m(0,2) * m(3,1) -
	           m(3,0) * m(0,1) * m(1,2) + m(3,0) * m(0,2) * m(1,1);

	mo(0,3) = -m(0,1) * m(1,2) * m(2,3) + m(0,1) * m(1,3) * m(2,2) +
	           m(1,1) * m(0,2) * m(2,3) - m(1,1) * m(0,3) * m(2,2) -
	           m(2,1) * m(0,2) * m(1,3) + m(2,1) * m(0,3) * m(1,2);

	mo(1,3) =  m(0,0) * m(1,2) * m(2,3) - m(0,0) * m(1,3) * m(2,2) -
	           m(1,0) * m(0,2) * m(2,3) + m(1,0) * m(0,3) * m(2,2) +
	           m(2,0) * m(0,2) * m(1,3) - m(2,0) * m(0,3) * m(1,2);

	mo(2,3) = -m(0,0) * m(1,1) * m(2,3) + m(0,0) * m(1,3) * m(2,1) +
	           m(1,0) * m(0,1) * m(2,3) - m(1,0) * m(0,3) * m(2,1) -
	           m(2,0) * m(0,1) * m(1,3) + m(2,0) * m(0,3) * m(1,1);

	mo(3,3) =  m(0,0) * m(1,1) * m(2,2) - m(0,0) * m(1,2) * m(2,1) -
	           m(1,0) * m(0,1) * m(2,2) + m(1,0) * m(0,2) * m(2,1) +
	           m(2,0) * m(0,1) * m(1,2) - m(2,0) * m(0,2) * m(1,1);

	double det = m(0,0) * mo(0,0) + m(0,1) * mo(1,0) + m(0,2) * mo(2,0) + m(0,3) * mo(0,3);
	mln_precondition(det != 0);

	det = 1.0 / det;

	for (int i = 0; i < 4; i++)
	  for (int j = 0; j < 4; j++)
	    mo(i,j) *= det;

	return mo;
      }

    } // end of namespace algebra::internal

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m,T>
    mat<n,m,T>::_1() const
    {
      mlc_bool(m == n)::check();
      return internal::inverse(*this);
    }



    // Operators.

    template <unsigned n, unsigned m, typename T, typename U>
    inline
    bool
    operator==(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  if (lhs(i, j) != rhs(i, j))
	    return false;
      return true;
    }

    template <unsigned n, unsigned m, typename T, typename U>
    inline
    mat<n, m, mln_trait_op_plus(T,U)>
    operator+(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      mat<n, m, mln_trait_op_plus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i, j) = lhs(i, j) + rhs(i, j);
      return tmp;
    }

    template <unsigned n, unsigned m, typename T, typename U>
    inline
    mat<n,m, mln_trait_op_minus(T,U)>
    operator-(const mat<n,m,T>& lhs, const mat<n,m,U>& rhs)
    {
      mat<n,m, mln_trait_op_minus(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i, j) = lhs(i, j) - rhs(i, j);
      return tmp;
    }

    template <unsigned n, unsigned m, typename T>
    inline
    mat<n,m, mln_trait_op_uminus(T)>
    operator-(const mat<n,m,T>& rhs)
    {
      mat<n,m, mln_trait_op_uminus(T)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; i < m; ++i)
	  tmp(i, j) = - rhs(i, j);
      return tmp;
    }

    // mat * mat

    template <unsigned n, unsigned o, typename T,
	      unsigned m, typename U>
    inline
    mat<n, m, mln_sum_product(T,U)>
    operator*(const mat<n,o,T>& lhs, const mat<o,m,U>& rhs)
    {
      mat<n,m, mln_sum_product(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  {
	    tmp(i, j) = literal::zero;
	    for (unsigned k = 0; k < o; ++k)
	      tmp(i, j) += lhs(i, k) * rhs(k, j);
	  }
      return tmp;
    }

    template <unsigned o, typename T,
	      typename U>
    inline
    mln_sum_product(T,U)
      operator*(const mat<1,o,T>& lhs, const mat<o,1,U>& rhs)
    {
      mln_sum_product(T,U) tmp(literal::zero);
      for (unsigned k = 0; k < o; ++k)
	tmp += lhs(0, k) * rhs(k, 0);
      return tmp;
    }

    // mat * vec

    template <unsigned n, unsigned m, typename T,
	      typename U>
    inline
    vec<n, mln_sum_product(T,U)>
    operator*(const mat<n,m,T>& lhs, const vec<m,U>& rhs)
    {
      vec<n, mln_sum_product(T,U)> tmp;
      for (unsigned i = 0; i < n; ++i)
	{
	  mln_sum_product(T,U) sum(literal::zero);
	  for (unsigned j = 0; j < m; ++j)
	    sum += lhs(i, j) * rhs[j];
	  tmp[i] = sum;
	}
      return tmp;
    }

    template <unsigned m, typename T,
	      typename U>
    inline
    mln_sum_product(T,U) // scalar
      operator*(const mat<1,m,T>& lhs, const vec<m,U>& rhs)
    {
      mln_sum_product(T,U) tmp(literal::zero);
      for (unsigned j = 0; j < m; ++j)
	tmp += lhs(0, j) * rhs[j];
      return tmp;
    }

    // vec * mat

    template <unsigned m, typename T,
	      typename U>
    inline
    mat<m, m, mln_trait_op_times(T,U)>
    operator*(const vec<m,T>& lhs, const mat<1,m,U>& rhs)
    {
      mat<m, m, mln_trait_op_times(T,U)> tmp;
      for (unsigned i = 0; i < m; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i, j) = lhs[i] * rhs(0, j);
      return tmp;
    }

    // mat * s

    template <unsigned n, unsigned m, typename T, typename S>
    inline
    mat<n, m, mln_trait_op_times(T,S)>
    operator*(const mat<n,m,T>& lhs, const value::scalar_<S>& s_)
    {
      S s = s_.to_equiv();
      mat<n, m, mln_trait_op_times(T,S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i, j) = lhs(i, j) * s;
      return tmp;
    }

    template <unsigned n, unsigned m, typename T, typename S>
    inline
    mat<n,m, mln_trait_op_div(T,S)>
    operator/(const mat<n,m,T>& lhs, const value::scalar_<S>& s_)
    {
      S s = s_.to_equiv();
      mat<n,m, mln_trait_op_times(T,S)> tmp;
      for (unsigned i = 0; i < n; ++i)
	for (unsigned j = 0; j < m; ++j)
	  tmp(i,j) = lhs(i, j) / s;
      return tmp;
    }

    // <<

    template <unsigned n, unsigned m, typename T>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const mat<n,m,T>& v)
    {
      for (unsigned i = 0; i < n; ++i)
	{
	  ostr << '[';
	  for (unsigned j = 0; j < m; ++j)
	    ostr << debug::format(v(i, j)) << (j == m - 1 ? "]" : ", ");
	  ostr << std::endl;
	}
      return ostr;
    }


    // Trace.

    template<unsigned n, typename T>
    inline
    mln_sum(T)
    tr(const mat<n,n,T>& m)
    {
      mln_sum(T) tr_ = literal::zero;
      for (unsigned i = 0; i < n; ++i)
        tr_ += m(i,i);
      return tr_;
    }


    // Determinant.

    namespace impl
    {

      template<unsigned n, typename T>
      mln_sum_product(T,T)
      det(const mat<n,n,T>& m)
      {
	// Not implemented.
	mlc_abort(T)::check();
      }

      template<typename T>
      inline
      mln_sum_product(T,T)
      det(const mat<2,2,T>& m)
      {
	return m(0,0) * m(1,1) - m(0,1) * m(1,0);
      }

      template<typename T>
      inline
      mln_sum_product(T,T)
      det(const mat<3,3,T>& m)
      {
	return
	  + m(0,0) * m(1,1) * m(2,2)
	  - m(0,0) * m(1,2) * m(2,1)
	  - m(0,1) * m(1,0) * m(2,2)
	  + m(0,1) * m(1,2) * m(2,0)
	  + m(0,2) * m(1,0) * m(2,1)
	  - m(0,2) * m(1,1) * m(2,0);
      }

      template<typename T>
      inline
      mln_sum_product(T,T)
      det(const mat<4,4,T>& m)
      {
	return m(0,3) * m(1,2) * m(2,1) * m(3,0) - m(0,2) * m(1,3) * m(2,1) * m(3,0) -
	  m(0,3) * m(1,1) * m(2,2) * m(3,0) + m(0,1) * m(1,3) * m(2,2) * m(3,0) +
	  m(0,2) * m(1,1) * m(2,3) * m(3,0) - m(0,1) * m(1,2) * m(2,3) * m(3,0) -
	  m(0,3) * m(1,2) * m(2,0) * m(3,1) + m(0,2) * m(1,3) * m(2,0) * m(3,1) +
	  m(0,3) * m(1,0) * m(2,2) * m(3,1) - m(0,0) * m(1,3) * m(2,2) * m(3,1) -
	  m(0,2) * m(1,0) * m(2,3) * m(3,1) + m(0,0) * m(1,2) * m(2,3) * m(3,1) +
	  m(0,3) * m(1,1) * m(2,0) * m(3,2) - m(0,1) * m(1,3) * m(2,0) * m(3,2) -
	  m(0,3) * m(1,0) * m(2,1) * m(3,2) + m(0,0) * m(1,3) * m(2,1) * m(3,2) +
	  m(0,1) * m(1,0) * m(2,3) * m(3,2) - m(0,0) * m(1,1) * m(2,3) * m(3,2) -
	  m(0,2) * m(1,1) * m(2,0) * m(3,3) + m(0,1) * m(1,2) * m(2,0) * m(3,3) +
	  m(0,2) * m(1,0) * m(2,1) * m(3,3) - m(0,0) * m(1,2) * m(2,1) * m(3,3) -
	  m(0,1) * m(1,0) * m(2,2) * m(3,3) + m(0,0) * m(1,1) * m(2,2) * m(3,3);
      }

    } // end of namespace mln::algebra::impl


    template<unsigned n, typename T>
    inline
    mln_sum_product(T,T)
      det(const mat<n,n,T>& m)
    {
      return mln::algebra::impl::det(m);
    }




    // vec methods.

    template <unsigned n, typename T>
    inline
    mat<1,n,T>
    vec<n,T>::t() const
    {
      mat<1,n,T> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp(0,i) = data_[i];
      return tmp;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::algebra

} // end of namespace mln

# include <mln/make/mat.hh>

#endif // ! MLN_ALGEBRA_MAT_HH
