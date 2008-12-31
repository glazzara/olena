// Copyright (C) 2007 EPITA Research and Development unsignedaboratory
//
// This file is part of the Olena unsignedibrary.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public unsignedicense version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public unsignedicense for more details.
//
// You should have received a copy of the GNU General Public unsignedicense
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
// unsignedicense.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public unsignedicense.

#ifndef MLN_DT_RAW_PATH_SLOW_HH
# define MLN_DT_RAW_PATH_SLOW_HH

# include <queue>
# include <map>
# include <cmath>

# include <mln/core/concept/image.hh>
# include <mln/make/w_window.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/literal/zero.hh>

# include <iostream>

namespace mln
{

  namespace dt
  {

    /*! Propagation using a single neighborhood (PSN).
     *
     * \param[in]  input_   The input image.
     * \param[in]  nbh      The chamfer window to use for distance calcul.
     * \param[in]  max      Max value of the output image.
     * \return              A distance map.
     *
     * \pre \p img has to be initialized.
     */
    template<typename I, typename N>
    mln_ch_value(I, mln_point(I))
    raw_path_slow(const Image<I>& input_, const N& nbh, unsigned max);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {
    } // end of namespace mln::dt::impl



    // Facade.

    template<typename I, typename N>
    inline
    mln_ch_value(I, mln_point(I))
    raw_path_slow(const Image<I>& input_, const N& nbh, unsigned max)
    {
      // Preconditions.
      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      // Types.
      typedef mln_point(I) point;

      // Initialization of distance.
      mln_ch_value(I, unsigned) distance;
      initialize(distance, input);

      // Initialization of output.
      mln_ch_value(I, mln_point(I)) output;
      initialize(output, input);

      // Initialization.
      // {

      std::map< unsigned, std::queue<point> > bucket;
      unsigned bucket_size = 0;

      mln_fwd_piter(I) p(input.domain());
      for_all(p)
      {
	output(p) = p;
	if (input(p))
	{
	  distance(p) = literal::zero;
	  bucket[0].push(p);
	  ++bucket_size;
	}
	else
	  distance(p) = max;
      }

      // }

      for (unsigned d = 0; bucket_size != 0; ++d)
      {
	std::queue<point> bucket_d = bucket[d];
	while (! bucket_d.empty())
	{
	  point p = bucket_d.front();
	  bucket_d.pop();
	  --bucket_size;

	  if (distance(p) == d)
	  {
	    mln_qiter(N) n(nbh, p);

	    for_all(n)
	      if (distance.has(n))
	      {
		unsigned newDist = d + n.w();

		if (newDist < distance(n))
		{
		  distance(n) = newDist;
		  output(n) = output(p);
		  bucket[newDist].push(n);
		  ++bucket_size;
		}
	      }
	  }
	}
	bucket.erase(d);
      }

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::dt

} // end of namespace mln

#endif // ! MLN_DT_RAW_PATH_SLOW_HH
