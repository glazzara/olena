// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_STAT_DEVIATION_HH
# define MLN_ACCU_STAT_DEVIATION_HH

/// \file mln/accu/stat/deviation.hh
///
/// Define an accumulator that computes a standard deviation.

# include <mln/accu/internal/base.hh>
# include <mln/accu/count.hh>
# include <mln/accu/sum.hh>
# include <mln/math/sqr.hh>
# include <mln/math/sqrt.hh>


namespace mln
{

  namespace accu
  {

    namespace stat
    {

    /// Generic standard deviation accumulator class.
    /*!
     * Parameter \c T is the type of values that we sum.  Parameter \c
     * S is the type to store the standard deviation; the default type of
     * \c S is the summation type (property) of \c T.  Parameter \c M
     * is the type of the mean value; the default type of \c M is \c
     * S.
     */
    template <typename T,
	      typename S = mln_sum(T),
	      typename M = S>
    struct deviation : public mln::accu::internal::base< M , deviation<T,S,M> >
    {
      typedef T argument;
      typedef M result;

      deviation(const T mean);

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take(const deviation<T,S,M>& other);
      /// \}

      /// Get the value of the accumulator.
      M to_result() const;
      operator M () const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      accu::count<T> count_;
      accu::sum<T,S>   sum_;
      T mean_;
    };



    template <typename I, typename S, typename M>
    struct deviation< util::pix<I>, S,M >;


    namespace meta
    {

      /// Meta accumulator for deviation.
      struct deviation : public Meta_Accumulator< deviation >
      {
	template < typename T,
		   typename S = mln_sum(T),
		   typename M = S >
	struct with
	{
	  typedef accu::stat::deviation<T,S,M> ret;
	};
      };

    } // end of namespace mln::accu::meta


# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename S, typename M>
    inline
    deviation<T,S,M>::deviation(const T mean)
    {
      init();
      mean_ = mean;
    }

    template <typename T, typename S, typename M>
    inline
    void
    deviation<T,S,M>::init()
    {
      count_.init();
      sum_.init();
    }

    template <typename T, typename S, typename M>
    inline
    void deviation<T,S,M>::take(const argument& t)
    {
      count_.take(t);
      sum_.take(math::sqr(t - mean_));
    }

    template <typename T, typename S, typename M>
    inline
    void
    deviation<T,S,M>::take(const deviation<T,S,M>& other)
    {
      // FIXME
      count_.take(other.count_);
      sum_.take(other.sum_);
    }

    template <typename T, typename S, typename M>
    inline
    M
    deviation<T,S,M>::to_result() const
    {
      unsigned n = count_.to_result();
      if (n == 0u)
	return M(); // Safety.
      return static_cast<M>(math::sqrt(sum_.to_result() / n));
    }

    template <typename T, typename S, typename M>
    inline
    deviation<T,S,M>::operator M() const
    {
      return to_result();
    }

    template <typename T, typename S, typename M>
    inline
    bool
    deviation<T,S,M>::is_valid() const
    {
      return count_.to_result() != 0;
    }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_DEVIATION_HH
