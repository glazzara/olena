// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_VALUE_ARRAY_HH
# define MLN_VALUE_VALUE_ARRAY_HH

/// \file
///
/// Define a generic array indexed by an iterable type.

# include <mln/value/set.hh>


namespace mln
{

  namespace value
  {


    /// Generic array class over indexed by a value set with type \c T.
    template <typename T, typename V>
    struct value_array
    {
      enum {
	nvalues = mln_card(T)
      };

      /// Constructors.
      /// {
      value_array();
      value_array(const V& v);
      value_array(const value_array<T, V>& other);
      value_array& operator=(const value_array<T, V>& other);
      /// }

      /// Access elements through a value of \p T.
      /// {
      const V& operator()(const T& v) const;
      V& operator()(const T& v);
      /// }

      /// Access elements through array indexes.
      /// {
      const V& operator[](unsigned i) const;
      V& operator[](unsigned i);
      /// }

      /// Reference to the set of \p T.
      const mln::value::set<T>& vset() const;

    protected:

      const mln::value::set<T>& s_;
      V v_[nvalues];
    };


    template <typename T, typename V>
    std::ostream& operator<<(std::ostream& ostr, const value_array<T, V>& a);


# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename V>
    inline
    value_array<T,V>::value_array()
      : s_ (mln::value::set<T>::the())
    {
      typedef value::internal::iterable_set< T, set<T> > U;
      mlc_is(set<T>, U)::check();
    }

    template <typename T, typename V>
    inline
    value_array<T,V>::value_array(const V& v)
      : s_(mln::value::set<T>::the())
    {
      typedef value::internal::iterable_set< T, set<T> > U;
      mlc_is(set<T>, U)::check();

      memset(v_, v, nvalues * sizeof(V));
    }

    template <typename T, typename V>
    inline
    value_array<T,V>::value_array(const value_array<T, V>& other)
      : s_(other.s_)
    {
      memcpy(v_, other.v_, nvalues * sizeof(V));
    }

    template <typename T, typename V>
    inline
    value_array<T,V>&
    value_array<T,V>::operator=(const value_array<T, V>& other)
    {
      if (&other != this)
	memcpy(v_, other.v_, nvalues * sizeof(V));
      return *this;
    }

    template <typename T, typename V>
    inline
    const V&
    value_array<T,V>::operator()(const T& v) const
    {
      return v_[s_.index_of(v)];
    }

    template <typename T, typename V>
    inline
    V&
    value_array<T,V>::operator()(const T& v)
    {
      return v_[s_.index_of(v)];
    }

    template <typename T, typename V>
    inline
    const mln::value::set<T>&
    value_array<T,V>::vset() const
    {
      return s_;
    }

    template <typename T, typename V>
    inline
    const V&
    value_array<T,V>::operator[](unsigned i) const
    {
      mln_precondition(i < nvalues);
      return v_[i];
    }

    template <typename T, typename V>
    inline
    V&
    value_array<T,V>::operator[](unsigned i)
    {
      mln_precondition(i < nvalues);
      return v_[i];
    }

    template <typename T, typename V>
    inline
    std::ostream& operator<<(std::ostream& ostr, const value_array<T,V>& a)
    {
      mln_viter(mln::value::set<T>) v(a.vset());
      for_all(v)
	ostr << v << ':' << h(v) << ' ';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_VALUE_ARRAY_HH
