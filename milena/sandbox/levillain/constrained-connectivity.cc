/* The Work:

   - Prendre une image 2d
   - En doubler les pixels (un pixel -> un carré de quatre pixels)
   - En calculer un graphe d'arrête (passage en complexe cubique)
   - Valuer les arrêtes avec une magnitude de gradient (|v1 - v2|)
   - Calculer une LPE topo sur ce gradient
   - Seuiller les valeurs de cette LPE doit être équivalent aux
     alpha-CC de Pierre Soille (hypothèse de Laurent)
   
   - Puis, sur l'arbre des coupes (correspondant à la LPE topo
     calculée ci-avant), calculer pour chaque arrête `e' un attribut
     initialisé tel que `a(e) = (min(v1,v2), max(v1,v2))' (couple) où
     `v1' et `v2' sont adjacents à `e', et propager ce min et ce max
     sur tous les noeuds.
   - Il doit normalement être possible d'obtenir les (alpha,
     omega)-CC de Pierre Soille en filtrant cet arbre.  */


#include <cstdio>

#include <set>
#include <iostream>

#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>

#include <mln/pw/all.hh>

#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>

// From Théo's sandbox.
#include <theo/cplx2d.hh>

// From Alexandre Abraham's sandbox.
#include <alexandre/topo_wst.hh>


int main(int argc, char* argv[])
{
  if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " input.pbm" << std::endl;
      std::exit(1);
    }

  using namespace mln;
  using mln::value::int_u8;

  // Load an image.
  image2d<int_u8> ima;
  io::pgm::load(ima, argv[1]);

  // Double its resolution.
  image2d<int_u8> ima_x2 (10, 10);
  mln_piter_(image2d<int_u8>) p(ima_x2.domain());
  for_all(p)
  {
    /* This conversion from ``piter'' to ``point'' is required, since
       an iterator does not expose the interface of the underlying
       point (among which the methods row(), col(), etc.).  */
    point2d p_ = p;
    point2d q(p_.row() / 2, p_.col() / 2);
    ima_x2(p) = ima(q);
  }
  debug::println(ima_x2);

  // Compute the associated line graph gradient.
  mln_VAR(g, cplx2d::f_to_g(ima_x2) );
  debug::println("g:", g);

  // Compute a topological watershed transform on this gradient.
  morpho::topo_wst<g_t, cplx2d::dbl_neighb2d> tree(g, cplx2d::e2e());
  tree.go();
  mln_VAR(w, morpho::topo_watershed(tree));
  debug::println("w:", w);

  // Computing the set of values of W.
  // FIXME: Milena may provide something simpler than this.
  std::set<int_u8> values;
  mln_piter_(w_t) p2(w.domain());
  for_all(p2)
    values.insert (w(p2));

  // Thresholding for each value of W.
  for (std::set<int_u8>::iterator alpha = values.begin();
       alpha != values.end(); ++alpha)
    {
      mln_VAR(alpha_cc, w | (pw::value(w) > pw::cst(*alpha)));
      /* FIXME: There should be variants of debug::println allowing
	 the user to pass an optional ``support'' larger than the
	 actual domain of the image.  For now, use a low-level routine
	 as a workaround.  */
      std::cout << *alpha << "-cc:" << std::endl;
      debug::impl::println(w.unmorph_().domain(), alpha_cc);
    }
}
