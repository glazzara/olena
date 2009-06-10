// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_INTERNAL_ITERABLE_SET_HH
# define MLN_VALUE_INTERNAL_ITERABLE_SET_HH

/*! \file
 *
 * \brief Define the iterable value set derived from a type.
 */

# include <mln/core/concept/value_set.hh>
# include <mln/trait/value_.hh>
# include <mln/value/builtin/all.hh>
# include <mln/value/internal/convert.hh>


namespace mln
{

  namespace value
  {

    // Fwd decls.
    template <typename S> struct fwd_viter_;
    template <typename S> struct bkd_viter_;


    namespace internal
    {

      /*! Class that defines the set of values of type \c T.
       * This is the exhaustive iterable_set of values obtainable from type \c T.
       */
      template <typename T, typename E>
      struct iterable_set : public Value_Set<E>
      {
	/// Value associated type.
	typedef T value;

	/// Forward Value_Iterator associated type.
	typedef fwd_viter_<E> fwd_viter;

	/// Backward Value_Iterator associated type.
	typedef bkd_viter_<E> bkd_viter;

	/// Test if \p v belongs to this iterable_set: always true!
	bool has(const T& v) const;

	/// Give the \p i-th value.
	T operator[](unsigned i) const;

	/// Give the index of value \p v in this iterable_set.
	unsigned index_of(const T& v) const;

	/// Give the number of values.
	unsigned nvalues() const;

      };



# ifndef MLN_INCLUDE_ONLY

      template <typename T, typename E>
      inline
      bool
      iterable_set<T,E>::has(const T&) const
      {
	return true;
      }

      template <typename T, typename E>
      inline
      T
      iterable_set<T,E>::operator[](unsigned i) const
      {
	mln_precondition(i < nvalues());
	return mln::value::internal::convert_<T>::value_at_index(i);
      }

      template <typename T, typename E>
      inline
      unsigned
      iterable_set<T,E>::index_of(const T& v) const
      {
	return mln::value::internal::convert_<T>::index_of_value(v);
      }

      template <typename T, typename E>
      inline
      unsigned
      iterable_set<T,E>::nvalues() const
      {
	return mln_card(T);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln


# include <mln/value/viter.hh>


#endif // ! MLN_VALUE_INTERNAL_ITERABLE_SET_HH
