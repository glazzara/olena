//						       		-*- C++ -*-
// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

/// \file rgb.i
/// \brief A wrapper of mln::value::rgb.

%module rgb

%{
#include "mln/value/rgb.hh"
%}

// FIXME: Wrap mln::rgb by hand, for Milena macros disturb swig.
// Annotate the original source code instead?
namespace mln
{

  namespace value
  {

    /*! \brief Unsigned integer value class.
     *
     * The parameter is \c n the number of encoding bits.
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

      /// Acces to red/green/blue component.
      /// \{
      mln::value::int_u<n>  red() const;
      mln::value::int_u<n>& red();

      mln::value::int_u<n>  green() const;
      mln::value::int_u<n>& green();

      mln::value::int_u<n>  blue() const;
      mln::value::int_u<n>& blue();

      mln::value::int_u<n>  comp(unsigned k);
      mln::value::int_u<n>& comp(unsigned k);
      /// \}

      /// Constructor without argument.
      rgb<n>();

      /// Constructor from component values.
      rgb<n>(int r, int g, int b);

      /// Constructor from a algebra::vec.
      rgb<n>(const mln::algebra::vec<3, int>& rhs);
      rgb<n>(const mln::algebra::vec<3, unsigned>& rhs);
      rgb<n>(const mln::algebra::vec<3, mln::value::int_u<n> >& rhs);
      rgb<n>(const mln::algebra::vec<3, float>& rhs);

      // Conversion to the interoperation type.
      operator mln::algebra::vec<3, int>() const;
      // Conversion to the sum type.
      operator mln::algebra::vec<3, float>() const;

      /// Constructors with literals.
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


    /// Print an RGB \p c value on the output stream \p ostr.
    ///
    /// \param[in,out] ostr An output stream.
    /// \param[in] c An RGB value.
    ///
    /// \return The modified output stream \p ostr.
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const rgb<n>& i);

    /// Get an RGB value \p c from the input stream \p istr.
    ///
    /// \param[in,out] istr An input stream.
    /// \param[out] c An RGB value (destination).
    ///
    /// \return The modified input stream \p istr.
    template <unsigned n>
    std::istream& operator>>(std::istream& istr, rgb<n>& i);

  } // end of namespace mln::value

} // end of namespace mln


/*--------------------------------.
| Conversion helpers for Python.  |
`--------------------------------*/

#if SWIGPYTHON
%include python-utils.ixx
generate__str__(mln::value::rgb)
#endif // !SWIGPYTHON
