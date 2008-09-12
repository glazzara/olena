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


#ifndef MLN_FIXME_FLLT_COMPUTE_LEVEL_SET_FAST_HH
# define MLN_FIXME_FLLT_COMPUTE_LEVEL_SET_FAST_HH

/*! \file compute_level_set_fast2.hh
 *
 * \brief Compute Level Set for Fast level line transform.
 *        Version with one image for A, R, N.
 *
 */

# include <mln/core/p_image2d.hh>
# include <mln/core/image_if_value.hh>
# include "local_configurations.hh"

# define SET_R 3
# define SET_A 2
# define SET_N 1

# define image_sub_if_value image_if_value<const sub_image<image2d<value::int_u8>, box2d> >

namespace mln
{
  namespace fllt_fast
  {
    using namespace fllt;

    class fllt::ran_domains;

    template <typename V>
    void step_fast1 (const image2d<V>& ima,
		point2d p,
		V& g,
		point2d& x0)
    {
      //std::cout << "entering step_fast 1" << std::endl;
      // x0 <- a not tagged local mininum of ima.
      //std::cout << std::endl << "x0 = " << p << std::endl;
      x0 = p;
      // g <- u(x0)
      g = ima(x0);
      //std::cout << "g = " << g << std::endl;
      //std::cout << "exiting step_fast 1" << std::endl;
    }

    void step_fast2 (ran_domains& r_a_n,
		     point2d& x0)
    {
      //std::cout << "entering step_fast 2" << std::endl;
      // R <- {}
      // N <- {}
      // A <- {x0}
      r_a_n.clear();
      r_a_n.add_to<SET_A>(x0);


      //std::cout << "exiting step_fast 2" << std::endl;
    }


    template <typename V, typename F>
    void step_fast3 (const image2d<V>& u,
		     image2d<bool>& tagged,
		     ran_domains& r_a_n,
		     V& gn,
		     fllt_node(point2d, V)*& current_region,
		     image2d<fllt_node(point2d, V)*>& regions)
    {
      static bool finished = false;
      //std::cout << "entering step_fast 3" << std::endl;

      // Stop the algorithm.
      if (finished)
      { finished = false; gn -= 2 * F::inc; return; }

      // N <- N union {x neighbor of a pixel in a\R}
      //mln_piter(p_image2d<P>) qa(A);

      //      p_image2d_fwd_pixter<point2d> qa(A);
      image_sub_if_value ima(r_a_n.image() | SET_A);
      mln_assertion(ima.has_data());
      image_sub_if_value::fwd_piter qa(ima.domain());
      mln_assertion(ima.has_data());
      for_all(qa)
	{
      mln_assertion(ima.has_data());
	  mln_niter(neighb2d) n(F::reg_nbh(), qa);
	  for_all (n)
	    if (u.has(n) && !r_a_n.belongs_to<SET_R>(n) &&
		!r_a_n.belongs_to<SET_A>(n))
	      r_a_n.add_to<SET_N>(n);
	}

      //      debug::println(u);

//       std::cout << "A :" << A << std::endl;
//       debug::println(A.image());
//       std::cout << "N :" << N << std::endl;
//       debug::println(N.image());
//       std::cout << "R :" << R << std::endl;
//       debug::println(R.image());

      // gn <- min u(x) x belongs to N.

      image_sub_if_value tmp(r_a_n.image() | SET_N);
      finished = tmp.nsites() == 0;

      if (!finished)
 	gn = level::compute< typename F::accu_for_gn >(u | tmp.domain());

      if (finished)
 	gn += F::inc;

      //std::cout << std::endl << "gN = " << gn << std::endl;
      // R <- R union A
      // tag the pixels of A.

      for_all(qa)
	{
	  if (!r_a_n.belongs_to<SET_A>(qa))
	    continue;
// 	  std::cout << "Added to R" << tmp << std::endl;
	  mln_assertion(!r_a_n.belongs_to<SET_R>(qa));
	  r_a_n.move_to<SET_A, SET_R>(qa);
	  tagged(qa) = true;
	  // Update the number of holes of R.
	  //n_added_holes<point2d, F>(R, qa);
	}


      //std::cout << "exiting step_fast 3" << std::endl;
    }


    template <typename V>
    void update_set(const image2d<V>& u,
		     ran_domains& r_a_n,
		     V& g)
    {
      //    A <- {x belongs to N / u(x) == g}

      // This is slow.
      // A.insert(set::inter(N, u.domain()) | pw::value(u) == pw::cst(g));

      //    N <- N\{x belongs to N / u(x) == g}
      //       N.remove(set::inter(N, u.domain()) | pw::value(u) == pw::cst(g));
      //       mln_assertion(check_step_4_2(u, N, g));

      // This is faster.

//       if (N.nsites() >= 0)
//       {
      image_sub_if_value tmp(r_a_n.image() | SET_N);
      image_sub_if_value::fwd_piter qn(tmp.domain());
      for_all(qn)
      {
	if (u(qn) == g)
	  r_a_n.move_to<SET_N, SET_A>(qn);
      }
//       }
//       else {
// 	mln_fwd_piter(p_image2d<P>) p(N);
// 	for_all(p)
// 	{
// 	  if (u(p) == g)
// 	  {
// 	    A.insert(p);
// 	    N.remove(p);
// 	  }
// 	}
// 	mln_assertion(check_step_4_2(u, N, g));
//       }
    }
    /// IF g < gn.
    template <typename V, typename F>
    void step_fast4_1 (image2d<V>& u,
		       ran_domains& r_a_n,
		       V& g,
		       V& gn,
		       fllt_node(point2d, V)*& current_region,
		       image2d<fllt_node(point2d, V)*>& regions
		       )
    {
//       std::cout << "entering step_fast 4_1" << std::endl;

      // If the region is bounded
      // Create a new conected component.
      // FIXME : we can make it faster.

      // FIXME : rewrite this test.
      // if ((R.bbox() < u.domain()) || (R.nsites() == u.domain().nsites()))
//       {
      image_sub_if_value tmp(r_a_n.image() | SET_R);
      image_sub_if_value::fwd_piter p(tmp.domain());
      current_region = new fllt_node(point2d, V)();
      current_region->elt().brighter = F::parent_is_brighter;
      current_region->elt().value = g;
      for_all(p)
      {
	current_region->elt().points.insert(p);

	if (regions(p) == 0)
	{
	  //current_region->elt().points.insert(p);
	  regions(p) = current_region;
	}
	else
	{
	  if (regions(p)->parent() == 0)
	    regions(p)->set_parent(current_region);
	}
      }


// 	// Count the number of conected components of the border of R.
// 	static image2d<unsigned>  tmp(u.domain().to_larger(1));
// 	static image2d<bool> border_ima(tmp.domain());
// 	level::fill(border_ima, false);

// 	//       level::fill(inplace(border_ima | N), true);
// 	//       std::cout << "tmp border = " << tmp.border () << std::endl;
// 	//       std::cout << "ima border = " << border_ima.border () << std::endl;
// 	mln_piter(p_image2d<P>) z(N);
// 	for_all(z)
// 	  {
// 	    mln_assertion(border_ima.has(z));
// 	    border_ima(z) = true;
// 	  }
// 	unsigned n;
// 	tmp = labeling::level(border_ima, true, F::bdr_nbh(), n);

// 	//     debug::println(border_ima);
// 	//std::cout << "nb composantes :" << n << std::endl;
// 	//      debug::println(tmp);
// 	if (n > 1)
// 	{

// 	  //   IF number of conected components of the border > 1
// 	  for (int i = 2; i <= n; i++)
// 	  {
// 	    //       follow each border to find which is the exterior border
// 	    //       and which are the holes. Keep one pixel of each holes.

// 	    // WARNING : We trust labeling::level to label the exterior border with 1.
// 	    current_region->elt().holes.insert(a_point_of(tmp | pw::value(tmp) == pw::cst(i)));

// 	    //       FIXME : [optimisation] Remove from N border of holes???.
// 	    //       Recompute gn <- min u(x) x belongs to A
// 	  }
// 	}


//      }

      g = gn;

      update_set(u, r_a_n, g);

      //       std::cout << "A :" << std::endl;
      //       if (A.nsites())
      // 	debug::println(u | A);
      //       std::cout << "N :" << std::endl;
      //       if (N.nsites())
      // 	debug::println(u | N);

//       std::cout << "exiting step_fast 4_1" << std::endl;
    }

    /// IF g == gn.
    template <typename V, typename P>
    void step_fast4_2 (const image2d<V>& u,
		       ran_domains& r_a_n,
		       V& g,
		       fllt_node(P, V)* current_region,
		       image2d<fllt_node(P, V)*>& regions
		       )
    {
//       std::cout << "entering step_fast 4_2" << std::endl;

      update_set(u,r_a_n,g);

      //       std::cout << "A :" << std::endl;
      //       if (A.nsites())
      // 	debug::println(u | A);
      //       std::cout << "N :" << std::endl;
      //       if (N.nsites())
      // 	debug::println(u | N);

//       std::cout << "exiting step_fast 4_2" << std::endl;
    }

    /// IF g > gn.
    template <typename V>
    void step_fast4_3 (image2d<V>& u,
		       const image2d<bool>& tagged,
		       ran_domains& r_a_n,
		       const V& g)
    {
//       std::cout << "entering step_fast 4_3" << std::endl;

      //    set the gray-level of the pixels of R to g.
      image_sub_if_value tmp(r_a_n.image() | SET_R);
      image_sub_if_value::fwd_piter p(tmp.domain());
      for_all(p)
	{
	  mln_assertion (tagged(p));
	  u (p) = g;
	}
//       std::cout << "exiting step_fast 4_3" << std::endl;

    }


    template <typename V, typename F>
    fllt_tree(point2d, V)&
      compute_level_set_fast(const image2d<V>& ima,
		      image2d< fllt_node(point2d, V)* >& regions)
    {
      typedef point2d P;
      typedef image2d<V> I;

      // FIXME: not nice.
      typedef     mln::image_if<
	mln::image2d<unsigned>,
	mln::fun::greater_p2b_expr_<mln::pw::value_<mln::image2d<unsigned> >,
	mln::pw::cst_<int> >
	> I_IF;

      // Check
      mln_assertion(ima.domain() == regions.domain());

      // Declarations.
      ran_domains r_a_n(ima.domain());
      V g, gn;
      point2d x0;
      image2d<unsigned> min_locals(ima.domain());
      image2d<V> u = clone(ima);
      border::fill(u, 0);

      //std::cout << "image U:" << std::endl;
      //      debug::println_with_border(u);
      image2d<bool> tagged(ima.domain());
      fllt_node(P, V)* current_region;

      // INIT
      g=  0;
      gn = 0;
      current_region = 0;

      level::fill(regions, 0);
      level::fill(tagged, false);

      // Get the locals extremums
      unsigned nlabels;
      min_locals = F::regional_extremum(ima, F::reg_nbh(), nlabels);

      //       debug::println(min_locals);
      //       debug::println(min_locals | (pw::value(min_locals) > pw::cst(0)));

      /// Algorithm.
      {
	// For all locals extremums
	I_IF min_locals_list(min_locals | (pw::value(min_locals) > pw::cst(0)));
	mln_piter(I_IF) p(min_locals_list.domain());
	int cpt = 0, snapshop = 0;
	for_all(p)
	  {
	    if (tagged(p))
	      continue;

	    //std::cout << "boucle no " << cpt++ << std::endl;
	    step_fast1(ima, p, g, x0);
	    step_fast2(r_a_n, x0);
	    while (1)
	    {
	      // To have a view of the progression of the algotithm.
#ifdef FLLT_TRACE
	      save_state(ima, r_a_n);
#endif
	      //std::cout << "g = " << g << std::endl;
	      step_fast3<V, F>(u, tagged, r_a_n, gn, current_region, regions);
	      /// step_fast4.
	      if (F::compare(g, gn))
	      {
		step_fast4_1<V, F>(u, r_a_n, g, gn, current_region, regions);
		// GO TO 3)
		continue;
	      }


	      if (g == gn)
	      {
		step_fast4_2(u, r_a_n, g, current_region, regions);
		// GO TO 3)
		continue;
	      }


	      if (!F::compare(g, gn))
	      {
		step_fast4_3(u, tagged, r_a_n, g);
		// GO TO 1)
		break;
	      }
	    }
	    //std::cout << "current_region = " << current_region << std::endl;
	  }
      } // end of Algorithm

      image2d<value::int_u8> output (ima.domain ());
      fllt_tree(P, V)& tree = *new fllt_tree(P, V)(current_region);
      util::tree_to_image (tree, output);

      //       util::display_tree(ima, tree);

      //       debug::println(output);
      //       std::cout << std::endl;
      //       debug::println(ima);

      //       if (output != ima)
      //       {
      // 	std::cerr << "BUG!!!" << std::endl;
      // 	abort();
      //       }

      //       io::pgm::save(output, "out.pgm");
      //       std::cout << "out.pgm generate"
      // 		<< std::endl;


      //      debug::println(regions);
      //debug::println(ima | regions(make:defined reference to `mln::fllt::lower<mln::value::int_u<8u> >::inc':point2d(-4,-1))->elt().points);

      return (tree);

    } // end of compute_level_set_fast

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_COMPUTE_LEVEL_SET_FAST_HH
