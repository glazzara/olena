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

# include <mln/core/image2d.hh>
# include <mln/core/set_p.hh>
# include <mln/core/inplace.hh>
# include <mln/core/neighb2d.hh>
# include <mln/core/pset_if_piter.hh>
# include <mln/core/pset_if.hh>
# include <mln/core/sub_image.hh>
# include <mln/core/image_if.hh>
# include <mln/core/clone.hh>
# include <mln/core/a_point_of.hh>

# include <mln/debug/println.hh>
# include <mln/debug/println_with_border.hh>

# include <mln/convert/to_image.hh>

# include <mln/border/fill.hh>

# include <mln/level/compute.hh>
# include <mln/level/fill.hh>
# include <mln/accu/min.hh>
# include <mln/accu/max.hh>

# include <mln/set/uni.hh>
# include <mln/set/diff.hh>
# include <mln/set/inter.hh>
# include <mln/set/is_subset_of.hh>

# include <mln/util/tree.hh>
# include <mln/util/branch_iter_ind.hh>

# include <mln/labeling/regional_minima.hh>
# include <mln/labeling/regional_maxima.hh>
# include <mln/labeling/level.hh>

# include <mln/fun/ops.hh>
# include <mln/pw/value.hh>
# include <mln/pw/cst.hh>

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
    struct fllt_node_elt
    {
      V	value;
      set_p<P> points;
      set_p<P> holes;
      /// Tell if his parent if brighter or not.  Nb : if the parent
      /// if brighter, the node come from the lower level set
      bool brighter;
    };

# define fllt_tree(P, V)  util::tree< fllt_node_elt<P, V> >
# define fllt_node(P, V)  util::node< fllt_node_elt<P, V> >
# define fllt_branch(P, V)  util::branch< fllt_node_elt<P, V> >
# define fllt_branch_iter(P, V)  util::branch_iter< fllt_node_elt<P, V> >

    //    # define fllt_node(P, V)  typename fllt_tree(P, V)::node_t



    //   LOWER LEVEL SET : region = c4, border = c8
    //   UPPER LEVEL SET : region = c8, border = c4

    // 1)
    // x0 <- a not tagged local mininum of ima.
    // g <- u(x0)

    // 2)
    // A <- {x0}
    // R <- {}
    // N <- {}

    // 3)
    // N <- N union {x neighbor of a pixel in a}
    // gn <- min u(x) x belongs to N.
    // R <- R union A
    // tag the pixels of A.

    // 4)
    // IF g < gn
    //    IF number of conected components of the border > 1
    //       follow each border to find which is the exterior border
    //       and which are the holes. Keep one pixel of each holes.
    //
    //       Remove from N border of holes.
    //       Recompute gn <- min u(x) x belongs to A
    //
    //	  g <- gn
    //    A <- {x belongs to N / u(x) == g}
    //    N <- N\{x belongs to N / u(x) == g}
    //    GO TO 3)
    // IF g == gn
    //    A <- {x belongs to N / u(x) == g}
    //    N <- N\{x belongs to N / u(x) == g}
    //    GO TO 3)
    // IF g > gn
    //    set the gray-level of the pixels of R to g.
    //    GO TO 1)

    template <typename P, typename V>
    struct fllt_env
    {
      const image2d<V>& input;
      image2d<V> u;
      image2d<bool> tagged;
      fllt_node(P, V)* current_region;
      image2d<fllt_node(P, V)*>& regions;
      set_p<P> A;
      set_p<P> R;
      set_p<P> N;
      V g,gn;
      point2d x0;

      fllt_env(const image2d<V>& input,
		 image2d<fllt_node(P, V)*>& regions_)
	: input(input),
	  regions(regions_),
	  tagged(input.domain())
      {

	// INIT
	R.clear();
	N.clear();
	A.clear();
	g=  0;
	gn = 0;
	current_region = 0;

	level::fill(regions, 0);
	level::fill(tagged, false);

	u = clone(input);
	border::fill(u, 0);
      }

    };

    template <typename P, typename V>
    void step1 (fllt_env<P, V>& env,
		point2d p)
    {
      //std::cout << "entering step 1" << std::endl;
      // x0 <- a not tagged local mininum of ima.
      //std::cout << std::endl << "x0 = " << p << std::endl;
      env.x0 = p;
      // g <- u(x0)
      env.g = env.input(env.x0);
      //std::cout << "g = " << g << std::endl;
      //std::cout << "exiting step 1" << std::endl;
    }

    template <typename P, typename V>
    void step2 (fllt_env<P, V>& env)
    {
      //std::cout << "entering step 2" << std::endl;
      // A <- {x0}
      env.A.clear();
      env.A.insert(env.x0);
      // R <- {}
      env.R.clear();
      // N <- {}
      env.N.clear();
      //std::cout << "exiting step 2" << std::endl;
    }


    template <typename V, typename P, typename F>
    void step3 (fllt_env<P, V>& env)
    {
      static bool finished = false;
      //std::cout << "entering step 3" << std::endl;

      // Stop the algorithm.
      if (finished)
      { finished = false; env.gn -= 2 * F::inc; return; }

      // N <- N union {x neighbor of a pixel in a\R}
      mln_piter(set_p<P>) qa(env.A);
      for_all(qa)
	{
	  mln_niter(neighb2d) n(F::reg_nbh(), qa);
	  for_all (n)
	    if (!env.R.has (n))
	      env.N.insert (n);
	}

      //      debug::println(u);

      //       //std::cout << "A :" << std::endl;
      //       if (A.npoints())
      // 	//debug::println(u | A);
      //       //std::cout << "N :" << std::endl;
      //       if (N.npoints())
      // 	//debug::println(u | N);
      //       //std::cout << "R :" << std::endl;
      //       if (R.npoints())
      // 	//debug::println(u | R);

      // gn <- min u(x) x belongs to N.
      if ((env.u | set::inter(env.N, env.u.domain())).npoints() > 0)
	env.gn = level::compute< typename F::accu_for_gn >(env.u | set::inter(env.N, env.u.domain()));
      else
      {
	finished = true;
 	env.gn += F::inc;
      }
      //std::cout << std::endl << "gN = " << gn << std::endl;
      // R <- R union A
      // tag the pixels of A.

      for_all(qa)
	{
	  env.R.insert(qa);
	  env.tagged(qa) = true;
	  //Update the number of connected components.
	}
      //std::cout << "exiting step 3" << std::endl;
    }


    /// IF g < gn.
    template <typename V, typename P, typename F>
    void step4_1 (fllt_env<P, V>& env)
    {
      //std::cout << "entering step 4_1" << std::endl;

      // If the region is bounded
      // Create a new conected component.
      // FIXME : we can make it faster.

      if ((env.R.bbox() < env.u.domain()) || (env.R.npoints() == env.u.domain().npoints()))
      {
	mln_piter(set_p<P>) p(env.R);
	env.current_region = new fllt_node(P, V)();
	env.current_region->elt().brighter = F::parent_is_brighter;
	env.current_region->elt().value = env.g;
	for_all(p)
	  {
 	    env.current_region->elt().points.insert(p);

	    if (env.regions(p) == 0)
	    {
	      //current_region->elt().points.insert(p);
	      env.regions(p) = env.current_region;
	    }
	    else
	    {
	      if (env.regions(p)->parent() == 0)
		env.regions(p)->set_parent(env.current_region);
	    }
	  }


	// Count the number of conected components of the border of R.
	static image2d<int>  tmp(env.u.domain().to_larger(1));
	static image2d<bool> border_ima(tmp.domain());
	level::fill(border_ima, false);

	//       level::fill(inplace(border_ima | N), true);
	//       std::cout << "tmp border = " << tmp.border () << std::endl;
	//       std::cout << "ima border = " << border_ima.border () << std::endl;
	mln_piter(set_p<P>) z(env.N);
	for_all(z)
	  {
	    mln_assertion(border_ima.owns_(z));
	    border_ima(z) = true;
	  }
	std::cout << "labeling::level" << std::endl;
	unsigned n;
	labeling::level(border_ima, true, F::bdr_nbh(), tmp, n);

	//     debug::println(border_ima);
	//std::cout << "nb composantes :" << n << std::endl;
	//      debug::println(tmp);
	if (n > 1)
	{

	  //   IF number of conected components of the border > 1
	  for (int i = 2; i <= n; i++)
	  {
	    //       follow each border to find which is the exterior border
	    //       and which are the holes. Keep one pixel of each holes.

	    // WARNING : We trust labeling::level to label the exterior border with 1.
	    env.current_region->elt().holes.insert(a_point_of(tmp | pw::value(tmp) == pw::cst(i)));

	    //       FIXME : [optimisation] Remove from N border of holes???.
	    //       Recompute gn <- min u(x) x belongs to A
	  }
	}

      }
      env.g = env.gn;
      //    A <- {x belongs to N / u(x) == g}
      env.A.clear();
      env.A = set::uni(env.A, set::inter(env.N, env.u.domain()) | pw::value(env.u) == pw::cst(env.g));
      //    N <- N\{x belongs to N / u(x) == g}
      env.N = set::diff(env.N, set::inter(env.N, env.u.domain()) | pw::value(env.u) == pw::cst(env.g));

      //       std::cout << "A :" << std::endl;
      //       if (A.npoints())
      // 	debug::println(u | A);
      //       std::cout << "N :" << std::endl;
      //       if (N.npoints())
      // 	debug::println(u | N);

      //std::cout << "exiting step 4_1" << std::endl;
    }


    /// IF g == gn.
    template <typename V, typename P>
    void step4_2 (fllt_env<P, V>& env)
    {
      //std::cout << "entering step 4_2" << std::endl;

      //    A <- {x belongs to N / u(x) == g}
      env.A = set::uni(env.A, set::inter(env.N, env.u.domain()) | pw::value(env.u) == pw::cst(env.g));
      //    N <- N\{x belongs to N / u(x) == g}
      env.N = set::diff(env.N, set::inter(env.N, env.u.domain()) | pw::value(env.u) == pw::cst(env.g));

      //       std::cout << "A :" << std::endl;
      //       if (A.npoints())
      // 	debug::println(u | A);
      //       std::cout << "N :" << std::endl;
      //       if (N.npoints())
      // 	debug::println(u | N);

      //std::cout << "exiting step 4_2" << std::endl;
    }

    /// IF g > gn.
    template <typename V, typename P>
    void step4_3 (fllt_env<P, V>& env)
    {
      //std::cout << "entering step 4_3" << std::endl;

      //    set the gray-level of the pixels of R to g.
      mln_piter(set_p<P>) p(env.R);
      for_all(p)
	{
	  mln_assertion (env.tagged(p));
	  env.u (p) = env.g;
	}

      //std::cout << "exiting step 4_3" << std::endl;

    }


    template <typename V, typename F>
    fllt_tree(point2d, V)&
    compute_level_set(const image2d<V>& input,
		      image2d< fllt_node(point2d, V)* >& regions)
    {
      typedef point2d P;
      typedef image2d<V> I;

      // FIXME: not nice.
      typedef     mln::image_if<
	mln::image2d<V>,
	mln::fun::greater_p2b_expr_<mln::pw::value_<mln::image2d<V> >,
	mln::pw::cst_<int> >
	> I_IF;

      // Check
      mln_assertion(input.domain() == regions.domain());

      // FIXME : rename it.
      fllt_env<P,V> env(input, regions);

      // Declarations.
//       set_p<P> R, N, A;
//       V g, gn;
//       point2d x0;
      image2d<V> min_locals(input.domain());


      // Get the locals extremums
      unsigned nlabels;
      F::regional_extremum(input, F::reg_nbh(), min_locals, nlabels);

      //       debug::println(min_locals);
      //       debug::println(min_locals | (pw::value(min_locals) > pw::cst(0)));

      /// Algorithm.
      {
	// For all locals extremums
	//void* x = min_locals | (pw::value(min_locals) > pw::cst(0));
	I_IF min_locals_list(min_locals | (pw::value(min_locals) > pw::cst(0)));
	mln_piter(I_IF) p(min_locals_list.domain());
	for_all(p)
	  {
	    if (env.tagged(p))
	      continue;

	    step1(env, p);
	    step2(env);
	    while (1)
	    {
	      //std::cout << "g = " << g << std::endl;
	      step3<V, P, F>(env);
	      /// step4.
	      if (F::compare(env.g, env.gn))
	      {
		step4_1<V, P, F>(env);
		// GO TO 3)
		continue;
	      }


	      if (env.g == env.gn)
	      {
		step4_2(env);
		// GO TO 3)
		continue;
	      }


	      if (!F::compare(env.g, env.gn))
	      {
		step4_3(env);
		// GO TO 1)
		break;
	      }
	    }
	    //std::cout << "current_region = " << current_region << std::endl;
	  }
      } // end of Algorithm

      image2d<value::int_u8> output (input.domain ());
      fllt_tree(P, V)& tree = *new fllt_tree(P, V)(env.current_region);
      util::tree_to_image (tree, output);

      //       util::display_tree(input, tree);

      //       debug::println(output);
      //       std::cout << std::endl;
      //       debug::println(input);

      //       if (output != input)
      //       {
      // 	std::cerr << "BUG!!!" << std::endl;
      // 	abort();
      //       }

      //       io::pgm::save(output, "out.pgm");
      //       std::cout << "out.pgm generate"
      // 		<< std::endl;


      //      debug::println(regions);
      //debug::println(input | regions(make:defined reference to `mln::fllt::lower<mln::value::int_u<8u> >::inc':point2d(-4,-1))->elt().points);

      return (tree);

    } // end of compute_level_set

    //Fwd declarations.
    template <typename V> struct lower;
    template <typename V> struct upper;

    //   LOWER LEVEL SET : region = c4, border = c8
    template <typename V>
    struct lower
    {
      typedef upper<V> opposite;
      static bool
      compare(const V& u, const V& v)
      {
	return u < v;
      }

      template <typename I, typename N, typename O>
      static bool
      regional_extremum(const Image<I>& input, const Neighborhood<N>& nbh,
			Image<O>& output, unsigned& nlabels)
      {
	return labeling::regional_minima(input, nbh,
					 output, nlabels);
      }

      static const int inc = 1;
      static const bool parent_is_brighter = true;
      typedef accu::min accu_for_gn;

      static const neighb2d& bdr_nbh() { return c8(); }
      static const neighb2d& reg_nbh() { return c4(); }

    };



    //   UPPER LEVEL SET : region = c8, border = c4
    template <typename V>
    struct upper
    {
      typedef lower<V> opposite;

      static bool
      compare(const V& u, const V& v)
      {
	return u > v;
      }

      template <typename I, typename N, typename O>
      static bool
      regional_extremum(const Image<I>& input, const Neighborhood<N>& nbh,
			Image<O>& output, unsigned& nlabels)
      {
	return labeling::regional_maxima(input, nbh,
					 output, nlabels);
      }

      static const int inc = -1;
      static const bool parent_is_brighter = false;
      typedef accu::max accu_for_gn;

      static const neighb2d& bdr_nbh() { return c4(); }
      static const neighb2d& reg_nbh() { return c8(); }
    };

    // Fwd declarations.
    template <typename P, typename V, typename F>
    void
    fill_a_shape(fllt_node(P, V)& node,
		 fllt_tree(P, V)& tree,
		 const image2d<fllt_node(P, V)*>& node_reg,
		 const image2d<fllt_node(P, V)*>& hole_reg);

    template <typename P, typename V, typename F>
    void
    move_shape(fllt_node(P, V)& node,
	       fllt_node(P, V)& hole,
	       fllt_tree(P, V)& tree,
	       const image2d<fllt_node(P, V)*>& hole_reg,
	       const image2d<fllt_node(P, V)*>& other_reg)
    {
      // FIXME : debug to remove.
      //      std::cout << "       [move_shape] "<< &hole << " as son of "<< &node << std::endl;
      //node.elt().points = set::uni(hole.elt().points, node.elt().points);
      node.add_child(&hole);
      fill_a_shape<P,V,typename F::opposite>(hole, tree, hole_reg, other_reg);
    }

    template <typename P, typename V, typename F>
    fllt_node(P, V)*
    find_the_hole(fllt_node(P, V)& node,
		  const P p,
		  const image2d<fllt_node(P, V)*>& other_reg)
    {
      fllt_node(P, V)* s = other_reg(p);
      mln_assertion(s);
      while (s->parent() && F::opposite::compare(s->parent()->elt().value, node.elt().value))
	//FIXME : Was while (s->parent() && (s->parent()->elt().value < node.elt().value))
      {
	mln_assertion(s);
	s = s->parent();
	mln_assertion(s);
      }
//       std::cout << "   [Find the hole] of " << p
// 		<< " from " << &node
// 		<< " return " << s
// 		<< std::endl;
      return s;
    }

    template <typename P, typename V, typename F>
    void
    fill_a_shape(fllt_node(P, V)& node,
		 fllt_tree(P, V)& tree,
		 const image2d<fllt_node(P, V)*>& node_reg,
		 const image2d<fllt_node(P, V)*>& hole_reg)
    {
//       std::cout << "[Start fill_a_shape] " << &node << " "
// 		<< node.elt().holes.npoints()
// 		<< " holes." << std::endl;

      if (node.elt().holes.npoints() == 0)
      {
	//	std::cout << "[End fill_a_shape]" << std::endl;
	return;
      }
      mln_piter(set_p<P>) p(node.elt().holes);
      for_all(p)
	{
	  bool h = true;

	  fllt_node(P, V)* hole;
	  if (node.elt().brighter == F::parent_is_brighter)
	    hole = find_the_hole<P,V,F>(node, point2d(p), hole_reg);
	  else
	    hole = find_the_hole<P,V,typename F::opposite>(node, point2d(p), node_reg);

	  mln_assertion(hole);

	  typename fllt_node(P, V)::children_t::iterator it;
	  for (it = node.children().begin();
	       it != node.children().end();
	       it++)
	  {
	    // Browse the hole of each child.
	    mln_piter(set_p<P>) q((*it)->elt().holes);
	    for_all(q)
	      {
		fllt_node(P, V)* child_hole = find_the_hole<P,V,F>((**it), point2d(q), hole_reg);
		if (set::is_subset_of(hole->elt().points,
				      child_hole->elt().points))

//		if (hole->elt().points < child_hole->elt().points)
		{
		  h = false;
		  break;
		}

	      }
	    if (!h)
	      break;
	  }
	  if (h)
	    move_shape<P,V,F>(node, *hole, tree, hole_reg, node_reg);
	}

      node.elt().holes.clear();
      //      std::cout << "[end fill_a_shape]" << std::endl;
    }

    template <typename P, typename V>
    fllt_tree(P, V)
      merge_trees(fllt_tree(P, V)& lower_tree,
		  fllt_tree(P, V)& upper_tree,
		  const image2d<fllt_node(P, V)*>& low_reg,
		  const image2d<fllt_node(P, V)*>& upp_reg,
		  const image2d<V>& ima)
    {

      //   In order to merge the trees, we only have to find for each shape S
      //   with a hole H in it whether one of its children has a hole H´
      //   containing H. If it is the case, we do nothing. Otherwise, we
      //   put the shape of the hole H (and all its descendants) as child of
      //   the shape .
      {
	std::cout << "[Merge first tree]------------" << std::endl;

	fllt_branch_iter(P, V) p(lower_tree.main_branch());
	for_all(p)
	  {
	    fllt_node(P, V)& n(p);
	    fill_a_shape< P, V, lower<V> >(n, lower_tree, low_reg, upp_reg);
	    mln_assertion(lower_tree.check_consistency());
	    mln_assertion(upper_tree.check_consistency());
	  }

      }

      {
	std::cout << "[Merge second tree]------------" << std::endl;

	fllt_branch_iter(P, V) p(upper_tree.main_branch());
	for_all(p)
	  {
	    fllt_node(P, V)& n(p);
	    fill_a_shape< P, V, upper<V> >(n, upper_tree, upp_reg, low_reg);
	    mln_assertion(lower_tree.check_consistency());
	    mln_assertion(upper_tree.check_consistency());
	  }
      }

      fllt_tree(P, V)* main_tree = &lower_tree;
      fllt_tree(P, V)* other_tree = &upper_tree;

      if (lower_tree.root()->elt().points.npoints() >= ima.domain().npoints())
      {
	main_tree = &upper_tree;
	other_tree = &lower_tree;
      }

      typename fllt_node(P, V)::children_t::iterator it;
      for (it = other_tree->root()->children().begin();
      	   it != other_tree->root()->children().end(); )
      {
      	main_tree->root()->add_child(*it);
      }
      mln_assertion(main_tree->check_consistency());
      return *main_tree;
    }


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
	  mln_piter(set_p<point2d>) q((*p).elt().points);
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
	    mln_piter(set_p<point2d>) q((*p).elt().points);
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

    template <typename V>
    // Fixme : return type
    void
    fllt(const image2d<V>& ima)
    {
      typedef point2d P;

      fllt_tree(P, V) upper_tree;
      fllt_tree(P, V) lower_tree;
      image2d<fllt_node(P, V)*> low_reg(ima.domain());
      image2d<fllt_node(P, V)*> upp_reg(ima.domain());

      std::cout << "1/ Compute the lower level set." << std::endl;
      lower_tree = compute_level_set<V, lower<V> >(ima, low_reg);
      //draw_tree(ima, lower_tree);
      std::cout << "2/ Compute the upper level set." << std::endl;
      upper_tree = compute_level_set<V, upper<V> >(ima, upp_reg);

      //draw_tree(ima, upper_tree);

      std::cout << "3/ Merge the two trees." << std::endl;

      // FIXME : the algorithm is contrast invariant.
      //         -> the both calls have to give the same result
      //         -> check it.
      // FIXME : call merge_tree one time will be enough.
      std::cout << "upp_reg = " << &upp_reg << std::endl;
      std::cout << "low_reg = " << &low_reg << std::endl;

      //fllt_tree(P, V) result_tree = merge_trees(upper_tree, lower_tree, upp_reg, low_reg, ima);
      fllt_tree(P, V) result_tree = merge_trees(lower_tree, upper_tree, low_reg, upp_reg, ima);


      std::cout << "4/ Generate outputs." << std::endl;

      image2d<value::int_u8> output (ima.domain ());
      util::tree_to_image (result_tree, output);


      //       io::pgm::save(output, "out_final.pgm");
      //       std::cout << "out_final.pgm generate"
      // 		<< std::endl;


      //      util::display_tree(ima, lower_tree);
      //draw_tree(ima, result_tree);

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

      visualize_bounds(viz, result_tree, 8);
      io::pgm::save(viz, "fllt_bounds_8.pgm");

    }

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_HH
