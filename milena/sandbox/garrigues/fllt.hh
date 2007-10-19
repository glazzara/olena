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


#ifndef MLN_LEVEL_MEDIAN_HH
# define MLN_LEVEL_MEDIAN_HH

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
# include <mln/core/sub_image.hh>
# include <mln/core/image_if.hh>

# include <mln/debug/println.hh>
# include <mln/convert/to_image.hh>

# include <mln/level/compute.hh>
# include <mln/accu/min.hh>

# include <mln/labeling/regional_minima.hh>
# include <mln/labeling/level.hh>

# include <mln/fun/ops.hh>
# include <mln/pw/value.hh>
# include <mln/pw/cst.hh>
# include <mln/fun/p2b/chess.hh>

namespace mln
{
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
    // x0 <- a not tagged local mininum of ima.
    x0 = p;
    // g <- u(x0)
    g = ima(x0);
  }

  template <typename P>
  void step2 (set_p<P>& A,
	      set_p<P>& R,
	      set_p<P>& N,
	      point2d& x0)
  {
    // A <- {x0}
    A.clear();
    A.insert(x0);
    // R <- {}
    R.clear();
    // N <- {}
    N.clear();
  }


  template <typename V, typename P>
  void step3 (const image2d<V>& ima,
	      image2d<bool>& tagged,
	      set_p<P>& A,
	      set_p<P>& R,
	      set_p<P>& N,
	      V& gn)
  {
    // N <- N union {x neighbor of a pixel in a\R}
    mln_piter(set_p<P>) qa(A);
    
    for_all(qa)
      {
	mln_niter(neighb2d) n(c4(), qa);
	for_all (n)
	  if (!R.has (n))
	    N.insert (n);
      }
    
    // gn <- min u(x) x belongs to N.
    gn = level::compute<accu::min>(ima | N);
    
    // R <- R union A
    // tag the pixels of A.
    
    for_all(qa)
      {
	R.insert(qa);
	tagged(qa) = true;
      }
  }


  /// IF g < gn.
  template <typename V, typename P>
  void step4_1 (const image2d<V>& ima,
		image2d<bool>& tagged,
		set_p<P>& A,
		set_p<P>& R,
		set_p<P>& N,
		V& gn)
  {
    // Count the number of conected components of the border of R.
    image2d<int> tmp;
    unsigned n;
    labeling::level(convert::to_image(N), true, c8(), tmp, n);

    if (n > 1)
      {
//   IF number of conected components of the border > 1

//       follow each border to find which is the exterior border
//       and which are the holes. Keep one pixel of each holes.
	      
//       Remove from N border of holes.
//       Recompute gn <- min u(x) x belongs to A
      }
	      
//    g <- gn
//    A <- {x belongs to N / u(x) == g}
//    N <- N\{x belongs to N / u(x) == g}

  }


  /// IF g == gn.
  template <typename V, typename P>
  void step4_2 (const image2d<V>& ima,
		set_p<P>& A,
		set_p<P>& N,
		V& g)
  {

    //    A <- {x belongs to N / u(x) == g}
    //    N <- N\{x belongs to N / u(x) == g}

    set_p<P> N_tmp (N);

    A.clear();
    N.clear();
    mln_piter(set_p<P>) p(N_tmp);
    for_all(p)
      {
	if (ima (p) == g)
	  {
	    A.insert(p);
	    N.insert(p);
	  }
      }
  }

  /// IF g > gn.
  template <typename V, typename P>
  void step4_3 (image2d<V>& u,
		set_p<P>& R,
		V& g)
  {
    //    set the gray-level of the pixels of R to g.
    mln_piter(set_p<P>) p(R);
    for_all(p)
      {
	mln_assertion (tagged(p));
	u (p) = g;
      }
  }





  template <typename V>
  void compute_level_set(const image2d<V>& ima)
  {
    typedef point2d P;
    typedef image2d<V> I;
    typedef     mln::image_if<
      mln::image2d<V>,
      mln::fun::greater_p2b_expr_<mln::pw::value_<mln::image2d<V> >,
      mln::pw::cst_<V> >
      > I_IF;


    set_p<P> R, N, A;
    V g, gn;
    point2d x0;
    image2d<V> min_locals(ima.domain());
    //    image2d<V> u = clone(ima);
    image2d<bool> tagged(ima.domain());

    unsigned nlabels;
    labeling::regional_minima(ima, c4(), min_locals, nlabels);

    debug::println(min_locals);
    debug::println(min_locals | (pw::value(min_locals) > pw::cst(0)));

    /// Algorithm.
    {
      mln_piter(I_IF) p((min_locals | (pw::value(min_locals) > pw::cst(0))).domain());
      for_all(p)
	{
	  if (tagged(p))
	    continue;

	  step1(ima, p, g, x0);
	  step2(A, R, N, x0);
	  while (1)
	    {
	      step3(ima, tagged, A, R, N, gn);
	      /// step4.
	      if (g < gn)
		{
// 		  step4_1();
		  // GO TO 3)
		  continue;
		}
	      
	      
	      if (g == gn)
		{
		  step4_2(ima, A, N, g);
		  // GO TO 3)
		  continue;
		}
	      
	      
	      if (g > gn)
		{
		  /// FIXME ima -> u
		  // 		  step4_3(ima, R, g);
 		  //    GO TO 1)
		  break;
		}
	    }
	}
    }
  }

} // end of namespace mln



#endif // ! MLN_LEVEL_MEDIAN_HH
