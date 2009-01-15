#include <vector>
#include <algorithm>

#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>

#include <mln/util/ord_pair.hh>

#include <mln/core/routine/extend.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>

#include <mln/labeling/regional_minima.hh>

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




  template < typename F,
	     typename G,
	     typename N_e2p,
	     typename N_e2e,
	     typename L >
  mln_ch_value(G, L)
  f_to_wst_g(F& f, // On pixels.
	     G& g, // On edges.
	     const N_e2p& e2p,
	     const N_e2e& e2e,
	     L& n_basins)
  {
    data::paste(morpho::gradient(extend(g, f),
				 e2p.win()),
		g);

    mln_VAR( wst_g,
	     morpho::meyer_wst(g, e2e, n_basins) );

    // Test the consistency with regional minima.
    {
      L n_regmins;
      mln_VAR( regmin_g,
	       labeling::regional_minima(g, e2e, n_regmins) );
      mln_invariant(n_basins == n_regmins);
    }

    return wst_g;
  }


} // end of namespace mln
