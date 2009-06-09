// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
// software; you can blueistribute it and/or modify it under the terms
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
// resulting executable to be coveblue by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be coveblue by the GNU General
// Public License.

#ifndef MLN_FUN_COMPONENT_BLUE_HH
# define MLN_FUN_COMPONENT_BLUE_HH

/// \file
///
/// Meta function to retrieve/modify the blue component.

# include <mln/fun/unary.hh>
# include <mln/value/rgb.hh>

namespace mln
{

  namespace fun
  {

    struct blue : unary<blue> {};

  } // end of namespace mln::fun

# ifndef MLN_INCLUDE_ONLY

  namespace trait
  {

    namespace next
    {

      template <unsigned n>
      struct set_precise_unary_<mln::fun::blue, mln::value::rgb<n> >
      {
	typedef set_precise_unary_ ret;
	typedef mln::value::rgb<n> argument;
	typedef typename argument::blue_t result;
	typedef argument& lvalue;

	static result read(const argument& x)
	{
	  return x.blue();
	}

	static void write(lvalue l, const result& r)
	{
	  l.blue() = r;
	}
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_FUN_COMPONENT_BLUE_HH
