// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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

#ifndef XTD_BUILTIN_TRAITS_HH
# define XTD_BUILTIN_TRAITS_HH

# include <xtd/traits.hh>



# define xtd_intprom(Type) typename xtd::internal::intprom_<Type>::ret



namespace xtd
{

  // built-in traits and usual arithmetic conversions

  namespace internal
  {

    // see page 90 of ISO-IEC_14882.2003.pdf


    struct UAC; // for "usual arithmetic conversions"

    // Integral promotion such as defined in FIXME: std.

    template <class T>
    struct intprom_
    {
      typedef T ret;
    };

    // FIXME: approx?

    template <> struct intprom_ <char> { typedef int ret; };
    template <> struct intprom_ <signed char> { typedef int ret; };
    template <> struct intprom_ <unsigned char> { typedef int ret; };
    template <> struct intprom_ <unsigned short int> { typedef int ret; };

    template <> struct intprom_ <bool> { typedef int ret; }; // FIXME: right?

  } // end of namespace mlc::internal


  // 1) Prevent bool to be involved in arithmetics.
  template <typename L, typename R>
  struct case_ < internal::UAC, mlc::pair_<L, R>,
		 1 >
    : public mlc::where_< mlc::or_< mlc::eq_<L, bool>,
				    mlc::eq_<R, bool> > >
  {
    // FIXME: check that both L and R are bool and that it is not an arithmetical trait
    typedef bool ret;
  };

  // 2) If either operand is of type long double, the other shall be
  // 2) converted to long double.
  template <typename L, typename R>
  struct case_ < internal::UAC, mlc::pair_<L, R>,
		 2 >
    : public mlc::where_< mlc::or_< mlc::eq_<L, long double>,
				    mlc::eq_<R, long double> > >
  {
    typedef long double ret;
  };

  // 3) Otherwise, if either operand is double, the other shall be
  // 3) converted to double.
  template <typename L, typename R>
  struct case_ < internal::UAC, mlc::pair_<L, R>,
		 3 >
    : public mlc::where_< mlc::or_< mlc::eq_<L, double>,
				    mlc::eq_<R, double> > >
  {
    typedef double ret;
  };

  // 4) Otherwise, if either operand is float, the other shall be
  // 4) converted to float.
  template <typename L, typename R>
  struct case_ < internal::UAC, mlc::pair_<L, R>,
		 4 >
    : public mlc::where_< mlc::or_< mlc::eq_<L, float>,
				    mlc::eq_<R, float> > >
  {
    typedef float ret;
  };
  
  // 5) If either operand is unsigned long the other shall be
  // 5) converted to unsigned long.
  template <typename L, typename R>
  struct case_ < internal::UAC, mlc::pair_<L, R>,
		 5 >
    : public mlc::where_< mlc::or_< mlc::eq_<L, unsigned long>,
				    mlc::eq_<R, unsigned long> > >
  {
    typedef unsigned long ret;
  };
  
  // 6) Otherwise, if one operand is a long int and the other unsigned
  // 6) int, then if a long int can represent all the values of an
  // 6) unsigned int, the unsigned int shall be converted to a long
  // 6) int; otherwise both operands shall be converted to unsigned
  // 6) long int.
  template <typename L, typename R>
  struct case_ < internal::UAC, mlc::pair_<L, R>,
		 6 >
    : public mlc::where_< mlc::or_< mlc::and_< mlc::eq_<xtd_intprom(L), long int>,
					       mlc::eq_<xtd_intprom(R), unsigned int> >,
				    mlc::and_< mlc::eq_<xtd_intprom(R), long int>,
					       mlc::eq_<xtd_intprom(L), unsigned int> > > >
  {
    typedef long int ret; // FIXME: approx...
  };

  // 7) Otherwise, if either operand is long, the other shall be
  // 7) converted to long.
  template <typename L, typename R>
  struct case_ < internal::UAC, mlc::pair_<L, R>,
		 7 >
    : public mlc::where_< mlc::or_< mlc::eq_<xtd_intprom(L), long>,
				    mlc::eq_<xtd_intprom(R), long> > >
  {
    typedef long ret;
  };

  // 8) Otherwise, if either operand is unsigned, the other shall be
  // 8) converted to unsigned.
  template <typename L, typename R>
  struct case_ < internal::UAC, mlc::pair_<L, R>,
		 8 >
    : public mlc::where_< mlc::or_< mlc::eq_<xtd_intprom(L), unsigned>,
				    mlc::eq_<xtd_intprom(R), unsigned> > >
  {
    typedef unsigned ret;
  };


  // 0) Note: otherwise, the only remaining case is that both operands
  // 0) are int.
  template <typename L, typename R>
  struct default_case_ < internal::UAC, mlc::pair_<L, R> >
  {
    // FIXME: test that:
    // mlc::or_<mlc::eq_<xtd_intprom(L), int>, mlc::eq_<xtd_intprom(R), int> >
    typedef int ret;
  };
  

} // end of namespace xtd



#endif // ! XTD_BUILTIN_TRAITS_HH
