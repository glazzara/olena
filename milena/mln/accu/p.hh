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

#ifndef MLN_ACCU_P_HH
# define MLN_ACCU_P_HH

/*! \file mln/accu/p.hh
 *
 * \brief Define an accumulator that computes a min and a max.
 */

# include <mln/core/concept/meta_accumulator.hh>

# include <mln/accu/internal/base.hh>
# include <mln/metal/is_a.hh>


namespace mln
{

  namespace accu
  {


    /*!
     * \brief Generic p of accumulators.
     *
     * The parameter \c V is the type of values.
     */
    template <typename A>
    struct p_ : public mln::accu::internal::base< mln_result(A) , p_<A> >
    {
      typedef mln_argument(A)  argument;
      typedef mln_result(A) result;


      p_();
      p_(const A& a);

      void init();
      void take_as_init(const argument& t);
      void take(const argument& t);
      void take(const p_<A>& other);

      result to_result() const;

    protected:
      A a_;
    };


    /*!
     * \brief Meta accumulator for p.
     */
    template <typename mA>
    struct p : public Meta_Accumulator< p<mA> >
    {
      template <typename V>
      struct with
      {
	typedef mln_accu_with(mA, mln_psite(V)) ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename A>
    inline
    p_<A>::p_()
    {
      init();
    }

    template <typename A>
    inline
    p_<A>::p_(const A& a)
      : a_(a)
    {
      init();
    }

    template <typename A>
    inline
    void
    p_<A>::init()
    {
      a_.init();
    }

    template <typename A>
    inline
    void
    p_<A>::take_as_init(const argument& t)
    {
      a_.take_as_init(t.p()); // FIXME: Generalize with "psite(t)".
    }

    template <typename A>
    inline
    void
    p_<A>::take(const argument& t)
    {
      a_.take(t.p());
    }

    template <typename A>
    inline
    void
    p_<A>::take(const p_<A>& other)
    {
      a_.take(other.a_);
    }

    template <typename A>
    inline
    typename p_<A>::result
    p_<A>::to_result() const
    {
      return a_.to_result();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_P_HH
