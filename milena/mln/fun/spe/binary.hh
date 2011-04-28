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

#ifndef MLN_FUN_SPE_BINARY_HH
# define MLN_FUN_SPE_BINARY_HH

# include <mln/core/concept/function.hh>
# include <mln/trait/next/solve.hh>
# include <mln/trait/functions.hh>

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

	template <bool has_param, typename Fun, typename T1, typename T2>
	struct binary_impl;

	template <typename Fun, typename T1, typename T2>
	struct binary_impl<false, Fun, T1, T2>
	: mln::Function_v2v< binary<Fun, T1, T2> >
	{
	  typedef Fun flag;
	  typedef mln_trait_nbinary(flag, T1, T2) def;

	  typedef typename def::argument1 argument1;
	  typedef typename def::argument2 argument2;
	  typedef typename def::result    result;

	  result operator () (const argument1& a, const argument2& b) const
	  {
	    return def::read(a, b);
	  }


	  template <typename U>
	  void init(const U& value)
	  {
	    (void) value;
	  }

	};

	template <typename Fun, typename T1, typename T2>
	struct binary_impl<true, Fun, T1, T2>
	: mln::Function_v2v< binary<Fun, T1, T2> >
	{
	  typedef Fun flag;
	  typedef mln_trait_nbinary(flag, T1, T2) def;

	  typedef typename def::argument1 argument1;
	  typedef typename def::argument2 argument2;
	  typedef typename def::result    result;

	  typedef mln_trait_fun_param(def)   param;
	  typedef mln_trait_fun_storage(def) storage;

	  result operator () (const argument1& a, const argument2& b) const
	  {
	    return def::read(state_, a, b);
	  }

	  template <typename U>
	  void init(const U& value)
	  {
	    state_ = mln::trait::function::internal::introspect::has_storage_t<def, void>::compute(value);
	  }

	  stored<storage>& state()
	  {
	    return state_;
	  }

	  const stored<storage>& state() const
	  {
	    return state_;
	  }

	  protected:
	    mln::fun::stored<storage> state_;
	};

      } // end of namespace mln::fun::spe::impl

      template <typename Fun, typename T1, typename T2>
      struct binary
      : impl::binary_impl<mln_trait_fun_is_parametrable_(Fun)::value, Fun, T1, T2>
      {
	typedef impl::binary_impl<mln_trait_fun_is_parametrable_(Fun)::value, Fun, T1, T2> super;

	binary()
	{
	}

	template <typename U>
	binary(const U& param)
	{
	  this->super::init(param);
	}

	using super::operator();
      };

    } // end of namespace mln::fun::spe

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_SPE_BINARY_HH
