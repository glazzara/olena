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

#ifndef MLN_ACCU_COUNTER_HH
# define MLN_ACCU_COUNTER_HH

/*! \file mln/accu/counter.hh
 *
 * \brief Define an accumulator that counts.
 */

# include <mln/core/concept/accumulator.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic counter accumulator class.
     */
    template <typename V>
    struct counter : public Accumulator< counter<V> >
    {
      typedef V value;

      counter();
      void take(const value&);
      void init();

      operator std::size_t() const;
      std::size_t to_value() const;
      
    protected:

      std::size_t count_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename V>
    counter<V>::counter()
    {
      init();
    }

    template <typename V>
    void
    counter<V>::take(const value&)
    {
      ++count_;
    }

    template <typename V>
    void
    counter<V>::init()
    {
      count_ = 0;
    }

    template <typename V>
    counter<V>::operator std::size_t() const
    {
      return to_value();
    }

    template <typename V>
    std::size_t
    counter<V>::to_value() const
    {
      return count_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_COUNTER_HH
