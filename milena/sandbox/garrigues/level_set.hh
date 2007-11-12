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


#ifndef MLN_FIXME_LEVEL_SET_HH
# define MLN_FIXME_LEVEL_SET_HH

/*! \file level_set.hh
 *
 * \brief Algorithm to compulte the upper or lower level set.
 *
 */

# include "fllt_types.hh"
# include "lower.hh"
# include "upper.hh"

# include <mln/core/image2d.hh>
# include <mln/core/point2d.hh>

# include <mln/core/p_set.hh>
# include <mln/core/inplace.hh>
# include <mln/core/neighb2d.hh>
# include <mln/core/clock_neighb2d.hh>
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
    struct ls_env
    {
      const image2d<V>& input;
      image2d<V> u;
      image2d<bool> tagged;
      image2d<bool> shape;
      int n_cc;

      fllt_node(P, V)* current_region;
      image2d<fllt_node(P, V)*>& regions;
      p_set<P> A;
      p_set<P> R;
      p_set<P> N;
      V g,gn;
      point2d x0;

      ls_env(const image2d<V>& input,
		 image2d<fllt_node(P, V)*>& regions_)
	: input(input),
	  regions(regions_),
	  tagged(input.domain()),
	  shape(input.domain()),
	  g(0),
	  gn(0),
	  n_cc(0),
	  current_region(0)
      {
	// INIT
	R.clear();
	N.clear();
	A.clear();

	level::fill(regions, 0);
	level::fill(tagged, false);

	u = clone(input);
	border::fill(u, 0);
      }

    };

    template <typename P, typename V>
    void update_n_cc(lower_t,
		     ls_env<P, V>& env,
		     point2d p)
    {
      // region in c4.
      bool previous_is_false;
      unsigned res = 0;

      dpoint2d dp(-1,-1);
      clock_neighb2d nbh = cc8(dp);

      mln_fwd_niter(clock_neighb2d)   n(nbh , p);

      // just to get the last previous_is_false.
      // FIXME replace it with bkd_niter.
      for_all(n)
	previous_is_false = !env.shape(n);

      for_all(n)
	{
	  //y in c4
	  bool x = env.shape(n);
	  n.next();
	  mln_assertion(n.is_valid());
	  bool y = env.shape(n);

	  if (!x && y)
	    ++res;
	  else
	    if (x && y && previous_is_false)
              ++res;

	  previous_is_false = !y;

	}
      env.n_cc += (res == 0 ? 0 : (res - 1));

    }

    template <typename P, typename V>
    void update_n_cc(upper_t,
		     ls_env<P, V>& env,
		     point2d p)
    {
      // region in c8.
      bool previous_is_false;
      unsigned res = 0;

      dpoint2d dp(-1,0);
      clock_neighb2d nbh = cc8(dp);

      mln_fwd_niter(clock_neighb2d)   n(nbh , p);

      // just to get the last previous_is_false.
      // FIXME optimise it with bkd_niter.
      for_all(n)
	{
	  bool x = env.shape(n);
	  n.next();
	  mln_assertion(n.is_valid());
	  bool y = env.shape(n);
	  previous_is_false = !y && !x;
	}


      for_all(n)
	{
	  //y in c4
	  bool x = env.shape(n);
	  n.next();
	  mln_assertion(n.is_valid());
	  bool y = env.shape(n);

	  if (!x && y)
	    ++res;
	  else
	    if (x && previous_is_false)
              ++res;

	  previous_is_false = !y && !x;

	}
      env.n_cc += (res == 0 ? 0 : (res - 1));
    }

    template <typename P, typename V>
    void step1 (ls_env<P, V>& env,
		point2d p)
    {
      // x0 <- a not tagged local mininum of ima.
      env.x0 = p;
      // g <- u(x0)
      env.g = env.input(env.x0);
    }

    template <typename P, typename V>
    void step2 (ls_env<P, V>& env)
    {
      // A <- {x0}
      env.A.clear();
      env.A.insert(env.x0);
      // R <- {}
      env.R.clear();
      // N <- {}
      env.N.clear();
    }


    template <typename V, typename P, typename F>
    void step3 (ls_env<P, V>& env)
    {
      static bool finished = false;

      // Stop the algorithm.
      if (finished)
      { finished = false; env.gn -= 2 * F::inc; return; }

      // N <- N union {x neighbor of a pixel in a\R}
      mln_piter(p_set<P>) qa(env.A);
      for_all(qa)
	{
	  mln_niter(neighb2d) n(F::reg_nbh(), qa);
	  for_all (n)
	    if (!env.R.has (n))
	      env.N.insert (n);
	}

      // gn <- min u(x) x belongs to N.
      if ((env.u | set::inter(env.N, env.u.domain())).npoints() > 0)
	env.gn = level::compute< typename F::accu_for_gn >(env.u | set::inter(env.N, env.u.domain()));
      else
      {
	finished = true;
 	env.gn += F::inc;
      }

      // R <- R union A
      // tag the pixels of A.
      for_all(qa)
	{
	  env.R.insert(qa);
	  env.tagged(qa) = true;
	  //Update the number of connected components.
	  update_n_cc(typename F::tag(), env, qa);
	}
    }

    /// IF g < gn.
    template <typename V, typename P, typename F>
    void step4_1 (ls_env<P, V>& env)
    {
      // If the region is bounded
      // Create a new conected component.
      // FIXME : we can make it faster.

      if ((env.R.bbox() < env.u.domain()) || (env.R.npoints() == env.u.domain().npoints()))
      {
	mln_piter(p_set<P>) p(env.R);
	env.current_region = new fllt_node(P, V)();
	env.current_region->elt().brighter = F::parent_is_brighter;
	env.current_region->elt().value = env.g;
	for_all(p)
	  {
 	    env.current_region->elt().points.insert(p);

	    if (env.regions(p) == 0)
	    {
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

	mln_piter(p_set<P>) z(env.N);
	for_all(z)
	  {
	    mln_assertion(border_ima.owns_(z));
	    border_ima(z) = true;
	  }
	unsigned n;
	labeling::level(border_ima, true, F::bdr_nbh(), tmp, n);

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
    }


    /// IF g == gn.
    template <typename V, typename P>
    void step4_2 (ls_env<P, V>& env)
    {
      //    A <- {x belongs to N / u(x) == g}
      env.A = set::uni(env.A, set::inter(env.N, env.u.domain()) | pw::value(env.u) == pw::cst(env.g));
      //    N <- N\{x belongs to N / u(x) == g}
      env.N = set::diff(env.N, set::inter(env.N, env.u.domain()) | pw::value(env.u) == pw::cst(env.g));
    }

    /// IF g > gn.
    template <typename V, typename P>
    void step4_3 (ls_env<P, V>& env)
    {
      //    set the gray-level of the pixels of R to g.
      mln_piter(p_set<P>) p(env.R);
      for_all(p)
	{
	  mln_assertion (env.tagged(p));
	  env.u (p) = env.g;
	}
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
      ls_env<P,V> env(input, regions);

      // Declarations.
      image2d<V> min_locals(input.domain());


      // Get the locals extremums
      unsigned nlabels;
      F::regional_extremum(input, F::reg_nbh(), min_locals, nlabels);

      /// Algorithm.
      {
	// For all locals extremums
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
	  }
      } // end of Algorithm

      image2d<value::int_u8> output (input.domain ());
      fllt_tree(P, V)& tree = *new fllt_tree(P, V)(env.current_region);
      util::tree_to_image (tree, output);

      return (tree);

    } // end of compute_level_set

  } // end of namespace mln::fllt

} // end of namespace mln

#endif // ! MLN_FIXME_LEVEL_SET_HH
