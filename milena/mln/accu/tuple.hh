// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_TUPLE_HH
# define MLN_ACCU_TUPLE_HH

/*! \file mln/accu/tuple.hh
 *
 * \brief Define a tuple of accumulators.
 */

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

# define RESULT_ACCU(z, n, data) BOOST_PP_COMMA_IF(n) typename internal::tuple_helper_<T ## n>::result
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
      template <typename T> struct tuple_helper_;
      template <unsigned n, typename T> struct tuple_helper;
    }

    /*! \brief Generic tuple of accumulators.
     *
     * The parameter \c T is the type of values.
     *
     */
    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(10, typename T, boost::tuples::null_type)>
    struct tuple_
      : public mln::accu::internal::base< boost::tuple< BOOST_PP_REPEAT(10, RESULT_ACCU, Le Ricard ya que ca de vrai !) > , tuple_<A, n, BOOST_PP_ENUM_PARAMS(10, T)> >
    {
      typedef A argument;

      typedef boost::tuple< BOOST_PP_REPEAT(10, RESULT_ACCU, Le Ricard ya que ca de vrai !)> result;
      typedef boost::tuple< BOOST_PP_ENUM_PARAMS(10, T)> intern;
      typedef tuple_<A, n, BOOST_PP_ENUM_PARAMS(10, T)> self;

      tuple_();

      void init();
      void take_as_init(const argument& t);
      void take(const argument& t);
      void take(const tuple_<A, n, BOOST_PP_ENUM_PARAMS(10, T)>& other);

      result to_result() const;

    protected:

      intern a_;
    };

    /*!
     * \brief Meta accumulator for tuple.
     */
    template <unsigned n, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(10, typename T, boost::tuples::null_type)>
    struct tuple : public Meta_Accumulator< tuple<n, BOOST_PP_ENUM_PARAMS(10, T)> >
    {
      template <typename A>
      struct with
      {
# include BOOST_PP_LOCAL_ITERATE()

	typedef tuple_<A, n, BOOST_PP_ENUM_PARAMS(10, AT)> ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename T>
      struct tuple_helper_
      {
	typedef typename T::result result;
      };

      template <>
      struct tuple_helper_<boost::tuples::null_type>
      {
	typedef boost::tuples::null_type result;
      };

      template <unsigned n, typename T>
      struct tuple_helper
      {
	static void init(typename T::intern& a)
	{
	  boost::get<n - 1>(a).init();
	  tuple_helper<n - 1, T>::init(a);
	}

	static void take_as_init(typename T::intern& a, const typename T::argument& argument)
	{
	  boost::get<n - 1>(a).take_as_init(argument);
	  tuple_helper<n - 1, T>::take_as_init(a, argument);
	}

	static void take(typename T::intern& a, const typename T::argument& argument)
	{
	  boost::get<n - 1>(a).take(argument);
	  tuple_helper<n - 1, T>::take(a, argument);
	}

	static void take(typename T::intern& a, const typename T::intern& other)
	{
	  boost::get<n - 1>(a).take(boost::get<n - 1>(other));
	  tuple_helper<n - 1, T>::take(a, other);
	}

	static void to_result(const typename T::intern& a, typename T::result& res)
	{
	  boost::get<n - 1>(res) = boost::get<n - 1>(a).to_result();
	  tuple_helper<n - 1, T>::to_result(a, res);
	}
      };

      template <typename T>
      struct tuple_helper<0, T>
      {
	static void init(typename T::intern&) {}
	static void take_as_init(typename T::intern&, const typename T::argument&) {}
	static void take(typename T::intern&, const typename T::argument) {}
	static void take(typename T::intern&, const typename T::intern&) {}
	static void to_result(const typename T::intern&, typename T::result&) {}
      };
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    tuple_<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::tuple_()
    {
      init();
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    void
    tuple_<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::init()
    {
      internal::tuple_helper<n, self>::init(this->a_);
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    void
    tuple_<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::take_as_init(const argument& t)
    {
      internal::tuple_helper<n, self>::take_as_init(this->a_, t);
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    void
    tuple_<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::take(const argument& t)
    {
      internal::tuple_helper<n, self>::take(this->a_, t);
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    void
    tuple_<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::take(const tuple_<A,n,BOOST_PP_ENUM_PARAMS(10,T) >& other)
    {
      internal::tuple_helper<n, self>::take(this->a_, other.a_);
    }

    template <typename A, unsigned n, BOOST_PP_ENUM_PARAMS(10, typename T)>
    inline
    typename tuple_<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::result
    tuple_<A,n,BOOST_PP_ENUM_PARAMS(10,T) >::to_result() const
    {
      result tmp;
      internal::tuple_helper<n, self>::to_result(this->a_, tmp);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_TUPLE_HH
