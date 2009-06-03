// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_ACCU_SUM_PIX_HH
# define MLN_ACCU_SUM_PIX_HH

/// \file mln/accu/sum_pix.hh
///
/// Define an accumulator that computes a sum.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>

# include <mln/util/pix.hh> // To prevent accu::sum_pix to work on pixels (ambiguous).

# include <mln/trait/value_.hh>      // For mln_sum_pix.
# include <mln/value/builtin/all.hh> // In the case of summing builtin values.
# include <mln/literal/zero.hh>      // For initialization.


namespace mln
{

  namespace accu
  {


    /// Generic sum_pix accumulator class.
    /*!
     * Parameter \c T is the type of values that we sum.  Parameter \c
     * S is the type to store the value sum; the default type of
     * \c S is the summation type (property) of \c T.
     */
    template <typename P, typename S = mln_sum(mln_value(P))>
    struct sum_pix : public mln::accu::internal::base< const S&, sum_pix<P,S> >
    {
      typedef P argument;

      sum_pix();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take(const sum_pix<P,S>& other);
      /// \}

      void set_value(S v)
      {
	s_ = v;
      }

      /// Get the value of the accumulator.
      const S& to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      S s_;
    };


    namespace meta
    {

      /// Meta accumulator for sum_pix.
      struct sum_pix : public Meta_Accumulator< sum_pix >
      {
	template <typename P, typename S = mln_sum(mln_value(P))>
	struct with
	{
	  typedef accu::sum_pix<P, S> ret;
	};
      };

    } // end of namespace mln::accu::meta


# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename S>
    inline
    sum_pix<P,S>::sum_pix()
    {
      init();
    }

    template <typename P, typename S>
    inline
    void
    sum_pix<P,S>::init()
    {
      s_ = literal::zero;
    }

    template <typename P, typename S>
    inline
    void sum_pix<P,S>::take(const argument& p)
    {
      s_ += /* 1 + */ p.v();
    }

    template <typename P, typename S>
    inline
    void
    sum_pix<P,S>::take(const sum_pix<P,S>& other)
    {
      s_ += other.s_;
    }

    template <typename P, typename S>
    inline
    const S&
    sum_pix<P,S>::to_result() const
    {
      return s_;
    }

    template <typename P, typename S>
    inline
    bool
    sum_pix<P,S>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_SUM_PIX_HH
