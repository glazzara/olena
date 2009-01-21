// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_SKELETON_HH
# define MLN_SKELETON_HH

# include <iomanip>
# include <iostream>
# include <sstream>

# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/cast_image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/site_set/p_priority.hh>

# include <mln/value/int_u8.hh>
# include <mln/arith/revert.hh>
# include <mln/transform/distance.hh>

# include <mln/make/w_window2d_int.hh>

# include <mln/data/fill.hh>

# include <mln/debug/println.hh>

# include <mln/logical/not.hh>

# include "simple_point.hh"

#include <mln/make/w_window2d_int.hh>

# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/save.hh>

namespace mln
{

  template <typename V>
  void save_state(const image2d<V>& ima)
  {
    static int id = 0;
    std::stringstream filename;

    std::cout << id << std::endl;
    filename << "skel_trace_" << std::setw(5) << std::setfill('0')
	     << std::right << id++ << ".ppm";

    io::pbm::save(ima, filename.str());
  }

  image2d<bool> crest(const image2d<bool>& input,
		      const image2d<value::int_u8>& dist_map,
		      const neighb2d& nbh)
  {
    image2d<bool> is_crest;
    initialize(is_crest, input);
    data::fill(is_crest, false);

    mln_piter_(image2d<bool>) p(input.domain());
    mln_niter_(neighb2d) n(nbh, p);
    for_all(p)
    {
      if  (!input(p) || dist_map(p) < 0)
	continue;

      unsigned nb_eq = 0;
      unsigned nb_gt = 0;
      unsigned nb_lt = 0;
      for_all(n)
	if (input.domain().has(n))
	{
	  if (dist_map(n) == dist_map(p))
	    nb_eq++;
	  else if (dist_map(n) > dist_map(p))
	    nb_gt++;
	  else
	    nb_lt++;
	}


//       if ((nb_gt == 1 && nb_eq == 0) ||
// 	  (nb_gt == 0))

      if ((nb_lt + nb_eq) > 5) // Pixel Superiority index
	is_crest(p) = true;
    }
    return is_crest;
  }


  image2d<bool>
  skeleton_with_constraint(const image2d<bool>& input,
			   unsigned nbh_i,
			   const image2d<bool>& K,
			   const image2d<value::int_u8>& priority)
  {
    mln_assertion(nbh_i == 4 || nbh_i == 8);

    neighb2d nbh = int_to_neighb(nbh_i);
    image2d<bool> output;
    initialize(output, input);

    typedef mln_site_(image2d<bool>) P;
    p_priority<value::int_u8, p_queue_fast<P> > q;

    // Initialization.
    {
      p_priority<value::int_u8, p_queue_fast<P> > q_tmp;

      data::fill(output, input);
      mln_piter_(image2d<bool>) p(input.domain());
      for_all(p)
	if (!input(p) &&
	    is_simple_point2d(input, nbh_i, p)) // p is a simple point of background
	  q.push(priority(p), p);
    }

    // Propagation.
    {
      P p;
      mln_niter_(neighb2d) n(nbh, p);
      while (! q.is_empty())
      {
	p = q.pop_front();

	for_all(n)
	  if (output.domain().has(n) &&
	      output(n) &&
	      K(n) == false &&
	      is_simple_point2d(output, nbh_i, n)
	      //   && // n is simple
	      //  	      !is_curve_extremum(output, nbh_i, n, 1)
	      )
	  {
 	    output(n) = false; // Remove n from object
	    //  	    save_state(output);
	    q.push(priority(n), n);
	  }
      }
    }

    return output;
  }


  image2d<bool>
  skeleton(const image2d<bool>& input, unsigned nbh_i)
  {
    mln_assertion(nbh_i == 4 || nbh_i == 8);
    neighb2d nbh = int_to_neighb(nbh_i);

    int vals[] = { 0, 9, 0, 9, 0,
		   9, 6, 4, 6, 9,
		   0, 4, 0, 4, 0,     // Values of distances.
		   9, 6, 4, 6, 9,
		   0, 9, 0, 9, 0 };

    image2d<value::int_u8> dist_map_n = transform::distance(value::int_u8(), logical::not_(input), nbh, make::w_window2d_int(vals));
    image2d<value::int_u8> dist_map = arith::revert(dist_map_n);

    io::pgm::save(dist_map, "distance.pgm");
    io::pgm::save(dist_map_n, "distance_n.pgm");

    // Make K
    image2d<bool> K = crest(input, dist_map_n, nbh);
    io::pbm::save(K, "K.pbm");

    return skeleton_with_constraint(input, nbh_i, K, dist_map);
  }

} // end of namespace mln

#endif
