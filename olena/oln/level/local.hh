// Copyright (C) 2007 EPITA Research and
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

#ifndef   	OLN_LEVEL_LOCAL_HH_
# define   	OLN_LEVEL_LOCAL_HH_

namespace oln
{

  namespace impl
  {

    // LOCAL APPLICATION ON NBH //
    //----------------------------

    /// Local Apply on neighborhood ( nbh included in image ).

    template <typename R, typename A, typename I>
    typename A::result
    local_apply(const Accumulator< A >&		fun,
		const Image_with_Nbh< I >&	input,
		const oln_point( I )&		p)
    {
      fun.init();
      oln_niter(I) n(p, input.points());
      for_all(n)
	fun(input(n));
      return fun.value();
    }


    /// Local Apply on neighborhood ( nhb has to be given ).

    // ...FIXME


    // LOCAL APPLICATION ON WINDOW //
    //-------------------------------

    /// Local Apply on window ( window included in image).

    // ...FIXME


    /// Local Apply on window ( window is given ).

    template <typename F, typename I, typename W>
    typename F::result
    local_apply(const Accumulator<F>&	fun,
		const Image<I>&		input,
		const oln_point(I)&	p,
		const Window<W>&	win)
    {
      fun.init();
      oln_qiter(W) q(p, win);
      for_all(q)
	fun(input(q));
      return fun.value();
    }

  }

  /// Facades.

  local_apply( )
  {
    
  }

}
#endif	    /* !OLN_LEVEL_LOCAL_HH_ */
