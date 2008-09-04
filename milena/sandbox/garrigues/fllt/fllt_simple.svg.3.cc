// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#include <iomanip>
#include <iostream>
#include <sstream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/core/clone.hh>

#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/level/fill.hh>
#include <mln/level/compare.hh>
#include <mln/debug/println.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/accu/bbox.hh>
#include <mln/io/pgm/save.hh>

#include <mln/util/tree.hh>
#include <mln/util/tree_to_image.hh>
#include <mln/util/branch_iter_ind.hh>

#include <mln/core/image/cast_image.hh>
#include <mln/core/site_set/p_queue_fast.hh>

namespace mln
{
  // Un autre version recursive de la fllt.

  // Idee :
  // Parcourir l'image en pratant d'un coin de preference,
  // le parcour des pixels se fait dans un ordre conexe.
  //

  /*

   fonction fllt_rec(racine R, domaine D, marques M, conexite C)
   {
     pour tout les points du domaine non marquees
       recuperer la composante conexe A (en conexite C) qui inclut ce point :
            Ceci est fait avec la methode de grossissement de region
            pour avoir acces a la bordure externe et donc aux trous
            de la composante.

       Cette composante sera fille de R dans l'arbre d'inclusion.

       Marquer les points de A (Peut etre fait en meme temps que la
       construction de A)

       Pour chaque trous T de A :
         appel recursif : fllt_rec(A, T.domaine(), marques, inv(C))
         T.domaine() sera calculer en recuperant la composante connexe
         de la bordure de A lui correspondant (On recupere facilement
	 les segments horizontaux qui le composent -> parcour ok).

   }
   inv(c4) = c8
   inv(c8) = c4


    Autre version qui Marche sur les images hexagonales (Plus rapide et plus simple)
    car il y a plus le probleme du theoreme de jordan non verifie.
    FIXME: Est-ce que rendre l'image hexagonale altere vraiment le resultat?

    pour tout les points du domaine non marquees
       recuperer la composante conexe A qui inclut ce point :
            Ceci est fait avec la methode de grossissement de region
            pour avoir acces a la bordure externe et donc aux trous
            de la composante.

       Marquer les points de A (Peut etre fait en meme temps que la
       construction de A)

       Marquer les extremites des trou de A.

       Si un point de A est l'extremite d'un trou,
           alors on a facilement le pere de A (la forme qui inclue A).
       Sinon
           C'est qu'il y a une composante connexe adjacente qui
	   a deja ete construite, et qui a donc deja un pere, qui
	   est aussi celui de A.

    continuer jusqu'a avoir marquer tout les points

    Cette methode me marche que dans le cas ou les courbes de niveau corespondent
    au formes car on creer les formes en parcourant les pixels.

    Le probleme peut etre resolu en grossissant la forme en passant sur les courbes
    de niveaux qui se sitent entre les pixels.
  */

  namespace my
  {

# define fllt_tree(P, V)  mln::util::tree< fllt_node_elt<P, V> >
# define fllt_node(P, V)  mln::util::tree_node< fllt_node_elt<P, V> >
# define fllt_branch(P, V)  mln::util::branch< fllt_node_elt<P, V> >
# define fllt_branch_iter_ind(P, V)  mln::util::branch_iter_ind< fllt_node_elt<P, V> >


    template <typename P, typename V>
    struct fllt_node_elt
    {
      V	value;
      p_array<P> points;
      p_array<P> holes;
      /// Tell if his parent if brighter or not.  Nb : if the parent
      /// if brighter, the node come from the lower level set
      bool brighter;
      unsigned npoints;

    };

    template <typename P, typename V>
    void add_npoints_to(fllt_node(P, V)* node, unsigned npoints)
    {
      mln_assertion(node);

      node->elt().npoints += npoints;
      if (node->parent())
	add_npoints_to(node->parent(), npoints);
    }


    template <typename R>
    struct map_cell
    {
      R* border_of_hole_of;
      R* old_border_of_hole_of;
      R* belongs_to;

      map_cell() : border_of_hole_of(0), belongs_to(0) {}

      void restore_border()
      { border_of_hole_of = old_border_of_hole_of; }

      void set_border_of_hole_of(R* b)
      {
	old_border_of_hole_of = border_of_hole_of;
	border_of_hole_of = b;
      }
    };



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
		    << " npoints = " << (*p).elt().npoints << std::endl
		    << std::endl;

	  if ((*p).elt().points.npoints() > 0)
	    debug::println(ima | (*p).elt().points);
      	  std::cout << std::endl;
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
	  if ((*p).elt().npoints > limit)
	  {
	    mln_piter(p_array<point2d>) q((*p).elt().points);
	    for_all(q)
	      {
		output(q) = 0;
	      }
	  }
	}
    }

    void save_u(const image2d<value::int_u8>& u,
		const image2d<unsigned char>& is,
		box2d R_box)
    {
      static int id = 0;
      std::stringstream filename;
      filename << "fllt_u_" << std::setw(5) << std::setfill('0')
	       << std::right << id++ << ".ppm";

      image2d<value::int_u8> out = clone(u);
      const unsigned in_R = 255;

      mln_piter_(box2d) p(R_box);
      for_all(p)
	if (is(p) == in_R)
	  out(p) = 255;

      io::pgm::save(out, filename.str());
    }

    void swap_arrays(p_array<point2d>*& A,
		     p_array<point2d>*& N)
    {
      p_array<point2d>* tmp = A;
      A = N;
      N = tmp;
    }


    template <typename N_t>
    void clear_N(N_t& N)
    {
      for (unsigned i = 0; i < 256; ++i)
	N[i]->clear();
    }

    template <typename I, typename CC>
    void erase_exterior_border(I& map, const box2d& bbox, const CC& current_cc, const std::vector<dpoint2d> nbh[])
    {
      typedef const std::vector<dpoint2d> arr_dp_t;
      typedef std::vector<dpoint2d>::const_iterator arr_dp_t_it;
      mln_piter(I) p(bbox);
      for_all(p)
	if (map(p).border_of_hole_of == current_cc)
	{
	  //mln_assertion(map(p).belongs_to != current_cc);
	  break;
	}
      mln_assertion(map(p).belongs_to != current_cc);
      p_queue_fast<mln_point(I)> qu;
      map(p).restore_border();
      qu.push(p);
//       std::cout << std::endl;

      while(!qu.is_empty())
      {
	mln_point(I) a = qu.pop_front();
	//map(a).border_of_hole_of = 0;
	arr_dp_t* n_dp = &nbh[abs(a[1] % 2 > 0)];

// 	unsigned i = 0;
	for(arr_dp_t_it x = n_dp->begin(); x != n_dp->end(); x++)
	{
	  mln_point(I) n = mln_point(I)(a) + mln_dpoint(I)(*x);

	  if(!map.has(n))
	    continue;
	  if (map(n).border_of_hole_of == current_cc &&
	      map(n).belongs_to != current_cc)
	  {
// 	    i++;
// 	    std::cout << i << std::endl;
// 	    mln_assertion(i <= 2);
	    map(n).restore_border();
	    qu.push(n);
	    break;
	  }
	}
      }
    }

    template <typename I>
    fllt_tree(mln_point(I), mln_value(I))&
    fllt(const Image<I>& input_)
    {
      const I& input = exact(input_);

      typedef std::vector<dpoint2d> arr_dp_t;
      typedef std::vector<dpoint2d>::const_iterator arr_dp_t_it;
      typedef fllt_node(mln_point(I), mln_value(I)) node_type;
      typedef fllt_tree(mln_point(I), mln_value(I)) tree_type;

      // C6 neigboohood.
      arr_dp_t neighb_c6[2];
      neighb_c6[0].push_back(make::dpoint2d(-1,-1));
      neighb_c6[0].push_back(make::dpoint2d(-1,0));
      neighb_c6[0].push_back(make::dpoint2d(-1,1));
      neighb_c6[0].push_back(make::dpoint2d(0,1));
      neighb_c6[0].push_back(make::dpoint2d(1,0));
      neighb_c6[0].push_back(make::dpoint2d(0,-1));

      neighb_c6[1].push_back(make::dpoint2d(-1,0));
      neighb_c6[1].push_back(make::dpoint2d(0,1));
      neighb_c6[1].push_back(make::dpoint2d(1,1));
      neighb_c6[1].push_back(make::dpoint2d(1,0));
      neighb_c6[1].push_back(make::dpoint2d(1,-1));
      neighb_c6[1].push_back(make::dpoint2d(0,-1));

      // Variables.
      image2d<map_cell<node_type> > map(input.domain().to_larger(1));
      I u = mln::clone(input);
      mln_point(I) x0;
      mln_value(I) g, gN;
      image2d<unsigned char> is(input.domain().to_larger(1));
      image2d<bool> tagged(input.domain().to_larger(1));
      const unsigned in_R = 255, in_N = 2, in_O = 0;
      node_type* root = new node_type();
      node_type* current_cc;
      node_type* current_parent = root;

      typedef p_array<mln_point(I)> arr_t;
      arr_t* A = new arr_t();
      arr_t* N[256];
      for (unsigned i = 0; i < 256; ++i)
	N[i] = new arr_t();

      accu::bbox<mln_point(I)> R_box, N_box;

      unsigned n_step_1 = 0, n_step_3 = 0;
      unsigned cc_cpt = 0;
      bool parent_found = false;

      level::fill(tagged, false);
      mln_piter(I) min(input.domain());
      min.start();
      // Step 1.
    step_1:
      {
#ifdef FLLTDEBUG
	std::cout << "Step 1" << std::endl;
#endif
	++n_step_1;
	for(;min.is_valid(); min.next())
	  if (!tagged(min))
	    break;

	if (!min.is_valid())
	  goto end;

	x0 = min;
	g = input(x0);
	current_cc = new node_type();
	++cc_cpt;
	current_cc->elt().value = g;
	current_cc->elt().npoints = 0;
	if (cc_cpt > 1)
	  current_parent = 0;
#ifdef FLLTDEBUG
	std::cout << "current_cc: " << current_cc << std::endl;
#endif
      }

      // Step 2.
    step_2:
      {
#ifdef FLLTDEBUG
	std::cout << "Step 2" << std::endl;
#endif
	if (N_box.is_valid())
	  level::fill(inplace(is | N_box.to_result()), in_O);

	N_box.init();
	R_box.init();
	A->clear();
	A->append(x0);
	clear_N(N);

	mln_assertion(map(x0).belongs_to == 0);
	if (map(x0).border_of_hole_of != 0)
	{
	  mln_assertion(map(x0).border_of_hole_of != current_cc);
	  current_parent = map(x0).border_of_hole_of;
// 	  std::cout << "propagation : " << x0 << std::endl;
	}
      }

      // Step 3.
    step_3:
      {
	++n_step_3;
#ifdef FLLTDEBUG
	std::cout << "Step 3" << std::endl;
#endif
	mln_piter(arr_t) a(*A);

	// Stop.
	if (A->npoints() == 0)
	  goto end;

	// R <- R U A
#ifdef FLLTDEBUG
	std::cout << "Add To R : ";
#endif
	for_all(a)
	{
	  mln_assertion(map(a).belongs_to == 0);
	  map(a).belongs_to = current_cc;
	  current_cc->elt().points.append(a);
	  current_cc->elt().npoints++;
	  tagged(a) = true;
	  is(a) = in_R;
#ifdef FLLTDEBUG
	  std::cout << a;
#endif
	}
#ifdef FLLTDEBUG
	std::cout << std::endl;
#endif


#ifdef FLLTDEBUG
	std::cout << "points of A : " << A->npoints() << std::endl;
#endif
	mln_assertion(A->npoints() > 0);
	R_box.take(A->bbox());
	mln_assertion(R_box.is_valid());

#ifdef FLLTTRACE
	save_u(u, is, R_box);
#endif
	// N <- N U { x in nbh of A and not in R / input(x) == g}
#ifdef FLLTDEBUG
	std::cout << "Add To N : ";
#endif
	for_all(a)
	  {
	    arr_dp_t* nbh = &neighb_c6[a[1] % 2];
	    for(arr_dp_t_it x = nbh->begin(); x != nbh->end(); x++)
	      {
		mln_point(I) n = mln_point(I)(a) + mln_dpoint(I)(*x);
		if (is(n) == in_O)
		{
		  if (!current_parent)
		  {
		    if (map(n).border_of_hole_of != 0 &&
			map(n).border_of_hole_of != map(n).belongs_to)
		    {
		      mln_assertion(map(n).border_of_hole_of != current_cc);
		      current_parent = map(n).border_of_hole_of;
// 		      std::cout << "propagation : " << n << std::endl;
		    }
		    else if (map(n).belongs_to)
		    {
		      current_parent = map(n).belongs_to->parent();
		      mln_assertion(current_parent != current_cc);
		    }
		  }
		  map(n).set_border_of_hole_of(current_cc);
		  N_box.take(n);
		  is(n) = in_N;
		  if (u.has(n))
		    N[u(n)]->append(n);
#ifdef FLLTDEBUG
		  std::cout << n;
#endif
		}
	      }
	  }
#ifdef FLLTDEBUG
	std::cout << std::endl;
#endif

#ifdef FLLTDEBUG
	std::cout << "g = " << g << std::endl;
#endif

	// Stop if N[g] is empty.
	if (N[g]->npoints() == 0)
	{
	  add_npoints_to(current_parent, current_cc->elt().npoints);
	  // Was : current_parent->elt().npoints += current_cc->elt().npoints;
	  current_cc->set_parent(current_parent);
	  erase_exterior_border(map, N_box, current_cc, neighb_c6);
	  goto step_1;
	}
	else
	{
	  // CC is growing.
	  swap_arrays(A, N[g]);
	  N[g]->clear();
	  goto step_3;
	}
      }

    end:
      {
	std::cout << "n_step_1 : " << n_step_1
		  << " n_step_3 : " << n_step_3
		  << " number of cc : " << cc_cpt  << std::endl;
      }

      return *new tree_type(root);
    }

  } // end of namespace mln::my

} // end of namespace mln


int main()
{
  using namespace mln;
  using namespace mln::my;
  using value::int_u8;

  typedef image2d<int_u8> I;
  typedef fllt_node(mln_point_(I), mln_value_(I)) node_type;
  typedef fllt_tree(mln_point_(I), mln_value_(I)) tree_type;

//   image2d<int_u8> ima;
//   //io::pgm::load(ima, "../../../img/lena.pgm");
//   io::pgm::load(ima, "../tapis.pgm");


  //   int_u8 vs[9][9] = {

  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
  //     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },

  //   };

  //image2d<int_u8> lena = make::image2d(vs);

//   int vs[3][6] = { {0, 0, 0, 1, 1, 1},
// 		   {0, 1, 0, 1, 0, 1},
// 		   {0, 0, 0, 1, 1, 1} };

//   int vs[5][5] = { {100, 100, 100, 100, 100},
//                    {100, 200, 255, 200, 100},
//                    {100, 200, 200, 200, 100},
//                    {100, 200, 255, 200, 100},
//                    {100, 100, 100, 100, 100} };

  int vs[9][9] = { {2,2,2,2,2,2,2,2,2},
		   {2,2,2,2,2,2,2,2,2},
		   {2,1,1,1,1,1,1,1,2},
		   {2,1,2,2,1,0,0,1,2},
		   {2,1,2,2,1,0,0,1,2},
		   {2,1,2,2,1,0,0,1,2},
		   {2,1,1,1,1,1,1,1,2},
		   {2,1,1,1,1,1,1,1,2},
		   {2,2,2,2,2,2,2,2,2} };

//   int vs[7[7] = { {101, 101, 101, 191, 204, 255, 191},
// 		  {101, 101, 191, 204, 204, 204, 204},
// 		  {101, 191, 191, 204, 255, 204, 191},
// 		  {204, 204, 204, 191, 191, 191,  76},
// 		  {191, 191, 204, 191, 101, 101,  76},
// 		  {204, 204, 191, 204, 101,  76,  76},
// 		  {191, 191, 191, 101,  76, 101,   0}};


//   int vs[2][1] = { {121}, {101} };

  image2d<int> ima_(make::image2d(vs));
  image2d<int_u8> ima(ima_.domain());
  level::fill(ima, ima_);

  tree_type tree = my::fllt(ima);


  draw_tree(ima, tree);



  image2d<value::int_u8> output (ima.domain ());
  util::tree_to_image (tree, output);
  mln_assertion(output == ima);
  io::pgm::save(output, "out.pgm");

  image2d<value::int_u8> bounds (ima.domain ());
  visualize_bounds(bounds, tree, 2);
  io::pgm::save(bounds, "bounds.pgm");

}
