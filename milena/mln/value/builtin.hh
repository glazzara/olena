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

#ifndef MLN_VALUE_BUILTIN_HH
# define MLN_VALUE_BUILTIN_HH

/*! \file mln/value/builtin.hh
 *
 * \brief Some definitions about builtins.
 */

# include <mln/core/category.hh>
# include <mln/metal/int.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Value;
  namespace value {
    template <typename E> struct Symbolic;
    template <typename E> struct Integer;
    template <typename E> struct Floating;
  }

  // The case of built-in types.

  // Fwd decl.
  template <typename E> struct Built_In;

  // Category flag type.
  template <>
  struct Built_In<void>
  {
    typedef void* super; // Every builtin belongs to a sub-category of Value but we do not know which one.
  };

  template <> struct category< bool >     {  typedef Built_In<void> ret;  typedef value::Symbolic<void> super;  };

  template <> struct category< int >      {  typedef Built_In<void> ret;  typedef value::Integer<void>  super;  };
  template <> struct category< unsigned > {  typedef Built_In<void> ret;  typedef value::Integer<void>  super;  };

  template <> struct category< float >    {  typedef Built_In<void> ret;  typedef value::Floating<void> super;  };
  template <> struct category< double >   {  typedef Built_In<void> ret;  typedef value::Floating<void> super;  };
  // FIXME: ...


  namespace trait
  {

    // Fwd decl.
    template <typename V> struct value_;


    // Bool.

    template <>
    struct value_< bool>
    {
      typedef metal::int_<1>          nbits;
      typedef value::nature::symbolic nature;
      typedef value::kind::binary     kind;
      typedef value::quant::low       quant;
      typedef metal::int_<2>          card;
    };


    // Integer.

    template <>
    struct value_< unsigned char > : value_integer_<8>
    {
    };

    template <>
    struct value_< signed char > : value_integer_<8>
    {
    };

    template <>
    struct value_< unsigned short > : value_integer_<16>
    {
    };

    template <>
    struct value_< signed short > : value_integer_<16>
    {
    };

    template <>
    struct value_< unsigned int > : value_integer_<8 * sizeof(unsigned int), 0>
    {
    };

    template <>
    struct value_< signed int > : value_integer_<8 * sizeof(signed int), 0>
    {
    };

    template <>
    struct value_< unsigned long int > : value_integer_<8 * sizeof(unsigned long), 0>
    {
    };

    template <>
    struct value_< signed long int > : value_integer_<8 * sizeof(signed long), 0>
    {
    };


    // Floating.

    template <>
    struct value_< float >
    {
      typedef metal::int_<8*sizeof(float)> nbits;
      typedef value::nature::floating      nature;
      typedef value::kind::data            kind;
      typedef metal::int_<0>               card;
      typedef value::quant::high           quant;
      typedef float                        sum;
    };

    template <>
    struct value_< double >
    {
      typedef metal::int_<8*sizeof(double)> nbits;
      typedef value::nature::floating       nature;
      typedef value::kind::data             kind;
      typedef metal::int_<0>                card;
      typedef value::quant::high            quant;
      typedef double                        sum;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_HH
