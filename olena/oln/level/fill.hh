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

#ifndef OLN_LEVEL_FILL_HH
# define OLN_LEVEL_FILL_HH

# include <iostream>

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/iterator.hh>
# include <oln/core/abstract/functions.hh>


namespace oln
{

  namespace ERROR
  {
    struct FIRST_ARGUMENT_OF_oln_level_fill_IS_NOT_MUTABLE;
  }


  /// Fwd decl.
  template <typename I, typename V>
  I& operator<<(abstract::mutable_image<I>& input, const V values[]);

  /// Fwd decl.
  template <typename I>
  I& operator<<(abstract::mutable_image<I>& input, const oln_value(I)& value);


  namespace level
  {

    /// Fwd decl.
    template <typename I>
    void fill(abstract::mutable_image<I>& input, const oln_value(I)& value);

    /// Fwd decl.
    template <typename I, typename V>
    void fill(abstract::mutable_image<I>& input, const V values[]);

    /// Fwd decl.
    template <typename I, typename V, typename P>
    void fill(abstract::mutable_image<I>& input, V (*fun)(const P&));

    /// Fwd decl.
    template <typename I, typename F>
    void fill(abstract::mutable_image<I>& input, const abstract::fun_p2v<F>& fun);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      /// Generic version.
      template <typename I>
      void fill(abstract::mutable_image<I>& input, const oln_value(I)& value)
      {
	oln_piter(I) p(input.topo());
	for_all(p)
	  input(p) = value;
      }

      /// Generic version.
      template <typename I, typename V>
      void fill(abstract::mutable_image<I>& input, const V values[])
      {
	oln_piter(I) p(input.topo());
	unsigned i = 0;
	for_all(p)
	  input(p) = values[i++];
      }

      /// Generic version.
      template <typename I, typename V, typename P>
      void fill(abstract::mutable_image<I>& input, V (*fun)(const P&))
      {
	oln_piter(I) p(input.topo());
	unsigned i = 0;
	for_all(p)
	  input(p) = fun(p);
      }

      /// Generic version.
      template <typename I, typename F>
      void fill(abstract::mutable_image<I>& input, const abstract::fun_p2v<F>& fun)
      {
	oln_piter(I) p(input.topo());
	for_all(p)
	  input(p) = fun.exact()(p);
      }

    } // end of namespace oln::level::impl


    /// Facade.
    template <typename I>
    void fill(abstract::mutable_image<I>& input, const oln_value(I)& value)
    {
      impl::fill(input.exact(), value);
    }

    /// Facade.
    template <typename I, typename V>
    void fill(abstract::mutable_image<I>& input, const V values[])
    {
      return impl::fill(input.exact(), values);
    }

    /// Facade.
    template <typename I, typename V, typename P>
    void fill(abstract::mutable_image<I>& input, V (*fun)(const P&))
    {
      return impl::fill(input.exact(), fun);
    }

    /// Facade.
    template <typename I, typename F>
    void fill(abstract::mutable_image<I>& input, const abstract::fun_p2v<F>& fun)
    {
      return impl::fill(input.exact(), fun);
    }

# endif

  } // end of namespace oln::level


# ifndef OLN_INCLUDE_ONLY
    
  /// Fwd decl.
  template <typename I, typename V>
  I& operator<<(abstract::mutable_image<I>& input, const V values[])
  {
    level::fill(input, values);
    return input.exact();
  }
 
  /// Fwd decl.
  template <typename I>
  I& operator<<(abstract::mutable_image<I>& input, const oln_value(I)& value)
  {
    level::fill(input, value);
    return input.exact();
  }

# endif

} // end of namespace oln


#endif // ! OLN_LEVEL_FILL_HH
