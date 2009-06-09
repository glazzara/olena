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

#ifndef MLN_FUN_COMPONENT_GREEN_HH
# define MLN_FUN_COMPONENT_GREEN_HH

/// \file
///
/// Meta function to retrieve/modify the green component.

# include <mln/fun/unary.hh>
# include <mln/value/rgb.hh>

namespace mln
{

  namespace fun
  {

    struct green : unary<green> {};

  } // end of namespace mln::fun

# ifndef MLN_INCLUDE_ONLY

  namespace trait
  {

    namespace next
    {

      template <unsigned n>
      struct set_precise_unary_<mln::fun::green, mln::value::rgb<n> >
      {
	typedef set_precise_unary_ ret;
	typedef mln::value::rgb<n> argument;
	typedef typename argument::green_t result;
	typedef argument& lvalue;

	static result read(const argument& x)
	{
	  return x.green();
	}

	static void write(lvalue l, const result& r)
	{
	  l.green() = r;
	}
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_FUN_COMPONENT_GREEN_HH
