
#include <mln/core/concept/image.hh>
#include <mln/core/site_set/p_queue.hh>
#include <mln/core/site_set/p_priority.hh>
#include <mln/data/fill.hh>
#include <mln/util/array.hh>



namespace mln
{


  // Function-Object "e -> (l1, l2)".

  template <typename W, typename N>
  struct e_to_l1_l2_t
  {
    const W& w;
    const N& nbh;
    typedef mln_value(W) L;

    e_to_l1_l2_t(const W& w, const N& nbh)
      : w(w), nbh(nbh)
    {
    }

    template <typename E>
    void operator()(const E& e, // in
		    L& l1,      // out
		    L& l2       // out 
		    ) const
    {
      mln_niter(N) n(nbh, e);
      n.start();
      l1 = w(n);
      n.next();
      l2 = w(n);
      mln_invariant(l2 != l1);
      if (l1 > l2)
	std::swap(l1, l2);
    }
  };

  template <typename W, typename N>
  e_to_l1_l2_t<W, N>
  function_e_to_l1_l2(const W& w, const N& nbh)
  {
    e_to_l1_l2_t<W, N> tmp(w, nbh);
    return tmp;
  }



  template <typename A, typename L>
  util::array<L>
  sort_by_increasing_attributes(const util::array<A>& a, L l_max)
  {
    typedef std::pair<A,L> pair_t;
    std::vector<pair_t> v;
    v.reserve(l_max.next());

    v.push_back(pair_t(mln_min(A), 0)); // First elt, even after sorting.
    for (L l = 1; l <= l_max; ++l)
      v.push_back(pair_t(a[l], l));

    std::sort(v.begin(), v.end());

    util::array<L> ls(l_max.next());
    for (unsigned i = 1; i <= l_max; ++i)
      ls[i] = v[i].second;

    return ls;
  }



  // Find-Root for a region labeled with 'l'.

  template <typename L>
  inline
  L find_root_l(util::array<L>& lpar, L l)
  {
    if (lpar[l] == l)
      return l;
    else
      return lpar[l] = find_root_l(lpar, lpar[l]);
  }



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
  bool test_q_emptiness(const Qs& qs, L l, util::array<L>& lpar)
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

  template <typename Q_,
	    typename W, typename L, typename F,
	    typename E>
  void
  get_smallest_edge(Q_& q_, // in-out
		    L l, const W& w, util::array<L>& lpar, const F& e_to_l1_l2, // in
		    E& e, L& l1, L& l2)   // out
  {
    typedef mln_element(Q_) Q; // q_ is an array of queues with type Q.

    // Test that, when a region has merged, its edge queue has been
    // emptied.
    mln_invariant(test_q_emptiness(q_, l, lpar));

    L lr = find_root_l(lpar, l);
    Q& q = q_[lr];

    while (! q.is_empty())
      {
	e = q.pop_front();

	e_to_l1_l2(e, l1, l2);

	mln_invariant(l1 != l2);

	L l1r = find_root_l(lpar, l1),
	  l2r = find_root_l(lpar, l2);

	mln_invariant(l1r == lr || l2r == lr);

	if (l1r == l2r)
	  // 'e' is an internal edge => forget it.
	  continue;

	// Otherwise 'e' has been found.
	return;
      }

    mln_invariant(0); // We should not be here!
  }




  // ###################################################################
  // ###################################################################
  // ###################################################################
  // ###################################################################
  // ###################################################################



  template <typename W,
	    typename G,
	    typename L, typename A,
	    typename F>
  void
  compute_pseudo_tree(const W& w,
		      const G& g,
		      const util::array<L>& ls,
		      const A& a,
		      const F& e_to_l1_l2)
  {

    typedef mln_value(G) T;    //  <---  Type of edge values.
    typedef mln_psite(G) E;    //  <---  Type of edges.

    const L l_max = ls.nelements() - 1;

    mln_VAR( g_line, g | (pw::value(w) == 0) );


    // Edges -> Priority queue.

    typedef p_priority< T, p_queue<E> > Q;
    util::array<Q> q(l_max.next());

    {
      L l1, l2;

      mln_piter(g_line_t) e(g_line.domain());
      for_all(e)
      {
	mln_invariant(w(e) == 0);
	e_to_l1_l2(e, l1, l2);
	q[l1].push(mln_max(T) - g(e), e);
	q[l2].push(mln_max(T) - g(e), e);
      }
    }



    // Initialization.
    // -----------------------------------



    // Information "label l -> edge e".

    E null = E(0,0);  // Impossible value.  FIXME: lack of genericity.


    util::array<E> edge(l_max.next());
    for (L l = 0; l <= l_max; ++l)
      edge[l] = null;


    util::array<L> lpar(l_max.next());
    for (L l = 0; l <= l_max; ++l)
      lpar[l] = l; // Make-Set.


    //   To know if an edge is out of a given region (label l), we
    //   maintain the information about region merging using an
    //   union-find structure named "lpar".
    //
    //   In the following "lpar[l]" is shortly denoted by lr, meaning
    //   l-root.


    //   Given a region R with label l and an edge e = (l1, l2) from the
    //   priority queue, we get know if that edge is out of the set of
    //   regions containing l: we shall have l1r = lr or l2r = lr.

    //   Please note that an edge (l1, l2) is internal to a set of
    //   regions if l1r = l2r.




    mln_ch_value(g_line_t, E)
      epar,   // Edge forest.
      z_epar; // Auxiliary data: edge forest with compression and balancing.

    {
      initialize(epar, g_line);
      initialize(z_epar, g_line);
      mln_piter(g_line_t) e(g_line.domain());
      for_all(e)
      {
	// Make-Set.
	epar(e) = e;
	z_epar(e) = e;
      }
      debug::println("epar (init):", epar); // epar(e) == e so we depict the edges!
    }



    // GO GO GO !!!


    for (unsigned i = 1; i < l_max; ++i)
      {
	L l = ls[i]; // Region label.
	L l1, l2;
	E e;
	get_smallest_edge(q, // in-out
			  l, w, lpar, e_to_l1_l2, // in
			  e, l1, l2); // out

	L l1r = find_root_l(lpar, l1),
	  l2r = find_root_l(lpar, l2);

	// Consistency tests.
	{
	  if (i > 1)
	    {
	      L former_l = ls[i-1];
	      mln_invariant(a[l] >= a[former_l]);
	    }
	  mln_invariant(epar(e) == e); // 'e' has not been processed yet.
	  mln_invariant(l1 != l2);     // It is a valid edge, i.e., between 2 different regions...
	  mln_invariant(l1r != l2r);   // ...that are not already merged.
	  
	  L lr = find_root_l(lpar, l);
	  mln_invariant(lr == l // Either l is not deja-vu
			|| ((lr == l1r && lr != l2r) || // or l belongs to l1r xor l2r.
			    (lr == l2r && lr != l1r)));
	}



	// aa(e) = a[l]; // FIXME: Re-activate.



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
	  for (unsigned i = 1; i <= l_max; ++i)
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


    {
      // Display edge tree.

      mln_ch_value(g_line_t, bool) deja_vu;
      initialize(deja_vu, g_line);
      data::fill(deja_vu, false);

      std::cout << "edge tree: " << std::endl;
      for (L l = 1; l <= l_max; ++l)
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
    }

  }

} // end of namespace mln

