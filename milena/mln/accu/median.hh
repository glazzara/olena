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

#ifndef MLN_ACCU_MEDIAN_HH
# define MLN_ACCU_MEDIAN_HH

/*! \file mln/accu/median.hh
 *
 * \brief Define a generic median accumulator class.
 */

# include <mln/accu/internal/base.hh>
# include <mln/accu/histo.hh>


namespace mln
{

  namespace accu
  {


    /*! \brief Generic median function based on histogram over a value
     * set with type \c S.
     */
    template <typename S>
    struct median : public mln::accu::internal::base_< mln_value(S), median<S> >
    { // FIXME: Should median be named 'median_' like other accumulators ?
      typedef mln_value(S) argument;
      typedef argument result;

      median(const Value_Set<S>& s);
      median();

      void init();
      void   take(const argument& t);
      void   take(const median<S>& other);
      void untake(const argument& t);

      unsigned card() const { return h_.sum(); }

      argument to_result() const;

      const accu::histo<S>& histo() const;

    protected:

      mutable accu::histo<S> h_;
      const S& s_; // derived from h_

      mutable std::size_t sum_minus_, sum_plus_;

      mutable bool valid_;
      mutable std::size_t i_; // the median index
      mutable argument t_;       // the median value

      // Auxiliary methods
      void update_() const;
      void go_minus_() const;
      void go_plus_() const;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    inline
    median<S>::median(const Value_Set<S>& s)
      : h_(s),
	s_(h_.vset())
    {
      init();
    }

    template <typename S>
    inline
    median<S>::median()
      : h_(),
	s_(h_.vset())
    {
      init();
    }

    template <typename S>
    inline
    void
    median<S>::take(const argument& t)
    {
      h_.take(t);

      if (t < t_)
	++sum_minus_;
      else if (t > t_)
	++sum_plus_;

      if (valid_)
	valid_ = false;
    }

    template <typename S>
    inline
    void
    median<S>::take(const median<S>& other)
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

    template <typename S>
    inline
    void
    median<S>::untake(const argument& t)
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

    template <typename S>
    inline
    void
    median<S>::update_() const
    {
      valid_ = true;

      if (h_.sum() == 0)
	return;

      if (2 * sum_minus_ > h_.sum())
	go_minus_();
      else
	if (2 * sum_plus_ > h_.sum())
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

    template <typename S>
    inline
    void
    median<S>::go_minus_() const
    {
      do
	{
	  sum_plus_ += h_[i_];
	  do
	    --i_;
	  while (h_[i_] == 0);
	  sum_minus_ -= h_[i_];
	}
      while (2 * sum_minus_ > h_.sum());
      t_ = s_[i_];
    }

    template <typename S>
    inline
    void
    median<S>::go_plus_() const
    {
      do
	{
	  sum_minus_ += h_[i_];
	  do
	    ++i_;
	  while (h_[i_] == 0);
	  sum_plus_ -= h_[i_];
	}
      while (2 * sum_plus_ > h_.sum());
      t_ = s_[i_];
    }

    template <typename S>
    inline
    void
    median<S>::init()
    {
      h_.init();
      sum_minus_ = 0;
      sum_plus_ = 0;
      i_ = (s_.index_of(mln_max(argument))
	    - s_.index_of(mln_min(argument))) / 2;
      t_ = s_[i_];
      valid_ = true;
    }

    template <typename S>
    inline
    typename median<S>::argument
    median<S>::to_result() const
    {
      if (! valid_)
	update_();
      return t_;
    }

    template <typename S>
    inline
    const accu::histo<S>&
    median<S>::histo() const
    {
      return h_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MEDIAN_HH
