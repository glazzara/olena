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

#ifndef MLN_ACCU_MIN_HH
# define MLN_ACCU_MIN_HH

/*! \file mln/accu/min.hh
 *
 * \brief Define an accumulator that computes a min.
 */

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/value/props.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic min accumulator class.
     *
     * The parameter \c V is the type of values.
     */
    template <typename V>
    struct min_ : public mln::accu::internal::base_< V, min_<V> >
    {
      typedef V argument;
      typedef V result;

      min_();

      void init();
      void take_as_init(const argument& x);
      void take(const argument& x);
      void take(const min_<V>& other);

      V to_result() const;

    protected:

      V x_;
    };


    template <typename I> struct min_< util::pix<I> >;


    // FIXME: Doc!
    struct min : public Meta_Accumulator< min >
    {
      template <typename V>
      struct with
      {
	typedef min_<V> ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename V>
    min_<V>::min_()
    {
      init();
    }

    template <typename V>
    void
    min_<V>::init()
    {
      x_ = mln_max(V);
    }

    template <typename V>
    void min_<V>::take_as_init(const argument& x)
    {
      x_ = x;
    }

    template <typename V>
    void min_<V>::take(const argument& x)
    {
      if (x < x_)
	x_ = x;
    }

    template <typename V>
    void
    min_<V>::take(const min_<V>& other)
    {
      if (other.x_ < x_)
	x_ = other.x_;
    }

    template <typename V>
    V
    min_<V>::to_result() const
    {
      return x_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MIN_HH
