// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// You should have receiv a copy of the GNU General Public License
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

#include <oln/core/concept/image.hh>

#ifndef	OLN_CANVAS_UNION_FIND_HH
# define OLN_CANVAS_UNION_FIND_HH

namespace canvas
{

  template <template <class> class F,
	    typename I>
  void union_find(F<I> fun)
  {
    fun.init();

    // first pass
    oln_bkd_piter(I) p1(fun.f.points());
    for_all(p1)
    {
      if (fun.is_in_I(p1))
      {
	fun.make_set(p);
	oln_niter(I) n(fun.f.points(), p);
	for_all(n)
	  {
	    if ( fun.p_in_D_upper_or_equal(p1) )
	    {
	      if (is_processed(n))
		fun.first_pass_body1(p1);
	    }
	    else
	      if ( fun.p_in_Do(p1) )
		fun.is_processed(p1) = false;
	      else ( fun.p_in_D_lower_or_equal(p1) )
		{
		  fun.first_pass_body2(p1);
		}
	  }
      }
    }

    // second pass
    oln_fwd_piter(I) p2(fun.f.points());
    for_all(p2)
    {
      if (fun.is_in_I(p2))
      {
	if (fun.is_root(p2))
	  fun.set_output_value(p2);
	else
	  fun.output(p) = fun.output(fun.parent(p)) ; //bg label
      }
    }

    fun.final();
  }

}


#endif // ! OLN_CANVAS_UNION_FIND_HH
