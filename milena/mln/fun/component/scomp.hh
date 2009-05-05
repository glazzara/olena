// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_FUN_COMPONENT_SCOMP_HH
# define MLN_FUN_COMPONENT_SCOMP_HH

/// \file mln/fun/component/scomp.hh
///
/// Meta function to retrieve/modify the i'th component, where i is
/// statically defined.

# include <mln/fun/unary.hh>
# include <mln/fun/component/comp.hh>

namespace mln
{

  namespace fun
  {

    template <unsigned ith>
    struct scomp : unary< scomp<ith> >
    {
    };

  } // end of namespace mln::fun

# ifndef MLN_INCLUDE_ONLY

  namespace trait
  {

    namespace next
    {

      template <unsigned ith, typename T>
      struct set_unary_<fun::scomp<ith>, mln::Object, T>
      {
	typedef set_unary_ ret;
	typedef mln_trait_nunary(fun::comp, T) comp_t;

	typedef T argument;
	typedef mln_result(comp_t) result;
	typedef mln_lvalue(comp_t) lvalue;

	static result read(const argument& v)
	{
	  return comp_t::read(ith, v);
	}

	static void write(lvalue l, const result& r)
	{
	  comp_t::write(ith, l, r);
	}
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // MLN_FUN_COMPONENT_COMP_HH
