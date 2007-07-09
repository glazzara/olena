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

#ifndef MLN_VALUE_MEDIAN_HH
# define MLN_VALUE_MEDIAN_HH

/*! \file mln/value/median.hh
 *
 * \brief Define FIXME
 */

# include <mln/value/histo.hh>


namespace mln
{

  namespace value
  {


    /*! Generic median function based on histogram over a value set
     * with type \c S.
     */
    template <typename S>
    struct median
    {
      typedef mln_value(S) value;

      median(const Value_Set<S>& s);

      void   take(const value& v);
      void untake(const value& v);
      void clear();

      operator value() const;
      value to_value() const;

      // FIXME: remove
      void debug__() const
      {
	std::cout << "  i = " << i_
		  << "  v = " << v_
		  << "  s = " << sum_minus_ << " ; " << h_[i_] << " ; " << sum_plus_ << " = " << h_.sum()
		  << std::endl;
      }

    protected:

      histo_on_set<S> h_;
      const S& s_; // derived from h_

      std::size_t sum_minus_, sum_plus_;

      std::size_t i_; // the median index
      value v_;       // the median value

      // Auxiliary methods
      void go_minus_();
      void go_plus_();
    };



# ifndef MLN_INCLUDE_ONLY


    template <typename S>
    median<S>::median(const Value_Set<S>& s)
      : h_(s),
	s_(h_.vset())
    {
      clear();
    }


    template <typename S>
    void
    median<S>::take(const value& v)
    {
      // update h_
      h_.take(v);

      // particular case:
      // current state was initialization
      if (h_[i_] == 0)
	{
	  // std::cout << "init!" << std::endl;
	  i_ = s_.index_of(v);
	  v_ = v;
	  return;
	}

      // particular case:
      // the median does not change
      if (v == v_)
	{
	  // std::cout << "no change!" << std::endl;
	  return;
	}

      // general case:

      if (v < v_)
	{
	  ++sum_minus_;
	  if (2 * sum_minus_ > h_.sum())
	    go_minus_();
	}
      else
	// v > v_
	{
	  ++sum_plus_;
	  if (2 * sum_plus_ > h_.sum())
	    go_plus_();
	}
    }

    
    template <typename S>
    void
    median<S>::untake(const value& v)
    {
      mln_precondition(h_(v) != 0);

      // update h_
      h_.untake(v);

      // particular case:
      // the only value has been removed
      if (h_.sum() == 0)
	{
	  clear();
	  return;
	}

      // general case:
      if (v < v_)
	{
	  --sum_minus_;
	  if (2 * sum_plus_ > h_.sum())
	    go_plus_();
	}
      else if (v > v_)
	{
	  --sum_plus_;
	  if (2 * sum_minus_ > h_.sum())
	    go_minus_();
	}
      else
	// v == v_
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
    void
    median<S>::go_minus_()
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
      v_ = s_[i_];
    }


    template <typename S>
    void
    median<S>::go_plus_()
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
      v_ = s_[i_];
    }


    template <typename S>
    void
    median<S>::clear()
    {
      h_.clear();
      sum_minus_ = 0;
      sum_plus_ = 0;
      i_ = (mln_max(value) - mln_min(value)) / 2;
      v_ = s_[i_];
    }

    template <typename S>
    median<S>::operator typename median<S>::value () const
    {
      return v_;
    }

    template <typename S>
    typename median<S>::value
    median<S>::to_value() const
    {
      return v_;
    }

    template <typename S>
    std::ostream& operator<<(std::ostream& ostr, const median<S>& m)
    {
      m.debug__();
      return ostr << m.to_value();
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_MEDIAN_HH
