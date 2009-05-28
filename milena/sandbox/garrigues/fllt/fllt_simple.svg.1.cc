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
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/data/fill.hh>
#include <mln/debug/println.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/accu/bbox.hh>
#include <mln/io/pgm/save.hh>


#include <mln/core/image/vmorph/cast_image.hh>

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

  */

  namespace my
  {


    void save_u(const image2d<value::int_u8>& u,
		const image2d<unsigned char>& is,
		box2d R_box)
    {
      static int id = 0;
      std::stringstream filename;
      filename << "fllt_u_" << std::setw(5) << std::setfill('0')
	       << std::right << id++ << ".ppm";

      image2d<value::int_u8> out = duplicate(u);
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

    template <typename I, typename Nbh>
    void fllt(const Image<I>& input_, const Neighborhood<Nbh>& nbh_)
    {
      const I& input = exact(input_);
      const Nbh& nbh = exact(nbh_);

      // Variables.
      I u = mln::duplicate(input);
      mln_point(I) x0;
      mln_value(I) g, gN;
      image2d<unsigned char> is(input.domain());
      image2d<bool> tagged(input.domain());
      const unsigned in_R = 255, in_N = 2, in_O = 0;

      typedef p_array<mln_point(I)> arr_t;
      arr_t* A = new arr_t();
      arr_t* N = new arr_t();

      accu::bbox<mln_point(I)> R_box, N_box;

      unsigned n_step_1 = 0, n_step_3 = 0;

      data::fill(tagged, false);
      mln_piter(I) min(input.domain());
      min.start();
      // Step 1.
    step_1:
      {
#ifdef FLLTDEBUG
	std::cout << "Step 1" << std::endl;
#endif
	++n_step_1;
	for(; min.is_valid(); min.next())
	  if (!tagged(min))
	    break;

	if (!min.is_valid())
	  goto end;

	x0 = min;
	g = input(x0);
      }

      // Step 2.
    step_2:
      {
#ifdef FLLTDEBUG
	std::cout << "Step 2" << std::endl;
#endif
	if (N_box.is_valid())
	  data::fill((is | N_box.to_result()).rw(), in_O);

	N_box.init();
	R_box.init();
	A->clear();
	A->append(x0);
	N->clear();
      }

      // Step 3.
    step_3:
      {
	++n_step_3;
#ifdef FLLTDEBUG
	std::cout << "Step 3" << std::endl;
#endif
	mln_piter(arr_t) a(*A);
	mln_niter(Nbh) x(nbh, a);

	// Stop.
	if (A->nsites() == 0)
	  goto end;

	// R <- R U A
#ifdef FLLTDEBUG
	std::cout << "Add To R : ";
#endif
	for_all(a)
	{
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
	std::cout << "points of A : " << A->nsites() << std::endl;
#endif
	mln_assertion(A->nsites() > 0);
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
	  for_all(x)
	  if (u.has(x) && is(x) == in_O && input(x) == g)
	  {
	    mln_assertion(!tagged(x));
	    N_box.take(x);
	    is(x) = in_N;
	    N->append(x);
#ifdef FLLTDEBUG
	    std::cout << x;
#endif
	  }
#ifdef FLLTDEBUG
	std::cout << std::endl;
#endif

#ifdef FLLTDEBUG
	std::cout << "g = " << g << std::endl;
#endif

	// Stop if N is empty.
	if (N->nsites() == 0)
	  goto step_1;
	else
	{
	  swap_arrays(A, N);
	  N->clear();
	  goto step_3;
	}
      }

    step_4:
      {
	// Follow N, find the holes, and browse it.
      }

    end:
      {
	std::cout << n_step_1 << ' ' << n_step_3 << std::endl;
      }
    }

  } // end of namespace mln::my

} // end of namespace mln


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;

  io::pgm::load(lena, "../../../img/lena.pgm");


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

  my::fllt(lena, c4());
  io::pgm::save(lena, "./out.pgm");

}
