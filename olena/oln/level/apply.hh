// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_LEVEL_APPLY_HH
# define OLN_LEVEL_APPLY_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/iterator.hh>
# include <oln/core/abstract/functions.hh>


namespace oln
{

  namespace level
  {

    /// Fwd decl.
    template <typename I, typename V>
    oln_plain_value(I, V)
    apply(const abstract::image<I>& input, V (*fun)(const oln_value(I)&));

    /// Fwd decl.
    template <typename I, typename F>
    oln_plain_value(I, typename F::result_value)
    apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      /// Generic version.
      template <typename I, typename V>
      oln_plain_value(I, V)
      apply(const abstract::image<I>& input, V (*fun)(const oln_value(I)&))
      {
	oln_plain_value(I, V) output(input.topo());
	oln_piter(I) p(input.topo());
	for_all(p)
	  output(p) = fun(input(p));
	return output;
      }

      /// Generic version.
      template <typename I, typename F>
      oln_plain_value(I, typename F::result_value)
      apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun)
      {
	oln_plain_value(I, typename F::result_value) output(input.topo());
	oln_piter(I) p(input.topo());
	for_all(p)
	  output(p) = fun.exact()(input(p));
	return output;
      }

    } // end of namespace oln::level::impl


    /// Facade.
    template <typename I, typename V>
    oln_plain_value(I, V)
    apply(const abstract::image<I>& input, V (*fun)(const oln_value(I)&))
    {
      return impl::apply(input, fun);
    }

    /// Facade.
    template <typename I, typename F>
    oln_plain_value(I, typename F::result_value)
    apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun)
    {
      return impl::apply(input, fun);
    }

# endif

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_APPLY_HH
