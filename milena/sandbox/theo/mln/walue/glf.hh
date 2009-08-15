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

#ifndef MLN_WALUE_GLF_HH
# define MLN_WALUE_GLF_HH

/// \file
///
/// Define a gray-level type with floating quantization.

# include <mln/core/concept/proxy.hh>
# include <mln/value/scalar.hh>
# include <mln/value/builtin/ops.hh>
# include <mln/walue/set_encoding.hh>
# include <mln/walue/approx_equal.hh>


namespace mln
{


  // Forward declarations.
  namespace walue {
    template <typename F> struct glf;
    template <unsigned n> struct gl;
  }
  namespace literal {
    struct white_t;
    struct black_t;
  }


  namespace walue
  {

    /// Type aliases.
    typedef glf<float>  glf1;
    typedef glf<double> glf2;


    /// FIXME
    //
    template <typename F>
    class glf
      : public Proxy< glf<F> >,
	public mln::internal::proxy_impl< F, glf<F> >,
	private metal::or_< mlc_equal(F, float),
			    mlc_equal(F, double) >::check_t
    {
    public:

      /// Encoding associated type.
      typedef F encoding_t;

      /// Constructor without argument.
      glf()
      {
      }

      /// Constructor from a floating value.
      glf(F f)
      {
	mln_precondition(f >= def::minus_epsilon &&
			 f <= def::one_plus_epsilon);
	v_ = f;
      }

      /// Assignment from a floating value.
      glf& operator=(F f)
      {
	mln_precondition(f >= def::minus_epsilon &&
			 f <= def::one_plus_epsilon);
	v_ = f;
	return *this;
      }


      glf(const mln::literal::white_t&) { v_ = 1; }
      glf& operator=(const mln::literal::white_t&) { v_ = 1; return *this; }

      glf(const mln::literal::black_t&) { v_ = 0; }
      glf& operator=(const mln::literal::black_t&) { v_ = 0; return *this; }


      template <typename T>
      glf(const internal::encoding<T>& e)
      {
	mlc_converts_to(T, F)::check();
	v_ = e.value();
	mln_postcondition(static_cast<T>(v_) == e.value());
      }

      F encoding() const
      {
	return v_;
      }

      glf& set_encoding(F f)
      {
	mln_precondition(f >= def::minus_epsilon &&
			 f <= def::one_plus_epsilon);
	v_ = f;
	return *this;
      }


      // As a proxy.
      F subj_()
      {
	return v_;
      }

    protected:

      F v_;
    };


# ifndef MLN_INCLUDE_ONLY

    // FIXME

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::walue

} // end of namespace mln


#endif // ! MLN_WALUE_GLF_HH
