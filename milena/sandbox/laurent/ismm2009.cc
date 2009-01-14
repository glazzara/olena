#include <vector>
#include <algorithm>

#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/util/ord_pair.hh>
#include <mln/debug/println.hh>

#include <mln/core/routine/extend.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/compute.hh>

#include <mln/accu/count.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/meyer_wst.hh>



namespace mln
{

  // Functions.

  inline
  bool is_row_odd(const point2d& p)
  {
    return p.row() % 2;
  }

  inline
  bool is_edge(const point2d& p)
  {
    return p.row() % 2 + p.col() % 2 == 1;
  }

  inline
  bool is_pixel(const point2d& p)
  {
    return p.row() % 2 == 0 && p.col() % 2 == 0;
  }

  inline
  bool is_point(const point2d& p)
  {
    return p.row() % 2 && p.col() % 2;
  }
  

  // Neighborhoods.

  typedef neighb< win::multiple<window2d, bool(*)(const point2d&)> > dbl_neighb2d;
  
  const dbl_neighb2d& e2p() // Edge (1D face) to neighboring original pixels (2D faces).
  {
    static bool e2p_h[] = { 0, 1, 0,
			    0, 0, 0,
			    0, 1, 0 };
    static bool e2p_v[] = { 0, 0, 0,
			    1, 0, 1,
			    0, 0, 0 };
    static dbl_neighb2d nbh = make::double_neighb2d(is_row_odd, e2p_h, e2p_v);
    return nbh;
  }

  const dbl_neighb2d& e2e() // Edge to neighboring edges.
  {
    static bool e2e_h[] = { 0, 0, 1, 0, 0,
			    0, 1, 0, 1, 0,
			    0, 0, 0, 0, 0,
			    0, 1, 0, 1, 0,
			    0, 0, 1, 0, 0 };
    static bool e2e_v[] = { 0, 0, 0, 0, 0,
			    0, 1, 0, 1, 0,
			    1, 0, 0, 0, 1,
			    0, 1, 0, 1, 0,
			    0, 0, 0, 0, 0 };
    static dbl_neighb2d nbh = make::double_neighb2d(is_row_odd, e2e_h, e2e_v);
    return nbh;
  }


  inline
  point2d p1_from_e(const point2d& e)
  {
    return e + (is_row_odd(e) ? up : left);
  }

  inline
  point2d p2_from_e(const point2d& e)
  {
    return e + (is_row_odd(e) ? down : right);
  }


  // Transform to make room for edges.

  template <typename T>
  image2d<T>
  add_edges(const image2d<T>& input)
  {
    image2d<T> output(2 * input.nrows() - 1,
		      2 * input.ncols() - 1);
    data::fill(output, 0); // Useless but for display!
    for (int row = 0; row < input.nrows(); ++row)
      for (int col = 0; col < input.ncols(); ++col)
	opt::at(output, 2 * row, 2 * col) = opt::at(input, row, col);
    return output;
  }


  template <typename I, typename N>
  mln_concrete(I)
  magnitude(const I& input, const N& nbh)
  {
    mln_concrete(I) output;
    initialize(output, input);
    data::fill(output, 0);

    mln_piter(I) p(input.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      n.start();
      mln_value(I) v1 = input(n);
      n.next();
      mln_value(I) v2 = input(n);
      output(p) = v2 > v1 ? v2 - v1 : v1 - v2;
    }

    return output;
  }


  template <typename I, typename N>
  mln_ch_value(I, util::ord_pair<mln_value(I)>)
  adjacency(const I& input, const N& nbh)
  {
    typedef mln_value(I) L;
    typedef util::ord_pair<L> LL;

    mln_ch_value(I, LL) output;
    initialize(output, input);

    mln_piter(I) p(input.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      if (input(p) == 0) // Watershed edge.
	{
	  L l1 = 0, l2 = 0;
	  for_all(n)
	    if (input.has(n) && input(n) != 0)
	      {
		if (l1 == 0) // First label to be stored.
		  l1 = input(n);
		else
		  if (input(n) != l1 && l2 == 0) // Second label to be stored.
		    l2 = input(n);
		  else
		    mln_invariant(input(n) == l1 || input(n) == l2);
	      }
	  mln_invariant(l1 != 0 && l2 != 0);
	  output(p) = LL(l1, l2);
	}
      else
	{
	  L l = input(p);
	  output(p) = LL(l, l);
	  // Tests:
	  for_all(n)
	    if (input.has(n))
	      mln_invariant(input(n) == 0 || input(n) == l);
	}
    }   

    return output;
  }




  // Get the smallest edge out of a basin.
  //
  // Version with the watershed extended to all faces.

  template <typename W, typename N, typename G>
  std::vector<mln_site(W)> // FIXME: Use p_array!
  smallest_edges(const W& wst, unsigned nlabels,
		 const N& nbh, // edge (1D-face) -> pixels (2D-faces)
		 const G& g)
  {
    typedef mln_value(W) L;
    std::vector<mln_site(W)> edge(nlabels + 1);
    std::vector<L> g_min(nlabels + 1);
    std::fill(g_min.begin(), g_min.end(), mln_max(mln_value(G)));
    mln_piter(W) e(wst.domain());
    mln_niter(N) n(nbh, e);
    for_all(e)
      {
	mln_invariant(wst(e) == 0); // Watershed line only.
	n.start();
	L l1 = wst(n);
	n.next();
	L l2 = wst(n);
	if (g(e) < g_min[l1])
	  {
	    g_min[l1] = g(e);
	    edge[l1] = e;
	  }
	if (g(e) < g_min[l2])
	  {
	    g_min[l2] = g(e);
	    edge[l2] = e;
	  }
      }
    return edge;
  }



  // Get the smallest edge out of a basin.
  //
  // Version with the watershed on edges only (not extended to other faces).
  // This is an ALTERNATE version (just to test that we get the same result
  // as the "regular" version given above).

  template <typename W, typename N, typename G>
  std::vector<mln_site(W)>
  smallest_edges_alt(const W& wst, unsigned nlabels,
		     const N& nbh, // edge -> edges
		     const G& g)
  {
    typedef mln_value(W) L;
    std::vector<mln_site(W)> edge(nlabels + 1);
    std::vector<L> g_min(nlabels + 1);
    std::fill(g_min.begin(), g_min.end(), mln_max(mln_value(G)));
    mln_piter(W) e(wst.domain());
    mln_niter(N) n(nbh, e);
    for_all(e) if (wst(e) == 0) // Watershed edge.
      {
	L l1 = 0, l2 = 0;
	for_all(n)
	  if (wst.has(n) && wst(n) != 0)
	    {
	      if (l1 == 0) // First label.
		l1 = wst(n);
	      else
		if (wst(n) != l1 && l2 == 0) // Second label.
		  l2 = wst(n);
		else
		  mln_invariant(wst(n) == l1 || wst(n) == l2);
	    }
	mln_invariant(l1 != 0 && l2 != 0);
	if (g(e) < g_min[l1])
	  {
	    g_min[l1] = g(e);
	    edge[l1] = e;
	  }
	if (g(e) < g_min[l2])
	  {
	    g_min[l2] = g(e);
	    edge[l2] = e;
	  }
      }
    return edge;
  }



  // Sort attributes.

  template <typename A, typename L>
  std::vector< std::pair<A,L> >
  sort_attributes(const util::array<A>& a, L n_basins)
  {
    typedef std::pair<A,L> pair_t;
    std::vector<pair_t> v(n_basins.next());

    v[0] = pair_t(mln_min(A), 0); // First elt, even after sorting.

    for (L l = 1; l <= n_basins; ++l)
      v[l] = pair_t(a[l], l);

    std::sort(v.begin(), v.end());

    return v;
  }


  // Find root.

  template <typename L>
  inline
  L find_root(std::vector<L>& par, L l)
  {
    if (par[l] == l)
      return l;
    else
      return par[l] = find_root(par, par[l]);
  }



  // Display.

  template <typename I>
  I display_edge(const I& ima, mln_value(I) bg, unsigned zoom)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
	     ncols * (zoom + 1) - 1);
    data::fill(output, bg);
    mln_VAR( edge, ima | is_edge );
    mln_piter(edge_t) p(edge.domain());
    for_all(p)
      if (p.row() % 2) // horizontal edge
	{
	  unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
	  unsigned col = (p.col() / 2) * (zoom + 1);
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row, col + i) = ima(p);
	}
      else // vertical edge
	{
	  unsigned row = (p.row() / 2) * (zoom + 1);
	  unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row + i, col) = ima(p);
	}
    return output;
  }


} // end of namespace mln



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
  debug::println("raw_f:", raw_f);

  image2d<int_u8> f_ = add_edges(raw_f);
  debug::println("f_:", f_);

  mln_VAR(f, f_ | is_pixel);
  debug::println("f:", f);

  mln_VAR(g, f_ | is_edge);
  data::fill(g, magnitude(extend(f_ | is_edge, pw::value(f_)),
			  e2p()));
  debug::println("g:", g);

  // surprise:
  debug::println("g without the 'edge' predicate:", g.unmorph_());

  
  typedef label_8 L; // Type of labels.


  L n_regmins;
  mln_VAR( regmin_g,
	   labeling::regional_minima(g, e2e(), n_regmins) );
  debug::println("regmin(g):", regmin_g);

  L n_basins;
  mln_VAR( wst_g,
	   morpho::meyer_wst(g, e2e(), n_basins) );
  mln_invariant(n_basins == n_regmins);
  debug::println("wst(g):", wst_g);


  // Just to see things.
  mln_VAR(adj, adjacency(wst_g, e2e()));
  debug::println("adjacency:", adj | (pw::value(wst_g) == pw::cst(0)));


  /* // Délire!
  {
    box2d b = make::box2d(1,1, n_basins, n_basins);
    point2d null(0, 0);
    image2d<point2d> adj_edge(b);
    data::fill(adj_edge, null);

    mln_piter_(adj_t) e(adj.domain());
    for_all(e)
      if (adj(e).first() != adj(e).second())
	adj_edge.at_(adj(e).first(), adj(e).second()) = e;

    debug::println(adj_edge);
  }
  */


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



  // Get the smallest esge out of every basin.

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
