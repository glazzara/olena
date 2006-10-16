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

#ifndef XTD_TRAITS_HH
# define XTD_TRAITS_HH

# include <xtd/internal/mlc.hh>


namespace xtd
{

  namespace ERROR
  {
    struct FIXME;

  } // end of namespace xtd::ERROR



  // This class aims at being specialized by the client.
  template < typename T >
  struct set_traits_ : public mlc::undefined
  {
    // Unary operators.
    typedef mlc::undefined op_lnot_ret;
    typedef mlc::undefined op_uminus_ret;

    // Values.
    typedef mlc::undefined min_ret;
    typedef mlc::undefined max_ret;
    typedef mlc::undefined zero_ret;
    typedef mlc::undefined one_ret;

    // Quantization.
    typedef mlc::undefined nbits;

    // Features.
    typedef mlc::undefined is_scalar;
    typedef mlc::undefined is_signed;

    // FIXME: ...
  };



  namespace internal
  {

    // This class should be specialized for each 'subject' so
    // that set_trait_ gets related to set_traits_.
    template < typename subject,
	       typename T >
    struct from_set_traits_ : public mlc::undefined
    {
      // FIXME: Add a static check to make sure that this class has
      // been specialized.
    };

  } // end of namespace xtd::internal



  // This class aims at being specialized by the client.
  template < typename subject,
	     typename T1,
	     typename T2 = mlc::none >
  struct set_trait_ : public mlc::undefined
  {
    // A definition, e.g.,
    // typedef int ret;
  };

  template < typename subject,
	     typename T >
  struct set_trait_< subject, T, mlc::none > : public internal::from_set_traits_<subject, T>
  {
    // A definition, e.g.,
    // typedef int ret;
  };



  namespace internal
  {

    template < typename subject,
	       typename T1,
	       typename T2 = mlc::none >
    struct get_trait_

      : private mlc::assert_< mlc::is_defined_< xtd::set_trait_<subject, T1, T2> >,
			      xtd::ERROR::FIXME >
    {
    };

  } // end of namespace xtd::internal


} // end of namespace xtd



#endif // ! XTD_TRAITS_HH
