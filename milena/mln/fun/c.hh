// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_FUN_C_HH
# define MLN_FUN_C_HH

/// \file
///
/// Encapsulate a plain (C language-like) pointer to function
/// into a functor.
///
/// \todo Fix category: super is not defined!
/// See mln/value/builtin/integers.hh for an example.

# include <mln/fun/internal/selector.hh>
# include <mln/metal/unqualif.hh>


namespace mln
{

  /*!
    \internal
    \brief Concept-like.
  */
  template <typename E>
  struct C_Function;


  /*!
    \internal
    \brief Category declaration for a unary C function.
  */
  template <typename R, typename A>
  struct category< R (*)(A) >
  {
    typedef C_Function<void> ret;
  };



  // Forward declaration.
  namespace fun { template <typename F> struct C; }



  namespace trait
  {

    template <template <class> class Op,
	      typename R, typename A>
    struct set_unary_< Op, C_Function, R (*)(A) >
    {
      typedef Op< fun::C<R (*)(A)> > Op_;
      typedef typename Op_::ret ret;
    };

    template <template <class, class> class Op,
	      typename O,
	      typename R, typename A>
    struct set_binary_< Op,
			Object,      O,
			C_Function,  R (*)(A) >
    {
      typedef Op< O, fun::C<R (*)(A)> > Op_;
      typedef typename Op_::ret ret;
    };

  } // end of namespace trait



  namespace fun
  {

    template <typename F> struct C;


    // FIXME: Doc!
    template <typename R, typename A>
    struct C< R (*)(A) >
      :
      fun::internal::selector_< R, A, C<R(*)(A)> >::ret
    {
      C();
      C(R (*f)(A));
      typedef R result;
      R operator()(const mlc_unqualif(A)& a) const;
    protected:
      R (*f_)(A);
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename R, typename A>
    inline
    C<R(*)(A)>::C()
    {
    }

    template <typename R, typename A>
    inline
    C<R(*)(A)>::C(R (*f)(A))
      : f_(f)
    {
    }

    template <typename R, typename A>
    inline
    R
    C<R(*)(A)>::operator()(const mlc_unqualif(A)& a) const
    {
      return f_(a);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_C_HH
