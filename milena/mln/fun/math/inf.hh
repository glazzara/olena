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

#ifndef MLN_FUN_MATH_INF_HH
# define MLN_FUN_MATH_INF_HH

# include <mln/fun/binary.hh>
# include <mln/math/min.hh>

namespace mln
{

  // Cosinus, bijective
  namespace fun
  {
    struct inf : binary<inf> {};
  }

  namespace trait
  {

    namespace next
    {

      template <typename T>
      struct set_binary_<mln::fun::inf, mln::Object, T, mln::Object, T>
      {
	typedef set_binary_ ret;
	typedef T result;
	typedef T argument1;
	typedef T argument2;

	static result read(const argument1& a, const argument1& b)
	{
	  return math::min(a, b);
	}
      };

    }

  }

}

#endif /* ! MLN_FUN_MATH_INF_HH */

