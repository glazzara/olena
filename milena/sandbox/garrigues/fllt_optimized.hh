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


#ifndef MLN_FIXME_FLLT_HH
# define MLN_FIXME_FLLT_HH

/*! \file fllt.hh
 *
 * \brief Fast level line transform of an image.
 *
 */


# include "fllt_types.hh"
# include "level_set.hh"
# include "fllt_merge.hh"
# include "lower.hh"
# include "upper.hh"

# include <mln/util/tree_to_image.hh>
# include <mln/value/int_u8.hh>
# include <mln/level/stretch.hh>
# include <mln/level/compare.hh>
# include <mln/io/pgm/save.hh>

namespace mln
{
  namespace fllt
  {

    template <typename P, typename V>
    void
    visualize_deepness(image2d<value::int_u8>& output,
		       fllt_tree(P, V)& tree)
    {
      fllt_branch_iter(P, V) p(tree.main_branch());
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
      fllt_branch_iter(P, V) p(tree.main_branch());
      level::fill(output, 255);
      for_all(p)
	{
	  if ((*p).elt().points.npoints() > limit)
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
      fllt_branch_iter(P, V) p(tree.main_branch());
      for_all(p)
      	{
      	  std::cout << "region mere : " << (*p).parent() << std::endl;
      	  std::cout << "               ^" << std::endl;
      	  std::cout << "               |" << std::endl;
      	  std::cout << "region : " << &*p
		    << " value = " << (*p).elt().value << std::endl
		    << " holes : "
		    << (*p).elt().holes.npoints()
		    << (*p).elt().holes
		    << std::endl;

      	  debug::println(ima | (*p).elt().points);
      	  std::cout << std::endl;
      	}
    }

    template <typename P, typename V>
    void
    debug(const image2d<V>& ima,
	  fllt_tree(P, V)& tree)
    {

      std::cout << "4/ Generate outputs." << std::endl;

      image2d<value::int_u8> output (ima.domain());
      util::tree_to_image (tree, output);

      util::display_tree(ima, tree);
      draw_tree(ima, tree);

      if (output != ima)
	std::cerr << "Warning: input and output differ." << std::endl;

      image2d<value::int_u8> out(ima.domain());
      image2d<value::int_u8> out2(ima.domain());
      visualize_deepness(out, tree);
      level::stretch(out, out2);
      io::pgm::save(out2, "fllt_deepnees.pgm");

      visualize_bounds(out, tree, 800);
      io::pgm::save(out, "fllt_bounds_800.pgm");
      visualize_bounds(out, tree, 400);
      io::pgm::save(out, "fllt_bounds_400.pgm");
      visualize_bounds(out, tree, 200);
      io::pgm::save(out, "fllt_bounds_200.pgm");
      visualize_bounds(out, tree, 100);
      io::pgm::save(out, "fllt_bounds_100.pgm");
      visualize_bounds(out, tree, 50);
      io::pgm::save(out, "fllt_bounds_50.pgm");
      visualize_bounds(out, tree, 20);
      io::pgm::save(out, "fllt_bounds_20.pgm");
      visualize_bounds(out, tree, 8);
      io::pgm::save(out, "fllt_bounds_8.pgm");
    }

    template <typename V>
    fllt_tree(mln_point(image2d<V>), V)
    fllt(const image2d<V>& ima)
    {
      typedef point2d P;

      fllt_tree(P, V) upper_tree;
      fllt_tree(P, V) lower_tree;
      image2d<fllt_node(P, V)*> low_reg(ima.domain());
      image2d<fllt_node(P, V)*> upp_reg(ima.domain());

      std::cout << "1/ Compute the lower level set." << std::endl;
      lower_tree = compute_level_set<V, lower<V> >(ima, low_reg);
      std::cout << "2/ Compute the upper level set." << std::endl;
      upper_tree = compute_level_set<V, upper<V> >(ima, upp_reg);

      std::cout << "3/ Merge the two trees." << std::endl;
      fllt_tree(P, V) result_tree = merge_trees(lower_tree, upper_tree, low_reg, upp_reg, ima);

      return result_tree;
    }

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_HH
