// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
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

#ifndef MLN_VALUE_INT_U_SAT_HH
# define MLN_VALUE_INT_U_SAT_HH

/*! \file mln/value/int_u_sat.hh
 *
 * \brief Define a generic class for unsigned integers with saturation
 * behavior.
 */

# include <mln/metal/math/pow.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/concept/integer.hh>
# include <mln/value/internal/encoding.hh>
# include <mln/value/int_u.hh>
# include <mln/trait/value_.hh>
# include <mln/debug/format.hh>


namespace mln
{

  namespace value
  {
    // Fwd decls.
    template <unsigned n> struct int_u_sat;
  }

  namespace trait
  {

    template <unsigned n>
    struct value_< mln::value::int_u_sat<n> >
    {
      // FIXME: Overhaul these traits (see other value traits).
      static const std::size_t card = metal::math::pow_int<2, n>::value;
      static const mln::value::int_u_sat<n> min() { return 0; }
      static const mln::value::int_u_sat<n> max() { return card - 1; }
      static const unsigned nbits = n;

      typedef trait::value::nature::integer nature;
      typedef trait::value::kind::data kind;
      // FIXME: Is that right?
      typedef mln_value_quant_from_(card) quant;

      typedef float sum;
    };
    
  } // end of namespace mln::trait


  namespace value
  {


    /*! \brief Unsigned integer value class with saturation behavior.
     *
     * The parameter is \c n the number of encoding bits.
     */
    template <unsigned n>
    struct int_u_sat
      :
      public Integer< int_u_sat<n> >,

      public internal::value_like_< int_u<n>,          // Equivalent.
				    mln_enc(int_u<n>), // Encoding.
				    unsigned,          // Interoperation.
				    int_u_sat<n> >     // Exact.
    {
      /// Constructor without argument.
      int_u_sat();

      /// Constructor from an integer.
      int_u_sat(int i);

      /// Conversion to an integer.
      operator int() const;

      /// Assignment from an integer.
      int_u_sat<n>& operator=(int i);

      /// Zero value.
      static const int_u_sat<n> zero;

      /// Unit value.
      static const int_u_sat<n> one;

      /// Self addition.
      int_u_sat<n>& operator+=(int i);

      /// Self subtraction.
      int_u_sat<n>& operator-=(int i);
    };


    // Safety.
    // FIXME: We shouldn't have to do that.
    template <> struct int_u_sat<0>;
    template <> struct int_u_sat<1>;

    /*! \brief Print a saturated unsigned integer \p i into the output
     *  stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] i A saturated unsigned integer.
     *
     * \return The modified output stream \p ostr.
     */
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const int_u_sat<n>& i);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n>
    inline
    int_u_sat<n>::int_u_sat()
    {
    }

    template <unsigned n>
    inline
    int_u_sat<n>::int_u_sat(int i)
    {
      static const unsigned max_ = mln_max(int_u<n>);
      if (i < 0)
	this->v_ = 0;
      // Explicitly cast I to unsigned to avoid a warning between
      // signed and unsigned values from the compiler.
      else if (static_cast<unsigned>(i) > max_)
	this->v_ = max_;
      else
	this->v_ = i;
    }

    template <unsigned n>
    inline
    int_u_sat<n>::operator int() const
    {
      return this->v_;
    }

    template <unsigned n>
    inline
    int_u_sat<n>&
    int_u_sat<n>::operator=(int i)
    {
      static const unsigned max_ = mln_max(int_u<n>);
      if (i < 0)
	this->v_ = 0;
      // Explicitly cast I to unsigned to avoid a warning between
      // signed and unsigned values from the compiler.
      else if (static_cast<unsigned>(i) > max_)
	this->v_ = max_;
      else
	this->v_ = i;
      return *this;
    }

    template <unsigned n>
    inline
    int_u_sat<n>&
    int_u_sat<n>::operator+=(int i)
    {
      int v = int(this->v_) + i;
      *this = v;
      return *this;
    }

    template <unsigned n>
    inline
    int_u_sat<n>&
    int_u_sat<n>::operator-=(int i)
    {
      int v = int(this->v_) - i;
      *this = v;
      return *this;
    }

    template <unsigned n>
    const int_u_sat<n> int_u_sat<n>::zero = 0;

    template <unsigned n>
    const int_u_sat<n> int_u_sat<n>::one = 1;

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const int_u_sat<n>& i)
    {
      return ostr << debug::format(i.to_equiv());
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INT_U_SAT_HH
