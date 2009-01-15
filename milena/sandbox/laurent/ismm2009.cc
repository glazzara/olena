#include "ismm2009.hh"

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/util/ord_pair.hh>
#include <mln/debug/println.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/labeling/compute.hh>
#include <mln/accu/count.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::cerr << "Laurent ISMM 2009 scheme." << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::label_8;

  if (argc != 2)
    usage(argv);

  image2d<int_u8> raw_f;
  io::pgm::load(raw_f, argv[1]);

  image2d<int_u8> f_ = add_edges(raw_f);

  mln_VAR(f, f_ | is_pixel);
  // debug::println("f:", f);

  mln_VAR(g, f_ | is_edge);

  typedef label_8 L; // Type of labels.

  L n_basins;
  mln_VAR( wst_g,
	   f_to_wst_g(f, g, e2p(), e2e(), n_basins) );

  // debug::println("g:", g);
  debug::println("wst(g):", wst_g);



  // Just to see things.
  mln_VAR(adj, adjacency(wst_g, e2e()));
  debug::println("adjacency:", adj | (pw::value(wst_g) == pw::cst(0)));



  image2d<L> wst_g_full = wst_g.unmorph_();
  {
    // edges (1D-faces) -> pixels (2D-faces)
    mln_VAR(w_pixels, wst_g_full | is_pixel);
    data::paste(morpho::dilation(extend(w_pixels, pw::value(wst_g_full)),
				 c4().win()),
		wst_g_full);
    // edges (1D-faces) -> points (0D-faces)
    mln_VAR(w_points, wst_g_full | is_point);
    data::paste(morpho::erosion(extend(w_points, pw::value(wst_g_full)),
				c4().win()),
		wst_g_full);
  }
  debug::println("wst(g) fully valuated:", wst_g_full);


  // Depict the watershed line.
  mln_VAR(is_line, pw::value(wst_g_full) == pw::cst(0));
  debug::println("wst(g) line:", wst_g | is_line);



  // Get the smallest edge out of every basin.

  std::vector<point2d> edge = smallest_edges(extend(wst_g | is_line, wst_g_full),
					     n_basins, e2p(), g);
  for (L l = 1; l <= n_basins; ++l)
    std::cout << int(l) << ": " << edge[l] << std::endl;


//   {
//     // Test the result with an alternate code.
//     std::vector<point2d> edge_alt = smallest_edges_alt(wst_g, n_basins, e2e(), g);
//     for (L l = 1; l <= n_basins; ++l)
//       mln_invariant(edge_alt[l] == edge[l]);
//   }



  // Compute an attribute per region.

  typedef unsigned A;
  util::array<A> a = labeling::compute(accu::meta::count(),
				       g,
				       wst_g,
				       n_basins);

  typedef std::pair<A,L> pair_t;
  std::vector<pair_t> v = sort_attributes(a, n_basins); // Sort regions.


  std::cout << "attributes =  ";
  for (unsigned i = 1; i <= n_basins; ++i)
    std::cout << v[i].first   // Attribute (increasing).
	      << ':'
	      << v[i].second  // Region label.
	      << " - ";
  std::cout << std::endl;


  std::vector<L> lpar(n_basins.next());
  for (L l = 1; l <= n_basins; ++l)
    lpar[l] = l; // Make-set.


  util::array<A> a_merged = a;
  for (unsigned i = 1; i <= n_basins; ++i)
    {
      L l = v[i].second, // Region label.
	lr = find_root(lpar, l);

      point2d e = edge[l]; // FIXME: Use the heap!

      mln_invariant(wst_g_full(p1_from_e(e)) == l ||
		    wst_g_full(p2_from_e(e)) == l);
      L l2 = ( wst_g_full(p1_from_e(e)) == l ?
	       wst_g_full(p2_from_e(e)) :
	       wst_g_full(p1_from_e(e)) ),
	l2r = find_root(lpar, l2);

      if (lr == l2r)
	continue; // Already merged.
      if (l2r < lr)
	std::swap(lr, l2r);
      mln_invariant(l2r > lr);
      lpar[lr] = l2r;
      a_merged[l2r] += lr; // FIXME: We want accumulators here!
    }

  for (unsigned i = 1; i <= n_basins; ++i)
    {
      L l = v[i].second;
      std::cout << l << " -> " << lpar[l] << std::endl;
    }


} // end of main




/*


for (i=0; i<V; i++) // Initialiser les heaps de fibonacci
{
  fh_setcmp(G->hp[i], cmp); // Chaque region a une heap de ses edges
}

forall edges z that separates two regions v and w
{
  fh_insert(G->hp[v], (void *)(z));   // Ajouter les edges dans les heaps
  fh_insert(G->hp[w], (void *)(z));
}

UFinit(G->V); // Initialiser l'union-find

// Parcourir les regions par attribut croissant
for (j=0; j<V-1; j++)
{
  i = find(j);

  do
    { // trouver l'edge minimum qui sorte de la region
      e = fh_extractmin(G->hp[i]);
    }
  while ((UFfind(e->v, e->w)) && (e !=NULL));

  if (e != NULL)
    { // Normalement, e != NULL, sinon c'est un BIG pb!!!
      int ui, uj, uk;
      ui = find(e->v);
      uj = find(e->w);
      uk = UFunion(e->v,e->w); // Merger les regions
      if (uk == ui)
	{ // et merger les edges
	  G->hp[ui] = fh_union(G->hp[ui], G->hp[uj]);
	}
      else
	{
	  G->hp[uj] = fh_union(G->hp[uj], G->hp[ui]);
	}
      mst[k] = *e; // Garder l'arete
      SaliencyWeight[k] = attribut[ui];// Poids dans la nouvelle hierarchie
      OldWeight[k] = e->weight; // Poids dans l'ancienne hierarchie (inutile)
      k++;
    }

  // Calcul de la sortie
  Pour toutes les edges separantes z=(x,y)
    {
	S[z] = max {SaliencyWeight[k] | sur le chemin reliant x a y dans mst}
    }
}


 */
