// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_UTIL_INTEGRAL_SUB_SUM_SUM2_FUNCTOR_HH
# define SCRIBO_UTIL_INTEGRAL_SUB_SUM_SUM2_FUNCTOR_HH

/// \file
///
///

# include <mln/util/couple.hh>

namespace scribo
{

  namespace util
  {

    template <typename I, typename S = mln_sum(mln_value(I))>
    class integral_sub_sum_sum2_functor
    {
      typedef mln_concrete(I) J;
      typedef mln_value(I) V;

    public:
      typedef mln::util::couple<S, S> result;

      integral_sub_sum_sum2_functor(const I& ima, unsigned scale);
      integral_sub_sum_sum2_functor(unsigned scale,
				    const mln_box(I)& output_domain,
				    unsigned border);


      void take(const V& v);

      result to_result_first_row() const;
      result to_result(const result& up_result) const;

      void begin_of_col();
      void end_of_col();

      void begin_of_row();
      void begin_of_first_row();
      void end_of_row();

      J sub;

    private:
      void init_(unsigned scale, const mln_box(I)& b, unsigned border);

      S local_sum;
      S h_sum;
      S h_sum_2;

      V *p_sub;
      unsigned b_next;
      unsigned area;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename S>
    integral_sub_sum_sum2_functor<I,S>::integral_sub_sum_sum2_functor(unsigned scale,
								      const mln_box(I)& output_domain,
								      unsigned border)
    {
      init_(scale, output_domain, border);
    }

    template <typename I, typename S>
    integral_sub_sum_sum2_functor<I,S>::integral_sub_sum_sum2_functor(const I& ima,
								      unsigned scale)
    {
      box<mln_site(I)>
	b = mln::make::box2d((ima.nrows() + scale - 1) / scale,
			     (ima.ncols() + scale - 1) / scale);
      init_(scale, b, ima.border());
    }

    template <typename I, typename S>
    void
    integral_sub_sum_sum2_functor<I,S>::init_(unsigned scale, const mln_box(I)& b,
					      unsigned border)
    {
      sub = J(b, border);

      area = scale * scale;

      b_next = 2 * border;
      unsigned b_offset = sub.delta_index(dpoint2d(border, border));
      p_sub = sub.buffer() + b_offset;
    }

    template <typename I, typename S>
    inline
    void
    integral_sub_sum_sum2_functor<I,S>::begin_of_first_row()
    {
      h_sum = 0;
      h_sum_2 = 0;
    }

    template <typename I, typename S>
    inline
    void
    integral_sub_sum_sum2_functor<I,S>::begin_of_row()
    {
      h_sum = 0;
      h_sum_2 = 0;
    }

    template <typename I, typename S>
    inline
    void
    integral_sub_sum_sum2_functor<I,S>::take(const V& v)
    {
      local_sum += v;
      h_sum_2 += v * v;
    }

    template <typename I, typename S>
    inline
    typename integral_sub_sum_sum2_functor<I,S>::result
    integral_sub_sum_sum2_functor<I,S>::to_result_first_row() const
    {
      return result(h_sum + local_sum, h_sum_2);
    }

    template <typename I, typename S>
    inline
    typename integral_sub_sum_sum2_functor<I,S>::result
    integral_sub_sum_sum2_functor<I,S>::to_result(const result& up_result) const
    {
      return result(h_sum + local_sum + up_result.first(),
		    h_sum_2 + up_result.second());
    }

    template <typename I, typename S>
    inline
    void
    integral_sub_sum_sum2_functor<I,S>::begin_of_col()
    {
      local_sum = 0;
    }

    template <typename I, typename S>
    inline
    void
    integral_sub_sum_sum2_functor<I,S>::end_of_col()
    {
      mln::convert::from_to(local_sum / area, *p_sub++);
      h_sum += local_sum;
    }

    template <typename I, typename S>
    inline
    void
    integral_sub_sum_sum2_functor<I,S>::end_of_row()
    {
      p_sub += b_next;
    }


#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_INTEGRAL_SUB_SUM_SUM2_FUNCTOR_HH
