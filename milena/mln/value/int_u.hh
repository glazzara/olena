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

#ifndef MLN_VALUE_INT_U_HH
# define MLN_VALUE_INT_U_HH

/// \file
///
/// Define a generic class for unsigned integers.

# include <mln/value/ops.hh>

# include <mln/metal/math/pow.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/internal/encoding.hh>
# include <mln/value/concept/integer.hh>
# include <mln/trait/value_.hh>
# include <mln/debug/format.hh>

# include <mln/value/internal/make_generic_name.hh>


namespace mln
{

  // Forward declarations.
  namespace value {
    namespace qt {
      struct rgb32;
    }
    template <unsigned n> struct int_u;
    template <unsigned n> struct rgb;
  }

  namespace literal
  {
    struct zero_t;
    struct one_t;
  }
  // End of forward declarations

  namespace trait
  {

    template <unsigned n>
    struct set_precise_unary_< op::uminus, mln::value::int_u<n> >
    {
      typedef int ret;
    };


    template <unsigned n>
    struct value_< mln::value::int_u<n> >
    {
    private:
      typedef mln::value::int_u<n> self_;
      typedef typename mln::value::internal::encoding_unsigned_<n>::ret enc_;
    public:

      enum constants_ {
	dim = 1,
	nbits = n,
	card  = mln_value_card_from_(n)
      };

      typedef trait::value::nature::integer nature;
      typedef trait::value::kind::data      kind;
      typedef mln_value_quant_from_(card)   quant;

      static const self_ min() { return 0; }
      static const self_ max() { return mlc_pow_int(2, n) - 1; }
      static const self_ epsilon() { return 0; }

      typedef unsigned comp;

      typedef float sum;

      static const char* name()
      {
	static std::string
	  s = mln::value::internal::make_generic_name("int_u", n);
	return s.c_str();
      }

    };

  } // end of namespace mln::trait


  namespace value
  {

    /*!
      \brief Unsigned integer value class.

      The parameter is \c n the number of encoding bits.

      \ingroup valueuint
    */
    template <unsigned n>
    struct int_u
      :
      public Integer< int_u<n> >,

      public internal::value_like_< unsigned,    // Equivalent.
				    typename internal::encoding_unsigned_<n>::ret, // Enc.
				    int,         // Interoperation.
				    int_u<n> >   // Exact.
    {
    protected:
      /// Encoding associated type.
      typedef typename internal::encoding_unsigned_<n>::ret enc_;

    public:

      /// Constructor without argument.
      int_u();

      /// Constructor from an integer.
      int_u(int i);

      /// \{ Constructors/assignments with literals.
      int_u(const mln::literal::zero_t&);
      int_u& operator=(const mln::literal::zero_t&);
      int_u(const mln::literal::one_t&);
      int_u& operator=(const mln::literal::one_t&);
      /// \}

      /// Conversion to an unsigned integer.
      operator unsigned() const;

      /// Unary operator minus.
      int operator-() const;

      /// Assignment from an integer.
      int_u<n>& operator=(int i);

      /// Give the next value (i.e., i + 1).
      int_u<n> next() const;
    };


    // Safety.
    template <> struct int_u<0>;
    template <> struct int_u<1>;



    /// \brief Print an unsigned integer \p i into the output stream \p ostr.
    ///
    /// \param[in,out] ostr An output stream.
    /// \param[in] i An unsigned integer.
    ///
    /// \return The modified output stream \p ostr.
    ///
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const int_u<n>& i);


    // FIXME: Doc!
    template <unsigned n>
    std::istream& operator>>(std::istream& istr, int_u<n>& i);



    // Conversions

    /// \internal Conversion: int_u -> unsigned.
    template <unsigned n>
    void from_to_(const int_u<n>& from, unsigned& to_);


    /// \internal Conversion: int_u -> bool.
    template <unsigned n>
    void from_to_(const int_u<n>& from, bool& to_);


    /// \internal Conversion: int_u -> float.
    template <unsigned n>
    void from_to_(const int_u<n>& from, float& to_);


    /// \internal Conversion: int_u -> double.
    template <unsigned n>
    void
    from_to_(const int_u<n>& from, double& to_);

    /// \internal Conversion: Conversion: int_u -> rgb.
    template <unsigned m>
    void from_to_(const int_u<m>& from, qt::rgb32& to);

    /// \internal Conversion: int_u -> rgb.
    template <unsigned m>
    void from_to_(const int_u<m>& from, rgb<m>& to);

  } // end of namespace mln::value

# ifndef MLN_INCLUDE_ONLY

  namespace value
  {

    template <unsigned n>
    inline
    int_u<n>::int_u()
    {
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(int i)
    {
      mln_precondition(i >= 0);
      mln_precondition(unsigned(i) <= mln_max(enc_));
      this->v_ = static_cast<enc_>(i);
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(const mln::literal::zero_t&)
    {
      this->v_ = 0;
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(const mln::literal::zero_t&)
    {
      this->v_ = 0;
      return *this;
    }

    template <unsigned n>
    inline
    int_u<n>::int_u(const mln::literal::one_t&)
    {
      this->v_ = 1;
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(const mln::literal::one_t&)
    {
      this->v_ = 1;
      return *this;
    }

    template <unsigned n>
    inline
    int_u<n>::operator unsigned() const
    {
      return this->v_;
    }

    template <unsigned n>
    inline
    int
    int_u<n>::operator-() const
    {
      return - int(this->v_);
    }

    template <unsigned n>
    inline
    int_u<n>&
    int_u<n>::operator=(int i)
    {
      mln_precondition(i >= 0);
      mln_precondition(unsigned(i) <= mln_max(enc_));
      this->v_ = static_cast<enc_>(i);
      return *this;
    }

    template <unsigned n>
    inline
    int_u<n>
    int_u<n>::next() const
    {
      return this->v_ + 1;
    }

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const int_u<n>& i)
    {
      // FIXME: This code could be factored for almost every Value<*>...
      return ostr << debug::format(i.to_equiv()); // FIXME: is to_equiv OK?
    }

    template <unsigned n>
    inline
    std::istream& operator>>(std::istream& istr, int_u<n>& i)
    {
      return istr >> i.handle_();
    }


    // Conversions

    template <unsigned n>
    inline
    void
    from_to_(const int_u<n>& from, unsigned& to_)
    {
      to_ = from;
    }

    template <unsigned n>
    inline
    void
    from_to_(const int_u<n>& from, bool& to_)
    {
      to_ = (from != 0u);
    }

    template <unsigned n>
    inline
    void
    from_to_(const int_u<n>& from, float& to_)
    {
      to_ = static_cast<float>(from);
    }

    template <unsigned n>
    inline
    void
    from_to_(const int_u<n>& from, double& to_)
    {
      to_ = static_cast<double>(from);
    }

    template <unsigned m>
    void
    from_to_(const int_u<m>& from, qt::rgb32& to)
    {
      mlc_bool(m <= 8)::check();
      to = qt::rgb32(from, from, from);
    }

    template <unsigned m>
    void
    from_to_(const int_u<m>& from, rgb<m>& to)
    {
      to = rgb<m>(from, from, from);
    }

  } // end of namespace mln::value

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_VALUE_INT_U_HH

