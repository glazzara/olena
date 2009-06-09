// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_TUPLE_HH
# define MLN_ACCU_TUPLE_HH

/// \file
///
/// Define a tuple of accumulators.


# include <utility>

# include <mln/core/concept/meta_accumulator.hh>

# include <mln/accu/internal/base.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/unqualif.hh>

# include "boost/tuple/tuple.hpp"
# include "boost/preprocessor/iteration/local.hpp"
# include "boost/preprocessor/repetition/repeat.hpp"
# include "boost/preprocessor/repetition/enum_params.hpp"
# include "boost/preprocessor/repetition/enum_params_with_a_default.hpp"

# define RESULT_ACCU(z, n, data) BOOST_PP_COMMA_IF(n) typename internal::tuplehelper_<T ## n>::result
# define ARG(z, n, data) BOOST_PP_COMMA_IF(n) const T ## n& p ## n = T ## n()
# define BOOST_PP_LOCAL_MACRO(n) typedef mln_accu_with(T ## n, A) AT ## n;
# define BOOST_PP_LOCAL_LIMITS (0, 9)

namespace mln
{

  namespace accu
  {

    namespace internal
    {
      // Fwd decl.
      template <typename T> struct tuplehelper_;
      template <unsigned n, typename T> struct tuplehelper;
    }

    /// \brief Generic tuple of accumulators.
    /*!
     * The parameter \c T is the type of values.
     *
     * \ingroup modaccumulti
     */
    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(10, typename T, boost::tuples::null_type)>
    struct tuple
      : public mln::accu::internal::base< boost::tuple< BOOST_PP_REPEAT(10, RESULT_ACCU, Le Ricard ya que ca de vrai !) >, tuple<A, n, BOOST_PP_ENUM_PARAMS(10, T)> >
    {
      typedef A argument;

      typedef boost::tuple< BOOST_PP_REPEAT(10, RESULT_ACCU, Le Ricard ya que ca de vrai !)> res;
      typedef boost::tuple< BOOST_PP_ENUM_PARAMS(10, T)> intern;
      typedef tuple<A, n, BOOST_PP_ENUM_PARAMS(10, T)> self;

      tuple();

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const argument& t);
      void take(const argument& t);
      void take(const tuple<A, n, BOOST_PP_ENUM_PARAMS(10, T)>& other);
      /// \}

      /// Get the value of the accumulator.
      res to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      intern a_;
    };

    namespace meta
    {

      ///Meta accumulator for tuple.

      template <unsigned n, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(10, typename T, boost::tuples::null_type)>
      struct tuple : public Meta_Accumulator< tuple<n, BOOST_PP_ENUM_PARAMS(10, T)> >
      {
	template <typename A>
	struct with
	{
# include BOOST_PP_LOCAL_ITERATE()

	  typedef accu::tuple<A, n, BOOST_PP_ENUM_PARAMS(10, AT)> ret;
	};
      };

    }


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename T>
      struct tuplehelper_
      {
	typedef typename T::result result;
      };

      template <>
      struct tuplehelper_<boost::tuples::null_type>
      {
	typedef boost::tuples::null_type result;
      };

      template <unsigned n, typename T>
      struct tuplehelper
      {
	static void init(typename T::intern& a)
	{
	  boost::get<n - 1>(a).init();
	  tuplehelper<n - 1, T>::init(a);
	}

	static void take_as_init_(typename T::intern& a, const typename T::argument& argument)
	{
	  boost::get<n - 1>(a).take_as_init_(argument);
	  tuplehelper<n - 1, T>::take_as_init_(a, argument);
	}

	static void take(typename T::intern& a, const typename T::argument& argument)
	{
	  boost::get<n - 1>(a).take(argument);
	  tuplehelper<n - 1, T>::take(a, argument);
	}

	static void take(typename T::intern& a, const typename T::intern& other)
	{
	  boost::get<n - 1>(a).take(boost::get<n - 1>(other));
	  tuplehelper<n - 1, T>::take(a, other);
	}

	static void to_result(const typename T::intern& a, typename T::result& res)
	{
	  boost::get<n - 1>(res) = boost::get<n - 1>(a).to_result();
	  tuplehelper<n - 1, T>::to_result(a, res);
	}
      };

      template <typename T>
      struct tuplehelper<0, T>
      {
	static void init(typename T::intern&) {}
	static void take_as_init_(typename T::intern&, const typename T::argument&) {}
	static void take(typename T::intern&, const typename T::argument) {}
	static void take(typename T::intern&, const typename T::intern&) {}
	static void to_result(const typename T::intern&, typename T::result&) {}
      };
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::tuple()
    {
      init();
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    void
    tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::init()
    {
      internal::tuplehelper<n, self>::init(this->a_);
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    void
    tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::take_as_init_(const argument& t)
    {
      internal::tuplehelper<n, self>::take_as_init_(this->a_, t);
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    void
    tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::take(const argument& t)
    {
      internal::tuplehelper<n, self>::take(this->a_, t);
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    void
    tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::take(const tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >& other)
    {
      internal::tuplehelper<n, self>::take(this->a_, other.a_);
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    typename tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::res
    tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::to_result() const
    {
      res tmp;
      internal::tuplehelper<n, self>::to_result(this->a_, tmp);
      return tmp;
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    bool
    tuple<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_TUPLE_HH
