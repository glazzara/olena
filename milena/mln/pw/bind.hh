// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE).
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_PW_BIND_HH
# define MLN_PW_BIND_HH

/// \file
/// \brief Definition of point-wise function bindings.

# include <mln/core/concept/function.hh>
# include <mln/fun/internal/selector.hh>


namespace mln
{

  namespace pw
  {

    /*-------------------------.
    | Unary function binding.  |
    `-------------------------*/

    /// \brief Point-wise unary function binding.
    ///
    /// This point-wise class is actually a function object embedding
    /// a Milena function (functor) and another point-wise expression.
    /// Its operator() returns the application of the embedded
    /// function to the embedded point-wise expression.
    template <typename F, typename PW>
    struct unary_bound_fun_
      : fun::internal::selector_from_result_< typename F::result,
                                              unary_bound_fun_<F, PW> >::ret
    {
      typedef typename F::result result;

      unary_bound_fun_(const Function_v2v<F>& f, const Function_v2v<PW>& pw);

      template <typename P>
      result operator()(const P& p) const;

    protected:
      /// The function to apply.
      const F f_;
      /// The point-wise expression.
      const PW pw_;
    };


    /// \brief Bind the unary function \a f to the point-wise
    /// expression \a pw.
    template <typename F, typename PW>
    unary_bound_fun_<F, PW>
    bind(const Function_v2v<F>& f, const Function_v2v<PW>& pw);



    /*--------------------------.
    | Binary function binding.  |
    `--------------------------*/

    /// \brief Point-wise binary function binding.
    ///
    /// This point-wise class is actually a function object embedding
    /// a Milena function (functor) and another point-wise expression.
    /// Its operator() returns the application of the embedded
    /// function to the embedded point-wise expression.
    template <typename F, typename PW1, typename PW2>
    struct binary_bound_fun_
      : fun::internal::selector_from_result_< typename F::result,
                                              binary_bound_fun_<F,
                                                                PW1,
                                                                PW2> >::ret
    {
      typedef typename F::result result;

      binary_bound_fun_(const Function_vv2v<F>& f,
                        const Function_v2v<PW1>& pw1,
                        const Function_v2v<PW2>& pw2);

      template <typename P>
      result operator()(const P& p) const;

    protected:
      /// The function to apply.
      const F f_;
      /// The first point-wise expression.
      const PW1 pw1_;
      /// The second point-wise expression.
      const PW2 pw2_;
    };


    /// \brief Bind the binary function \a f to the point-wise
    /// expressions \a pw1 and \a pw2.
    template <typename F, typename PW1, typename PW2>
    binary_bound_fun_<F, PW1, PW2>
    bind(const Function_vv2v<F>& f,
         const Function_v2v<PW1>& pw1, const Function_v2v<PW2>& pw2);



# ifndef MLN_INCLUDE_ONLY

    // pw::unary_bound_fun_<F, PW>.

    template <typename F, typename PW>
    inline
    unary_bound_fun_<F, PW>::unary_bound_fun_(const Function_v2v<F>& f,
                                              const Function_v2v<PW>& pw)
      : f_(exact(f)),
        pw_(exact(pw))
    {
    }

    template <typename F, typename PW>
    template <typename P>
    inline
    typename F::result
    unary_bound_fun_<F, PW>::operator()(const P& p) const
    {
      return f_(pw_(p));
    }


    // pw::bind(f, pw).

    template <typename F, typename PW>
    inline
    unary_bound_fun_<F, PW>
    bind(const Function_v2v<F>& f, const Function_v2v<PW>& pw)
    {
      return unary_bound_fun_<F, PW>(f, pw);
    }



    // pw::binary_bound_fun_<F, PW1, PW2>.

    template <typename F, typename PW1, typename PW2>
    inline
    binary_bound_fun_<F, PW1, PW2>::binary_bound_fun_(const Function_vv2v<F>& f,
                                                      const Function_v2v<PW1>& pw1,
                                                      const Function_v2v<PW2>& pw2)
      : f_(exact(f)),
        pw1_(exact(pw1)),
        pw2_(exact(pw2))
    {
    }

    template <typename F, typename PW1, typename PW2>
    template <typename P>
    inline
    typename F::result
    binary_bound_fun_<F, PW1, PW2>::operator()(const P& p) const
    {
      return f_(pw1_(p), pw2_(p));
    }


    // pw::bind(f, pw1, pw2).

    template <typename F, typename PW1, typename PW2>
    inline
    binary_bound_fun_<F, PW1, PW2>
    bind(const Function_vv2v<F>& f,
         const Function_v2v<PW1>& pw1, const Function_v2v<PW2>& pw2)
    {
      return binary_bound_fun_<F, PW1, PW2>(f, pw1, pw2);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::pw

} // end of namespace mln


#endif // ! MLN_PW_BIND_HH
