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

# include <mln/core/concept/accumulator.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic sum accumulator class.
     *
     * Parameter \c V is the type of values that we sum.  Parameter \c
     * S is the type to store the sum value; the default type of
     * \c S is \c V.
     */
    template <typename V, typename S = V>
    struct sum : public Accumulator< sum<V,S> >
    {
      typedef V value;

      sum();
      void take(const value& v);
      void init();

      operator S() const;
      S to_value() const;
      
    protected:

      S sum_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename V, typename S>
    sum<V,S>::sum()
    {
      init();
    }

    template <typename V, typename S>
    void sum<V,S>::take(const value& v)
    {
      sum_ += v;
    }

    template <typename V, typename S>
    void
    sum<V,S>::init()
    {
      sum_ = 0;
    }

    template <typename V, typename S>
    sum<V,S>::operator S() const
    {
      return to_value;
    }

    template <typename V, typename S>
    S
    sum<V,S>::to_value() const
    {
      return sum_ / count_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_SUM_HH
