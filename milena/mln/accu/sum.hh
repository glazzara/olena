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

#ifndef MLN_ACCU_SUM_HH
# define MLN_ACCU_SUM_HH

/*! \file mln/accu/sum.hh
 *
 * \brief Define an accumulator that computes a sum.
 */

# include <mln/core/concept/meta_accumulator.hh>

# include <mln/accu/internal/base.hh>
# include <mln/value/props.hh>
# include <mln/util/pix.hh>
# include <mln/literal/zero.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic sum accumulator class.
     *
     * Parameter \c T is the type of values that we sum.  Parameter \c
     * S is the type to store the value sum; the default type of
     * \c S is the summation type (property) of \c T.
     */
    template <typename T, typename S = mln_sum(T)>
    struct sum_ : public mln::accu::internal::base_< S, sum_<T,S> >
    {
      typedef T argument;
      typedef S result;

      sum_();

      void init();
      void take(const argument& t);
      void take(const sum_<T,S>& other);

      S to_result() const;

    protected:

      S s_;
    };


    template <typename I, typename S>
    struct sum_< util::pix<I>, S >;


    // FIXME: Doc!
    struct sum : public Meta_Accumulator< sum >
    {
      template <typename T, typename S = mln_sum(T)>
      struct with
      {
	typedef sum_<T, S> ret;
      };
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename S>
    sum_<T,S>::sum_()
    {
      init();
    }

    template <typename T, typename S>
    void
    sum_<T,S>::init()
    {
      s_ = literal::zero;
    }

    template <typename T, typename S>
    void sum_<T,S>::take(const argument& t)
    {
      s_ += t;
    }

    template <typename T, typename S>
    void
    sum_<T,S>::take(const sum_<T,S>& other)
    {
      s_ += other.s_;
    }

    template <typename T, typename S>
    S
    sum_<T,S>::to_result() const
    {
      return s_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_SUM_HH
