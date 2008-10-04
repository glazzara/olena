// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_RANK_HH
# define MLN_ACCU_RANK_HH

/*! \file mln/accu/rank.hh
 *
 * \brief Define an rank accumulator.
 */

# include <vector>
# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {


    /*! \brief Generic rank accumulator class.
     *
     * The parameter \c T is the type of values.
     */
    template <typename T>
    struct rank_ : public mln::accu::internal::base< T, rank_<T> >
    {
      typedef T argument;
      typedef T result;

      rank_(unsigned k, unsigned n);

      void init();
      void take_as_init(const argument& t);
      void take(const argument& t);
      void take(const rank_<T>& other);
      void sort();

      T to_result() const;

    protected:

      std::vector<T> elts_;
      bool is_sorted_;
      unsigned k_; // 0 <= k_ < n
      unsigned n_;
    };


    template <typename I> struct rank_< util::pix<I> >;


    /*!
     * \brief Meta accumulator for rank.
     */
    struct rank : public Meta_Accumulator< rank >
    {
      template <typename T>
      struct with
      {
	typedef rank_<T> ret;
      };
    };






# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    rank_<T>::rank_(unsigned k, unsigned n)
      : k_(k),
	n_(n),
	is_sorted_(false)
    {
      mln_assertion(k_ < n_);
      init();
    }

    template <typename T>
    inline
    void
    rank_<T>::init()
    {
      elts_.clear();
    }

    template <typename T>
    inline
    void rank_<T>::take_as_init(const argument& t)
    {
      elts_.push_back(t);
      is_sorted_ = false;
    }

    template <typename T>
    inline
    void rank_<T>::take(const argument& t)
    {
      elts_.push_back(t);
      is_sorted_ = false;
    }

    template <typename T>
    inline
    void
    rank_<T>::take(const rank_<T>& other)
    {
      elts_.insert(elts_.end(),
		   other.elts_.begin(),
		   other.elts_.end());
      is_sorted_ = false;
    }

    template <typename T>
    inline
    T
    rank_<T>::to_result() const
    {
      const_cast<rank_<T>&>(*this).sort();

      if (n_ == elts_.size())
	return elts_[k_];
      else
	// FIXME : This alternative is used to handle images edges.
	return elts_[(elts_.size() * k_) / n_];
    }

    template <typename T>
    inline
    void
    rank_<T>::sort()
    {
      if (! is_sorted_)
      {
	is_sorted_ = true;
	std::sort(elts_.begin(), elts_.end());
      }
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln

#include <mln/accu/rank_bool.hh>

#endif // ! MLN_ACCU_RANK_HH
