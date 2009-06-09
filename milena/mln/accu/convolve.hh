// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_CONVOLVE_HH
# define MLN_ACCU_CONVOLVE_HH

/// \file
///
/// Define an accumulator that computes a convolution.

# include <mln/accu/internal/base.hh>
# include <mln/value/ops.hh>
# include <mln/literal/zero.hh>


namespace mln
{

  namespace accu
  {


    /// \brief Generic convolution accumulator class.
    ///
    /// Parameters \c T1 and \c T2 are the type of values to be
    /// convolved.  Parameter \c R is the result type.
    ///
    /// \ingroup modaccuvalues
    //
    template <typename T1, typename T2,
	      typename R = mln_sum_product(T1, T2)>
    struct convolve : public mln::accu::internal::base< R, convolve<T1,T2,R> >,
		      private metal::converts_to< mln_sum_product(T1, T2),
						  R >::check_t
    {
      typedef std::pair<T1,T2> argument;

      convolve();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take(const T1& t1, const T2& t2);
      void take(const convolve<T1,T2,R>& other);
      /// \}

      /// Get the value of the accumulator.
      R to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      typedef mln_sum_product(T1, T2) S;
      S s_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T1, typename T2, typename R>
    inline
    convolve<T1,T2,R>::convolve()
    {
      init();
    }

    template <typename T1, typename T2, typename R>
    inline
    void
    convolve<T1,T2,R>::init()
    {
      static S zero = literal::zero;
      s_ = zero;
    }

    template <typename T1, typename T2, typename R>
    inline
    void
    convolve<T1,T2,R>::take(const argument& t)
    {
      s_ = static_cast<S>(s_ + static_cast<S>(t.first) * static_cast<S>(t.second));
    }

    template <typename T1, typename T2, typename R>
    inline
    void
    convolve<T1,T2,R>::take(const T1& t1, const T2& t2)
    {
      s_ = static_cast<S>(s_ + static_cast<S>(t1) * static_cast<S>(t2));
    }

    template <typename T1, typename T2, typename R>
    inline
    void
    convolve<T1,T2,R>::take(const convolve<T1,T2,R>& other)
    {
      s_ = static_cast<S>(s_ + other.s_);
    }

    template <typename T1, typename T2, typename R>
    inline
    R
    convolve<T1,T2,R>::to_result() const
    {
      return s_;
    }

    template <typename T1, typename T2, typename R>
    inline
    bool
    convolve<T1,T2,R>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_CONVOLVE_HH
