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

#ifndef MLN_FUN_COMPOSE_HH
# define MLN_FUN_COMPOSE_HH

# include <mln/fun/binary.hh>
# include <mln/fun/composition.hh>
# include <mln/fun/param.hh>

namespace mln
{
  // Composition
  namespace fun
  {
    struct compose : binary<compose> {};

    namespace internal
    {

      template <template <class> class CatF, typename F, template <class> class CatG, typename G>
      struct compose_helper;

    }

    template <template <class> class CatF,  typename F,
	      template <class> class CatG, typename G>
    struct parameter< internal::compose_helper<CatF, F, CatG, G> >
    {
      typedef typename internal::composition<CatF, F, CatG, G>::exact_type result;
      typedef typename result::param param;
    };

    namespace internal
    {

      template <template <class> class CatF, typename F, template <class> class CatG, typename G>
      struct compose_helper
      {
	typedef F argument1;
	typedef G argument2;

	typedef typename composition<CatF, F, CatG, G>::exact_type result;
	typedef mln_trait_fun_param(result) param;

	static result read(const F& f, const G& g)
	{
	  return result(param(f, g));
	}
      };

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun

  namespace trait
  {

    namespace next
    {

      // All kinds of supported compositions (meta : unary) with (meta or not : unary or binary)
      template <typename F, typename G>
      struct set_binary_< mln::fun::compose, mln::Meta_Function_v2v, F, mln::Meta_Function_v2v, G>
      {
	typedef mln::fun::internal::compose_helper<mln::Meta_Function_v2v, F, mln::Meta_Function_v2v, G> ret;
      };

      template <typename F, typename G>
      struct set_binary_< mln::fun::compose, mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G>
      {
	typedef mln::fun::internal::compose_helper<mln::Meta_Function_v2v, F, mln::Meta_Function_vv2v, G> ret;
      };

      template <typename F, typename G>
      struct set_binary_< mln::fun::compose, mln::Meta_Function_v2v, F, mln::Function_v2v, G>
      {
	typedef mln::fun::internal::compose_helper<mln::Meta_Function_v2v, F, mln::Function_v2v, G> ret;
      };

      template <typename F, typename G>
      struct set_binary_< mln::fun::compose, mln::Meta_Function_v2v, F, mln::Function_vv2v, G>
      {
	typedef mln::fun::internal::compose_helper<mln::Meta_Function_v2v, F, mln::Function_vv2v, G> ret;
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

}  // end of namespace mln

#endif /* ! MLN_FUN_COMPOSE_HH */
