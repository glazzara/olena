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

#ifndef	OLN_CANVAS_RECONSTRUCTION_HH
# define OLN_CANVAS_RECONSTRUCTION_HH

# include <oln/level/clone.hh>
# include <oln/level/local_sup.hh>
# include <oln/level/local_inf.hh>

// FIXME : similar to two_pass_until_stability

namespace oln
{

  namespace canvas
  {

    template <typename F>
    void reconstruction(F& fun)
    {
      typedef typename F::Ima I;

      bool stability;
      fun.output = level::clone(fun.marker);

      fun.init();

      do
      {
	fun.memo = level::clone(fun.output);

	// Forward Scan
	oln_fwd_piter(I) p1(fun.marker.points());
	for_all(p1)
	  {
	    if ( fun.condition_fwd(p1) )
	      fun.output(p1) = level::local_sup(fun.accu, fun.output, p1);
	  }

	// Backward Scan
	oln_bkd_piter(I) p2(fun.marker.points());
	for_all(p2)
	  {
	    if ( fun.condition_bkd(p2) )
	      fun.output(p2) = level::local_sup(fun.accu, fun.output, p2);
	  }

	// stability check
	stability = fun.is_stable();
	if (stability)
	  break;

	// prepare a new loop iteration
	fun.re_loop();
      } while (true);

      fun.final();
    }

  } // end of namespace canva

} // end of namespace oln

#endif // ! OLN_CANVAS_RECONSTRUCTION_HH
