// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
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

#include <iostream>

# include <oln/core/concept/image.hh>
# include <oln/core/concept/point.hh>
# include <oln/core/gen/fun.hh>



namespace oln
{

  namespace ERROR
  {
    struct FIRST_ARGUMENT_OF_oln_level_fill_IS_NOT_MUTABLE; // FIXME: Use it!
  }


  namespace level
  {

    // Fwd decls.

    template <typename I>
    void fill(Mutable_Image<I>& target, /* with */ const oln_value(I)& value);

    template <typename I, typename J>
    void fill(Mutable_Image<I>& target, /* with */ const Image<J>& data);

    template <typename I, typename F>
    void fill(Mutable_Image<I>& target, /* with */ const Function_p2v<F>& fun);

    template <typename I, typename V, typename P>
    void fill(Mutable_Image<I>& target, /* with */ V (*fun)(P));

    // FIXME: Inactivated.

//     template <typename I>
//     void fill(Value_Wise_Mutable_Image<I>& target, const oln_value(I)& value);


# ifndef OLN_INCLUDE_ONLY


    namespace impl
    {

      template <typename I>
      void fill_from_value_(Mutable_Image<I>& target, const oln_value(I)& value)
      {
	oln_piter(I) p(target.points());
	for_all(p)
	  target(p) = value;
      }

      template <typename I, typename J>
      void fill_from_image_(Mutable_Image<I>& target, const Image<J>& data)
      {
	oln_piter(I) p(target.points());
	for_all(p)
	  target(p) = data(p);
      }

      template <typename I, typename F>
      void fill_from_function_(Mutable_Image<I>& target, const F& f)
      {
	oln_piter(I) p(target.points());
	for_all(p)
	  target(p) = f(p);
      }

//       template <typename I>
//       void fill_(Value_Wise_Mutable_Image<I>& target,
// 		const oln_value(I)& value)
//       {
// 	oln_viter(I) v(target);
// 	for_all(v)
// 	  target.value_(v) = value;
//       }

    } // end of namespace oln::level::impl



    /// Facades.

    template <typename I>
    void fill(Mutable_Image<I>& target, const oln_value(I)& value)
    {
      impl::fill_from_value_(exact(target), value);
    }

    template <typename I, typename J>
    void fill(Mutable_Image<I>& target, const Image<J>& data)
    {
      assert_same_grid_<I, J>::check();
      precondition(target.points() <= data.points());
      impl::fill_from_image_(exact(target), exact(data));
    }

    template <typename I, typename F>
    void fill(Mutable_Image<I>& target, const Function_p2v<F>& fun)
    {
      impl::fill_from_function_(exact(target), exact(fun));
    }

    template <typename I, typename V, typename P>
    void fill(Mutable_Image<I>& target, V (*f)(P))
    {
      mlc::assert_< mlc_is_a(P, Point) >::check(); // FIXME: Add err msg.
      impl::fill_from_function_(exact(target), functorize_p2v(f));
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_FILL_HH
