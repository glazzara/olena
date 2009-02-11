
#include <mln/core/concept/image.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/core/site_set/p_queue.hh>
#include <mln/core/site_set/p_priority.hh>

#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/util/array.hh>

#include "lca.hh"


// FIXME: Move elsewhere.
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/dilation.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/save.hh>
#include <mln/level/stretch.hh>
#include <mln/level/compute.hh>
#include <mln/accu/max.hh>




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


  template <typename I, typename E, typename L>
  lca_t<L, mln_ch_value(I, std::vector<mln_psite(I)>), E>
  compute_lca(const I& epar, const util::array<E>& edge, L l_max)
  {
    std::vector<E> roots;
    mln_VAR(chl, compute_children(epar, edge, l_max, roots));
    
    // Connected domain so:
    mln_invariant(roots.size() == 1);
    E root = roots[0]; // THE root.
    
    lca_t<L,chl_t,E> lca(l_max, chl, roots);
    return lca;
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
		      const G& g, // FIXME: Directly input g_line?
		      const util::array<L>& l_,
		      const util::array<A>& a,
		      const F& e_to_l1_l2)
  {

    typedef mln_value(G) T;    //  <---  Type of edge values.
    typedef mln_psite(G) E;    //  <---  Type of edges.

    const L l_max = l_.nelements() - 1;

    mln_VAR( g_line, g | (pw::value(w) == 0) );


    // Initialization.
    // -----------------------------------


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




    // 'aa' is the resulting attributes on edges.

    mln_ch_value(g_line_t, A) aa;
    initialize(aa, g_line);
    data::fill(aa, 0); // FIXME: Safety iff 0 is an invalidate attribute value!



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
	L l = l_[i]; // Region label.
	L l1, l2;
	E e;
	get_smallest_edge(q, // in-out
			  l, w, lpar, e_to_l1_l2, // in
			  e, l1, l2); // out
	aa(e) = a[l];

	L l1r = find_root_l(lpar, l1),
	  l2r = find_root_l(lpar, l2);

	// Consistency tests.
	{
	  if (i > 1)
	    {
	      L former_l = l_[i-1];
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
      // Display 'aa' over all edges.

      debug::println("aa (on 'passing' edges):", aa);

      // Display "edge tree".

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
	      std::cout << e << " [" << aa(e) << "] -> ";
	      mln_invariant(aa(e) != 0 && aa(epar(e)) != 0); // aa is valid
	      mln_invariant(aa(epar(e)) >= aa(e));           // edge parenthood goes with 'aa' increasing
	      deja_vu(e) = true;
	      e = epar(e);
	    }
	  std::cout << e << std::endl;
	}
      std::cout << std::endl;

      // Display "region l -> edge e".

      std::cout << "region:(edge)  =  ";
      for (L l = 1; l <= l_max; ++l)
	std::cout << l << ':' << edge[l] << "  ";
      std::cout << std::endl
		<< std::endl;
	
    } // end of Display.



    // Testing both that all regions and all "smallest" edges have
    // merged (assumption: connected domain!).

    {
      L l_1 = l_[1],
	l_root = find_root_l(lpar, l_1);
      mln_invariant(edge[l_1] != null);
      E e_root = find_root_e(z_epar, edge[l_1]);
      for (unsigned i = 2; i <= l_max; ++i)
	{
	  L l = l_[i];
	  mln_invariant(find_root_l(lpar, l) == l_root);
	  mln_invariant(edge[l] != null);
	  mln_invariant(find_root_e(z_epar, edge[l]) == e_root);
	}
    }


    // Finalization.

    mln_VAR(lca, compute_lca(epar, edge, l_max));


    mln_piter(g_line_t) e(g_line.domain());
    for_all(e)
    {
      L l1, l2;
      e_to_l1_l2(e, l1, l2);
      mln_invariant(l1 != 0 && l2 > l1);
      E e_ = lca(edge[l1],edge[l2]);
      
      mln_invariant(g_line.has(e_)); // e_ is a valid edge.
      mln_invariant(aa(e_) != 0);    // aa is valid at e_.
      
      // The attribute value propagates from the lca to the current edge
      // of the line:
      aa(e) = aa(e_);
    }

    mln_VAR(aa_ext, aa.unmorph_().unmorph_());
    {
      mln_VAR( aa_line, aa_ext | (pw::value(w) == 0) );
      
      data::paste(morpho::dilation(extend(aa_line | (pw::value(aa_line) == 0),
					  aa_line),
				   c4().win()),
		  aa_line);
      debug::println("aa ext:", aa_ext);
    }
    


    // Outputing.

    {
      A aa_max = level::compute(accu::max<A>(), aa);
      using value::int_u8;
      if (aa_max < 256)
	{
	  image2d<int_u8> output(aa_ext.domain());
	  data::fill(output, 0);
	  data::paste(aa_ext, output);
	  io::pgm::save(output, "aa_line.pgm");
	}
      else
	{
	  std::cerr << "warning: stretching [0," << aa_max << "] to int_u8" << std::endl;
	  
	  image2d<int_u8> output(aa_ext.domain());
	  data::fill(output, 0);
	  data::paste(aa_ext, output);
	  io::pgm::save(level::stretch(int_u8(), output),
			"aa_line.pgm");
	}
    }


//       mln_VAR(aa_basins, aa_ext | (pw::value(w) != 0));
//       {
// 	mln_piter(aa_basins_t) p(aa_basins.domain());
// 	for_all(p)
// 	{
// 	  L l = w(p);
// 	  aa_basins(p) = aa( edge[l] ); // FIXME: was: a[w(p)];
// 	}
//       }


//       debug::println("aa ext with basins:", aa_ext);


  }


} // end of namespace mln

