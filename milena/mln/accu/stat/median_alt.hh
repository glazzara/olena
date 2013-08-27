// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_ACCU_STAT_MEDIAN_ALT_HH
# define MLN_ACCU_STAT_MEDIAN_ALT_HH

/// \file
///
/// Define alternative generic median accumulator class.

# include <mln/accu/internal/base.hh>
# include <mln/accu/histo.hh>


namespace mln
{

  namespace accu
  {

    namespace stat
    {


      /// \brief Generic median_alt function based on histogram over a
      /// value set with type \c S.
      ///
      /// \ingroup modaccuvalues
      //
      template <typename S>
      struct median_alt : public mln::accu::internal::base< const mln_value(S)&, median_alt<S> >
      {
	typedef mln_value(S) argument;

	median_alt(const Value_Set<S>& s);

	/// Manipulators.
	/// \{
	void   take(const argument& t);
	void untake(const argument& t);
	void init();
	/// \}

	/// Get the value of the accumulator.
	const argument& to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:

	histo<S> h_;
	/// derived from h_
	const S& s_;

	unsigned sum_minus_, sum_plus_;

	/// the median index
	unsigned i_;
	/// the median argument
	argument t_;

	// Auxiliary methods
	void go_minus_();
	void go_plus_();
      };

      template <typename T>
      struct median_alt_on : public median_alt< value::set<T> >
      {
	median_alt_on()
	  : median_alt< value::set<T> >(value::set<T>::the())
	{
	}
      };

    } // end of mln::accu::stat


    namespace meta
    {

      namespace stat
      {

	/// Meta accumulator for median_alt.

	template <typename T>
	struct median_alt : public Meta_Accumulator< median_alt<T> >
	{
	  median_alt(const Value_Set<T>& s_) : s(s_) {}

	  struct with
	  {
	    typedef accu::stat::median_alt<T> ret;
	  };

	  Value_Set<T> s;
	};

      } // end of namespace mln::accu::meta::stat

    } // end of namespace mln::accu::meta


    template <typename T>
    stat::median_alt<T> unmeta(const meta::stat::median_alt<T>& m, T)
    {
      stat::median_alt<T> a(m.s);
      return a;
    }


# ifndef MLN_INCLUDE_ONLY

    namespace stat
    {

      template <typename S>
      inline
      median_alt<S>::median_alt(const Value_Set<S>& s)
	: h_(s),
	  s_(h_.vset())
      {
	init();
      }


      template <typename S>
      inline
      void
      median_alt<S>::take(const argument& t)
      {
	// update h_
	h_.take(t);

	// particular case:
	// current state was initialization
	if (h_[i_] == 0)
	  {
	    i_ = s_.index_of(t);
	    t_ = t;
	    return;
	  }

	// particular case:
	// the median does not change
	if (t == t_)
	  {
	    return;
	  }

	// general case:

	if (t < t_)
	  {
	    ++sum_minus_;
	    if (2 * sum_minus_ > h_.sum())
	      go_minus_();
	  }
	else
	  // t > t_
	  {
	    ++sum_plus_;
	    if (2 * sum_plus_ > h_.sum())
	      go_plus_();
	  }
      }


      template <typename S>
      inline
      void
      median_alt<S>::untake(const argument& t)
      {
	mln_precondition(h_(t) != 0);

	// update h_
	h_.untake(t);

	// particular case:
	// the only value has been removed
	if (h_.sum() == 0)
	  {
	    init();
	    return;
	  }

	// general case:
	if (t < t_)
	  {
	    --sum_minus_;
	    if (2 * sum_plus_ > h_.sum())
	      go_plus_();
	  }
	else if (t > t_)
	  {
	    --sum_plus_;
	    if (2 * sum_minus_ > h_.sum())
	      go_minus_();
	  }
	else
	  // t == t_
	  {
	    if (h_[i_] == 0)
	      {
		// go to the heaviest side
		if (sum_plus_ > sum_minus_)
		  go_plus_();
		else
		  go_minus_(); // default when both sides are balanced
	      }
	    else
	      {
		if (2 * sum_plus_ > h_.sum())
		  go_plus_();
		else if (2 * sum_minus_ > h_.sum())
		  go_minus_();
		// else no change
	      }
	  }
      }

      template <typename S>
      inline
      void
      median_alt<S>::init()
      {
	h_.init();
	sum_minus_ = 0;
	sum_plus_ = 0;
	i_ = (mln_max(argument) - mln_min(argument)) / 2;
	t_ = s_[i_];
      }

      template <typename S>
      inline
      const typename median_alt<S>::argument&
      median_alt<S>::to_result() const
      {
	return t_;
      }

      template <typename S>
      inline
      bool
      median_alt<S>::is_valid() const
      {
	return true;
      }

      template <typename S>
      inline
      void
      median_alt<S>::go_minus_()
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
      median_alt<S>::go_plus_()
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
      std::ostream& operator<<(std::ostream& ostr, const median_alt<S>& m)
      {
	return ostr << m.to_result();
      }

    } // end of namespace mln::accu::stat

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_MEDIAN_ALT_HH
