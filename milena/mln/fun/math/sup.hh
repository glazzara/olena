// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR F PARTICULAR PURPOSE.  See the GNU
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

#ifndef MLN_FUN_MATH_SUP_HH
# define MLN_FUN_MATH_SUP_HH

# include <mln/fun/binary.hh>
# include <mln/math/max.hh>

namespace mln
{

  // Cosinus, bijective
  namespace fun
  {
    struct sup : binary<sup> {};
  }

  namespace trait
  {

    namespace next
    {

      template <typename T>
      struct set_binary_<mln::fun::sup, mln::Object, T1, mln::Object, T2>
      {
	typedef set_binary_ ret;
	typedef mln_trait_promote(T1, T2) result;
	typedef T1 argument1;
	typedef T2 argument2;

	static result read(const argument1& a, const argument1& b)
	{
	  return math::max(a, b);
	}
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

} // end of namespace mln

#endif /* ! MLN_FUN_MATH_SUP_HH */

