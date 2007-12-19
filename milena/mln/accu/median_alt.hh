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

#ifndef MLN_ACCU_MEDIAN_ALT_HH
# define MLN_ACCU_MEDIAN_ALT_HH

/*! \file mln/accu/median_alt.hh
 *
 * \brief Define alternative generic median accumulator class.
 */

# include <mln/accu/internal/base.hh>
# include <mln/accu/histo.hh>


namespace mln
{

  namespace accu
  {


    /*! \brief Generic median_alt function based on histogram over a
     * value set with type \c S.
     */
    template <typename S>
    struct median_alt :  : public mln::accu::internal::base_< mln_value(S), median_alt<S> >
    {
      typedef mln_value(S) argument;

      median_alt(const Value_Set<S>& s);

      void   take(const argument& t);
      void untake(const argument& t);
      void init();

      argument to_result() const;

      // FIXME: remove
      void debug__() const
      {
	std::cout << "  i = " << i_
		  << "  t = " << t_
		  << "  s = " << sum_minus_ << " ; " << h_[i_] << " ; " << sum_plus_ << " = " << h_.sum()
		  << std::endl;
      }

    protected:

      histo_on_set<S> h_;
      const S& s_; // derived from h_

      std::size_t sum_minus_, sum_plus_;

      std::size_t i_; // the median index
      argument t_;       // the median argument

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


# ifndef MLN_INCLUDE_ONLY


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
	  // std::cout << "init!" << std::endl;
	  i_ = s_.index_of(t);
	  t_ = t;
	  return;
	}

      // particular case:
      // the median does not change
      if (t == t_)
	{
	  // std::cout << "no change!" << std::endl;
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
    typename median_alt<S>::argument
    median_alt<S>::to_result() const
    {
      return t_;
    }

    template <typename S>
    inline
    std::ostream& operator<<(std::ostream& ostr, const median_alt<S>& m)
    {
      m.debug__();
      return ostr << m.to_result();
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MEDIAN_ALT_HH
