// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_CONCEPT_META_FUNCTION_HH
# define MLN_CORE_CONCEPT_META_FUNCTION_HH

/// \file
///
/// Definition of the concept of mln::Meta_Function.

# include <mln/core/concept/object.hh>
# include <mln/core/concept/function.hh>

# define mln_fun_with(F, T) \
typename F::template with< T >::ret

# define mln_fun_with_(F, T) \
F::with< T >::ret

# define mln_fun_withbin(F, T1, T2) \
typename F::template with< T1, T2 >::ret

# define mln_fun_withbin_(F, T1, T2) \
F::with< T1, T2 >::ret

# define mln_fun_result(F, T) \
typename F::template with< T >::ret::result

# define mln_fun_result_(F, T) \
F::with< T >::ret::result

namespace mln
{

  // Fwd decl.
  template <typename E> struct Meta_Function;
  template <typename E> struct Meta_Function_v2v;
  template <typename E> struct Meta_Function_vv2v;

  /// \cond INTERNAL_API
  /// Meta_Function category flag type.
  template <>
  struct Meta_Function<void>
  {
    typedef Object<void> super;
  };
  /// \endcond


  /*! \brief Base class for implementation of meta functions.

    The parameter \a E is the exact type.

    \ingroup modconcepts
   */
  template <typename E>
  struct Meta_Function : public Object<E>
  {
    typedef Meta_Function<void> category;
  protected:
    Meta_Function();
  };

  /*----------------------.
  | Unary meta function.  |
  `----------------------*/

  /// \cond INTERNAL_API
  template <>
  struct Meta_Function_v2v<void> { typedef Meta_Function<void> super; };
  /// \endcond

  /// Base class for implementation of function-objects from
  /// value to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Meta_Function_v2v : public Meta_Function<E>
  {
    typedef Meta_Function_v2v<void> category;

    protected:
      Meta_Function_v2v();
      Meta_Function_v2v(const Meta_Function_v2v&);
  };

  /*-----------------------.
  | Binary meta function.  |
  `-----------------------*/

  /// \cond INTERNAL_API
  template <>
  struct Meta_Function_vv2v<void> { typedef Meta_Function<void> super; };
  /// \endcond

  /// Base class for implementation of function-objects from
  /// value to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Meta_Function_vv2v : public Meta_Function<E>
  {
    typedef Meta_Function_vv2v<void> category;

    protected:
      Meta_Function_vv2v();
      Meta_Function_vv2v(const Meta_Function_vv2v&);
  };

  namespace fun
  {

    // To be specialized when some state (attributes) have to be transfered
    // from the meta-function to the function.
    // Warning: the first argument has to be an object with the exact type.
    template <typename M, typename T>
    mln_fun_with(M, T)
    unmeta(const M&, T);

    template <typename M, typename T>
    void
    unmeta(const Meta_Function<M>&, T); // Safety.

  } // end of namespace mln::fun



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Meta_Function<E>::Meta_Function()
  {
    // FIXME: Check "with" on E.
  }

  template <typename E>
  inline
  Meta_Function_v2v<E>::Meta_Function_v2v()
  {
  }

  template <typename E>
  inline
  Meta_Function_v2v<E>::Meta_Function_v2v(const Meta_Function_v2v<E>& rhs)
  : Meta_Function<E>(rhs)
  {
  }

  template <typename E>
  inline
  Meta_Function_vv2v<E>::Meta_Function_vv2v()
  {
  }

  template <typename E>
  inline
  Meta_Function_vv2v<E>::Meta_Function_vv2v(const Meta_Function_vv2v<E>& rhs)
  : Meta_Function<E>(rhs)
  {
  }

  namespace fun
  {

    template <typename M, typename T>
    inline
    mln_fun_with(M, T)
    unmeta(const M&, T)
    {
      mlc_is_a(M, Meta_Function)::check();
      mln_fun_with(M, T) a;
      return a;
    }

  } // end of namespace mln::fun

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_META_FUNCTION_HH
