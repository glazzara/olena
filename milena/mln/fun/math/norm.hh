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

#ifndef MLN_FUN_MATH_NORM_HH
# define MLN_FUN_MATH_NORM_HH

# include <mln/fun/unary.hh>
# include <mln/norm/all.hh>

namespace mln
{

  // Common norm functions, reversible
  namespace fun
  {
    namespace norm
    {
      struct l1 : unary<l1> {};

      struct l2 : unary<l2> {};

      struct linfty : unary<linfty> {};
    }
  }

  namespace trait
  {

    namespace next
    {

      template <unsigned n, typename T>
      struct set_precise_unary_<mln::fun::norm::l1, mln::algebra::vec<n, T> >
      {
	typedef set_precise_unary_ ret;
	typedef mln::algebra::vec<n, T> argument;
	typedef argument& lvalue;
	typedef mln_sum_product(argument,argument) result;
	
	static result read(const argument& x)
	{
	  return mln::norm::l1(x);
	}
	
	static void write(lvalue l, const result& x)
	{
	  l = l / read(l) * x;
	}
      };

      template <unsigned n, typename T>
      struct set_precise_unary_<mln::fun::norm::l2, mln::algebra::vec<n, T> >
      {
	typedef set_precise_unary_ ret;
	typedef mln::algebra::vec<n, T> argument;
	typedef argument& lvalue;
	typedef mln_sum_product(argument,argument) result;
	
	static result read(const argument& x)
	{
	  return mln::norm::l2(x);
	}
	
	static void write(lvalue l, const result& x)
	{
	  l = l / read(l) * x;
	}
      };

      template <unsigned n, typename T>
      struct set_precise_unary_<mln::fun::norm::linfty, mln::algebra::vec<n, T> >
      {
	typedef set_precise_unary_ ret;
	typedef mln::algebra::vec<n, T> argument;
	typedef argument& lvalue;
	typedef mln_sum_product(argument,argument) result;
	
	static result read(const argument& x)
	{
	  return mln::norm::linfty(x);
	}
	
	static void write(lvalue l, const result& x)
	{
	  l = l / read(l) * x;
	}
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait
  
} // end of namespace mln

#endif /* ! NORM_HH */
