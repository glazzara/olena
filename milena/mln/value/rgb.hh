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

/*! \file mln/value/rgb8.hh
 *
 * \brief Color class for red-green-blue where every component is
 * 8-bit encoded.
 */

# include <mln/core/concept/value.hh>
# include <mln/value/int_u8.hh>


namespace mln
{

  namespace value
  {


    typedef int_u8 int_u8_x3_t[3];
    typedef unsigned char uchar_x3_t[3];
    typedef float float_x3_t[3];


    /*! \brief Color class for red-green-blue where every component is
     * 8-bit encoded.
     */
    template <unsigned n>
    struct rgb : public Value< rgb<n> >
    {
    public:

      /// Encoding associated type.
      typedef int_u<n> enc;

      /// Equivalent associated type.
      typedef int_u<n> equiv[3];

      int_u<n>  red() const   { return c_[0]; }
      int_u<n>& red()         { return c_[0]; }

      int_u<n>  green() const { return c_[1]; }
      int_u<n>& green()       { return c_[1]; }

      int_u<n>  blue() const  { return c_[2]; }
      int_u<n>& blue()        { return c_[2]; }

      rgb<n>();
      rgb<n>(equiv a);
      rgb<n>(enc r, enc g, enc b);

      rgb<n>& operator=(rgb<n>& v);

      /// Zero value.
      static const rgb<n> zero;

      /// Colors.
      static const rgb<n> black;
      static const rgb<n> max_red;
      static const rgb<n> max_green;
      static const rgb<n> max_blue;
      static const rgb<n> white;

      /// Self addition
      rgb<n>& operator+=(rgb<n>& v);

      /// Self subtraction.
      rgb<n>& operator-=(rgb<n>& v);

      /// Comparaison.
      bool operator==(rgb<n>& v);
      bool operator!=(rgb<n>& v);


    private:
      equiv c_;
    };

    template <unsigned n>
    struct props< rgb<n> >
    {
      static const unsigned nbits = 24;
      static const std::size_t card_ = metal::pow<2, nbits>::value;
      typedef color_kind kind;
      typedef float_x3_t sum;
      typedef uchar_x3_t interop;
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


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n>
    rgb<n>::rgb()
    {
    }

    template <unsigned n>
    rgb<n>::rgb(equiv a)
    {
      std::memcpy(this->c_, a, 3);
    }

    template <unsigned n>
    rgb<n>::rgb(enc r, enc g, enc b)
    {
      this->c_[0] = r;
      this->c_[1] = g;
      this->c_[2] = b;
    }

    template <unsigned n>
    rgb<n>&
    rgb<n>::operator=(rgb<n>& v)
    {
      std::memcpy(this->c_, v.c_, 3);
      return *this;
    }

    template <unsigned n>
    const rgb<n> rgb<n>::zero(0,0,0);

    template <unsigned n>
    const rgb<n> rgb<n>::black(0,0,0);

    template <unsigned n>
    const rgb<n> rgb<n>::max_red(mln_max(enc), 0, 0);

    template <unsigned n>
    const rgb<n> rgb<n>::max_green(0, mln_max(enc), 0);

    template <unsigned n>
    const rgb<n> rgb<n>::max_blue(0, 0, mln_max(enc));

    template <unsigned n>
    const rgb<n> rgb<n>::white(mln_max(enc), mln_max(enc), mln_max(enc));

    template <unsigned n>
    rgb<n>&
    rgb<n>::operator+=(rgb<n>& v)
    {
      for (int i = 0; i < 3; i++)
	this->c_[i] += v.c_[i];
    }

    template <unsigned n>
    rgb<n>&
    rgb<n>::operator-=(rgb<n>& v)
    {
      for (int i = 0; i < 3; i++)
	this->c_[i] += v.c_[i];
    }

    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const rgb<n>& v)
    {
      return ostr << "(R:" << debug::format(v.red())
		  << ", G:" << debug::format(v.green())
		  << ", B:" << debug::format(v.blue())
		  << ")";
    }

    template <unsigned n>
    bool
    rgb<n>::operator==(rgb<n>& v)
    {
      return (this->green() == v.green() &&
	      this->red() == v.red() &&
	      this->blue() == v.blue());
    }

    template <unsigned n>
    bool
    rgb<n>::operator!=(rgb<n>& v)
    {
      return (!(*this == v));
    }
# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_RGB_HH
