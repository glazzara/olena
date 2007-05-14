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

#ifndef	OLN_CANVAS_SEQUENTIAL_HH
# define OLN_CANVAS_SEQUENTIAL_HH

// FIXME : similar to two_pass_until_stability

namespace oln
{

  namespace canvas
  {

    template <template <class> class F,
	      typename I>
    void sequential(F<I>& fun)
    {
      bool stability;

      fun.init();

      for (;;)
      {

	// Forward Scan
	oln_fwd_piter(I) p1(fun.f.points());
	oln_niter(I) n1(fun.f, p1);
	for_all(p1)
	  {
	    fun.accu.init_with( fun.f(p1) );
	    for_all(n1)
	      {
		if (n1 < p1)
		  fun.forward_scan(p1, n1);
	      }
	    fun.output(p1) = fun.accu.value();
	    //fun.accu.reload(); // FIXME : need reload
	  }

	// Backward Scan
	oln_bkd_piter(I) p2(fun.f.points());
	oln_niter(I) n2(fun.f, p2);
	for_all(p2)
	  {
	    fun.accu.init_with(fun.f(p2));
	    for_all(n2)
	      {
		if (n2 >= p2)
		  fun.backward_scan(p2, n2);
	      }
	    fun.output(p2) = fun.accu.value();
	    //fun.accu.reload();
	  }

	// stability check
	stability = fun.is_stable();
	if (stability)
	  break;

	// prepare a new loop iteration
	fun.re_loop();
      }

      fun.final();
    }

  } // end of namespace canva

} // end of namespace oln

#endif // ! OLN_CANVAS_SEQUENTIAL_HH
