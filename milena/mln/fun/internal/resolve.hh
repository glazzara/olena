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

#ifndef MLN_FUN_INTERNAL_RESOLVE_HH
# define MLN_FUN_INTERNAL_RESOLVE_HH

/// \file
///
/// \brief FIXME.

# include <mln/core/concept/function.hh>
# include <mln/trait/solve.hh>
# include <mln/value/builtin/all.hh>


# define mln_fun_internal_resolve(F) typename fun::internal::resolve< F >::ret



namespace mln
{

  namespace fun
  {

    namespace internal
    {

      template <typename F>
      struct resolve;

      template < template <class, class> class F,
		 typename L,
		 typename R >
      struct resolve< F<L,R> > : public mln::trait::solve_binary< F, L, R >,
				 private mlc_is_a__1comma(F<L,R>, Function)::check_t
      {
      };

      template < template <class> class F,
		 typename T >
      struct resolve< F<T> > : public mln::trait::solve_unary< F, T >,
			       private mlc_is_a(F<T>, Function)::check_t
      {
      };

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_INTERNAL_RESOLVE_HH
