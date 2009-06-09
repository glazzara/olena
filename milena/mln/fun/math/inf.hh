// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

      template <typename T1, typename T2>
      struct set_binary_<mln::fun::inf, mln::Object, T1, mln::Object, T2>
      {
	typedef set_binary_ ret;
	typedef mln_trait_promote(T1, T2) result;
	typedef T1 argument1;
	typedef T2 argument2;

	static result read(const argument1& a, const argument1& b)
	{
	  return math::min(a, b);
	}
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_FUN_MATH_INF_HH
