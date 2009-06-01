
namespace mln
{

  // Display.
  // --------


  template <typename T, typename I>
  void display_tree_attributes(const T& t, // Tree.
			       const I& a) // Attribute image.
  {
    typedef mln_site(I) P;

    mln_ch_value(I, bool) deja_vu;
    initialize(deja_vu, a);
    data::fill(deja_vu, false);

    mln_up_node_piter(T) p(t);
    for_all(p)
      {
	if (deja_vu(p))
	  continue;
	
	P e = p;
	do
	  {
	    std::cout << a(e) << ':' << e << "  ->  ";
	    deja_vu(e) = true;
	    e = t.parent(e);
	  }
	while (! deja_vu(e));
	std::cout << a(e) << ':' << e << std::endl;
      }
    std::cout << std::endl;
  }


  // Back Propagation.
  // -----------------

  template <typename T, typename I>
  void
  back_propagate_attributes(const T& t, // Tree.
			    I& a)       // Attribute image.
  {
    // Back-propagate attribute from a node to sites of its
    // component.  Below, p is a non-node component site and
    // parent(p) is a node, that is, the site representative of
    // the component p belongs to.
    mln_up_site_piter(T) p(t);
    for_all(p)
      if (! t.is_a_node(p))
	{
	  mln_assertion(t.is_a_node(t.parent(p)));
	  a(p) = a(t.parent(p));
	}
  }


  // Attribute Extinction (Vachier's version).
  // -----------------------------------------

  namespace internal
  {

    template <typename T>
    struct node_pred : Function_v2b< node_pred<T> >
    {
      typedef bool result;
      
      template <typename P>
      bool operator()(const P& p) const
      {
	return t->is_a_node(p);
      }
      
      const T* t;
    };

    template <typename T, typename I, typename M>
    inline
    mln_value(I)
    extinct_rec(const T& t, // tree
		I& a,       // attribute image
		M& mark,
		const mln_psite(I)& p)
    {
      mln_invariant(mark(p) == false);
      mark(p) = true;
      if (t.parent(p) == p || mark(t.parent(p)) == true) // Stop.
	return a(p);
      return a(p) = extinct_rec(t, a, mark, t.parent(p));
    }


  } // end of internal


  template <typename T, typename I, typename J>
  void
  extinct_attributes(const T& t, // Tree.
		     I& a,       // Attribute image.
		     const J& f, // Original image to sort pixels.
		     bool echo = false)
  {
    if (echo)
      {
	std::cout << "extinction input:" << std::endl;
	debug::println(a);
	display_tree_attributes(t, a);
      }

    typedef mln_site(I) P;
    typedef mln_value(I) A; // Type of attributes.

    mln_ch_value(I, bool) mark;
    initialize(mark, a);
    data::fill(mark, false);
    
    internal::node_pred<T> node_only;
    node_only.t = &t;

    typedef p_array<P> S;
    S s = data::sort_psites_increasing(f | node_only);

    if (echo)
    {
      mln_fwd_piter(S) p(s);
      for_all(p)
	std::cout << f(p) << ":a" << p << '=' << a(p) << "  ";
      std::cout << std::endl << std::endl;
    }

    mln_invariant(geom::nsites(a | t.nodes()) == s.nsites());

    mln_fwd_piter(S) p(s);
    for_all(p)
      {
	if (mark(p) == true)
	  continue;
	internal::extinct_rec(t, a, mark, p);
      }

    // debug::println(mark | t.nodes());

    if (echo)
      {
	std::cout << "after extinction on nodes and before back-propagation:"
		  << std::endl;
	debug::println(a);
	display_tree_attributes(t, a);
      }

    back_propagate_attributes(t, a);

    if (echo)
      {
	std::cout << "extinction output:" << std::endl;
	debug::println(a);
	display_tree_attributes(t, a);
      }
  }



} // mln
