// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_VALUE_QT_RGB32_HH
# define MLN_VALUE_QT_RGB32_HH

# include <cstring>

# include <mln/value/ops.hh>

# include <mln/value/concept/vectorial.hh>
# include <mln/value/int_u.hh>
# include <mln/algebra/vec.hh>

# include <mln/value/rgb8.hh>

# include <mln/literal/zero.hh>
# include <mln/literal/black.hh>
# include <mln/literal/white.hh>

namespace mln
{

  // Forward declaration.
  namespace value { namespace qt { struct rgb32; } }



  namespace literal
  {
    /// \{ Forward declarations.
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
    /// \}
  }

  namespace trait
  {
    template <>
    struct set_precise_binary_< op::plus, mln::value::qt::rgb32, mln::value::qt::rgb32 >
    {
      typedef mln::value::qt::rgb32 ret;
    };

    template <>
    struct set_precise_binary_< op::minus, mln::value::qt::rgb32, mln::value::qt::rgb32 >
    {
      typedef mln::value::qt::rgb32 ret;
    };

    template < typename S >
    struct set_precise_binary_< op::times, mln::value::qt::rgb32, mln::value::scalar_<S> >
    {
      typedef mln::value::qt::rgb32 ret;
    };

    template < typename S >
    struct set_precise_binary_< op::div, mln::value::qt::rgb32, mln::value::scalar_<S> >
    {
      typedef mln::value::qt::rgb32 ret;
    };


    // FIXME : Is there any way more generic? a way to factor
    //  set_precise_binary_< op::div, mln::value::qt::rgb32, mln::value::scalar_<S> >
    //  and
    //  set_precise_binary_< op::div, mln::value::qt::rgb32, mln::value::int_u<m> >
    //  as for op::times.

    template < unsigned m >
    struct set_precise_binary_< op::times, mln::value::qt::rgb32, mln::value::int_u<m> >
    {
      typedef mln::value::qt::rgb32 ret;
    };

    template < unsigned m >
    struct set_precise_binary_< op::div, mln::value::qt::rgb32, mln::value::int_u<m> >
    {
      typedef mln::value::qt::rgb32 ret;
    };


    template <>
    struct value_< mln::value::qt::rgb32 >
    {
      enum {
	dim = 3,
	nbits = 32,
	card  = mln_value_card_from_(nbits)
      };

      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::color       kind;
      typedef trait::value::quant::high /*mln_value_quant_from_(card)*/     quant;

      typedef void comp;
      typedef mln::value::int_u<8> comp_0;
      typedef mln::value::int_u<8> comp_1;
      typedef mln::value::int_u<8> comp_2;

      template <typename V> static comp_0 get_comp_0(const V& v) { return v.red(); }
      template <typename V> static comp_1 get_comp_1(const V& v) { return v.green(); }
      template <typename V> static comp_2 get_comp_2(const V& v) { return v.blue(); }

      typedef algebra::vec<dim, float> sum;

      static const char* name()
      {
	static std::string s = "qt_rgb32";
	return s.c_str();
      }

    };

  } // end of namespace trait



  namespace value
  {

    namespace qt
    {

      /// Color class for red-green-blue where every component is
      /// n-bit encoded.

      struct rgb32
	:
	public Vectorial< rgb32 >,
	public internal::value_like_< algebra::vec< 3, int_u<8> >, // Equivalent.
				      algebra::vec< 3, int_u<8> >, // Encoding.
				      algebra::vec< 3, int >,      // Interoperation.
				      rgb32 >			 // Exact.
      {
      public:

	typedef int_u<8> red_t;
	typedef int_u<8> green_t;
	typedef int_u<8> blue_t;

	/// \{ Acces to red/green/blue component.
	int_u<8>  red() const   { return this->v_[2]; }
	int_u<8>& red()         { return this->v_[2]; }

	int_u<8>  green() const { return this->v_[1]; }
	int_u<8>& green()       { return this->v_[1]; }

	int_u<8>  blue() const  { return this->v_[0]; }
	int_u<8>& blue()        { return this->v_[0]; }

	int_u<8>  comp(unsigned k) const { return this->v_[k]; }
	int_u<8>& comp(unsigned k)       { return this->v_[k]; }
	/// \}

	/// Constructor without argument.
	rgb32();

	/// Constructor from component values.
	rgb32(int r, int g, int b);

	/// Constructor from a algebra::vec.
	rgb32(const algebra::vec<3, int>& rhs);
	rgb32(const algebra::vec<3, unsigned>& rhs);
	rgb32(const algebra::vec<3, int_u<8> >& rhs);
	rgb32(const algebra::vec<3, float>& rhs);

	// Conversion to the interoperation type.
	operator algebra::vec<3, int>() const;
	// Conversion to the sum type.
	operator algebra::vec<3, float>() const;

	/// \{ Constructors with literals.
	rgb32(const mln::literal::zero_t&);

	rgb32(const mln::literal::white_t&);
	rgb32(const mln::literal::black_t&);

	rgb32(const mln::literal::light_gray_t&);
	rgb32(const mln::literal::medium_gray_t&);
	rgb32(const mln::literal::dark_gray_t&);

	rgb32(const mln::literal::red_t&);
	rgb32(const mln::literal::blue_t&);
	rgb32(const mln::literal::green_t&);
	rgb32(const mln::literal::brown_t&);
	rgb32(const mln::literal::lime_t&);
	rgb32(const mln::literal::orange_t&);
	rgb32(const mln::literal::pink_t&);
	rgb32(const mln::literal::purple_t&);
	rgb32(const mln::literal::teal_t&);
	rgb32(const mln::literal::violet_t&);
	rgb32(const mln::literal::cyan_t&);
	rgb32(const mln::literal::magenta_t&);
	rgb32(const mln::literal::yellow_t&);
	rgb32(const mln::literal::olive_t&);
	/// \}

	/// Assignment.
	rgb32& operator=(const rgb32& rhs);

	/// Zero value.
	static const rgb32 zero;


	/// Alpha value (unused).
	int_u<8> alpha_;
      };



      /// Print an rgb32 \p c into the output stream \p ostr.
      ///
      /// \param[in,out] ostr An output stream.
      /// \param[in] c An rgb32.
      ///
      /// \return The modified output stream \p ostr.

      std::ostream& operator<<(std::ostream& ostr, const rgb32& c);


      std::istream& operator>>(std::istream& istr, rgb32& c);


      /* FIXME: We should not need to define these operators, thanks to
	 Milena's global operator resolution mechanism based on
	 mln::Object.  See what prevent us to use this mechanism.  */

      /* FIXME: Cannot work for i negative; add traits! (2008-02-16,
	 Roland: What does this comment mean?)  */

      /// Addition.
      /// {

      rgb32::interop
      operator+(const rgb32& lhs, const rgb32& rhs);


      rgb32::interop
      operator+(const rgb32::interop& lhs, const rgb32& rhs);


      rgb32::interop
      operator+(const rgb32& lhs, const rgb32::interop& rhs);
      /// \}

      /// Subtraction.
      /// \{

      rgb32::interop
      operator-(const rgb32& lhs, const rgb32& rhs);


      rgb32::interop
      operator-(const rgb32::interop& lhs, const rgb32& rhs);


      rgb32::interop
      operator-(const rgb32& lhs, const rgb32::interop& rhs);
      /// \}

      /// Product.
      /// \{
      template <typename S>
      inline
      rgb32::interop
      operator*(const rgb32& lhs, const mln::value::scalar_<S>& s);

      template <typename S>
      inline
      rgb32::interop
      operator*(const mln::value::scalar_<S>& s, const rgb32& lhs);
      /// \}

      /// Division.
      /// \{
      template <typename S>
      inline
      rgb32::interop
      operator/(const rgb32& lhs, const mln::value::scalar_<S>& s);
      /// \}


      /// \internal Conversion: Conversion: #value::qt::rgb32 -> bool.
      void from_to_(const rgb32& from, bool& to);

      /// \internal Conversion: bool -> #value::qt::rgb32.
      void from_to_(const bool& from, value::qt::rgb32& to);

    } // end of namespace mln::value::qt

  } // end of namespace mln::value

} // end of namespace mln

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace value
  {

    namespace qt
    {

#  ifndef MLN_WO_GLOBAL_VARS

      const rgb32 rgb32::zero(0,0,0);

#  endif // !MLN_WO_GLOBAL_VARS


      /*---------------.
      | Construction.  |
      `---------------*/


      inline
      rgb32::rgb32()
      {
      }


      inline
      rgb32::rgb32(const algebra::vec<3, int>& v)
      {
	this->v_[0] = v[2];
	this->v_[1] = v[1];
	this->v_[2] = v[0];
      }


      inline
      rgb32::rgb32(const algebra::vec<3, unsigned>& v)
      {
	this->v_[0] = v[2];
	this->v_[1] = v[1];
	this->v_[2] = v[0];
      }


      inline
      rgb32::rgb32(const algebra::vec<3, int_u<8> >& v)
      {
	this->v_[0] = v[2];
	this->v_[1] = v[1];
	this->v_[2] = v[0];
      }


      inline
      rgb32::rgb32(const algebra::vec<3, float>& v)
      {
	this->v_[0] = unsigned(v[2]);
	this->v_[1] = unsigned(v[1]);
	this->v_[2] = unsigned(v[0]);
      }


      inline
      rgb32::rgb32(int r, int g, int b)
      {
	mln_precondition(r >= 0);
	mln_precondition(g >= 0);
	mln_precondition(b >= 0);
	mln_precondition(unsigned(r) <= mln_max(int_u<8>));
	mln_precondition(unsigned(g) <= mln_max(int_u<8>));
	mln_precondition(unsigned(b) <= mln_max(int_u<8>));
	this->v_[0] = b;
	this->v_[1] = g;
	this->v_[2] = r;
      }


      inline
      rgb32::rgb32(const mln::literal::zero_t&)
      {
	this->v_[0] = 0;
	this->v_[1] = 0;
	this->v_[2] = 0;
      }


      inline
      rgb32::rgb32(const mln::literal::white_t&)
      {
	this->v_[0] = mln_max(int_u<8>);
	this->v_[1] = mln_max(int_u<8>);
	this->v_[2] = mln_max(int_u<8>);
      }


      inline
      rgb32::rgb32(const mln::literal::black_t&)
      {
	this->v_[0] = 0;
	this->v_[1] = 0;
	this->v_[2] = 0;
      }


      inline
      rgb32::rgb32(const mln::literal::light_gray_t&)
      {
	this->v_[0] = unsigned(mln_max(int_u<8>) * 0.75);
	this->v_[1] = unsigned(mln_max(int_u<8>) * 0.75);
	this->v_[2] = unsigned(mln_max(int_u<8>) * 0.75);
      }


      inline
      rgb32::rgb32(const mln::literal::medium_gray_t&)
      {
	this->v_[0] = unsigned(mln_max(int_u<8>) * 0.50);
	this->v_[1] = unsigned(mln_max(int_u<8>) * 0.50);
	this->v_[2] = unsigned(mln_max(int_u<8>) * 0.50);
      }


      inline
      rgb32::rgb32(const mln::literal::dark_gray_t&)
      {
	this->v_[0] = unsigned(mln_max(int_u<8>) * 0.25);
	this->v_[1] = unsigned(mln_max(int_u<8>) * 0.25);
	this->v_[2] = unsigned(mln_max(int_u<8>) * 0.25);
      }


      inline
      rgb32::rgb32(const mln::literal::red_t&)
      {
	this->v_[0] = 0;
	this->v_[1] = 0;
	this->v_[2] = mln_max(int_u<8>);
      }


      inline
      rgb32::rgb32(const mln::literal::green_t&)
      {
	this->v_[0] = 0;
	this->v_[1] = mln_max(int_u<8>);
	this->v_[2] = 0;
      }


      inline
      rgb32::rgb32(const mln::literal::blue_t&)
      {
	this->v_[0] = mln_max(int_u<8>);
	this->v_[1] = 0;
	this->v_[2] = 0;
      }


      inline
      rgb32::rgb32(const mln::literal::brown_t&)
      {
	this->v_[0] = unsigned(mln_max(int_u<8>) * 0.25);
	this->v_[1] = unsigned(mln_max(int_u<8>) * 0.50);
	this->v_[2] = unsigned(mln_max(int_u<8>) * 0.75);
      }


      inline
      rgb32::rgb32(const mln::literal::lime_t&)
      {
	this->v_[0] = 0;
	this->v_[1] = mln_max(int_u<8>);
	this->v_[2] = unsigned(mln_max(int_u<8>) * 0.75);
      }


      inline
      rgb32::rgb32(const mln::literal::orange_t&)
      {
	this->v_[0] = 0;
	this->v_[1] = unsigned(mln_max(int_u<8>) * 0.50);
	this->v_[2] = mln_max(int_u<8>);
      }


      inline
      rgb32::rgb32(const mln::literal::pink_t&)
      {
	this->v_[0] = unsigned(mln_max(int_u<8>) * 0.75);
	this->v_[1] = unsigned(mln_max(int_u<8>) * 0.75);
	this->v_[2] = mln_max(int_u<8>);
      }


      inline
      rgb32::rgb32(const mln::literal::purple_t&)
      {
	this->v_[0] = unsigned(mln_max(int_u<8>) * 0.25);
	this->v_[1] = 0;
	this->v_[2] = unsigned(mln_max(int_u<8>) * 0.75);
      }


      inline
      rgb32::rgb32(const mln::literal::teal_t&)
      {
	this->v_[0] = unsigned(mln_max(int_u<8>) * 0.50);
	this->v_[1] = unsigned(mln_max(int_u<8>) * 0.50);
	this->v_[2] = 0;
      }


      inline
      rgb32::rgb32(const mln::literal::violet_t&)
      {
	this->v_[0] = unsigned(mln_max(int_u<8>) * 0.50);
	this->v_[1] = 0;
	this->v_[2] = unsigned(mln_max(int_u<8>) * 0.50);
      }


      inline
      rgb32::rgb32(const mln::literal::cyan_t&)
      {
	this->v_[0] = mln_max(int_u<8>);
	this->v_[1] = mln_max(int_u<8>);
	this->v_[2] = 0;
      }


      inline
      rgb32::rgb32(const mln::literal::magenta_t&)
      {
	this->v_[0] = mln_max(int_u<8>);
	this->v_[1] = 0;
	this->v_[2] = mln_max(int_u<8>);
      }


      inline
      rgb32::rgb32(const mln::literal::yellow_t&)
      {
	this->v_[0] = 0;
	this->v_[1] = mln_max(int_u<8>);
	this->v_[2] = mln_max(int_u<8>);
      }


      inline
      rgb32::rgb32(const mln::literal::olive_t&)
      {
	this->v_[0] = 0;
	this->v_[1] = unsigned(mln_max(int_u<8>) * 0.50);
	this->v_[2] = unsigned(mln_max(int_u<8>) * 0.50);
      }


      inline
      rgb32&
      rgb32::operator=(const rgb32& rhs)
      {
	if (& rhs == this)
	  return *this;
	this->v_ = rhs.v_;
	return *this;
      }


      inline
      rgb32::operator algebra::vec<3, int>() const
      {
	algebra::vec<3, int> out;
	out[0] = this->v_[2];
	out[1] = this->v_[1];
	out[2] = this->v_[0];
	return out;
      }

      inline
      rgb32::operator algebra::vec<3, float>() const
      {
	algebra::vec<3, float> out;
	out[0] = this->v_[2];
	out[1] = this->v_[1];
	out[2] = this->v_[0];
	return out;
      }


      /*------------.
      | Operators.  |
      `------------*/


      inline
      rgb32::interop
      operator+(const rgb32& lhs, const rgb32& rhs)
      {
	rgb32::interop tmp(lhs.to_interop() + rhs.to_interop());
	return tmp;
      }


      inline
      rgb32::interop
      operator+(const rgb32& lhs, const rgb32::interop& rhs)
      {
	rgb32::interop tmp(lhs.to_interop() + rhs);
	return tmp;
      }


      inline
      rgb32::interop
      operator+(const rgb32::interop& lhs, const rgb32& rhs)
      {
	rgb32::interop tmp(lhs + rhs.to_interop());
	return tmp;
      }


      inline
      rgb32::interop
      operator-(const rgb32& lhs, const rgb32& rhs)
      {
	rgb32::interop tmp(lhs.to_interop() - rhs.to_interop());
	return tmp;
      }


      inline
      rgb32::interop
      operator-(const rgb32& lhs, const rgb32::interop& rhs)
      {
	rgb32::interop tmp(lhs.to_interop() - rhs);
	return tmp;
      }


      inline
      rgb32::interop
      operator-(const rgb32::interop& lhs, const rgb32& rhs)
      {
	rgb32::interop tmp(lhs - rhs.to_interop());
	return tmp;
      }

      template <typename S>
      inline
      rgb32::interop
      operator*(const rgb32& lhs, const mln::value::scalar_<S>& s)
      {
	rgb32::interop tmp(lhs.to_interop() * s.to_equiv());
	return tmp;
      }

      template <typename S>
      inline
      rgb32::interop
      operator*(const mln::value::scalar_<S>& s, const rgb32& lhs)
      {
	rgb32::interop tmp(s.to_equiv() * lhs.to_interop());
	return tmp;
      }

      template <typename S>
      inline
      rgb32::interop
      operator/(const rgb32& lhs, const mln::value::scalar_<S>& s)
      {
	rgb32::interop tmp(lhs.to_interop() / s.to_equiv());
	return tmp;
      }


      inline
      std::ostream& operator<<(std::ostream& ostr, const rgb32& v)
      {
	return ostr << '(' << debug::format(v.red())
		    << ',' << debug::format(v.green())
		    << ',' << debug::format(v.blue())
		    << ')';
      }


      inline
      std::istream& operator>>(std::istream& istr, rgb32& c)
      {
	return istr >> c.red() >> c.green() >> c.blue();
      }


      // Conversions

      inline
      void from_to_(const rgb32& from, bool& to)
      {
	to = ((from == literal::black) ? false : true);
      }

      inline
      void from_to_(const bool& from, value::qt::rgb32& to)
      {
	if (from)
	  to = literal::white;
	else
	  to = literal::black;
      }

    } // end of namespace mln::value::qt

  } // end of namespace mln::value

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_VALUE_QT_RGB32_HH
