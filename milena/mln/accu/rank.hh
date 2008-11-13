// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file mln/accu/rank.hh
///
/// Define an rank accumulator.
///
/// \todo It should be renamed as rank_h since it relies on histogram
/// (thus low quantization).

# include <vector>
# include <mln/accu/internal/base.hh>
# include <mln/accu/histo.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {


    /// Generic rank accumulator class.
    /*!
     * The parameter \c T is the type of values.
     */
    template <typename T>
    struct rank : public mln::accu::internal::base< const T&, rank<T> >
    {
      typedef T argument;
      typedef mln::value::set<T> S;

      rank(unsigned k, unsigned n);

      /// Manipulators.
      /// \{
      void   init();
      void   take(const argument& t);
      void   take(const rank<T>& other);
      void untake(const argument& t);
      /// \}

      unsigned card() const { return h_.sum(); }

      /// Get the value of the accumulator.
      const T& to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

      /// Give the rank.
      unsigned k() const;

      /// Give the total number of elements.
      unsigned n() const;

    protected:

      unsigned k_; // 0 <= k_ < n
      unsigned n_;

      mutable accu::histo<T> h_;
      const S& s_; // derived from h_

      mutable unsigned sum_minus_, sum_plus_;

      mutable bool valid_;
      mutable unsigned i_; // the median index
      mutable argument t_;       // the median value

      // Auxiliary methods
      void update_() const;
      void go_minus_() const;
      void go_plus_() const;
    };


    template <typename I> struct rank< util::pix<I> >;


    namespace meta
    {

      /// Meta accumulator for rank.

      struct rank : public Meta_Accumulator< rank >
      {
	rank(unsigned k_, unsigned n_) : k(k_), n(n_) {}

	template <typename T>
	struct with
	{
	  typedef accu::rank<T> ret;
	};

	unsigned k;
	unsigned n;
      };

    } // end of namespace mln::accu::meta


    template <typename T>
    rank<T> unmeta(const meta::rank& m, T)
    {
      rank<T> a(m.k, m.n);
      return a;
    }




# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    rank<T>::rank(unsigned k, unsigned n)
      : k_(k),
	n_(n),
	h_(),
	s_(h_.vset())
    {
      mln_assertion(k_ < n_);
      init();
    }

    template <typename T>
    inline
    unsigned
    rank<T>::k() const
    {
      return k_;
    }

    template <typename T>
    inline
    unsigned
    rank<T>::n() const
    {
      return n_;
    }

    template <typename T>
    inline
    void rank<T>::take(const argument& t)
    {
      h_.take(t);

      if (t < t_)
	++sum_minus_;
      else if (t > t_)
	++sum_plus_;

      if (valid_)
	valid_ = false;
    }

    template <typename T>
    inline
    void
    rank<T>::take(const rank<T>& other)
    {
      // h_
      h_.take(other.h_);

      // sum_minus_
      for (unsigned i = 0; i < i_; ++i)
	sum_minus_ += other.h_[i];

      // sum_plus_
      for (unsigned i = i_ + 1; i < h_.nvalues(); ++i)
	sum_plus_ += other.h_[i];

      if (valid_)
	valid_ = false;
    }


    template <typename T>
    inline
    void
    rank<T>::untake(const argument& t)
    {
      mln_precondition(h_(t) != 0);
      h_.untake(t);

      if (t < t_)
	--sum_minus_;
      else if (t > t_)
	--sum_plus_;

      if (valid_)
	valid_ = false;
    }

    template <typename T>
    inline
    void
    rank<T>::update_() const
    {
      valid_ = true;

      if (h_.sum() == 0)
	return;

      if (sum_minus_ > k_)
	go_minus_();
      else
	if ((sum_minus_ + h_[i_]) < k_)
	  go_plus_();
      else
	if (h_[i_] == 0)
	  {
	    // go to the heaviest side
	    if (sum_plus_ > sum_minus_)
	      go_plus_();
	    else
	      go_minus_(); // default when both sides are balanced
	  }
    }

    template <typename T>
    inline
    void
    rank<T>::go_minus_() const
    {
      do
	{
	  sum_plus_ += h_[i_];
	  do
	    --i_;
	  while (h_[i_] == 0);
	  sum_minus_ -= h_[i_];
	}
      while (sum_minus_ > k_);
      t_ = s_[i_];
    }

    template <typename T>
    inline
    void
    rank<T>::go_plus_() const
    {
      do
	{
	  sum_minus_ += h_[i_];
	  do
	    ++i_;
	  while (h_[i_] == 0);
	  sum_plus_ -= h_[i_];
	}
      while ((sum_minus_ + h_[i_]) < k_);
      t_ = s_[i_];
    }

    template <typename T>
    inline
    void
    rank<T>::init()
    {
      h_.init();
      sum_minus_ = 0;
      sum_plus_ = 0;
      i_ = (s_.index_of(mln_max(argument))
	    - s_.index_of(mln_min(argument))) / 2;
      t_ = s_[i_];
      valid_ = true;
    }

    template <typename T>
    inline
    const T&
    rank<T>::to_result() const
    {
      if (! valid_)
	update_();
      return t_;
    }

    template <typename T>
    inline
    bool
    rank<T>::is_valid() const
    {
      return valid_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln

#include <mln/accu/rank_bool.hh>

#endif // ! MLN_ACCU_RANK_HH
