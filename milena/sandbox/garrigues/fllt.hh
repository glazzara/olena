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
# include <mln/convert/to_image.hh>

# include <mln/level/compute.hh>
# include <mln/level/fill.hh>
# include <mln/accu/min.hh>

# include <mln/set/uni.hh>
# include <mln/set/diff.hh>
# include <mln/set/inter.hh>

# include <mln/util/abr.hh>

# include <mln/labeling/regional_minima.hh>
# include <mln/labeling/level.hh>

# include <mln/fun/ops.hh>
# include <mln/pw/value.hh>
# include <mln/pw/cst.hh>

namespace mln
{
  namespace fllt
  {

    template <typename P>
    struct fllt_node
    {
      set_p<P> points;
      set_p<P> holes;
    };

    # define fllt_node(P)  util::abr< fllt_node<P> >


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

    template <typename V>
    void step1 (const image2d<V>& ima,
		point2d p,
		V& g,
		point2d& x0)
    {
      std::cout << "entering step 1" << std::endl;
      // x0 <- a not tagged local mininum of ima.
      x0 = p;
      // g <- u(x0)
      g = ima(x0);
      std::cout << "g = " << g << std::endl;
      std::cout << "exiting step 1" << std::endl;
    }

    template <typename P>
    void step2 (set_p<P>& A,
		set_p<P>& R,
		set_p<P>& N,
		point2d& x0)
    {
      std::cout << "entering step 2" << std::endl;
      // A <- {x0}
      A.clear();
      A.insert(x0);
      // R <- {}
      R.clear();
      // N <- {}
      N.clear();
      std::cout << "exiting step 2" << std::endl;
    }


    template <typename V, typename P>
    void step3 (const image2d<V>& u,
		image2d<bool>& tagged,
		set_p<P>& A,
		set_p<P>& R,
		set_p<P>& N,
		V& gn)
    {
      static bool finished = false;
      std::cout << "entering step 3" << std::endl;

      // Stop the algorithm.
      if (finished)
      { gn = 0; return; }

      // N <- N union {x neighbor of a pixel in a\R}
      mln_piter(set_p<P>) qa(A);
      for_all(qa)
	{
	  mln_niter(neighb2d) n(c4(), qa);
	  for_all (n)
	    if (!R.has (n))
	      N.insert (n);
	}

      std::cout << "A :" << std::endl;
      if (A.npoints())
	debug::println(u | A);
      std::cout << "N :" << std::endl;
      if (N.npoints())
	debug::println(u | N);
      std::cout << "R :" << std::endl;
      if (R.npoints())
	debug::println(u | R);

      // gn <- min u(x) x belongs to N.
      if ((u | set::inter(N, u.domain())).npoints() > 0)
	gn = level::compute<accu::min>(u | set::inter(N, u.domain()));
      else
      {
	finished = true;
 	gn++;
      }
      std::cout << std::endl << "gN = " << gn << std::endl;
      // R <- R union A
      // tag the pixels of A.

      for_all(qa)
	{
	  R.insert(qa);
	  tagged(qa) = true;
	}
      std::cout << "exiting step 3" << std::endl;
    }


    /// IF g < gn.
    template <typename V, typename P>
    void step4_1 (image2d<V>& u,
		  set_p<P>& A,
		  set_p<P>& R,
		  set_p<P>& N,
		  V& g,
		  V& gn,
		  fllt_node(P)* current_region,
		  image2d<fllt_node(P)*>& regions
		  )
    {
      std::cout << "entering step 4_1" << std::endl;

      // Create a new conected component.
      // FIXME : we can make it faster.
      //mln_piter(S) p(set::inter(N, u.domain()) | pw::value(u) == pw::cst(g));
      mln_piter(set_p<P>) p(R);
      current_region = new fllt_node(P)();
      for_all(p)
	{
	  if (regions(p) == 0)
	  {
	    current_region->content().points.insert(p);
	    regions(p) = current_region;
	  }
	  else
	  {
	    if (regions(p)->get_parent() == 0)
	      regions(p)->set_parent(current_region);
	  }
	}


      // Count the number of conected components of the border of R.
      image2d<int>  tmp(u.domain().to_larger(1));
      image2d<bool> border_ima(u.domain().to_larger(1));
      level::fill(border_ima, false);
      level::fill(inplace(border_ima | N), true);
      unsigned n;
      labeling::level(border_ima, true, c8(), tmp, n);

      //     debug::println(border_ima);
      std::cout << "nb composantes :" << n << std::endl;
      debug::println(tmp);

      if (n > 1)
      {

	//   IF number of conected components of the border > 1
	for (int i = 2; i < n; i++)
	{
	  //       follow each border to find which is the exterior border
	  //       and which are the holes. Keep one pixel of each holes.

	  // WARNING : We trust labeling::level to label the exterior border with 1.
	  current_region->content().holes.insert(a_point_of(tmp | pw::value(tmp) == pw::cst(n)));

	  //       FIXME : [optimisation] Remove from N border of holes???.
	  //       Recompute gn <- min u(x) x belongs to A
	}
      }

      g = gn;
      //    A <- {x belongs to N / u(x) == g}
      A.clear();
      A = set::uni(A, N | pw::value(u) == pw::cst(g));
      //    N <- N\{x belongs to N / u(x) == g}
      N = set::diff(N, N | pw::value(u) == pw::cst(g));

      std::cout << "A :" << std::endl;
      if (A.npoints())
	debug::println(u | A);
      std::cout << "N :" << std::endl;
      if (N.npoints())
	debug::println(u | N);

      std::cout << "exiting step 4_1" << std::endl;
    }


    /// IF g == gn.
    template <typename V, typename P>
    void step4_2 (const image2d<V>& u,
		  set_p<P>& A,
		  set_p<P>& N,
		  V& g,
		  fllt_node(P)* current_region,
		  image2d<fllt_node(P)*>& regions
		  )
    {
      std::cout << "entering step 4_2" << std::endl;

      //    A <- {x belongs to N / u(x) == g}
      A = set::uni(A, N | pw::value(u) == pw::cst(g));
      //    N <- N\{x belongs to N / u(x) == g}
      N = set::diff(N, N | pw::value(u) == pw::cst(g));

//       // FIXME
//       typedef mln::pset_if<
// 	mln::set_p<mln::point_<mln::grid::square, int> >,
// 	mln::fun::eq_p2b_expr_<mln::pw::value_<mln::image2d<int> >,
// 	mln::pw::cst_<int> > > S;

//       // Update the current region
//       //mln_piter(S) p(set::inter(N, u.domain()) | pw::value(u) == pw::cst(g));
//       mln_piter(set_p<P>) p(A);
//       for_all(p)
// 	{
// 	  if (regions(p) == 0)
// 	    regions(p) = current_region;
// 	}

      std::cout << "A :" << std::endl;
      if (A.npoints())
	debug::println(u | A);
      std::cout << "N :" << std::endl;
      if (N.npoints())
	debug::println(u | N);

      std::cout << "exiting step 4_2" << std::endl;
    }

    /// IF g > gn.
    template <typename V, typename P>
    void step4_3 (image2d<V>& u,
		  const image2d<bool>& tagged,
		  const set_p<P>& R,
		  const V& g)
    {
      std::cout << "entering step 4_3" << std::endl;

      //    set the gray-level of the pixels of R to g.
      mln_piter(set_p<P>) p(R);
      for_all(p)
	{
	  mln_assertion (tagged(p));
	  u (p) = g;
	}

      std::cout << "exiting step 4_3" << std::endl;

    }


    template <typename V>
    void compute_level_set(const image2d<V>& ima)
    {
      typedef point2d P;
      typedef image2d<V> I;
      // FIXME
      typedef     mln::image_if<
	mln::image2d<V>,
	mln::fun::greater_p2b_expr_<mln::pw::value_<mln::image2d<V> >,
	mln::pw::cst_<V> >
	> I_IF;


      set_p<P> R, N, A;
      V g, gn;
      point2d x0;
      image2d<V> min_locals(ima.domain());
      image2d<V> u = clone(ima);
      image2d<bool> tagged(ima.domain());

      fllt_node(P)* current_region;
      image2d<fllt_node(P)*> regions(ima.domain());
      level::fill(regions, 0);

      unsigned nlabels;
      labeling::regional_minima(ima, c4(), min_locals, nlabels);

      debug::println(min_locals);
      debug::println(min_locals | (pw::value(min_locals) > pw::cst(0)));

      /// Algorithm.
      {
	I_IF min_locals_list(min_locals | (pw::value(min_locals) > pw::cst(0)));
	mln_piter(I_IF) p(min_locals_list.domain());
	for_all(p)
	  {
	    if (tagged(p))
	      continue;

	    std::cout << "p = " << p << std::endl;

	    step1(ima, p, g, x0);

	    step2(A, R, N, x0);
	    while (1)
	    {
	      std::cout << "g = " << g << std::endl;
	      step3(u, tagged, A, R, N, gn);
	      /// step4.
	      if (g < gn)
	      {
		step4_1(u, A, R, N, g, gn, current_region, regions);
		// GO TO 3)
		continue;
	      }


	      if (g == gn)
	      {
		step4_2(u, A, N, g, current_region, regions);
		// GO TO 3)
		continue;
	      }


	      if (g > gn)
	      {
		step4_3(u, tagged, R, g);
		//    GO TO 1)
		break;
	      }
	    }
	  }
      } // end of Algorithm
      std::cout << "END OF ALGORITHM" << std::endl;


      debug::println(regions);
      debug::println(ima | regions(make::point2d(-4,-1))->content().points);

    } // end of compute_level_set

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_HH
