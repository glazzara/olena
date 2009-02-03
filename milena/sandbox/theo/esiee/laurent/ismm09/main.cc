
#include <mln/core/var.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>

#include <mln/core/image/image_if.hh>
#include <mln/core/routine/extend.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>

#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/labeling/compute.hh>
#include <mln/level/sort_psites.hh>

#include <mln/core/site_set/p_queue.hh>
#include <mln/core/site_set/p_priority.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

#include <mln/accu/count.hh>



namespace mln
{



  template <typename I, typename J>
  void data__paste_values(const Image<I>& input_, Image<J>& output_)
  {
    const I& input = exact(input_);
    J& output = exact(output_);
    
    mln_fwd_piter(I) pi(input.domain());
    mln_fwd_piter(J) po(output.domain());
    for_all_2(pi, po)
      output(po) = input(pi);
  }



  namespace cplx2d
  {

    // Neighborhoods.

    typedef neighb< win::multiple<window2d, bool(*)(const point2d&)> > dbl_neighb2d;

    inline
    bool is_row_odd(const point2d& p)
    {
      return p.row() % 2;
    }

    // Edge to (the couple of) pixels.
    const dbl_neighb2d& e2p()
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

    
    // Edge to neighboring edges.
    const dbl_neighb2d& e2e()
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


    // Predicates.

    typedef fun::C<bool (*)(const mln::point2d&)> predicate_t;

    inline
    bool is_pixel(const point2d& p)
    {
      // Original pixels.
      return p.row() % 2 == 0 && p.col() % 2 == 0;
    }

    inline
    bool is_edge(const point2d& p)
    {
      // Edges between pixels.
      return p.row() % 2 + p.col() % 2 == 1;
    }

    inline
    bool is_point(const point2d& p)
    {
      // Points in-between pixels.
      return p.row() % 2 && p.col() % 2;
    }



    image_if< image2d<value::int_u8>, predicate_t >
    f_to_g(const image2d<value::int_u8>& f)
    {

      image2d<value::int_u8> f_(2 * f.nrows() - 1, 2 * f.ncols() - 1);
      data::fill(f_, 0); // Useless but for display!

      data__paste_values(f, (f_ | is_pixel).rw());

      mln_VAR(g, f_ | is_edge);
      data::paste(morpho::gradient(extend(g, f_),
 				   e2p().win()),
 		  g);

      return g;
    }


    template <typename W>
    image2d<mln_value(W)>
    extend_w_edges_to_all_faces(W& w)
    {
      mln_VAR(w_ext, w.unmorph_());

      // edges (1D-faces) -> pixels (2D-faces)
      data::paste(morpho::dilation(extend(w_ext | is_pixel,
					  pw::value(w_ext)),
				   c4().win()),
		  w_ext);

      // edges (1D-faces) -> points (0D-faces)
      data::paste(morpho::erosion(extend(w_ext | is_point,
					 pw::value(w_ext)),
				  c4().win()),
		  w_ext);

      return w_ext;
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


    // Function-Object "e -> (l1, l2)".

    struct e_to_labels_t 
    {
      template <typename W, typename L>
      inline
      void
      operator()(const W& w, const point2d& e, L& l1, L& l2) const
      {
	mln_precondition(w(e) == 0);
	l1 = 0;
	l2 = 0;
	mln_niter(dbl_neighb2d) n(e2e(), e);
	for_all(n)
	  if (w.has(n) && w(n) != 0)
	    {
	      if (l1 == 0) // First label to be stored.
		l1 = w(n);
	      else
		if (w(n) != l1 && l2 == 0) // Second label to be stored.
		  l2 = w(n);
		else
		  mln_invariant(w(n) == l1 || w(n) == l2);
	    }
	mln_invariant(l1 != 0 && l2 != 0);
	if (l1 > l2)
	  std::swap(l1, l2);
	mln_postcondition(l2 >= l1);
      }
    };


  } // end of namespace mln::cplx2d



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
		    L l, const W& w, util::array<L>& lpar, const F& e_to_labels, // in
		    E& e)   // out
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

	L l1, l2;
	e_to_labels(w, e,    // input
		    l1, l2); // output

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
		      const F& e_to_labels)
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
	e_to_labels(w, e,    // input
		    l1, l2); // output
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
	E e;
	get_smallest_edge(q, // in-out
			  l, w, lpar, e_to_labels, // in
			  e); // out
	// FIXME: the call below is performed above!!!
	L l1, l2;
	e_to_labels(w, e, l1, l2);

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

  if (argc != 2)
    usage(argv);

  // f: regular image.

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);


  cplx2d::e_to_labels_t e_to_labels;

  // g: weights on edges.

  mln_VAR(g, cplx2d::f_to_g(f) );
  debug::println("g:", g);

  typedef mln_value_(g_t) T;                 //  <---  Type of edge values.
  typedef mln_psite_(g_t) E;                 //  <---  Type of edges.


  mln_VAR(nbh_g, cplx2d::e2e()); // Neighborhood between edges.


  // w: watershed labeling on edges.

  typedef label_16 L;                        //  <---  Type of labels.
  L l_max;
  mln_VAR( w, morpho::meyer_wst(g, nbh_g, l_max) );
  debug::println("w:", w);


  mln_VAR( w_line, pw::value(w) == pw::cst(0) );
  mln_VAR( g_line, g | w_line );
  debug::println("g | line:", g_line);


  {
    /*
    // debug::println("w | line:", w | w_line);
    mln_VAR(w_ext, cplx2d::extend_w_edges_to_all_faces(w));
    debug::println("w_ext:", w_ext);
    // debug::println("w_ext | line:", w_ext | (pw::value(w_ext) == pw::cst(0)));
    */
  }



  // a: array "label -> attribute".

  typedef unsigned A;                        //  <---  Type of attributes.

  util::array<A> a = labeling::compute(accu::meta::count(),
				       g, // image of values
				       w, // image of labels
				       l_max);

  util::array<L> ls = sort_by_increasing_attributes(a, l_max);


  {
    std::cout << "ls:" << std::endl;
    for (unsigned i = 1; i <= l_max; ++i)
      std::cout << ls[i] << "(" << a[ls[i]] << ") ";
    std::cout << std::endl
	      << std::endl;
  }


  //   {
  //     // Test adjacency "e -> (l1, l2)".
  //     L l1, l2;
  //     mln_piter_(g_t) e(g.domain());
  //     for_all(e)
  //       if (w(e) == 0)
  // 	{
  // 	  e_to_labels(w, e, l1, l2);
  // 	  std::cout << e << ':' << l1 << ',' << l2 << std::endl;
  // 	}
  //   }


  compute_pseudo_tree(w, g, ls, a,
		      e_to_labels);

}
