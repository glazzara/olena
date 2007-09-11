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

#ifndef MLN_ACCU_MAX_H_HH
# define MLN_ACCU_MAX_H_HH

/*! \file mln/accu/max_h.hh
 *
 * \brief Define a generic max accumulator class based on histogram.
 */

# include <mln/core/concept/accumulator.hh>
# include <mln/accu/histo.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic max function based on histogram over a value set with
     * type \c S.
     */
    template <typename S>
    struct max_h : public Accumulator< max_h<S> >
    {
      typedef mln_value(S) value;

      max_h(const Value_Set<S>& s);
      max_h();

      void init();
      void   take(const value& v);
      void   take_as_init(const value& v);
      void   take(const max_h<S>& other);
      void untake(const value& v);

      unsigned card() const { return h_.sum(); }

      value to_value() const;

      const accu::histo<S>& histo() const;

    protected:

      mutable accu::histo<S> h_;
      const S& s_; // derived from h_

      mutable std::size_t sum_;
      mutable bool valid_;
      mutable std::size_t i_; // the max index
      mutable value v_;       // the max value

      // Auxiliary methods
      void update_() const;
      void go_minus_() const;
      void go_plus_() const;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    max_h<S>::max_h(const Value_Set<S>& s)
      : h_(s),
	s_(h_.vset())
    {
      init();
    }

    template <typename S>
    max_h<S>::max_h()
      : h_(),
	s_(h_.vset())
    {
      init();
    }

    template <typename S>
    void
    max_h<S>::take(const value& v)
    {
      h_.take(v);
      if (h_.sum() == 1)
	{
	  this->take_as_init(v);
	  return;
	}
      if (v > v_)
	{
	  ++sum_;
	  valid_ = false;
	}
    }

    template <typename S>
    void
    max_h<S>::take(const max_h<S>& other)
    {
      // h_
      h_.take(other.h_);
      for (unsigned i = this->card() - 1; i > i_; --i)
	sum_ += other.h_[i];
      valid_ = false;
      // FIXME: Optimize.
    }

    template <typename S>
    void
    max_h<S>::untake(const value& v)
    {
      mln_precondition(h_(v) != 0);
      h_.untake(v);
      if (h_.sum() == 0)
	{
	  init();
	  return;
	}
      if (v > v_)
	{
	  mln_invariant(sum_ >= 1);
	  --sum_;
	  valid_ = false;
	}
      else
	if (v == v_ && h_[i_] == 0)
	  valid_ = false;
    }

    template <typename S>
    void
    max_h<S>::update_() const
    {
      if (sum_ != 0)
	go_plus_();
      else
	if (h_[i_] == 0)
	  go_minus_();
      valid_ = true;
    }

    template <typename S>
    void
    max_h<S>::go_minus_() const
    {
      do
	--i_;
      while (h_[i_] == 0);
    }

    template <typename S>
    void
    max_h<S>::go_plus_() const
    {
      do
      {
	++i_;
	if (h_[i_] != 0)
	  sum_ -= h_[i_];
      } while (sum_ != 0);
      v_ = s_[i_];
    }

    template <typename S>
    void
    max_h<S>::init()
    {
      h_.init();
      sum_ = 0;
      i_ = mln_min(value);
      v_ = s_[i_];
      valid_ = true;
    }

    template <typename S>
    void
    max_h<S>::take_as_init(const value& v)
    {
      h_.take(v);
      sum_ = 0;
      i_ = s_.index_of(v);
      v_ = v;
      valid_ = true;
    }

    template <typename S>
    typename max_h<S>::value
    max_h<S>::to_value() const
    {
      if (! valid_)
	update_();
      return v_;
    }

    template <typename S>
    const accu::histo<S>&
    max_h<S>::histo() const
    {
      return h_;
    }

    template <typename S>
    std::ostream& operator<<(std::ostream& ostr, const max_h<S>& m)
    {
      return ostr << m.to_value();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MAX_H_HH
