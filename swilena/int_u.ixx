//						       		-*- C++ -*-
// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file int_u.i
/// \brief A wrapper of mln::value::int_u.

%module int_u

%{
#include "mln/value/int_u.hh"
%}

// FIXME: Wrap mln::int_u by hand, for Milena macros disturb swig.
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
    };


    // Safety.
    template <> struct int_u<0>;
    template <> struct int_u<1>;



    /*! \brief Print an unsigned integer \p i into the output stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] i An unsigned integer.
     *
     * \return The modified output stream \p ostr.
     */
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const int_u<n>& i);


    // FIXME: Doc!
    template <unsigned n>
    std::istream& operator>>(std::istream& istr, int_u<n>& i);

  } // end of namespace mln::value

} // end of namespace mln

// Helper for the conversion to int.
%extend mln::value::int_u
{
  int __int__() const { return *$self; }
}

// Helper for the conversion to string.
%{
#include <cstring>
#include <string>
#include <sstream>
%}

%extend mln::value::int_u
{
  char* __str__() const
  {
    std::ostringstream s;
    s << *$self;
    // FIXME: This is admittedly ugly; can't we use std::string as
    // return type?  See Swig's manual.
    return strdup(s.str().c_str());
  }
}
