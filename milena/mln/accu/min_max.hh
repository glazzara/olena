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

#ifndef MLN_ACCU_MIN_MAX_HH
# define MLN_ACCU_MIN_MAX_HH

/*! \file mln/accu/min_max.hh
 *
 * \brief Define an accumulator that computes a min and a max.
 */

# include <utility>

# include <mln/accu/min.hh>
# include <mln/accu/max.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic min and max accumulator class.
     *
     * The parameter \c V is the type of values.
     */
    template <typename V>
    struct min_max : public Accumulator< min_max<V> >
    {
      typedef V value;

      min_max();

      void take(const value& v);
      void take(const min_max<V>& other);
      void init();

      V min() const;
      V max() const;

      std::pair<V,V> to_value() const;
      void values(V& min, V& max) const;

    protected:

      accu::min<V> min_;
      accu::max<V> max_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename V>
    min_max<V>::min_max()
    {
      init();
    }

    template <typename V>
    void
    min_max<V>::init()
    {
      min_.init();
      max_.init();
    }

    template <typename V>
    void min_max<V>::take(const value& v)
    {
      min_.take(v);
      max_.take(v);
    }
    
    template <typename V>
    void
    min_max<V>::take(const min_max<V>& other)
    {
      min_.take(other.min_);
      max_.take(other.max_);
    }

    template <typename V>
    V
    min_max<V>::min() const
    {
      return min_.to_value();
    }

    template <typename V>
    V
    min_max<V>::max() const
    {
      return max_.to_value();
    }

    template <typename V>
    std::pair<V,V>
    min_max<V>::to_value() const
    {
      std::pair<V,V> tmp(min_.to_value(),
			 max_.to_value());
      return tmp;
    }

    template <typename V>
    void
    min_max<V>::values(V& min, V& max) const
    {
      min = min_.to_value();
      max = max_.to_value();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MIN_MAX_HH
