// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_VALUE_EQUIV_HH
# define MLN_VALUE_EQUIV_HH

/*! \file
 *
 * \brief The mln::equiv routine.
 */

# include <mln/core/concept/value.hh>


# define mln_value_equiv(V)  typename mln::value::internal::equiv_<V>::ret
# define mln_value_equiv_(V)          mln::value::internal::equiv_<V>::ret



namespace mln
{

  namespace value
  {

    // Fwd decl.
    namespace internal { template <typename T> struct equiv_; }



    /// Access to the equivalent value.
    template <typename V>
    typename internal::equiv_<V>::ret
    equiv(const mln::Value<V>& v);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      typedef char yes_;
      struct no_ { char dummy[2]; };

      template <typename T>
      struct make_
      {
	static T* ptr();
      };


      // Fwd decl.
      template <unsigned id, typename T>
      struct equiv_ret_;

      // Fwd decl.
      template <typename V>
      const typename internal::equiv_<V>::ret&
      run_equiv_(const V& v);


      template <typename V, typename T>
      inline
      const typename internal::equiv_<V>::ret&
      run_equiv_(const Value<V>* v, const T*)
      {
	return run_equiv_(exact(v)->to_equiv()); // Rec.
      }

      template <typename V>
      inline
      const V&
      run_equiv_(const void*, const V* v)
      {
	return *v; // Stop rec.
      }

      template <typename V>
      inline
      const typename internal::equiv_<V>::ret&
      run_equiv_(const V& v)
      {
	return run_equiv_(&v, &v);
      }


      template <typename T>
      struct equiv_ret_< 1, T > // Rec.
      {
	typedef typename T::equiv V;
	typedef typename equiv_<V>::ret ret;
      };

      template <typename T>
      struct equiv_ret_< 2, T > // Stop rec.
      {
	typedef T ret;
      };

      template <typename V>
      yes_ equiv_selector_(Value<V>*);

      no_  equiv_selector_(void*);

      template <typename T>
      struct equiv_
      {
	enum { id = sizeof(equiv_selector_(make_<T>::ptr())) };
	typedef typename equiv_ret_<id, T>::ret ret;

	inline
	static ret run(const T& t)
	{
	  return ret::run(t);
	}
      };

    } // end of namespace mln::value::internal



    template <typename V>
    inline
    typename internal::equiv_<V>::ret
    equiv(const mln::Value<V>& v)
    {
      return internal::run_equiv_(exact(v));
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

# include <mln/value/cast.hh>

#endif // ! MLN_VALUE_EQUIV_HH
