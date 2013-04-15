// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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

#ifndef MLN_VALUE_INT_S_HH
# define MLN_VALUE_INT_S_HH

/// \file
///
/// Define a generic class for signed integers.

# include <mln/value/ops.hh>

# include <mln/metal/math/pow.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/concept/integer.hh>
# include <mln/value/internal/encoding.hh>
# include <mln/trait/value_.hh>
# include <mln/trait/all.hh>
# include <mln/debug/format.hh>


namespace mln
{


  namespace value
  {
    /// \{ Fwd decls.
    template <unsigned n> struct int_s;
    /// \}
  }

  namespace literal
  {
    /// \{ Fwd decls.
    struct zero_t;
    struct one_t;
    /// \}
  }



  namespace trait
  {

    template <unsigned n>
    struct value_< mln::value::int_s<n> >
    {
    private:
      typedef mln::value::int_s<n> self_;
    public:

      enum constants_ {
	dim = 1,
	nbits = n,
	card  = mln_value_card_from_(n) - 1
      };

      typedef trait::value::nature::integer nature;
      typedef trait::value::kind::data      kind;
      typedef mln_value_quant_from_(card)   quant;

      static const self_ max() { return mln_value_card_from_(n) / 2 - 1; }
      static const self_ min() { return - max(); }
      static const self_ epsilon() { return 0; }

      typedef mln::value::int_s<n> comp;

      typedef float sum;

      static const char* name()
      {
	static std::string s = std::string("int_s").append(1, n + '0');
	return s.c_str();
      }

    };

  } // end of namespace mln::trait



  namespace value
  {


    /*!
      \brief Signed integer value class.

      The parameter is \c n the number of encoding bits.

      \ingroup valueint
     */
    template <unsigned n>
    struct int_s
      :
      private metal::bool_<(n <= 32)>::check_t
      ,
      public Integer< int_s<n> >
      ,
      public internal::value_like_< int,       // Equivalent.
				    typename internal::encoding_signed_<n>::ret, // Enc.
				    int,       // Interoperation.
				    int_s<n> > // Exact.
    {
      /// Constructor without argument.
      int_s();

      /// Constructor from an integer.
      int_s(int i);

      /// \{ Constructors/assignments with literals.
      int_s(const mln::literal::zero_t&);
      int_s& operator=(const mln::literal::zero_t&);
      int_s(const mln::literal::one_t&);
      int_s& operator=(const mln::literal::one_t&);
      /// \}

      /// Conversion to an integer.
      operator int() const;

      /// Assignment from an integer.
      int_s<n>& operator=(int i);

      /// Zero value.
      static const int_s<n> zero;

      /// Unit value.
      static const int_s<n> one;

    private:
      typedef typename internal::encoding_signed_<n>::ret enc_;
    };



    // Safety.
    template <> struct int_s<0>;
    template <> struct int_s<1>;



    /*! \brief Print an signed integer \p i into the output stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] i An signed integer.
     *
     * \return The modified output stream \p ostr.
     */
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const int_s<n>& i);



# ifndef MLN_INCLUDE_ONLY

    template <unsigned n>
    inline
    int_s<n>::int_s()
    {
    }

    template <unsigned n>
    inline
    int_s<n>::operator int() const
    {
      return this->v_;
    }

    template <unsigned n>
    inline
    int_s<n>::int_s(int i)
    {
      static const int max = int(metal::math::pow_int<2, n-1>::value) - 1;
      static const int min = - max;
      mln_precondition(i >= min);
      mln_precondition(i <= max);
      (void) min;
      (void) max;

      this->v_ = static_cast<enc_>(i);
    }

    template <unsigned n>
    inline
    int_s<n>&
    int_s<n>::operator=(int i)
    {
      static const int max = int(metal::math::pow_int<2, n-1>::value) - 1;
      static const int min = - max;
      mln_precondition(i >= min);
      mln_precondition(i <= max);
      (void) min;
      (void) max;

      this->v_ = static_cast<enc_>(i);
      return *this;
    }

    template <unsigned n>
    inline
    int_s<n>::int_s(const mln::literal::zero_t&)
    {
      this->v_ = 0;
    }

    template <unsigned n>
    inline
    int_s<n>&
    int_s<n>::operator=(const mln::literal::zero_t&)
    {
      this->v_ = 0;
      return *this;
    }

    template <unsigned n>
    inline
    int_s<n>::int_s(const mln::literal::one_t&)
    {
      this->v_ = 1;
    }

    template <unsigned n>
    inline
    int_s<n>&
    int_s<n>::operator=(const mln::literal::one_t&)
    {
      this->v_ = 1;
      return *this;
    }

    template <unsigned n>
    const int_s<n> int_s<n>::zero = 0;

    template <unsigned n>
    const int_s<n> int_s<n>::one = 1;

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const int_s<n>& i)
    {
      return ostr << debug::format(i.to_equiv());
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INT_S_HH
