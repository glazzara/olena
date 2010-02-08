// Copyright (C) 2007, 2008, 2009, 2010, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_RGB_HH
# define MLN_VALUE_RGB_HH


# include <mln/value/ops.hh>

# include <mln/value/concept/vectorial.hh>
# include <mln/value/int_u.hh>
# include <mln/algebra/vec.hh>

# include <mln/value/internal/make_generic_name.hh>

# include <mln/convert/from_to.hh>

// FIXME: should we consider that mln_min may be negative? => wrong
// color formulae.

namespace mln
{

  // Forward declaration.
  namespace value {
    template <unsigned n> struct rgb;
    template <typename H, typename S, typename L> class hsl_;

  }
  namespace fun {
    namespace v2v {
      template <typename T_hsl> struct f_rgb_to_hsl_;
      typedef f_rgb_to_hsl_<value::hsl_<float, float, float> > f_rgb_to_hsl_f_t;
      extern f_rgb_to_hsl_f_t f_rgb_to_hsl_f;
    }
  }


  namespace literal
  {
    // Forward declarations.
    struct black_t;
    struct white_t;

    struct light_gray_t;
    struct medium_gray_t;
    struct dark_gray_t;

    struct red_t;
    struct green_t;
    struct blue_t;
    struct brown_t;
    struct lime_t;
    struct orange_t;
    struct pink_t;
    struct purple_t;
    struct teal_t;
    struct violet_t;
    struct cyan_t;
    struct magenta_t;
    struct yellow_t;
    struct olive_t;
    //
  }


  // Forward declaration.
  namespace value
  {
    template <typename H, typename S, typename L> class hsl_;
  }

  namespace trait
  {
    template < unsigned n >
    struct set_precise_binary_< op::plus, mln::value::rgb<n>, mln::value::rgb<n> >
    {
      typedef mln::value::rgb<n> ret;
    };

    template < unsigned n >
    struct set_precise_binary_< op::minus, mln::value::rgb<n>, mln::value::rgb<n> >
    {
      typedef mln::value::rgb<n> ret;
    };

    template < unsigned n, typename S >
    struct set_precise_binary_< op::times, mln::value::rgb<n>, mln::value::scalar_<S> >
    {
      typedef mln::value::rgb<n> ret;
    };

    template < unsigned n, typename S >
    struct set_precise_binary_< op::div, mln::value::rgb<n>, mln::value::scalar_<S> >
    {
      typedef mln::value::rgb<n> ret;
    };


    // FIXME : Is there any way more generic? a way to factor
    //  set_precise_binary_< op::div, mln::value::rgb<n>, mln::value::scalar_<S> >
    //  and
    //  set_precise_binary_< op::div, mln::value::rgb<n>, mln::value::int_u<m> >
    //  as for op::times.

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::times, mln::value::rgb<n>, mln::value::int_u<m> >
    {
      typedef mln::value::rgb<n> ret;
    };

    template < unsigned n, unsigned m >
    struct set_precise_binary_< op::div, mln::value::rgb<n>, mln::value::int_u<m> >
    {
      typedef mln::value::rgb<n> ret;
    };


//     template < unsigned n, typename I >
//     struct set_binary_< op::times,
// 			mln::value::Vectorial, mln::value::rgb<n>,
// 			mln::value::Integer, I >
//     {
//       typedef mln::value::rgb<n> ret;
//     };

//     template < unsigned n, typename S >
//     struct set_binary_< op::times,
// 			mln::value::Scalar, S,
// 			mln::value::Vectorial, mln::value::rgb<n> >
//     {
//       typedef mln::value::rgb<n> ret;
//     };

    template <unsigned n>
    struct value_< mln::value::rgb<n> >
    {
      enum {
	dim = 3,
	nbits = dim * n,
	card  = mln_value_card_from_(nbits)
      };

      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::color       kind;
      typedef trait::value::quant::high /*mln_value_quant_from_(card)*/     quant;

      typedef void comp;
      typedef mln::value::int_u<n> comp_0;
      typedef mln::value::int_u<n> comp_1;
      typedef mln::value::int_u<n> comp_2;

      template <typename V> static comp_0 get_comp_0(const V& v) { return v.red(); }
      template <typename V> static comp_1 get_comp_1(const V& v) { return v.green(); }
      template <typename V> static comp_2 get_comp_2(const V& v) { return v.blue(); }

      typedef algebra::vec<dim, float> sum;

      static const char* name()
      {
	static std::string
	  s = mln::value::internal::make_generic_name("rgb", n);
	return s.c_str();
      }

    };

  } // end of namespace trait



  namespace value
  {

    /*!
      \brief Color class for red-green-blue where every component is
      n-bit encoded.

      \ingroup valuergb
    */
    template <unsigned n>
    struct rgb
      :
      public Vectorial< rgb<n> >
      ,
      public internal::value_like_< algebra::vec< 3, int_u<n> >, // Equivalent.
				    algebra::vec< 3, int_u<n> >, // Encoding.
				    algebra::vec< 3, int >,      // Interoperation.
				    rgb<n> >			 // Exact.
    {
    public:

      typedef int_u<n> red_t;
      typedef int_u<n> green_t;
      typedef int_u<n> blue_t;

      /// \name Access to red/green/blue component.
      /// \{
      int_u<n>  red() const   { return this->v_[0]; }
      int_u<n>& red()         { return this->v_[0]; }

      int_u<n>  green() const { return this->v_[1]; }
      int_u<n>& green()       { return this->v_[1]; }

      int_u<n>  blue() const  { return this->v_[2]; }
      int_u<n>& blue()        { return this->v_[2]; }

      int_u<n>  comp(unsigned k) const { return this->v_[k]; }
      int_u<n>& comp(unsigned k)       { return this->v_[k]; }
      /// \}

      /// Constructor without argument.
      rgb<n>();

      /// Constructor from component values.
      rgb<n>(int r, int g, int b);

      /// Constructor from a algebra::vec.
      rgb<n>(const algebra::vec<3, int>& rhs);
      rgb<n>(const algebra::vec<3, unsigned>& rhs);
      rgb<n>(const algebra::vec<3, int_u<n> >& rhs);
      rgb<n>(const algebra::vec<3, float>& rhs);

      // Conversion to the interoperation type.
      operator algebra::vec<3, int>() const   { return this->v_; }
      // Conversion to the sum type.
      operator algebra::vec<3, float>() const { return this->v_; }

      /// \name Constructors with literals.
      /// \{
      rgb<n>(const mln::literal::white_t&);
      rgb<n>(const mln::literal::black_t&);

      rgb<n>(const mln::literal::light_gray_t&);
      rgb<n>(const mln::literal::medium_gray_t&);
      rgb<n>(const mln::literal::dark_gray_t&);

      rgb<n>(const mln::literal::red_t&);
      rgb<n>(const mln::literal::blue_t&);
      rgb<n>(const mln::literal::green_t&);
      rgb<n>(const mln::literal::brown_t&);
      rgb<n>(const mln::literal::lime_t&);
      rgb<n>(const mln::literal::orange_t&);
      rgb<n>(const mln::literal::pink_t&);
      rgb<n>(const mln::literal::purple_t&);
      rgb<n>(const mln::literal::teal_t&);
      rgb<n>(const mln::literal::violet_t&);
      rgb<n>(const mln::literal::cyan_t&);
      rgb<n>(const mln::literal::magenta_t&);
      rgb<n>(const mln::literal::yellow_t&);
      rgb<n>(const mln::literal::olive_t&);
      /// \}

      /// Assignment.
      rgb<n>& operator=(const rgb<n>& rhs);

      /// Zero value.
      static const rgb<n> zero;
    };



    /// Print an rgb \p c into the output stream \p ostr.
    ///
    /// \param[in,out] ostr An output stream.
    /// \param[in] c An rgb.
    ///
    /// \return The modified output stream \p ostr.
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const rgb<n>& c);

    template <unsigned n>
    std::istream& operator>>(std::istream& istr, rgb<n>& c);


    /* FIXME: We should not need to define these operators, thanks to
       Milena's global operator resolution mechanism based on
       mln::Object.  See what prevent us to use this mechanism.  */

    /* FIXME: Cannot work for i negative; add traits! (2008-02-16,
       Roland: What does this comment mean?)  */

    /// \name Addition.
    /// {
    template <unsigned n>
    typename rgb<n>::interop
    operator+(const rgb<n>& lhs, const rgb<n>& rhs);

    template <unsigned n>
    typename rgb<n>::interop
    operator+(const typename rgb<n>::interop& lhs, const rgb<n>& rhs);

    template <unsigned n>
    typename rgb<n>::interop
    operator+(const rgb<n>& lhs, const typename rgb<n>::interop& rhs);
    /// \}

    /// \name Subtraction.
    /// \{
    template <unsigned n>
    typename rgb<n>::interop
    operator-(const rgb<n>& lhs, const rgb<n>& rhs);

    template <unsigned n>
    typename rgb<n>::interop
    operator-(const typename rgb<n>::interop& lhs, const rgb<n>& rhs);

    template <unsigned n>
    typename rgb<n>::interop
    operator-(const rgb<n>& lhs, const typename rgb<n>::interop& rhs);
    /// \}

    /// \name Product.
    /// \{
    template <unsigned n, typename S>
    inline
    typename rgb<n>::interop
    operator*(const rgb<n>& lhs, const mln::value::scalar_<S>& s);

    template <unsigned n, typename S>
    inline
    typename rgb<n>::interop
    operator*(const mln::value::scalar_<S>& s, const rgb<n>& lhs);
    /// \}

    /// \name Division.
    /// \{
    template <unsigned n, typename S>
    inline
    typename rgb<n>::interop
    operator/(const rgb<n>& lhs, const mln::value::scalar_<S>& s);
    /// \}

  } // end of namespace mln::value


  // Conversions

  namespace value
  {

    /// \internal Conversion: rgb -> bool.
    template <unsigned m>
    void from_to_(const value::rgb<m>& from, bool& to);

    /// \internal Conversion: rgb to hsl_
    void
    from_to_(const rgb<16>& from, hsl_<float,float,float>& to);

    /// \internal Conversion: rgb to hsl_
    void
    from_to_(const rgb<8>& from, hsl_<float,float,float>& to);

    /// \internal Conversion: bool -> rgb.
    template <unsigned m>
    void from_to_(bool from, value::rgb<m>& to);

  } // end of namespace mln::value

} // end of namespace mln


// Used in from_to
# include <mln/value/hsl.hh>
# include <mln/fun/v2v/rgb_to_hsl.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace value
  {

    /*---------------.
    | Construction.  |
    `---------------*/

    template <unsigned n>
    inline
    rgb<n>::rgb()
    {
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const algebra::vec<3, int>& v)
    {
      this->v_ = v;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const algebra::vec<3, unsigned>& v)
    {
      this->v_ = v;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const algebra::vec<3, int_u<n> >& v)
    {
      this->v_ = v;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const algebra::vec<3, float>& v)
    {
      convert::from_to(v[0], this->v_[0]);
      convert::from_to(v[1], this->v_[1]);
      convert::from_to(v[2], this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(int r, int g, int b)
    {
      mln_precondition(r >= 0);
      mln_precondition(g >= 0);
      mln_precondition(b >= 0);
      mln_precondition(unsigned(r) <= mln_max(int_u<n>));
      mln_precondition(unsigned(g) <= mln_max(int_u<n>));
      mln_precondition(unsigned(b) <= mln_max(int_u<n>));
      this->v_[0] = r;
      this->v_[1] = g;
      this->v_[2] = b;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::white_t&)
    {
      this->v_[0] = mln_max(int_u<n>);
      this->v_[1] = mln_max(int_u<n>);
      this->v_[2] = mln_max(int_u<n>);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::black_t&)
    {
      this->v_[0] = 0;
      this->v_[1] = 0;
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::light_gray_t&)
    {
      convert::from_to(mln_max(int_u<n>) * 0.75, this->v_[0]);
      convert::from_to(mln_max(int_u<n>) * 0.75, this->v_[1]);
      convert::from_to(mln_max(int_u<n>) * 0.75, this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::medium_gray_t&)
    {
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[0]);
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[1]);
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::dark_gray_t&)
    {
      convert::from_to(mln_max(int_u<n>) * 0.25, this->v_[0]);
      convert::from_to(mln_max(int_u<n>) * 0.25, this->v_[1]);
      convert::from_to(mln_max(int_u<n>) * 0.25, this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::red_t&)
    {
      this->v_[0] = mln_max(int_u<n>);
      this->v_[1] = 0;
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::green_t&)
    {
      this->v_[0] = 0;
      this->v_[1] = mln_max(int_u<n>);
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::blue_t&)
    {
      this->v_[0] = 0;
      this->v_[1] = 0;
      this->v_[2] = mln_max(int_u<n>);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::brown_t&)
    {
      convert::from_to(mln_max(int_u<n>) * 0.75, this->v_[0]);
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[1]);
      convert::from_to(mln_max(int_u<n>) * 0.25, this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::lime_t&)
    {
      convert::from_to(mln_max(int_u<n>) * 0.75, this->v_[0]);
      this->v_[1] = mln_max(int_u<n>);
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::orange_t&)
    {
      this->v_[0] = mln_max(int_u<n>);
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[1]);
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::pink_t&)
    {
      this->v_[0] = mln_max(int_u<n>);
      convert::from_to(mln_max(int_u<n>) * 0.75, this->v_[1]);
      convert::from_to(mln_max(int_u<n>) * 0.75, this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::purple_t&)
    {
      convert::from_to(mln_max(int_u<n>) * 0.75, this->v_[0]);
      this->v_[1] = 0;
      convert::from_to(mln_max(int_u<n>) * 0.25, this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::teal_t&)
    {
      this->v_[0] = 0;
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[1]);
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::violet_t&)
    {
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[0]);
      this->v_[1] = 0;
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[2]);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::cyan_t&)
    {
      this->v_[0] = 0;
      this->v_[1] = mln_max(int_u<n>);
      this->v_[2] = mln_max(int_u<n>);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::magenta_t&)
    {
      this->v_[0] = mln_max(int_u<n>);
      this->v_[1] = 0;
      this->v_[2] = mln_max(int_u<n>);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::yellow_t&)
    {
      this->v_[0] = mln_max(int_u<n>);
      this->v_[1] = mln_max(int_u<n>);
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const mln::literal::olive_t&)
    {
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[0]);
      convert::from_to(mln_max(int_u<n>) * 0.50, this->v_[1]);
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>&
    rgb<n>::operator=(const rgb<n>& rhs)
    {
      if (& rhs == this)
	return *this;
      this->v_ = rhs.v_;
      return *this;
    }

    template <unsigned n>
    const rgb<n> rgb<n>::zero(0,0,0);

    /*------------.
    | Operators.  |
    `------------*/

    template <unsigned n>
    inline
    typename rgb<n>::interop
    operator+(const rgb<n>& lhs, const rgb<n>& rhs)
    {
      typename rgb<n>::interop tmp(lhs.to_interop() + rhs.to_interop());
      return tmp;
    }

    template <unsigned n>
    inline
    typename rgb<n>::interop
    operator+(const rgb<n>& lhs, const typename rgb<n>::interop& rhs)
    {
      typename rgb<n>::interop tmp(lhs.to_interop() + rhs);
      return tmp;
    }

    template <unsigned n>
    inline
    typename rgb<n>::interop
    operator+(const typename rgb<n>::interop& lhs, const rgb<n>& rhs)
    {
      typename rgb<n>::interop tmp(lhs + rhs.to_interop());
      return tmp;
    }

    template <unsigned n>
    inline
    typename rgb<n>::interop
    operator-(const rgb<n>& lhs, const rgb<n>& rhs)
    {
      typename rgb<n>::interop tmp(lhs.to_interop() - rhs.to_interop());
      return tmp;
    }

    template <unsigned n>
    inline
    typename rgb<n>::interop
    operator-(const rgb<n>& lhs, const typename rgb<n>::interop& rhs)
    {
      typename rgb<n>::interop tmp(lhs.to_interop() - rhs);
      return tmp;
    }

    template <unsigned n>
    inline
    typename rgb<n>::interop
    operator-(const typename rgb<n>::interop& lhs, const rgb<n>& rhs)
    {
      typename rgb<n>::interop tmp(lhs - rhs.to_interop());
      return tmp;
    }

    template <unsigned n, typename S>
    inline
    typename rgb<n>::interop
    operator*(const rgb<n>& lhs, const mln::value::scalar_<S>& s)
    {
      typename rgb<n>::interop tmp(lhs.to_interop() * s.to_equiv());
      return tmp;
    }

    template <unsigned n, typename S>
    inline
    typename rgb<n>::interop
    operator*(const mln::value::scalar_<S>& s, const rgb<n>& lhs)
    {
      typename rgb<n>::interop tmp(s.to_equiv() * lhs.to_interop());
      return tmp;
    }

    template <unsigned n, typename S>
    inline
    typename rgb<n>::interop
    operator/(const rgb<n>& lhs, const mln::value::scalar_<S>& s)
    {
      typename rgb<n>::interop tmp(lhs.to_interop() / s.to_equiv());
      return tmp;
    }

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const rgb<n>& v)
    {
      return ostr << '(' << debug::format(v.red())
		  << ',' << debug::format(v.green())
		  << ',' << debug::format(v.blue())
		  << ')';
    }

    template <unsigned n>
    inline
    std::istream& operator>>(std::istream& istr, rgb<n>& c)
    {
      return istr >> c.red() >> c.green() >> c.blue();
    }


    // Conversions

    template <unsigned m>
    void
    from_to_(const rgb<m>& from, bool& to)
    {
      to = (from.red() != 0 && from.green() != 0 && from.blue() != 0);
    }

    inline
    void
    from_to_(const rgb<16>& from, hsl_<float,float,float>& to)
    {
      to = fun::v2v::f_rgb_to_hsl_f(from);
    }

    inline
    void
    from_to_(const rgb<8>& from, hsl_<float,float,float>& to)
    {
      to = fun::v2v::f_rgb_to_hsl_f(from);
    }

    template <unsigned m>
    void
    from_to_(bool from, mln::value::rgb<m>& to)
    {
      static mln::literal::white_t* white_ = 0;
      static mln::literal::black_t* black_ = 0;
      // We do not use literal::white (the object) so that we
      // do not introduce any coupling with the file where
      // literals are defined.
      if (from)
	to = *white_;
      else
	to = *black_;
    }

  } // end of namespace mln::value

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_VALUE_RGB_HH
