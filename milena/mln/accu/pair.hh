// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_PAIR_HH
# define MLN_ACCU_PAIR_HH

/*! \file mln/accu/pair.hh
 *
 * \brief Define a pair of accumulators.
 */

# include <utility>

# include <mln/core/concept/meta_accumulator.hh>

# include <mln/accu/internal/base.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/unqualif.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic pair of accumulators.
     *
     * The parameter \c V is the type of values.
     *
     * \todo Check that, when V is not provided, A1 and A2 have the same value.
     */
    template <typename A1, typename A2, typename V = mln_argument(A1)>
    struct pair_ : public mln::accu::internal::base_< std::pair< mlc_unqualif(mln_result(A1)) , mlc_unqualif(mln_result(A2)) > , pair_<A1,A2,V> >
    {
      typedef V argument;

      typedef mlc_unqualif(mln_result(A1))  result_1;
      typedef mlc_unqualif(mln_result(A2))  result_2;
      typedef std::pair<result_1, result_2> result;

      pair_();
      pair_(const A1& a1, const A2& a2);

      void init();
      void take_as_init(const argument& x);
      void take(const argument& x);
      void take(const pair_<A1,A2,V>& other);

      result to_result() const;
      void get_result(result_1& r1, result_2& r2) const;

    protected:

      A1 a1_;
      A2 a2_;
    };



    // FIXME: Doc!
    template <typename A1, typename A2>
    struct pair : public Meta_Accumulator< pair<A1,A2> >
    {
      template <typename V>
      struct with
      {
	typedef mln_accu_with(A1, V) A1_V;
	typedef mln_accu_with(A2, V) A2_V;
	typedef pair_<A1_V, A2_V, V> ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename A1, typename A2, typename V>
    pair_<A1,A2,V>::pair_()
    {
      init();
    }

    template <typename A1, typename A2, typename V>
    void
    pair_<A1,A2,V>::init()
    {
      a1_.init();
      a2_.init();
    }

    template <typename A1, typename A2, typename V>
    void
    pair_<A1,A2,V>::take_as_init(const argument& x)
    {
      a1_.take_as_init(x);
      a2_.take_as_init(x);
    }

    template <typename A1, typename A2, typename V>
    void
    pair_<A1,A2,V>::take(const argument& x)
    {
      a1_.take(x);
      a2_.take(x);
    }

    template <typename A1, typename A2, typename V>
    void
    pair_<A1,A2,V>::take(const pair_<A1,A2,V>& other)
    {
      a1_.take(other.a1_);
      a2_.take(other.a2_);
    }

    template <typename A1, typename A2, typename V>
    typename pair_<A1,A2,V>::result
    pair_<A1,A2,V>::to_result() const
    {
      result tmp(a1_.to_result(), a2_.to_result());
      return tmp;
    }

    template <typename A1, typename A2, typename V>
    void
    pair_<A1,A2,V>::get_result(result_1& r1,
			    result_2& r2) const
    {
      r1 = a1_.to_result();
      r2 = a2_.to_result();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_PAIR_HH
