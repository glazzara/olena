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

#ifndef MLN_WALUE_GL_HH
# define MLN_WALUE_GL_HH

/// \file
///
/// Define a gray-level type with discrete quantization.

# include <mln/core/concept/walue.hh>
# include <mln/walue/int_u.hh>
# include <mln/walue/glf.hh>
# include <mln/walue/set_encoding.hh>

// Extra usefull includes.
# include <mln/value/scalar.hh>
# include <mln/value/builtin/ops.hh>


namespace mln
{


  // Forward declarations.
  namespace walue {
    template <unsigned n> struct gl;
  }
  namespace literal {
    struct white_t;
    struct black_t;
  }


  namespace walue
  {

    namespace internal
    {

      // Max encoding with n bits.

      template <unsigned n>
      struct pow_two
      {
	static const unsigned long max_ = 2 * pow_two<n - 1>::max_ + 1;
	static float max() { return float(max_); }
      };

      template <>
      struct pow_two< 1 >
      {
	static const unsigned long max_ = 1;
      };

    } // internal



    /// FIXME
    //
    template <unsigned n>
    class gl
      : public Proxy< gl<n> >,
	public mln::internal::proxy_impl< glf1, gl<n> >,
	private metal::bool_<(n > 1)>::check_t
    {
    public:

      /// Encoding associated type.
      typedef typename internal::encoding_type_<n>::ret encoding_t;

      /// Constructor without argument.
      gl()
      {
      }

      /// Copy constructor.
      gl(const gl<n>& g)
      {
	v_ = g.v_;
      }

      /// Assignment operator.
      gl& operator=(const gl<n>& g)
      {
	v_ = g.v_;
	return *this;
      }


      /// From a floating value.
      gl(float f) { set_(f); }
      gl& operator=(float f) { set_(f); return *this; }
      
      /// From a double precision floating value.
      gl(double f) { set_(f); }
      gl& operator=(double f) { set_(f); return *this; }

      /// From another discrete gray-level.
      template <unsigned m>
      gl(const gl<m>& g) { set_(g); }
      template <unsigned m>
      gl& operator=(const gl<m>& g) { set_(g); return *this; }

      /// From a floating encoded gray-level.
      template <typename F>
      gl(const glf<F>& f) { set_(f); }
      template <typename F>
      gl& operator=(const glf<F>& f) { set_(f); return *this; }

      // Safety.
      template <typename T>
      gl(T)
      {
	mlc_abort(T)::check();
      }

      // Safety.
      template <typename T>
      void operator=(T)
      {
	mlc_abort(T)::check();
      }


      gl(const mln::literal::white_t&)
      {
	v_ = internal::pow_two<n>::max_;
      }
      gl& operator=(const mln::literal::white_t&)
      {
	v_ = internal::pow_two<n>::max_;
	return *this;
      }

      gl(const mln::literal::black_t&) { v_ = 0; }
      gl& operator=(const mln::literal::black_t&) { v_ = 0; return *this; }


      template <typename T>
      gl(const internal::encoding<T>& e)
      {
	// Dev note: no need for operator==(internal::encoding).
	mlc_converts_to(T, encoding_t)::check();
	mln_precondition(e.value() >= 0 &&
			 e.value() <= internal::pow_two<n>::max_);
	v_ = e.value();
	mln_postcondition(static_cast<T>(v_) == e.value());
      }

      encoding_t encoding() const
      {
	return v_;
      }

      gl& set_encoding(encoding_t v)
      {
	v_ = v;
	return *this;
      }


      // Give the subject (as a Proxy).
      glf1 subj_()
      {
	return float(v_) / internal::pow_two<n>::max();
      }

    protected:

      encoding_t v_;

    private:

      void set_(float f)
	{
	  mln_precondition(f >= def::minus_epsilon &&
			   f <= def::one_plus_epsilon);
	  v_ = encoding_t(f * internal::pow_two<n>::max() + 0.5f);
	}

    }; // end of class gl<n>


    template <unsigned n>
    std::ostream&
    operator<<(std::ostream& ostr, const gl<n>& g)
    {
      return ostr << (unsigned long)(g.encoding());
    }


    // approx_equal

    template <unsigned n>
    inline
    bool approx_equal(const gl<n>& lhs, const gl<n>& rhs)
    {
      return lhs.encoding() == rhs.encoding();
    }

    template <unsigned n, unsigned m>
    inline
    bool approx_equal(const gl<n>& lhs, const gl<m>& rhs)
    {
      if (n < m)
	return lhs.encoding() == gl<n>(rhs).encoding();
      else
	return approx_equal(rhs, lhs);
    }

    template <unsigned n, typename F>
    inline
    bool approx_equal(const gl<n>& lhs, const glf<F>& rhs)
    {
      return lhs.encoding() == gl<n>(rhs).encoding();
    }

    template <unsigned n>
    inline
    bool approx_equal(const gl<n>& lhs, float rhs)
    {
      return lhs.encoding() == gl<n>(rhs).encoding();
    }

    template <unsigned n>
    inline
    bool approx_equal(const gl<n>& lhs, double rhs)
    {
      return lhs.encoding() == gl<n>(rhs).encoding();
    }

    // Safety.
    template <unsigned n, typename T>
    inline
    bool approx_equal(const gl<n>& lhs, T)
    {
      mlc_abort(T)::check();
      return false;
    }

    // With glf<F> as left hand side.
    template <typename F, unsigned n>
    inline
    bool approx_equal(const glf<F>& lhs, const gl<n>& rhs)
    {
      return approx_equal(rhs, lhs);
    }


    // Operators.

    template <unsigned n>
    bool operator==(const gl<n>& lhs, const gl<n>& rhs);

    template <unsigned n>
    bool operator<(const gl<n>& lhs, const gl<n>& rhs);


    template <unsigned n, unsigned m>
    bool operator==(const gl<n>& lhs, const gl<m>& rhs);

    template <unsigned n, unsigned m>
    bool operator<(const gl<n>& lhs, const gl<m>& rhs);

    template <unsigned n, unsigned m>
    bool operator<=(const gl<n>& lhs, const gl<m>& rhs);


# ifndef MLN_INCLUDE_ONLY


    // FIXME: Move definitions here...


    // Operators.

    template <unsigned n>
    inline
    bool operator==(const gl<n>& lhs, const gl<n>& rhs)
    {
      return lhs.encoding() == rhs.encoding();
    }

    template <unsigned n>
    inline
    bool operator<(const gl<n>& lhs, const gl<n>& rhs)
    {
      return lhs.encoding() < rhs.encoding();
    }

    template <unsigned n, unsigned m>
    inline
    bool operator==(const gl<n>& lhs, const gl<m>& rhs)
    {
      typedef unsigned long long ull_t;
      return
	(ull_t)lhs.encoding() * (ull_t)internal::pow_two<m>::max_ ==
	(ull_t)rhs.encoding() * (ull_t)internal::pow_two<n>::max_;
    }

    // FIXME: For low quantizations, some better code can be
    // implemented.  For instance
    //   op == { return std::abs(float(lhs) - float(rhs)) < def::epsilon; }
    //   op <  { return float(lhs) < float(rhs) - def::epsilon; }
    //   op <= { return float(lhs) < float(rhs) + def::epsilon; }

    template <unsigned n, unsigned m>
    inline
    bool operator<(const gl<n>& lhs, const gl<m>& rhs)
    {
      typedef unsigned long long ull_t;
      return
	(ull_t)lhs.encoding() * (ull_t)internal::pow_two<m>::max_ <
	(ull_t)rhs.encoding() * (ull_t)internal::pow_two<n>::max_;
    }

    template <unsigned n, unsigned m>
    inline
    bool operator<=(const gl<n>& lhs, const gl<m>& rhs)
    {
      typedef unsigned long long ull_t;
      return
	(ull_t)lhs.encoding() * (ull_t)internal::pow_two<m>::max_ <=
	(ull_t)rhs.encoding() * (ull_t)internal::pow_two<n>::max_;
    }


    // FIXME: Add specializations with float and double?


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::walue

} // end of namespace mln


#endif // ! MLN_WALUE_GL_HH
