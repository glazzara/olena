// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_VALUE_BIN_HH
# define OLN_CORE_VALUE_BIN_HH

# include <mlc/bexpr.hh>
# include <xtd/optraits.hh>
# include <xtd/valtraits.hh>
# include <oln/value/graylevel.hh>


namespace oln
{

  namespace value
  {

    typedef graylevel_<1> bin;


    // Binary ops.
    bin operator and(bin lhs, bin rhs);
    bin operator  or(bin lhs, bin rhs);
    bin operator xor(bin lhs, bin rhs);

    // Unary ops.
    bin operator not(bin val);


# ifndef OLN_INCLUDE_ONLY

    bin operator and(bin lhs, bin rhs)
    {
      return bin(bool(lhs) and bool(rhs));
    }

    bin operator  or(bin lhs, bin rhs)
    {
      return bin(bool(lhs) or bool(rhs));
    }

    bin operator xor(bin lhs, bin rhs)
    {
      return bin(bool(lhs) xor bool(rhs));
    }

    bin operator not(bin val)
    {
      return bin(not bool(val));
    }

# endif

  } // end of namespace oln::value

} // end of namespace oln



namespace xtd
{

  using oln::value::bin;


  // Logical traits.

  template <> struct set_trait_< op_land, bin,  bin  > { typedef bin ret; };
  template <> struct set_trait_< op_land, bool, bin  > { typedef bin ret; };
  template <> struct set_trait_< op_land, bin,  bool > { typedef bin ret; };

  template <> struct set_trait_< op_lor,  bin,  bin  > { typedef bin ret; };
  template <> struct set_trait_< op_lor,  bool, bin  > { typedef bin ret; };
  template <> struct set_trait_< op_lor,  bin,  bool > { typedef bin ret; };

  template <> struct set_trait_< op_lxor, bin,  bin  > { typedef bin ret; };
  template <> struct set_trait_< op_lxor, bool, bin  > { typedef bin ret; };
  template <> struct set_trait_< op_lxor, bin,  bool > { typedef bin ret; };

  template <> struct set_trait_< op_lnot, bin        > { typedef bin ret; };
  
} // end of namespace xtd


#endif // ! OLN_CORE_VALUE_BIN_HH
