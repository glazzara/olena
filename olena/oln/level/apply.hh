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

#ifndef OLN_LEVEL_APPLY_HH
# define OLN_LEVEL_APPLY_HH

# include <oln/core/concept/image.hh>
# include <oln/core/concept/iterator.hh>
# include <oln/core/internal/f_ch_value.hh>
# include <oln/level/local.hh>

namespace oln
{

  namespace level
  {

    /// Fwd decl.
    template <typename R, typename A, typename I>
    oln_plain_value(I, R)
      apply(R (*fun)(A), const Image<I>& input);


//     /// Fwd decl.
//     template <typename I, typename F>
//     oln_plain_value(I, typename F::result_value)
//     apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun);

//     /// Fwd decl.
//     template <typename I, typename V>
//     void apply_inplace(abstract::mutable_image<I>& input, V (*fun)(const oln_value(I)&));

//     /// Fwd decl.
//     template <typename I, typename F>
//     void apply_inplace(abstract::mutable_image<I>& input, const abstract::fun_v2v<F>& fun);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      /// apply

      template <typename F, typename I>
      oln_plain_value(I, typename F::result)
	apply( F& f, const Image<I>& input)
      {
	typedef typename F::result	result;
        typedef typename F::argument	argument;

        oln_ch_value(I, result) output(input.points());
        oln_piter(I) p(input.points());
        for_all(p)
          output(p) = f( static_cast< argument >(input(p)) );
        return output;
      }


      /// apply_local

      template <typename F, typename I>
      I apply_local(const Accumulator<F>&    f,
		    const Image_with_Nbh<I>& input)
      {
        typedef typename F::result	result;
        typedef typename F::argument	argument;

	typename I::delegatee out(input.points());
        I output(out, input.nbhood());
        // I output(exact(input).image(), input.nbhood());
        // FIXME: init(output, with, input);
        oln_piter(I) p(input.points());
        for_all(p)
	  output(p) = level::local(f, input, p);
        return output;
      }


//       /// Generic version.
//       template <typename I, typename F>
//       oln_plain_value(I, typename F::result_value)
//       apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun)
//       {
// 	oln_plain_value(I, typename F::result_value) output(input.topo());
// 	oln_piter(I) p(input.topo());
// 	for_all(p)
// 	  output(p) = fun.exact()(input(p));
// 	return output;
//       }


//       /// Generic version.
//       template <typename I, typename V>
//       void
//       apply_inplace(abstract::mutable_image<I>& input, V (*fun)(const oln_value(I)&))
//       {
// 	oln_piter(I) p(input.topo());
// 	for_all(p)
// 	  input(p) = fun(input(p));
//       }

//       /// Generic version.
//       template <typename I, typename F>
//       void
//       apply_inplace(abstract::mutable_image<I>& input, const abstract::fun_v2v<F>& fun)
//       {
// 	oln_piter(I) p(input.topo());
// 	for_all(p)
// 	  input(p) = fun.exact()(input(p));
//       }

    } // end of namespace oln::level::impl

    /// Facades.
    //----------------------------

    /// Apply.

    template <typename R, typename A, typename I>
    oln_plain_value(I, R)
      apply(R (*fun)(A), const Image<I>& input)
    {
      return impl::apply(fun, exact(input));
    }

    template <typename F, typename I>
    oln_plain_value(I, typename F::result)
      apply(F& f, const Image<I>& input)
    {
      return impl::apply(f, exact(input));
    }

    /// Apply local

    template <typename R, typename A, typename I>
    oln_plain_value(I, R)
      apply_local(R (*fun)(A), const Image<I>& input)
    {
      return impl::apply_local(fun, exact(input));
    }

    template <typename F, typename I>
    oln_plain_value(I, typename F::result)
    apply_local(const Accumulator<F>&    fun,
		const Image_with_Nbh<I>& input)
    {
      return impl::apply_local(fun, input);
    }

//     /// Facade.
//     template <typename I, typename F>
//     oln_plain_value(I, typename F::result_value)
//     apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun)
//     {
//       return impl::apply(input, fun);
//     }


//     /// Facade.
//     template <typename I, typename V>
//     void
//     apply_inplace(abstract::mutable_image<I>& input, V (*fun)(const oln_value(I)&))
//     {
//       return impl::apply_inplace(input, fun);
//     }

//     /// Facade.
//     template <typename I, typename F>
//     void
//     apply_inplace(abstract::mutable_image<I>& input, const abstract::fun_v2v<F>& fun)
//     {
//       return impl::apply_inplace(input, fun);
//     }

# endif

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_APPLY_HH
