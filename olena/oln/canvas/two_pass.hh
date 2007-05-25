// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
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

#ifndef	OLN_CANVAS_TWO_PASS_HH
# define OLN_CANVAS_TWO_PASS_HH

# include <oln/core/concept/image.hh>


namespace oln
{

  namespace canvas
  {

    namespace v1
    {
      template <typename F> // Data owned by f.
      void two_pass(F& fun)
      {

	typedef typename F::image I;
	mlc::assert_< mlc_is_a(I, Image) >::check();

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
      void two_pass(F& fun, I f)
      {
	mlc::assert_< mlc_is_a(I, Image) >::check();

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
      template <typename F, typename D>
      void two_pass(F& fun, D& data)
      {
	mlc::assert_< mlc_is_a(typename D::I, Image) >::check();

	fun.init(data);

	// first pass
	oln_bkd_piter(D::I) p1(data.f.points());
	for_all(p1)
	  fun.first_pass_body(p1, data);

	// second pass
	oln_fwd_piter(D::I) p2(data.f.points());
	for_all(p2)
	  fun.second_pass_body(p2, data);

	fun.final(data);
      }
    }


    namespace v4 // Via Inheritance.
    {
      template <typename I, typename Exact>
      struct two_pass : public virtual Any<Exact>
      {
	void init()
	{
	  exact(this)->impl_init();
	}

	void first_pass_body(const oln_point(I)& p)
	{
	  exact(this)->impl_first_pass_body(p);
	}

	void second_pass_body(const oln_point(I)& p)
	{
	  exact(this)->impl_second_pass_body(p);
	}

	void final()
	{
	  exact(this)->impl_final();
	}

	// Concrete method.
	void run()
	{
	  init();

	  // first pass
	  for_all(p1)
	    first_pass_body(p1);

	  // second pass

	  for_all(p2)
	    second_pass_body(p2);

	  final();
	}


      protected:

	// Ctor.
	two_pass(const Image<I>& f) :
	  p1(f.points()),
	  p2(f.points())
	{
	}

	oln_bkd_piter(I) p1;
	oln_fwd_piter(I) p2;

      };
    }

  } // end of namespace morpho

} // end of namespace oln

#endif // ! OLN_CANVAS_TWO_PASS_HH
