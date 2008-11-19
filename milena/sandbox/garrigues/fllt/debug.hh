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


#ifndef MLN_FIXME_FLLT_DEBUG_HH
# define MLN_FIXME_FLLT_DEBUG_HH

/*! \file debug.hh
 *
 * \brief Debug for Fast level line transform.
 *
 */

# include <iomanip>
# include <iostream>
# include <sstream>
# include "mln/core/p_image2d.hh"
# include "mln/literal/all.hh"
# include "mln/io/ppm/save.hh"
# include "types.hh"

namespace mln
{
  namespace fllt
  {


    template <typename P, typename V>
    void
    visualize_deepness(image2d<value::int_u8>& output,
		       fllt_tree(P, V)& tree)
    {
      fllt_branch_iter_ind(P, V) p(tree.main_branch());
      level::fill(output, 0);
      for_all(p)
	{
	  //std::cout << (&*p) << ":" << p.deepness() << std::endl;
	  mln_piter(p_set<point2d>) q((*p).elt().points);
	  for_all(q)
	    {
	      if (output(q) < p.deepness())
		output(q) = p.deepness();
	    }
	}
    }


    template <typename P, typename V>
    void
    visualize_bounds(image2d<value::int_u8>& output,
		     fllt_tree(P, V)& tree,
		     unsigned limit)
    {
      fllt_branch_iter_ind(P, V) p(tree.main_branch());
      level::fill(output, 255);
      for_all(p)
	{
	  if ((*p).elt().points.nsites() > limit)
	  {
	    mln_piter(p_set<point2d>) q((*p).elt().points);
	    for_all(q)
	      {
		mln_niter(neighb2d) n(c4(), q);
		bool is_border = false;
		for_all (n)
		  if (!((*p).elt().points).has (n))
		    is_border = true;
		if (is_border)
		  output(q) = 0;
	      }
	  }
	}
    }

    template <typename P, typename V>
    void
    draw_tree(const image2d<V>& ima,
	      fllt_tree(P, V)& tree)
    {
      fllt_branch_iter_ind(P, V) p(tree.main_branch());
      for_all(p)
      	{
      	  std::cout << "region mere : " << (*p).parent() << std::endl;
      	  std::cout << "               ^" << std::endl;
      	  std::cout << "               |" << std::endl;
      	  std::cout << "region : " << &*p
		    << " value = " << (*p).elt().value << std::endl
		    << " holes : "
		    << (*p).elt().holes.nsites()
		    << (*p).elt().holes
		    << std::endl;

      	  debug::println(ima | (*p).elt().points);
      	  std::cout << std::endl;
      	}
    }

    template <typename P, typename V>
    void
    debug(const image2d<V>& ima,
	  fllt_tree(P, V)& result_tree)
    {
      std::cout << "4/ Generate outputs." << std::endl;

      image2d<value::int_u8> output (ima.domain ());
      util::tree_to_image (result_tree, output);


      //       io::pgm::save(output, "out_final.pgm");
      //       std::cout << "out_final.pgm generate"
      // 		<< std::endl;


      //      util::display_tree(ima, lower_tree);
      draw_tree(ima, result_tree);

      //       debug::println(ima);
      //       debug::println(output);

      //       if (output != ima)
      //       {
      //       	std::cerr << "BUG!!!" << std::endl;
      //       	abort();
      //       }

      image2d<value::int_u8> viz(ima.domain());
      //       image2d<value::int_u8> viz2(ima.domain());

      //       visualize_deepness(viz, lower_tree);
      //       level::stretch(viz, viz2);
      //       debug::println(viz);
      //       debug::println(viz2);
      //       io::pgm::save(viz2, "fllt.pgm");

      visualize_bounds(viz, result_tree, 200);
      //debug::println(viz);
      io::pgm::save(viz, "fllt_bounds_200.pgm");

      visualize_bounds(viz, result_tree, 100);
      io::pgm::save(viz, "fllt_bounds_100.pgm");

      visualize_bounds(viz, result_tree, 50);
      io::pgm::save(viz, "fllt_bounds_50.pgm");

      visualize_bounds(viz, result_tree, 20);
      io::pgm::save(viz, "fllt_bounds_20.pgm");

    }

    template <typename P, typename V>
    void save_state(const image2d<V>& ima,
		    const p_image2d<P>& R,
		    const p_image2d<P>& A,
		    const p_image2d<P>& N)
    {
      static int id = 0;
      std::stringstream filename;
      filename << "fllt_trace_" << std::setw(5) << std::setfill('0')
	       << std::right << id++ << ".ppm";

      image2d<value::rgb8> out(ima.domain());

      level::fill(out, literal::white);

      if (R.nsites() != 0)
	level::fill((out | R).rw(), literal::green);
      if (A.nsites() != 0)
	level::fill((out | A).rw(), literal::blue);
      if (N.nsites() != 0)
	level::fill((out | N).rw(), literal::red);

      io::ppm::save(out, filename.str());
    }

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_DEBUG_HH
