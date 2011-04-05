// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_ACCU_STAT_MEDIAN_H_HH
# define MLN_ACCU_STAT_MEDIAN_H_HH

/// \file
///
/// Define a generic median accumulator class.

# include <mln/accu/internal/base.hh>
# include <mln/accu/histo.hh>
# include <mln/value/set.hh>


namespace mln
{

  namespace accu
  {

    namespace stat
    {

      // Forward declaration.
      template <typename V>
      struct median_h;

    } // end of namespace mln::accu::stat

    namespace meta
    {

      namespace stat
      {

	/// Meta accumulator for median_h.
	struct median_h : public Meta_Accumulator< median_h >
	{
	  template <typename V>
	  struct with
	  {
	    typedef accu::stat::median_h<V> ret;
	  };
	};

      } // end of namespace mln::accu::meta::stat

    } // end of namespace mln::accu::meta

    namespace stat
    {

      /// \brief Generic median function based on histogram over a value
      /// set with type \c V.
      ///
      /// \ingroup modaccuvalues
      template <typename V>
      struct median_h : public mln::accu::internal::base< const V&, median_h<V> >
      {
	typedef V argument;

	median_h();
	median_h& operator=(const median_h& rhs);

	/// Manipulators.
	/// \{
	void init();
	void   take(const argument& t);
	void   take(const median_h<V>& other);
	void untake(const argument& t);
	/// \}

	unsigned card() const { return h_.sum(); }

	/// Get the value of the accumulator.
	const argument& to_result() const;

	const accu::histo<V>& histo() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:

	mutable accu::histo<V> h_;
	const value::set<V>& s_;	// derived from h_

	mutable unsigned sum_minus_, sum_plus_;

	mutable bool valid_;
	mutable unsigned i_;	// the median_h index
	mutable argument t_;	// the median_h value

	// Auxiliary methods
	void update_() const;
	void go_minus_() const;
	void go_plus_() const;
      };

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      median_h<V>::median_h()
	: h_(),
	  s_(h_.vset())
      {
	init();
      }

      template <typename V>
      inline
      median_h<V>&
      median_h<V>::operator=(const median_h<V>& rhs)
      {
	h_ = rhs.h_;
	sum_minus_ = rhs.sum_minus_;
	sum_plus_ = rhs.sum_plus_;
	valid_ = rhs.valid_;
	i_ = rhs.i_;
	t_ = rhs.t_;

	return *this;
      }

      template <typename V>
      inline
      void
      median_h<V>::take(const argument& t)
      {
	h_.take(t);

	if (t < t_)
	  ++sum_minus_;
	else if (t > t_)
	  ++sum_plus_;

	if (valid_)
	  valid_ = false;
      }

      template <typename V>
      inline
      void
      median_h<V>::take(const median_h<V>& other)
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

      template <typename V>
      inline
      void
      median_h<V>::untake(const argument& t)
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

      template <typename V>
      inline
      void
      median_h<V>::update_() const
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

      template <typename V>
      inline
      void
      median_h<V>::go_minus_() const
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

      template <typename V>
      inline
      void
      median_h<V>::go_plus_() const
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

      template <typename V>
      inline
      void
      median_h<V>::init()
      {
	h_.init();
	sum_minus_ = 0;
	sum_plus_ = 0;
	i_ = (s_.index_of(mln_max(argument))
	      - s_.index_of(mln_min(argument))) / 2;
	t_ = s_[i_];
	valid_ = true;
      }

      template <typename V>
      inline
      const typename median_h<V>::argument&
      median_h<V>::to_result() const
      {
	if (! valid_)
	  update_();
	return t_;
      }

      template <typename V>
      inline
      const accu::histo<V>&
      median_h<V>::histo() const
      {
	return h_;
      }

      template <typename V>
      inline
      bool
      median_h<V>::is_valid() const
      {
	return true;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln

#endif // ! MLN_ACCU_STAT_MEDIAN_H_HH
