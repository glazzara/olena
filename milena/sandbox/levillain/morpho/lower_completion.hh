// Copyright (C) 2005, 2008 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef MLN_MORPHO_LOWER_COMPLETION_HH
# define MLN_MORPHO_LOWER_COMPLETION_HH

/* FIXME: This file comes from Olena proto-1.0, and needs some
   adjustments.

   Translate it into the Milena dialect, e.g.,
   - replace `oln_' and `oln::' by `mln_' and `mln::';
   - replace `internal::' by `impl::';
   - adjust the names of interfaces and types;
   - adjust other names;
   - adjust calling conventions;
   - adjust contracts (static/dynamic pre-/postconditions, etc.);
   - etc.  */

# include <queue>

# include <mln/data/fill.hh>

namespace mln {

  namespace morpho {

    template <typename DestValue, typename I>
    typename ch_value_type<I, DestValue>::ret
    lower_completion(const abstract::image_with_nbh<I>& input)
    {
      mlc_is_a(I, abstract::scalar_valued_image)::ensure();

      typedef oln_type_of(I, point) point_type;

      /*------------------------------------------.
      | Distance map to the closest lower point.  |
      `------------------------------------------*/

      typedef std::queue<point_type> queue_type;
      queue_type q;
      typename ch_value_type<I, bool>::ret processed(input.size(),
						     input.nbh_get ());
      data::fill (processed, false);

      typedef ntg_cumul_type(DestValue) cumul_type;
      typename ch_value_type<I, cumul_type>::ret distance(input.size(),
							  input.nbh_get());
      cumul_type cur_dist = 1;

      oln_type_of(I, piter) p(input.size());
      for_all_p (p)
	{
	  distance[p] = 0;
	  oln_type_of(I, niter) n(input);
	  for_all_n_of_p (n, p)
	    if (input.hold(n) && input[n] < input[p])
	      {
		q.push(p);
		processed[p] = true;
	      }
	}

      typedef oln_pt_type_of(point_type, grid) grid_type;
      typedef oln_grd_type_of(grid_type, dimvalue) dimvalue_type;
      const unsigned dim = dimvalue_type::val;
      // Fictitious point, used as a separator between ranges of
      // points in the queue.
      point_type fictitious;
      for (unsigned i = 0; i < dim; ++i)
	fictitious.nth (i) = -1;
      q.push(fictitious);

      while (!q.empty())
	{
	  point_type p = q.front();
	  q.pop();
	  if (p == fictitious)
	    {
	      if (!q.empty())
		{
		  q.push(fictitious);
		  ++cur_dist;
		}
	    }
	  else
	    {
	      distance[p] = cur_dist;
	      oln_type_of(I, niter) n(input);
	      for_all_n_of_p (n, p)
		if (input.hold(n) && input[n] == input[p] && !processed[n])
		  {
		    q.push(n);
		    processed[n] = true;
		  }
	    }
	}

      /*-------------------.
      | Lower completion.  |
      `-------------------*/

      typename ch_value_type<I, DestValue>::ret output(input.size(),
						       input.nbh_get());
      for_all_p (p)
	if (distance[p] == ntg_zero_val(DestValue))
	  output[p] = input[p] * cur_dist;
	else
	  /* Careful, the order of the operations matters here.  If we had
	     written is as

	       input[p] * cur_dist + distance[p] - 1

	     the compiler would have complained about the expression
	     « input[p] * cur_dist » not having an Integre property.  This
	     is because the resolution of operator+ would have taken place
	     inside the ntg namespace, as its first operand would be
	     an Integre value.  */
	  output[p] = distance[p] - 1 + input[p] * cur_dist;

      return output;
    }

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_LOWER_COMPLETION_HH
