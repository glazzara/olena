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
     * The parameter \c T is the type of values.
     */
    template <typename T>
    struct min_ : public mln::accu::internal::base_< T, min_<T> >
    {
      typedef T argument;
      typedef T result;

      min_();

      void init();
      void take_as_init(const argument& t);
      void take(const argument& t);
      void take(const min_<T>& other);

      T to_result() const;

    protected:

      T t_;
    };


    template <typename I> struct min_< util::pix<I> >;


    // FIXME: Doc!
    struct min : public Meta_Accumulator< min >
    {
      template <typename T>
      struct with
      {
	typedef min_<T> ret;
      };
    };






# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    min_<T>::min_()
    {
      init();
    }

    template <typename T>
    void
    min_<T>::init()
    {
      t_ = mln_max(T);
    }

    template <typename T>
    void min_<T>::take_as_init(const argument& t)
    {
      t_ = t;
    }

    template <typename T>
    void min_<T>::take(const argument& t)
    {
      if (t < t_)
	t_ = t;
    }

    template <typename T>
    void
    min_<T>::take(const min_<T>& other)
    {
      if (other.t_ < t_)
	t_ = other.t_;
    }

    template <typename T>
    T
    min_<T>::to_result() const
    {
      return t_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MIN_HH
