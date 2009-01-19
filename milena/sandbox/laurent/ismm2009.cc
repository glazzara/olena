#include "ismm2009.hh"

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>

#include <mln/io/pgm/load.hh>
#include <mln/util/ord_pair.hh>
#include <mln/debug/println.hh>
#include <mln/core/routine/duplicate.hh>

# include <mln/core/site_set/p_set.hh>
# include <mln/core/site_set/p_queue.hh>
# include <mln/core/site_set/p_priority.hh>

#include <mln/labeling/compute.hh>
#include <mln/accu/count.hh>


/*

   TO-DO list:
   -----------

   - having a heap for every lr (support merge)

   - handling adjacency on the fly (instead of having an image)

 */


namespace mln
{


  // Region label parenthood routines.


  template <typename L>
  inline
  void make_sets(std::vector<L>& lpar, L l_max)
  {
    for (L l = 1; l <= l_max; ++l)
      lpar[l] = l; // Make-set.
  }

  template <typename L>
  inline
  L find_root_l(std::vector<L>& lpar, L l)
  {
    if (lpar[l] == l)
      return l;
    else
      return lpar[l] = find_root_l(lpar, lpar[l]);
  }



  // Edge tree.


  template <typename I>
  inline
  mln_psite(I)
  find_root_e(I& z_epar, mln_psite(I) e)
  {
    if (z_epar(e) == e)
      return e;
    else
      return z_epar(e) = find_root_e(z_epar, z_epar(e));
  }




  // Test emptiness of the queue of a set of regions.

  template <typename Qs, typename L>
  bool test_q_emptiness(Qs& qs, L l, std::vector<L>& lpar)
  {
    L l_ = l;
    while (lpar[l_] != l_)
      {
	if (! qs[l_].is_empty())
	  return false;
	l_ = lpar[l_];
      }
    return true;
  }


  // Get smallest edge.

  template <typename Qs, typename L, typename W>
  mln_deduce(Qs, element, element)
  get_smallest_edge(Qs& qs, L l, const W& wst, std::vector<L>& lpar)
  {
    typedef mln_element(Qs) Q; // qs is an array of queues with type Q.

    // Test that, when a region has merged, its edge queue has been
    // emptied.
    mln_invariant(test_q_emptiness(qs, l, lpar));

    L lr = find_root_l(lpar, l);
    Q& q = qs[lr];

    typedef mln_element(Q) E;

    while (! q.is_empty())
      {
	E e = q.pop_front();

	L l1 = wst(p1_from_e(e)),
	  l2 = wst(p2_from_e(e));
	mln_invariant(l1 != l2);
	L l1r = find_root_l(lpar, l1),
	  l2r = find_root_l(lpar, l2);
      
	mln_invariant(l1r == lr || l2r == lr);

	if (l1r == l2r)
	  // 'e' is an internal edge => forget it.
	  continue;

	return e;
      }

    mln_invariant(0); // We should not be here!

    return E(0,0); // FIXME: null edge.
  }


  // Compute an LCA; reference(?) code.

  template <typename L, typename E, typename Epar>
  std::vector< std::vector<E> >
  compute_lca(const L& l_max,
	      const std::vector<E>& edge,
	      const Epar& epar)
  {
    mln_ch_value(Epar, std::vector<L>) chl_; // Children (known as array).
    {
      initialize(chl_, epar);
      E e;
      for (L l = 1; l <= l_max; ++l)
	{
	  e = edge[l];
	  do
	    {
	      chl_(e).push_back(l);
	      e = epar(e);
	    }
	  while (e != epar(e));
	  chl_(e).push_back(l);
	}
      // e is the root edge.
      mln_invariant(chl_(e).size() == l_max); // All labels are children.
    }

    /*
    // Display children tree.
    {
      std::cout << "chl_ tree: " << std::endl;
      for (L l = 1; l <= l_max; ++l)
	{
	  E e_ = edge[l];
	  std::cout << "l = " << l << "  =>  ";
	  do
	    {
	      std::cout << e_ << " : ";
	      for (unsigned i = 0; i < chl_(e_).size(); ++i)
		std::cout << chl_(e_)[i] << ' ';
	      std::cout << " ->  ";
	      e_ = epar(e_);
	    }
	  while (epar(e_) != e_);
	  std::cout << e_ << " : ";
	  for (unsigned i = 0; i < chl_(e_).size(); ++i)
	    std::cout << chl_(e_)[i] << ' ';
	  std::cout << std::endl;
	}
    }
    */

    mln_ch_value(Epar, std::set<L>) chl; // Children (as a set).
    {
      initialize(chl, epar);
    
      mln_piter(Epar) e(chl.domain());
      for_all(e)
	if (chl_(e).size() != 0)
	  chl(e).insert(chl_(e).begin(), chl_(e).end());
    }

    unsigned size = l_max.next();

    std::vector< std::vector<E> > lca(size);
    for (L l = 1; l <= l_max; ++l)
      {
	lca[l].resize(size);

	// Diagonal.
	lca[l][l] = edge[l]; // Itself.

	// Superior right.
	for (L l2 = l.next(); l2 <= l_max; ++l2)
	  {
	    E e = edge[l];
	    while (chl(e).find(l2) == chl(e).end())
	      e = epar(e);
	    lca[l][l2] = e;
	}
      }

    return lca;
  }




  // Transform attributes so that they are all different!
 

  template <typename A, typename L>
  void
  make_unique_attribute(util::array<A>& a,
			std::vector< std::pair<A,L> >& v,
			L l_max,
			bool echo)
  {
    // Display attributes.
    if (echo)
      {
	std::cout << "(attribute, label)  =  { ";
	for (unsigned i = 1; i <= l_max; ++i)
	  std::cout << '(' << v[i].first   // Attribute (increasing).
		    << ','
		    << v[i].second  // Region label.
		    << ") ";
	std::cout << '}' << std::endl
		  << std::endl;
      }

    std::map<A,A> lut; // old value -> new value
    for (unsigned l = 1; l <= l_max; ++l)
      {
	A old_a = v[l].first,
	  new_a = old_a + l - 1;
	lut[old_a] = new_a;
	v[l].first = new_a; // new value
      }
    for (unsigned l = 1; l <= l_max; ++l)
      a[l] = lut[ a[l] ];
  }



  // Compute g from f; then transform g into g' so that all values are
  // different; last return the watershed of g'.


  template < typename F,
	     typename G,
	     typename N_e2p,
	     typename N_e2e,
	     typename L >
  mln_ch_value(G, L)
  f_to_wst_unique_g(F& f, // On pixels.
		    G& g, // On edges.
		    const N_e2p& e2p,
		    const N_e2e& e2e,
		    L& n_basins,
		    bool echo)
  {
    data::paste(morpho::gradient(extend(g, f),
				 e2p.win()),
		g);

    // FIXME: Here, we want a unique value per edge!

    if (echo)
      debug::println("g (before):", g);

    {

      typedef std::pair<short, short> edge_t;
      typedef std::pair<mln_value(G), edge_t> ve_t;
      std::vector<ve_t> tmp;
      {
	mln_piter(G) e(g.domain());
	for_all(e)
	{
	  ve_t ve;
	  ve.first  = g(e);
	  ve.second = edge_t(e.row(), e.col());
	  tmp.push_back(ve);
	}
      }
      std::sort(tmp.begin(), tmp.end());
      {
	mln_value(G) v;
	mln_site(G) e;
	for (unsigned i = 0; i < tmp.size(); ++i)
	  {
	    v = tmp[i].first + i;
	    e.row() = tmp[i].second.first;
	    e.col() = tmp[i].second.second;
	    g(e) = v;
	  }
      }
    }

    mln_VAR( wst_g,
	     morpho::meyer_wst(g, e2e, n_basins) );

//     // Test the consistency with regional minima.
//     {
//       L n_regmins;
//       mln_VAR( regmin_g,
// 	       labeling::regional_minima(g, e2e, n_regmins) );
//       mln_invariant(n_basins == n_regmins);
//     }

    return wst_g;
  }


} // mln




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
  using value::label_16;

  bool echo = true;

  if (argc != 2)
    usage(argv);

  image2d<int_u8> raw_f;
  io::pgm::load(raw_f, argv[1]);

  typedef image2d<int_u8> Complex;

  typedef mln_pset_(Complex) full_D;
  Complex f_ = add_edges(raw_f);

  mln_VAR(f, f_ | is_pixel);
  if (echo)
    debug::println("f:", f);

  mln_VAR(g, f_ | is_edge);

  typedef mln_value_(g_t) T; // Type of edge values.
  typedef mln_psite_(g_t) E; // Type of edges.
  typedef label_16 L;        // Type of labels.

  L n_basins;

  mln_VAR( wst_g,
	   f_to_wst_g(f, g, e2p(), e2e(), n_basins) );

  /*
    UNIQUE:

  mln_VAR( wst_g,
	   f_to_wst_unique_g(f, g, e2p(), e2e(), n_basins, echo) );
  */


  if (echo)
    {
      std::cout << "n basins = " << n_basins << std::endl;
      debug::println("g:", g);
      debug::println("wst(g):", wst_g);
    }


  // Just to see things.

  mln_VAR(adj, adjacency(wst_g, e2e()));
  if (echo)
    debug::println("adj:", adj);

  mln_VAR(adj_line, adj | (pw::value(wst_g) == pw::cst(0)));
  if (echo)
    debug::println("adjacency:", adj_line);



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


  // Depict the watershed line.
  mln_VAR(is_line, pw::value(wst_g_full) == pw::cst(0));
  if (echo)
    debug::println("wst(g) line:", wst_g | is_line);

  mln_VAR(g_line, g | is_line);
  if (echo)
    debug::println("g | wst(g) line:", g_line);





  // Priority queue -> edge.

  typedef p_priority< T, p_queue<E> > Q;
  util::array<Q> q(n_basins.next());

  {
    mln_piter_(g_t) e(g.domain());
    for_all(e)
      if (wst_g(e) == 0)
	{
	  L l1 = wst_g_full(p1_from_e(e)),
	    l2 = wst_g_full(p2_from_e(e));
	  if (l1 > l2)
	    std::swap(l1, l2);
	  mln_invariant(l1 < l2);
	  q[l1].push(mln_max(T) - g(e), e);
	  q[l2].push(mln_max(T) - g(e), e);
	}
    // --- for (L l = 1; l <= n_basins; ++l)
    // ---   std::cout << "q["<< l << "] = " << q[l] << std::endl;
  }





  // To know if an edge is out of a given region (label l), we
  // maintain the information about region merging using
  // an union-find structure.
  std::vector<L> lpar(n_basins.next());
  make_sets(lpar, n_basins);


  // Given a region R with label l and an edge e = (l1, l2) from the
  // priority queue, we get know if that edge is out of the set of
  // regions containing l:  we shall have l1r = lr or l2r = lr.

  // Please note that an edge (l1, l2) is internal to a set of regions
  // if l1r = l2r.


  
  E null = E(0,0);  // Impossible value.


  // Information "label l -> edge e".

  std::vector<E> edge(n_basins.next());
  for (L l = 1; l <= n_basins; ++l)
    edge[l] = null;







  // Compute an attribute per region.
  // --------------------------------

  typedef unsigned A;
  util::array<A> a = labeling::compute(accu::meta::count(),
				       g,
				       wst_g,
				       n_basins);

  typedef std::pair<A,L> pair_t;
  std::vector<pair_t> v = sort_attributes(a, n_basins); // Sort regions.



  // Maybe activate to test purpose:
  /*
    make_unique_attribute(a, v, n_basins, echo);
  */


  // Display attributes.
  if (echo)
    {
      std::cout << "(attribute, label)  =  { ";
      for (unsigned i = 1; i <= n_basins; ++i)
	std::cout << '(' << v[i].first   // Attribute (increasing).
		  << ','
		  << v[i].second  // Region label.
		  << ") ";
      std::cout << '}' << std::endl
		<< std::endl;
    }




  // Go!
  // --------------------------------


  mln_ch_value_(g_line_t, E)
    epar,   // Edge forest.
    z_epar; // Auxiliary data: edge forest with compression and balancing.
  {
    initialize(epar, g_line);
    initialize(z_epar, g_line);
    mln_piter_(g_line_t) e(g_line.domain());
    for_all(e)
    {
      // Make-Set.
      epar(e) = e;
      z_epar(e) = e;
    }
    if (echo)
      debug::println("all edges:", epar); // epar(e) == e so we depict the edges!
  }



  // 'aa' is the result attribute image; it is defined on the complex
  // (so it has edges, pixels, and 0-face-points).

  image2d<A> aa;
  initialize(aa, f_);
  data::fill(aa, 0); // Safety iff 0 is an invalidate attribute value!


  // The last iteration (i == n_basins) is useless: all regions have
  // already merged.

  for (unsigned i = 1; i < n_basins; ++i)
    {
      L l = v[i].second; // Region label.
      E e = get_smallest_edge(q, l, wst_g_full, lpar);

      L l1 = wst_g_full(p1_from_e(e)),
	l2 = wst_g_full(p2_from_e(e)),
	l1r = find_root_l(lpar, l1),
	l2r = find_root_l(lpar, l2);

      aa(e) = a[l];

      // Consistency tests.
      {
	mln_invariant(a[l] == v[i].first);
	mln_invariant(epar(e) == e); // 'e' has not been processed yet.
	mln_invariant(l1 != l2);     // It is a valid edge, i.e., between 2 different regions...
	mln_invariant(l1r != l2r);   // ...that are not already merged.

	L lr = find_root_l(lpar, l);
	mln_invariant(lr == l // Either l is not deja-vu
		      || ((lr == l1r && lr != l2r) || // or l belongs to l1r xor l2r.
			  (lr == l2r && lr != l1r)));
      }

      /*
      std::cout << "l = " << l
		<< "   e = " << e
		<< "   (l1, l2) = (" << l1 << ", " << l2 << ")  =>  "
		<< " merging R" << l1r << " and R" << l2r
		<< std::endl;
      */


      // Merging both regions.
      {
	if (l2r < l1r)
	  std::swap(l1r, l2r);
	mln_invariant(l2r > l1r);
	lpar[l1r] = l2r;

	/*
	std::cout << "q[l1r] = " << q[l1r] << std::endl;
	std::cout << "q[l2r] = " << q[l2r] << std::endl;
	*/

	q[l2r].insert(q[l1r]);
	q[l1r].clear();

	/*
	std::cout << "q[l2r] = " << q[l2r] << std::endl
		  << std::endl;
	*/


	/*
	// Displaying lpar.
	{
	  std::cout << "lpar =  ";
	  for (unsigned i = 1; i <= n_basins; ++i)
	    {
	      L l = v[i].second;
	      std::cout << l << "->" << lpar[l] << "  ";
	    }
	  std::cout << std::endl;
	}
	*/
      }

      E e1 = edge[l1],
	e2 = edge[l2];

      if (e1 == null && e2 == null)
	{
	  // New edge-component (singleton)
	  // Put differently: new edge-node!
	  edge[l1] = e;
	  edge[l2] = e;
	  // after:
	  //   e
	  //  / \
	  // l1 l2
	}
      else if (e1 != null && e2 != null)
	{
	  // Two trees shall merge.
	  E e1r = find_root_e(z_epar, e1),
	    e2r = find_root_e(z_epar, e2);
	  mln_invariant(e1r != e2r); // Otherwise, there's a bug!
	  // before:
	  //  e1r    e2r
	  //   |...   |...
	  //  e1     e2
	  //  / \    / \
	  // l1  . l2  .
	  epar(e1r) = e; z_epar(e1r) = e;
	  epar(e2r) = e; z_epar(e2r) = e;
	  // after:
	  //      e
	  //    /   \
	  //  e1r   e2r
	  //   |...  |...
	  //  e1    e2
	  //  / \   / \
	  // l1  . l2  .
	}
      else if (e1 != null && e2 == null)
	{
	  E e1r = find_root_e(z_epar, e1);
	  // before: 
	  //  e1r
	  //   |...
	  //  e1      null
	  //  / \     /
	  // l1  .   l2
	  epar(e1r) = e;  z_epar(e1r) = e;
	  edge[l2] = e;
	  // after:
	  //      e
	  //    /   \
	  //  e1r    l2
	  //   |...
	  //  e1
	  //  / \
	  // l1  .
	}
      else
	{
	  mln_invariant(e1 == null && e2 != null);
	  E e2r = find_root_e(z_epar, e2);
	  epar(e2r) = e;  z_epar(e2r) = e;
	  edge[l1] = e;
	}

    } // end of "for every region with increasing attribute"



  // About the "edge tree" and attributes.

  {
    // Region l -> edge e.
    p_set<E> leaves;
    for (L l = 1; l <= n_basins; ++l)
      {
	mln_invariant(edge[l] != null);
	leaves.insert(edge[l]);
      }

    // Tests.
    mln_piter_(p_set<E>) e(leaves);
    for_all(e)
    {
      E e_ = e;
      do
	{
	  mln_invariant(aa(e_) != 0 && aa(epar(e_)) != 0); // aa is valid
	  mln_invariant(aa(epar(e_)) >= aa(e_)); // edge parenthood goes with 'a' increasing
	  e_ = epar(e_);
	}
      while (epar(e_) != e_);
    }

    // Display.
    if (echo)
      {
	std::cout << "region:(edge)  =  ";
	for (L l = 1; l <= n_basins; ++l)
	  std::cout << l << ':' << edge[l] << " ";
	std::cout << std::endl
		  << std::endl;
	
	std::cout << "edge tree: " << std::endl;
	for_all(e) {
	  E e_ = e;
	  do {
	    std::cout << e_ << " -> ";
	    e_ = epar(e_);
	  }
	  while (epar(e_) != e_);
	  std::cout << e_ << std::endl;
	}
	std::cout << std::endl;
	
	std::cout << "edge tree new attributes: " << std::endl;
	for_all(e)
	{
	  E e_ = e;
	  do
	    {
	      std::cout << aa(e_) << " -> ";
	      e_ = epar(e_);
	    }
	  while (epar(e_) != e_);
	  std::cout << aa(e_) << std::endl;
	}
	std::cout << std::endl;
	
      } // end of Display.
    
    
  } // end of About the "edge tree" and attributes.



  // Testing both that all regions and all "smallest" edges have
  // merged (assumption: connected domain!).

  {
    L l_1 = v[1].second,
      l_root = find_root_l(lpar, l_1);
    mln_invariant(edge[l_1] != null);
    E e_root = find_root_e(z_epar, edge[l_1]);
    for (unsigned i = 2; i <= n_basins; ++i)
      {
	L l = v[i].second;
 	mln_invariant(find_root_l(lpar, l) == l_root);
	mln_invariant(edge[l] != null);
  	mln_invariant(find_root_e(z_epar, edge[l]) == e_root);
     }
  }


  // Reminders:
  if (echo)
    {
      debug::println("wst(g) fully valuated:", wst_g_full);
      debug::println("g_line:", g_line);
    }



  // +---------------------------------------------------------------+
  // | We want an "image" of the "merging value of a" over all edges |
  // | (not only the set of 'e' used to merge).                      |
  // +---------------------------------------------------------------+


  {

    // Dealing with the watershed line.

    mln_VAR(aa_line, aa | is_line);

    {
      // First, processing the 1D-faces (edges) of the watershed line.

      std::vector< std::vector<E> > lca;
      lca = compute_lca(n_basins, edge, epar); // lca is auxiliary data.

      mln_piter_(g_line_t) e(g_line.domain());
      for_all(e)
      {
	L l1 = adj_line(e).first(),
	  l2 = adj_line(e).second();
	mln_invariant(l1 != 0 && l2 > l1);
	E e_ = lca[l1][l2];
	mln_invariant(is_line(e_));
	mln_invariant(aa(e_) != 0); // aa is valid at e_.

	// The attribute value propagates from the lca to the current edge
	// of the line:
	aa(e) = aa(e_);
      }

      if (echo)
	debug::println("aa | wst(g) line:", (aa | is_edge) | is_line);

      // Second, processing the 0D-faces (points) of the watershed line.

      data::paste(morpho::dilation(extend(aa_line | (pw::value(aa_line) == pw::cst(0)),
					  aa_line),
				   c4().win()),
		  aa_line);

      /*

	// Version with "aa(x) = 0" to separate sheds (a shed =
	// adjacency between a couple of basins).

      mln_piter_(aa_line_t) pxl(aa_line.domain());
      mln_niter_(neighb2d) ne(c4(), pxl); // FIXME: "pxl -> nbhoring edges" is explictly c4()...
      for_all(pxl)
      {
	if (aa(pxl) != 0)
	  continue;
	unsigned count = 0;
	A na, na2;
	for_all(ne)
	  if (aa_line.has(ne))
	    {
	      mln_invariant(aa(ne) != 0);
	      if (count == 0)
		na  = aa(ne); // First attribute value encountered.
	      else
		na2 = aa(ne); // Second (or more) attr value encountered.
	      ++count;
	    }
	if (count == 2)
	  {
	    mln_invariant(na == na2); // Both attribute values have to be the same.
	    aa(pxl) = na;
	  }
      }
      */


    }

    if (echo)
      debug::println("aa | line:", aa_line);


    // Now dealing with all elements of basins:

    //   -  2D-faces, i.e., original pixels;
    //   -  1D-faces, i.e., edges within regions;
    //   -  0D-faces, i.e., points within regions.

    mln_VAR(aa_basins, aa | (pw::value(wst_g_full) != pw::cst(0)));

    {
      mln_piter_(aa_basins_t) p(aa_basins.domain());
      for_all(p)
	aa(p) = a[wst_g_full(p)];
    }

    if (echo)
      debug::println("aa | basins", aa_basins);

  }



  if (echo)
    debug::println("aa:", aa);


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
