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

# include <oln/core/concept/image.hh>

#ifndef	OLN_CANVAS_TWO_PASS_HH
# define OLN_CANVAS_TWO_PASS_HH

namespace canvas
{

  namespace v1
  {
    template <template <class> class F,
	      typename I> // Data owned by f.
    void two_pass(F<I>& fun)
    {
//      mlc::assert_< mlc_is_a(I, Image) >::check();

      fun.init();

      // first pass
      oln_bkd_piter(I) p1(fun.f.points());
      for_all(p1)
	fun.first_pass_body(p1);

      // second pass
      oln_fwd_piter(I) p2(fun.f.points());
      for_all(p2)
	fun.second_pass_body(p2);

      fun.final();
    }
  }

  namespace v2 // Data owned by f but not input.
  {
    template <typename F, typename I>
    void two_pass(F fun, I f)
    {
//      mlc::assert_< mlc_is_a(I, Image) >::check();

      fun.init(f);

      // first pass
      oln_bkd_piter(I) p1(f.points());
      for_all(p1)
	fun.first_pass_body(p1, f);

      // second pass
      oln_fwd_piter(I) p2(f.points());
      for_all(p2)
	fun.second_pass_body(p2, f);

      fun.final(f);
    }
  }

  namespace v3 // Auxiliar data given as argument.
  {
    template <typename F, typename I, typename A>
    void two_pass(F fun, I f, A aux)
    {
//      mlc::assert_< mlc_is_a(I, Image) >::check();

      f.init(f, aux);

      // first pass
      oln_bkd_piter(I) p1(f.points());
      for_all(p1)
	f.first_pass_body(p1, f, aux);

      // second pass
      oln_fwd_piter(I) p2(f.points());
      for_all(p2)
	f.second_pass_body(p2, f, aux);

      f.final(f, aux);
    }
  }


  namespace v4 // Via Inheritance.
  {

    template <typename I>
    struct two_pass
    {

      const I&      f;

      

      void init();

      void final();

      void first_pass_body(const oln_point(I)& p);

      void second_pass_body(const oln_point(I)& p);

      void run()
      {
	init(f);

	// first pass
	oln_bkd_piter(I) p1(f.points());
	for_all(p1)
	  first_pass_body(p1);

	// second pass
	oln_fwd_piter(I) p2(f.points());
	for_all(p2)
	  second_pass_body(p2);

	final(f);

      }

    };

  }

}

#endif // ! OLN_CANVAS_TWO_PASS_HH
