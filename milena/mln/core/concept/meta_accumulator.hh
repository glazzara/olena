// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_META_ACCUMULATOR_HH
# define MLN_CORE_CONCEPT_META_ACCUMULATOR_HH

/// \file
///
/// Definition of the concept of mln::Meta_Accumulator.

# include <mln/core/concept/object.hh>
# include <mln/core/concept/accumulator.hh>


# define mln_accu_with(A, T) \
typename mln::internal::accu_with_helper<A,T>::ret

# define mln_accu_with_(A, T) \
mln::internal::accu_with_helper<A,T>::ret


# define mln_meta_accu_result(A, T) \
typename mln::internal::meta_accu_ret_result_helper<A,T>::result


# define mln_meta_accu_result_(A, T)				\
mln::internal::meta_accu_ret_result_helper<A,T>::result




namespace mln
{

  // Forward declaration.
  template <typename E> struct Meta_Accumulator;


  /// \cond INTERNAL_API
  namespace internal
  {

    /*!
      \brief Make the type resolution easier for the compiler.
      Introduced for ICC compatibility.
    */
    template <typename A, typename T>
    struct meta_accu_ret_result_helper
    {
	typedef typename A::template with< T >::ret::result result;
    };


    /*!
      \brief Make the type resolution easier for the compiler.
      Introduced for ICC compatibility.
    */
    template <typename A, typename T>
    struct accu_with_helper
    {
	typedef typename A::template with< T >::ret ret;
    };

  } // end of namespace mln::internal
  /// \endcond


  /// \cond INTERNAL_API
  /// Meta_Accumulator category flag type.
  template <>
  struct Meta_Accumulator<void>
  {
    typedef Object<void> super;
  };
  /// \endcond

  /*!
    \brief Base class for implementation of meta accumulators.

    The parameter \a E is the exact type.

    \see mln::doc::Meta_Accumulator for a complete documentation of
    this class contents.

    \ingroup modconcepts
   */
  template <typename E>
  struct Meta_Accumulator : public Object<E>
  {
    typedef Meta_Accumulator<void> category;
  protected:
    Meta_Accumulator();
  };


  namespace accu
  {

    // To be specialized when some state (attributes) have to be transfered
    // from the meta-accumulator to the accumulator.
    // Warning: the first argument has to be an object with the exact type.
    template <typename M, typename T>
    mln_accu_with(M, T)
    unmeta(const M&, T);

    template <typename M, typename T>
    void
    unmeta(const Meta_Accumulator<M>&, T); // Safety.

  } // end of namespace mln::accu



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Meta_Accumulator<E>::Meta_Accumulator()
  {
    // FIXME: Check "with" on E.
  }

  namespace accu
  {

    template <typename M, typename T>
    inline
    mln_accu_with(M, T)
    unmeta(const M&, T)
    {
      mlc_is_a(M, Meta_Accumulator)::check();
      mln_accu_with(M, T) a;
      return a;
    }

  } // end of namespace mln::accu

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_META_ACCUMULATOR_HH
