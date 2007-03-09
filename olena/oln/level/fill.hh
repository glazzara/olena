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

# include <oln/core/concept/image.hh>
// # include <oln/core/concept/function.hh>


namespace oln
{

  namespace ERROR
  {
    struct FIRST_ARGUMENT_OF_oln_level_fill_IS_NOT_MUTABLE;
  }


  namespace level
  {

    /// Fwd decls.

    template <typename I>
    void fill(Mutable_Image<I>& input, const oln_value(I)& value);

    template <typename I, typename V>
    void fill(Mutable_Image<I>& input, const V values[]);

    template <typename I, typename V, typename P>
    void fill(Mutable_Image<I>& input, V (*fun)(const P&));



    // FIXME: Inactivated.

//     template <typename I, typename F>
//     void fill(Mutable_Image<I>& input, const abstract::fun_p2v<F>& fun);

//     template <typename I>
//     void fill(Value_Wise_Mutable_Image<I>& input, const oln_value(I)& value);


# ifndef OLN_INCLUDE_ONLY


    namespace impl
    {

      template <typename I>
      void fill(Mutable_Image<I>& input, const oln_value(I)& value)
      {
	oln_piter(I) p(input.points());
	for_all(p)
	  input(p) = value;
      }

//       template <typename I>
//       void fill(Value_Wise_Mutable_Image<I>& input,
// 		const oln_value(I)& value)
//       {
// 	oln_viter(I) v(input);
// 	for_all(v)
// 	  input.value_(v) = value;
//       }

      template <typename I, typename V>
      void fill(Mutable_Image<I>& input, const V values[])
      {
	oln_piter(I) p(input.points());
	unsigned i = 0;
	for_all(p)
	  input(p) = values[i++];
      }

      template <typename I, typename V, typename P>
      void fill(Mutable_Image<I>& input, V (*fun)(const P&))
      {
	oln_piter(I) p(input.points());
	for_all(p)
	  input(p) = fun(p);
      }

//       template <typename I, typename F>
//       void fill(Mutable_Image<I>& input, const abstract::fun_p2v<F>& fun)
//       {
// 	oln_piter(I) p(input.points());
// 	for_all(p)
// 	  input(p) = fun.exact()(p);
//       }

    } // end of namespace oln::level::impl



    /// Facades.

    template <typename I>
    void fill(Mutable_Image<I>& input, const oln_value(I)& value)
    {
      impl::fill(exact(input), value);
    }

    template <typename I, typename V>
    void fill(Mutable_Image<I>& input, const V values[])
    {
      impl::fill(exact(input), values);
    }

    template <typename I, typename V, typename P>
    void fill(Mutable_Image<I>& input, V (*fun)(const P&))
    {
      impl::fill(exact(input), fun);
    }

//     template <typename I, typename F>
//     void fill(Mutable_Image<I>& input, const abstract::fun_p2v<F>& fun)
//     {
//       impl::fill(exact(input), fun);
//     }

//     template <typename I>
//     void fill(Value_Wise_Mutable_Image<I>& input, const oln_value(I)& value)
//     {
//       impl::fill(exact(input), value);
//     }

# endif

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_FILL_HH
