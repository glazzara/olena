// Copyright (C) 2012, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_UTIL_INTEGRAL_SUM_SUM2_GLOBAL_MIN_FUNCTOR_HH
# define SCRIBO_UTIL_INTEGRAL_SUM_SUM2_GLOBAL_MIN_FUNCTOR_HH

/// \file
///
///

# include <mln/util/couple.hh>
# include <mln/trait/value_.hh>

namespace scribo
{

  namespace util
  {

    template <typename V, typename S = mln_sum(V)>
    class integral_sum_sum2_global_min_functor
    {
    public:
      typedef mln::util::couple<S, S> result;

      void begin_of_first_row();
      void begin_of_row();
      void end_of_row();

      void begin_of_col();
      void end_of_col();


      void take(const V& v);

      result to_result_first_row() const;
      result to_result(const result& up_result) const;

      const V& global_min() const;

    private:
      S h_sum_;
      S h_sum_2_;

      V global_min_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename V, typename S>
    inline
    void
    integral_sum_sum2_global_min_functor<V,S>::begin_of_first_row()
    {
      h_sum_ = 0;
      h_sum_2_ = 0;
      global_min_ = 0;
    }

    template <typename V, typename S>
    inline
    void
    integral_sum_sum2_global_min_functor<V,S>::begin_of_row()
    {
      h_sum_ = 0;
      h_sum_2_ = 0;
    }

    template <typename V, typename S>
    inline
    void
    integral_sum_sum2_global_min_functor<V,S>::take(const V& v)
    {
      if (global_min_ > v)
	global_min_ = v;

      h_sum_   += v;
      h_sum_2_ += v * v;
    }

    template <typename V, typename S>
    inline
    typename integral_sum_sum2_global_min_functor<V,S>::result
    integral_sum_sum2_global_min_functor<V,S>::to_result_first_row() const
    {
      return result(h_sum_, h_sum_2_);
    }

    template <typename V, typename S>
    inline
    typename integral_sum_sum2_global_min_functor<V,S>::result
    integral_sum_sum2_global_min_functor<V,S>::to_result(const result& up_result) const
    {
      return result(h_sum_ + up_result.first(),
		    h_sum_2_ + up_result.second());
    }

    template <typename V, typename S>
    inline
    const V&
    integral_sum_sum2_global_min_functor<V,S>::global_min() const
    {
      return global_min_;
    }

    template <typename V, typename S>
    inline
    void
    integral_sum_sum2_global_min_functor<V,S>::begin_of_col()
    {
    }

    template <typename V, typename S>
    inline
    void
    integral_sum_sum2_global_min_functor<V,S>::end_of_col()
    {
    }


    template <typename V, typename S>
    inline
    void
    integral_sum_sum2_global_min_functor<V,S>::end_of_row()
    {
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_INTEGRAL_SUM_SUM2_GLOBAL_MIN_FUNCTOR_HH
