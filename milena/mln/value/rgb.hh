// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_RGB_HH
# define MLN_VALUE_RGB_HH

/*! \file mln/value/rgb.hh
 *
 * \brief Color class for red-green-blue where every component is
 * n-bit encoded.
 */

# include <mln/value/ops.hh>

# include <mln/value/concept/vectorial.hh>
# include <mln/value/int_u.hh>
# include <mln/metal/vec.hh>


namespace mln
{

  namespace literal
  {
    /// \{ Fwd decls.
    struct black_t;
    struct white_t;

    struct red_t;
    struct blue_t;
    struct green_t;
    /// \}
  }


  // Fwd decl.
  namespace value {
    template <unsigned n> struct rgb;
  }


  namespace trait
  {

    template <unsigned n>
    struct value_< mln::value::rgb<n> >
    {
      enum {
	nbits = 3 * n,
	card  = mln_value_card_from_(nbits)
      };

      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::color       kind;
      typedef mln_value_quant_from_(card)     quant;

      typedef metal::vec<3, float> sum;
    };

  } // end of namespace trait



  namespace value
  {

    /*! \brief Color class for red-green-blue where every component is
     * n-bit encoded.
     */
    template <unsigned n>
    struct rgb
      :
      public Vectorial< rgb<n> >
      ,
      public internal::value_like_< metal::vec< 3, int_u<n> >, // Equivalent.
				    metal::vec< 3, int_u<n> >, // Encoding.
				    metal::vec< 3, int >,      // Interoperation.
				    rgb<n> >                   // Exact.
    {
    public:

      /// \{ Acces to red/green/blue component.
      int_u<n>  red() const   { return this->v_[0]; }
      int_u<n>& red()         { return this->v_[0]; }

      int_u<n>  green() const { return this->v_[1]; }
      int_u<n>& green()       { return this->v_[1]; }

      int_u<n>  blue() const  { return this->v_[2]; }
      int_u<n>& blue()        { return this->v_[2]; }
      /// \}

      /// Constructor without argument.
      rgb<n>();

      /// Constructor from component values.
      rgb<n>(int r, int g, int b);

      /// Constructor from a metal::vec.
      rgb<n>(const metal::vec<3, int>& rhs);
      rgb<n>(const metal::vec<3, unsigned>& rhs);
      rgb<n>(const metal::vec<3, int_u<n> >& rhs);

      operator metal::vec<3, float>() const { return this->v_; }

      /// \{ Constructors with literals.
      rgb<n>(const literal::white_t&);
      rgb<n>(const literal::black_t&);
      rgb<n>(const literal::blue_t&);
      rgb<n>(const literal::red_t&);
      rgb<n>(const literal::green_t&);
      /// \}

      /// Assignment.
      rgb<n>& operator=(const rgb<n>& rhs);

      /// Zero value.
      static const rgb<n> zero;


      // FIXME: Cannot work for i negative; move operators outside the class; add traits!


      /// Addition.
      rgb<n> operator+(const rgb<n>& rhs) const;

      /// Substraction.
      rgb<n> operator-(const rgb<n>& rhs) const;

      /// Multiplication.
      rgb<n> operator*(int i) const;

      /// Division.
      rgb<n> operator/(int i) const;
    };



    /*! \brief Print an rgb \p c into the output stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] c An rgb.
     *
     * \return The modified output stream \p ostr.
     */
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const rgb<n>& c);

    template <unsigned n>
    std::istream& operator>>(std::istream& istr, rgb<n>& c);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n>
    inline
    rgb<n>::rgb()
    {
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const metal::vec<3, int>& v)
    {
      this->v_ = v;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const metal::vec<3, unsigned>& v)
    {
      this->v_ = v;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const metal::vec<3, int_u<n> >& v)
    {
      this->v_ = v;
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
    rgb<n>::rgb(const literal::white_t&)
    {
      this->v_[0] = mln_max(int_u<n>);
      this->v_[1] = mln_max(int_u<n>);
      this->v_[2] = mln_max(int_u<n>);
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const literal::black_t&)
    {
      this->v_[0] = 0;
      this->v_[1] = 0;
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const literal::red_t&)
    {
      this->v_[0] = mln_max(int_u<n>);
      this->v_[1] = 0;
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const literal::green_t&)
    {
      this->v_[0] = 0;
      this->v_[1] = mln_max(int_u<n>);
      this->v_[2] = 0;
    }

    template <unsigned n>
    inline
    rgb<n>::rgb(const literal::blue_t&)
    {
      this->v_[0] = 0;
      this->v_[1] = 0;
      this->v_[2] = mln_max(int_u<n>);
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

    template <unsigned n>
    inline
    rgb<n>
    rgb<n>::operator-(const rgb<n>& rhs) const
    {
      rgb<n> tmp(this->v_ - rhs.v_);
      return tmp;
    }

    template <unsigned n>
    inline
    rgb<n>
    rgb<n>::operator+(const rgb<n>& rhs) const
    {
      rgb<n> tmp(this->v_ + rhs.v_);
      return tmp;
    }

    template <unsigned n>
    inline
    rgb<n>
    rgb<n>::operator*(int i) const
    {
      rgb<n> tmp(this->v_ * i);
      return tmp;
    }

    template <unsigned n>
    inline
    rgb<n>
    rgb<n>::operator/(int i) const
    {
      rgb<n> tmp(this->v_ / i);
      return tmp;
    }

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const rgb<n>& v)
    {
      return ostr << "(" << debug::format(v.red())
		  << ", " << debug::format(v.green())
		  << ", " << debug::format(v.blue())
		  << ")";
    }

    template <unsigned n>
    inline
    std::istream& operator>>(std::istream& istr, rgb<n>& c)
    {
      return istr >> c.red() >> c.green() >> c.blue();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_RGB_HH
