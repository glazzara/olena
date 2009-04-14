// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_STAT_VARIANCE_HH
# define MLN_ACCU_STAT_VARIANCE_HH

/// \file mln/accu/stat/variance.hh
///
/// Define an accumulator that computes the variance of a set of values.

# include <cmath>
# include <mln/accu/internal/base.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {

    namespace stat
    {

      /// Variance accumulator class.
      /*!
       * Parameter \c T is the type of values that we sum.  Parameter
       * \c S is the type to store the value sum and the sum of value
       * * value; the default type of \c S is the summation type
       * (property) of \c T.  Parameter \c R is the type of the mean
       * and variance values; the default type of \c R is \c S.
       */
      template <typename T,
		typename S = mln_sum(T),
		typename R = S>
      struct variance : public mln::accu::internal::base< R , variance<T,S,R> >
      {
	typedef T argument;
	typedef R result;

	variance();

	/// Manipulators.
	/// \{
	void init();
	void take_as_init(const argument& t);
	void take(const argument& t);
	void take(const variance<T,S,R>& other);

	void take(unsigned n_times, const argument& t); // Extra.
	/// \}

	/// Get the accumulator result (the variance value).
	R to_result() const;

	/// Get the variance value.
	R var() const;

	/// Get the standard deviation value.
	R standard_deviation() const;

	/// Get the mean value.
	R mean() const;

	/// Get the sum value.
	S sum() const;

	/// Get the number of items.
	unsigned n_items() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:

	unsigned n_;
	S sum_, sum2_;
      };



    template <typename I, typename S, typename R>
    struct variance< util::pix<I>, S,R >;



# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename S, typename R>
    inline
    variance<T,S,R>::variance()
    {
      init();
    }

    template <typename T, typename S, typename R>
    inline
    void
    variance<T,S,R>::init()
    {
      n_ = 0;
      sum_ = sum2_ = 0;
    }

    template <typename T, typename S, typename R>
    inline
    void
    variance<T,S,R>::take(const argument& t)
    {
      ++n_;
      sum_  += t;
      sum2_ += t * t;
    }

    template <typename T, typename S, typename R>
    inline
    void
    variance<T,S,R>::take(unsigned n_times, const argument& t)
    {
      if (n_times == 0u)
	return;
      n_ += n_times;
      sum_  += n_times * t;
      sum2_ += n_times * t * t;
    }

    template <typename T, typename S, typename R>
    inline
    void
    variance<T,S,R>::take_as_init(const argument& t)
    {
      n_ = 1;
      sum_  = t;
      sum2_ = t * t;
    }

    template <typename T, typename S, typename R>
    inline
    void
    variance<T,S,R>::take(const variance<T,S,R>& other)
    {
      n_    += other.n_;
      sum_  += other.sum_;
      sum2_ += other.sum2_;
    }

    template <typename T, typename S, typename R>
    inline
    R
    variance<T,S,R>::to_result() const
    {
      if (n_ == 0u)
	return 0; // Safety.
      S m_ = sum_ / n_;
      return sum2_ / n_ - m_ * m_;
    }

    template <typename T, typename S, typename R>
    inline
    R
    variance<T,S,R>::mean() const
    {
      if (n_ == 0u)
	return 0; // Safety.
      return sum_ / n_;
    }

    template <typename T, typename S, typename R>
    inline
    S
    variance<T,S,R>::sum() const
    {
      return sum_;
    }

    template <typename T, typename S, typename R>
    inline
    unsigned
    variance<T,S,R>::n_items() const
    {
      return n_;
    }

    template <typename T, typename S, typename R>
    inline
    R
    variance<T,S,R>::var() const
    {
      return to_result();
    }

    template <typename T, typename S, typename R>
    inline
    R
    variance<T,S,R>::standard_deviation() const
    {
      return std::sqrt(to_result());
    }

    template <typename T, typename S, typename R>
    inline
    bool
    variance<T,S,R>::is_valid() const
    {
      return n_ != 0;
    }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_VARIANCE_HH
