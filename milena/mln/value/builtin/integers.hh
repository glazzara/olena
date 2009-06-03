// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_VALUE_BUILTIN_INTEGERS_HH
# define MLN_VALUE_BUILTIN_INTEGERS_HH

/// \file mln/value/builtin/integers.hh
///
/// Some definitions about built-in integer types.

# include <mln/value/internal/limits.hh>

# include <mln/value/concept/built_in.hh>
# include <mln/value/concept/integer.hh>
# include <mln/trait/value_.hh>

# include <mln/metal/int.hh>
# include <mln/metal/if.hh>
# include <mln/metal/bool.hh>


namespace mln
{

  template <> struct category< unsigned char  > { typedef value::Built_In< value::Integer<void> > ret; };
  template <> struct category<   signed char  > { typedef value::Built_In< value::Integer<void> > ret; };
  template <> struct category< unsigned short > { typedef value::Built_In< value::Integer<void> > ret; };
  template <> struct category<   signed short > { typedef value::Built_In< value::Integer<void> > ret; };
  template <> struct category< unsigned int   > { typedef value::Built_In< value::Integer<void> > ret; };
  template <> struct category<   signed int   > { typedef value::Built_In< value::Integer<void> > ret; };
  template <> struct category< unsigned long  > { typedef value::Built_In< value::Integer<void> > ret; };
  template <> struct category<   signed long  > { typedef value::Built_In< value::Integer<void> > ret; };


  namespace trait
  {

    // FIXME: Move the definitions below elsewhere.

    namespace internal
    {

      template <typename E>
      struct value_integer_
      {
      private:
	enum { n = 8 * sizeof(E) };
      public:

	enum {
	  dim = 1,
	  nbits = n,
	  card  = mln_value_card_from_(n)
	};

	typedef trait::value::nature::integer nature;
	typedef trait::value::kind::data      kind;
	typedef mln_value_quant_from_(card) quant;

	static const E min() { return mln::value::internal::limits<E>::min(); }
	static const E max() { return mln::value::internal::limits<E>::max(); }

	typedef float sum;
      };

    } // end of namespace mln::trait::internal


    template <> struct value_< unsigned char  >
      : internal::value_integer_< unsigned char  >
    {
      static const char* name()
      { return "unsigned char"; }
    };

    template <> struct value_<   signed char  >
      : internal::value_integer_<   signed char  >
    {
      static const char* name()
      { return "signed char"; }
    };

    template <> struct value_<   char  >
      : internal::value_integer_<  signed char  >
    {
      static const char* name()
      { return "char"; }
    };

    template <> struct value_< unsigned short >
      : internal::value_integer_< unsigned short >
    {
      static const char* name()
      { return "unsigned short"; }
    };

    template <> struct value_<   signed short >
      : internal::value_integer_<   signed short >
    {
      static const char* name()
      { return "signed short"; }
    };

    template <> struct value_< unsigned int   >
      : internal::value_integer_< unsigned int   >
    {
      static const char* name()
      { return "unsigned int"; }
    };

    template <> struct value_<   signed int   >
      : internal::value_integer_<   signed int   >
    {
      static const char* name()
      { return "signed int"; }
    };

    template <> struct value_< unsigned long  >
      : internal::value_integer_< unsigned long  >
    {
      static const char* name()
      { return "unsigned long"; }
    };

    template <> struct value_<   signed long  >
      : internal::value_integer_<   signed long  >
    {
      static const char* name()
      { return "signed long"; }
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_INTEGERS_HH
