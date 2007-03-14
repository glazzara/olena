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

#ifndef OLN_LEVEL_APPLY_HH
# define OLN_LEVEL_APPLY_HH

# include <oln/core/concept/image.hh>


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

      template < typename F >
      struct result
      {
	  typedef typename F::result ret;
      };

      template < typename R (*fun)(A) >
      struct result
      {
	  typedef typename R ret;
      };

      template < typename F >
      struct argument
      {
	  typedef typename F::argument ret;
      };

      template < typename R (*fun)(A) >
      struct argument
      {
	  typedef typename A	ret;
      };


      // APPLY //
      //---------

      template <typename F, typename I>
      oln_plain_value(I, result<typename F>::ret)
        apply(const F& fun, const Image<I>& input)
      {
	typedef argument<typename F>::ret	A;
        typedef result<typename F>::ret		R;

        oln_ch_value(I, R) output(input.points());
        oln_piter(I) p(input.points());
        for_all(p)
          output(p) = fun( static_cast<A>(input(p)) );
        return output;
      }


      // APPLY_LOCAL //
      //---------------

      template <typename F, typename I>
      oln_plain_value(I, result<typename F>::ret)
        apply_local(const F& fun, const Image<I>& input)
      {
        typedef argument<typename F>::ret	A;
        typedef result<typename F>::ret		R;

        oln_ch_value(I, R) output(input.points());
        oln_piter(I) p(input.points());
        for_all(p)
	  output(p) = local_apply(fun, input, p);
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
      apply(const F& fun, const Image<I>& input)
    {
      return impl::apply(fun, exact(input));
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
      apply_local(const F& fun, const Image<I>& input)
    {
      return impl::apply_local(fun, exact(input));
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
