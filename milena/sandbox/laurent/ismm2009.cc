#include "ismm2009.hh"

#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>

#include <mln/util/ord_pair.hh>
#include <mln/debug/println.hh>
#include <mln/core/routine/duplicate.hh>

# include <mln/core/site_set/p_set.hh>
# include <mln/core/site_set/p_queue.hh>
# include <mln/core/site_set/p_priority.hh>

#include <mln/data/stretch.hh>
#include <mln/labeling/compute.hh>

#include <mln/accu/math/count.hh>
#include <mln/accu/height.hh>

#include <mln/util/timer.hh>
#include <mln/util/fibonacci_heap.hh>

#include <stack>

/*
   TO-DO list:

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


  /// Compute an LCA
  /// Reference article: The LCA Problem Revisited, M. A. Bender and M.
  /// Farach-Colton, May 2000
  ///
  /// See also Topological Watershed, Alexandre Abraham, LRDE CSI report
  /// 0821, June 2008
  template <typename L, typename I, typename E>
  class lca_t
  {

  public:

    /// Constructor
    lca_t(const L& l_max_,
	const I& edge_children,
	const std::vector<E>& roots)
    {
      unsigned l_max = l_max_.next();
      euler_tour_edge_.resize(2 * l_max + 1);
      euler_tour_depth_.resize(2 * l_max + 1);
      initialize (euler_position_, edge_children);

      // BUILD EULER TOUR
      std::stack<E> to_treat;
      for (unsigned i = 0; i < roots.size(); ++i)
	to_treat.push(roots[i]);

      unsigned euler_edge = 0;
      mln_ch_value(I, bool) deja_vu;
      initialize(deja_vu, edge_children);
      data::fill(deja_vu, false);

      while (!to_treat.empty())
      {
	E e = to_treat.top();
	to_treat.pop();

	euler_tour_edge_[euler_edge] = e;
	if (deja_vu(e))
	  euler_tour_depth_[euler_edge] = euler_tour_depth_[euler_edge - 1] - 1;
	else
	  euler_tour_depth_[euler_edge] = euler_tour_depth_[euler_edge - 1] + 1;

	if (!deja_vu(e))
	{
	  for (int j = edge_children(e).size() - 1; j >= 0; --j)
	  {
	    to_treat.push(e);
	    to_treat.push(edge_children(e)[j]);
	  }
	  deja_vu(e) = true;
	  euler_position_(e) = euler_edge;
	}

	++euler_edge;
      }

      // BUILD MINIMAS
      int size = 2 * l_max - 1;
      int logn = (int)(ceil(log((double)(size))/log(2.0)));

      minim_ = new int [logn * size]; // FIXME : Think about freeing this
      Minim_ = new int* [logn];

      Minim_[0] = minim_;

      // Init
      for (unsigned i = 0; i < size - 1; ++i)
	if (euler_tour_depth_[i] < euler_tour_depth_[i+1])
	{
	  Minim_[0][i] = i;
	} else {
	  Minim_[0][i] = i+1;
	}

      int k;
      for (int j = 1; j < logn; j++)
      {
	k = 1 << (j - 1);
	Minim_[j] = &minim_[j * size];
	for (int i = 0; i < size; i++)
	{
	  if ((i + (k << 1)) < size)
	  {
	    if (euler_tour_depth_[Minim_[j - 1][i]] <= euler_tour_depth_[Minim_[j - 1][i + k]])
	      Minim_[j][i] = Minim_[j - 1][i];
	    else
	      Minim_[j][i] = Minim_[j - 1][i + k];
	  }
	}
      }
    }


    /// Destructor
    ~lca_t()
    {
      delete[] Minim_;
      delete[] minim_;
    }


    const E& operator() (const E& a, const E& b)
    {
      if (a == b)
	return a;

      int m = euler_position_(a),
	  n = euler_position_(b),
	  k;

      if (m > n)
	std::swap(m, n);

      k = (int)(log((double)(n - m))/log(2.));

      if (euler_tour_depth_[Minim_[k][m]]
	    < euler_tour_depth_[Minim_[k][n - (1 << k)]])
	return euler_tour_edge_[Minim_[k][m]];
      else
	return euler_tour_edge_[Minim_[k][n - (1 << k)]];
    }

  private:
    int *minim_;
    int **Minim_;
    std::vector<E> euler_tour_edge_;
    std::vector<unsigned> euler_tour_depth_;
    mln_ch_value(I, unsigned) euler_position_;
  };

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
  compute_wst_g_from_f(F& f, // On pixels.
		       G& g, // On edges.
		       const N_e2p& e2p,
		       const N_e2e& e2e,
		       L& n_basins,
		       bool echo)
  {
    data::paste(morpho::gradient(extend(g, f),
				 e2p.win()),
		g);

# ifdef MAKE_UNIQUE

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

# endif // MAKE_UNIQUE


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



  // From parent image to children image.

  template <typename I, typename E, typename L>
  mln_ch_value(I, std::vector<mln_psite(I)>)
    compute_children(const I& epar, const std::vector<E>& edge, L l_max, std::vector<E>& roots)
  {
    typedef std::vector<mln_psite(I)> C; // Children.
    mln_ch_value(I,C) chl;
    initialize(chl, epar);

    mln_ch_value(I,bool) deja_vu;
    initialize(deja_vu, epar);
    data::fill(deja_vu, false);

    for (L l = 1; l <= l_max; ++l)
      {
	E e = edge[l];
	while (deja_vu(e) == false)
	  {
	    deja_vu(e) = true;
	    if (epar(e) != e)
	      {
		chl(epar(e)).push_back(e);
		e = epar(e);
	      }
	    else
	      roots.push_back(e);
	  }
      }

    return chl;
  }


} // mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm echo output.pgm" << std::endl;
  std::cerr << "Laurent ISMM 2009 scheme with saliency map as output." << std::endl;
  std::cerr << "  echo = 0 or 1." << std::endl;
  abort();
}




int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::label_16;

  if (argc != 4)
    usage(argv);

  util::timer timer;



  // Step 1.  From f to wst_g.
  // ------------------------------------------------------------------------------


  timer.start();


  image2d<int_u8> raw_f;
  io::pgm::load(raw_f, argv[1]);

  bool echo;
  {
    int echo_ = std::atoi(argv[2]);
    if (echo_ != 0 && echo_ != 1)
      usage(argv);
    echo = bool(echo_);
  }

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
	   compute_wst_g_from_f(f, g, e2p(), e2e(), n_basins, echo) );


  if (echo)
    {
      debug::println("g:", g);
      debug::println("wst(g):", wst_g);
    }

  std::cout << "n basins = " << n_basins << std::endl;


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




  std::cout << "Computing wst(g) from f: " << timer << " s" << std::endl;




  // Step 2.   From wst(g) -> a.
  // ------------------------------------------------------------------------------

  timer.restart();



  // Compute an attribute per region.
  // --------------------------------

  {
    // Height.
    // FIXME: HERE...
  }

  typedef unsigned A;
  util::array<A> a = labeling::compute(accu::meta::math::count(),
				       g,
				       wst_g,
				       n_basins);

  typedef std::pair<A,L> pair_t;
  std::vector<pair_t> v = sort_attributes(a, n_basins); // Sort regions.


  // Optional code (for testing purpose): change attributes
  // so that they are all different.

# ifdef MAKE_UNIQUE
    make_unique_attribute(a, v, n_basins, echo);
# endif


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


  std::cout << "Computing region attributes: " << timer << " s" << std::endl;




  // Step 3.  wst(g) + a  --->  tree "e->e" + array "l->e" + aa.
  // ------------------------------------------------------------------------------


  timer.restart();



  // Edges -> Priority queue.

  typedef p_priority< T, p_queue<E> > Q;
  // typedef util::fibonacci_heap<T, E> Q;

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


  // Information "label l -> edge e".

  E null = E(0,0);  // Impossible value.

  std::vector<E> edge(n_basins.next());
  for (L l = 1; l <= n_basins; ++l)
    edge[l] = null;


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


	q[l2r].insert(q[l1r]);  // With p_priority<Q>.
	q[l1r].clear();         //

	// q[l2r].push(q[l1r]); // With Fib-Heap.



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

  std::vector<E> roots;
  mln_VAR(chl, compute_children(epar, edge, n_basins, roots));

  // Connected domain so:
  mln_invariant(roots.size() == 1);

  E root = roots[0]; // THE root.


  if (echo)
  {
    std::cout << "root: " << root << std::endl;

    // Display edge tree.
    mln_ch_value_(chl_t, bool) deja_vu;
    initialize(deja_vu, chl);
    data::fill(deja_vu, false);
    std::cout << "edge tree: " << std::endl;
    for (L l = 1; l <= n_basins; ++l)
      {
	std::cout << l << ": ";
	E e = edge[l];
	while (! deja_vu(e))
	  {
	    std::cout << e << " -> ";
	    deja_vu(e) = true;
	    e = epar(e);
	  }
	std::cout << e << std::endl;
      }

    // Display children.
    mln_piter_(chl_t) e(chl.domain());
    for_all(e)
      if (chl(e).size() != 0)
	{
	  std::cout << e << " children:  ";
	  for (unsigned i = 0; i < chl(e).size(); ++i)
	    std::cout << chl(e)[i] << "  ";
	  std::cout << std::endl;
	}
  }


  std::cout << "Computing tree (loop over regions): " << timer << " s" << std::endl;




  // Step 4.  Final.
  // ------------------------------------------------------------------------------


  timer.restart();



# ifndef NDEBUG

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

# endif // ndef NDEBUG




  // +---------------------------------------------------------------+
  // | We want an "image" of the "merging value of a" over all edges |
  // | (not only the set of 'e' used to merge).                      |
  // +---------------------------------------------------------------+


  // Dealing with the watershed line.

  mln_VAR(aa_line, aa | is_line);

  A aa_max = 0;

  {

    {
      // First, processing the 1D-faces (edges) of the watershed line.

      lca_t<L,chl_t,E> lca(n_basins, chl, roots);

      mln_piter_(g_line_t) e(g_line.domain());
      for_all(e)
      {
	L l1 = adj_line(e).first(),
	  l2 = adj_line(e).second();
	mln_invariant(l1 != 0 && l2 > l1);
	E e_ = lca(edge[l1],edge[l2]);
	mln_invariant(is_line(e_));
	mln_invariant(aa(e_) != 0); // aa is valid at e_.

	// The attribute value propagates from the lca to the current edge
	// of the line:
	aa(e) = aa(e_);
	if (aa(e) > aa_max)
	  aa_max = aa(e);
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
      debug::println("aa | basins:", aa_basins);

  }


  if (echo)
    debug::println("aa:", aa);


  std::cout << "Final: " << timer << " s" << std::endl;



  // Output is salency map.

  {
  
    if (aa_max < 256)
      {
	image2d<int_u8> output(f_.domain());
	data::fill(output, 0);
	data::paste(aa_line, output);
	io::pgm::save(output, argv[3]);
      }
    else
      {
	std::cerr << "warning: stretching [0," << aa_max << "] to int_u8" << std::endl;

	image2d<A> output(f_.domain());
	data::fill(output, 0);
	data::paste(aa_line, output);
 	io::pgm::save(data::stretch(int_u8(), output),
 		      argv[3]);

// 	image2d<int_u8> output(f_.domain());
// 	data::fill(output, 0);
// 	A threshold = aa_max - 255;
// 	mln_piter(aa_line_t) e(aa_line.domain());
// 	for_all(e)
// 	  if (aa_line(e) <= threshold)
// 	    output(e) = 1;
// 	  else
// 	    output(e) = aa_line(e) - threshold;
//  	io::pgm::save(output, argv[3]);
      }
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
