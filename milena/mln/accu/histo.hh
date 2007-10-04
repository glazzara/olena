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

#ifndef MLN_ACCU_HISTO_HH
# define MLN_ACCU_HISTO_HH

/*! \file mln/accu/histo.hh
 *
 * \brief Define a generic histogram accumulator class.
 *
 * \todo Use histo::data instead of std::vector!
 */

# include <vector>
# include <algorithm>

# include <mln/core/concept/value_set.hh>
# include <mln/accu/internal/base.hh>
# include <mln/value/set.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic histogram class over a value set with type \c S.
     */
    template <typename S>
    struct histo : public mln::accu::internal::base_< const std::vector<std::size_t>& , histo<S> >
    {
      histo(const Value_Set<S>& s);
      histo();

      typedef mln_value(S) value;
      typedef const std::vector<std::size_t>& result;

      void   take(const value& v);
      void   take(const histo<S>& other);
      void untake(const value& v);
      void init();

      std::size_t operator()(const value& v) const;
      std::size_t operator[](std::size_t i) const;
      std::size_t nvalues() const;
      std::size_t sum() const;

      const std::vector<std::size_t>& vect() const;
      const std::vector<std::size_t>& to_result() const;

      const S& vset() const;

    protected:

      const S& s_;
      std::vector<std::size_t> h_;
      std::size_t sum_;
    };


    template <typename S>
    std::ostream& operator<<(std::ostream& ostr, const histo<S>& h);




# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    histo<S>::histo(const Value_Set<S>& s)
      : s_(exact(s)),
	h_(s_.nvalues(), 0),
	sum_(0)
    {
    }

    template <typename S>
    histo<S>::histo()
      : s_(S::the()),
	h_(s_.nvalues(), 0),
	sum_(0)
    {
    }

    template <typename S>
    void
    histo<S>::take(const value& v)
    {
      ++h_[s_.index_of(v)];
      ++sum_;
    }

    template <typename S>
    void
    histo<S>::take(const histo<S>& other)
    {
      for (unsigned i = 0; i < h_.size(); ++i)
	h_[i] += other.h_[i];
      sum_ += other.sum_;
    }

    template <typename S>
    void
    histo<S>::untake(const value& v)
    {
      mln_precondition(h_[s_.index_of(v)] > 0);
      mln_precondition(sum_ > 0);
      --h_[s_.index_of(v)];
      --sum_;
    }

    template <typename S>
    void
    histo<S>::init()
    {
      std::fill(h_.begin(), h_.end(), 0);
      sum_ = 0;
    }

    template <typename S>
    std::size_t
    histo<S>::operator()(const value& v) const
    {
      return h_[s_.index_of(v)];
    }

    template <typename S>
    std::size_t
    histo<S>::operator[](std::size_t i) const
    {
      mln_precondition(i < s_.nvalues());
      return h_[i];
    }

    template <typename S>
    std::size_t
    histo<S>::nvalues() const
    {
      return s_.nvalues();
    }

    template <typename S>
    std::size_t
    histo<S>::sum() const
    {
      return sum_;
    }

    template <typename S>
    const std::vector<std::size_t>&
    histo<S>::vect() const
    {
      return h_;
    }

    template <typename S>
    const std::vector<std::size_t>&
    histo<S>::to_result() const
    {
      return h_;
    }

    template <typename S>
    const S&
    histo<S>::vset() const
    {
      return s_;
    }

    template <typename S>
    std::ostream& operator<<(std::ostream& ostr, const histo<S>& h)
    {
      mln_viter(S) v(h.vset());
      for_all(v)
	if (h(v) != 0)
	  ostr << v << ':' << h(v) << ' ';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_HISTO_HH
