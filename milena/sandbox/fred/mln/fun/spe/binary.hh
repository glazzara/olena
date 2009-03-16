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

#ifndef MLN_FUN_SPE_BINARY_HH
# define MLN_FUN_SPE_BINARY_HH

# include <mln/core/concept/function.hh>
# include <mln/trait/next/solve.hh>
# include <mln/trait/fun.hh>

/// \todo Implements parameter support

namespace mln
{

  namespace fun
  {

    namespace spe
    {

      // Forward declaration
      template <typename Fun, typename T1, typename T2>
      struct binary;

      namespace impl
      {

	template <typename Fun, typename T1, typename T2>
	struct binary_impl : mln::Function_v2v< binary<Fun, T1, T2> >
	{
	  typedef mln_trait_nbinary(Fun, T1, T2) impl;

	  typedef typename impl::argument1 argument1;
	  typedef typename impl::argument2 argument2;
	  typedef typename impl::result    result;

	  binary_impl()
	  : impl_()
	  {
	  }

	  binary_impl(const impl& f)
	  : impl_(f)
	  {
	  }

	  result operator () (const argument1& a, const argument2& b) const
	  {
	    return this->impl_.read(a, b);
	  }

	protected:
	  impl impl_;
	};

      } // end of namespace mln::fun::spe::impl

      template <typename Fun, typename T1, typename T2>
      struct binary
      : impl::binary_impl<Fun, T1, T2>
      {
	typedef impl::binary_impl<Fun, T1, T2> super;

	binary()
	: super()
	{
	}

	binary(const typename super::impl& f)
	: super(f)
	{
	}

	using super::operator();
      };

    } // end of namespace mln::fun::spe

  } // end of namespace mln::fun

} // end of namespace mln

#endif /* ! MLN_FUN_SPE_BINARY_HH */
