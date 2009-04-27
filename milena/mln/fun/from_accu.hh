// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_FROM_ACCU_HH
# define MLN_FUN_FROM_ACCU_HH

# include <mln/fun/unary.hh>
# include <mln/core/concept/accumulator.hh>
# include <mln/math/acos.hh>
# include <mln/math/cos.hh>

namespace mln
{

  // from_accu: wrap an accumulator into a function
  namespace fun
  {
    template <typename A>
    struct from_accu : unary_param<from_accu<A>, A*>
    {
      from_accu() : unary_param<from_accu<A>, A*>() {};
      from_accu(A* a) : unary_param<from_accu<A>, A*>(a) {};
    };
  }

  namespace trait
  {

    namespace next
    {
      template <typename A, typename T>
      struct set_unary_<mln::fun::from_accu<A>, mln::Object, T>
      {
	typedef set_unary_           ret;
	typedef typename A::result   result;
	typedef typename A::argument argument;
	typedef A*                   param;

	set_unary_()
	{
	}

	set_unary_(const param& accu)
	: accu_(accu)
	{
	}

	result read(const argument& x) const
	{
	  mln_precondition(accu_ != 0);

	  accu_->take(x);
	  return accu_->to_result ();
	}

	void init(const param& accu)
	{
	  accu_ = accu;
	}

      protected:
	A* accu_;
      };

    }

  }

}

#endif /* ! MLN_FUN_FROM_ACCU_HH */