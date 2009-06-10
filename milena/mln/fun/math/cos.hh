// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_MATH_COS_HH
# define MLN_FUN_MATH_COS_HH

# include <mln/fun/unary.hh>
# include <mln/value/builtin/floatings.hh>
# include <mln/value/builtin/integers.hh>
# include <mln/math/acos.hh>
# include <mln/math/cos.hh>

namespace mln
{

  // Cosinus, bijective
  namespace fun
  {
    struct cos : unary<cos> {};
  }

  namespace trait
  {

    namespace next
    {

      template <typename T>
      struct set_unary_<mln::fun::cos, mln::value::Floating, T>
      {
	typedef set_unary_ ret;
	typedef T result;
	typedef T argument;
	typedef T& lvalue;

	static result read(const argument& x)
	{
	  return math::cos(x);
	}

	static void write(lvalue l, const result& x)
	{
	  l = math::acos(x);
	}
      };

      template <typename T>
      struct set_unary_<mln::fun::cos, mln::value::Integer, T>
      {
	typedef set_unary_ ret;
	typedef double result;
	typedef T argument;

	static result read(const argument& x)
	{
	  return math::cos((result)x);
	}

      };

    } // end of namespace mln::trait::next

  } // end of namespace mln

} // end of namespace mln

#endif /* ! MLN_FUN_MATH_COS_HH */
