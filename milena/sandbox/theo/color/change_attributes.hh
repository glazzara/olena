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

    // WAS:
    // typedef typename T::nodes_t nodes_t;
    // mln_fwd_piter(nodes_t) p(t.nodes());

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



  // Test attribute growing property.
  // --------------------------------

  template <typename T, typename A>
  inline
  bool
  test_attribute_growing_property(const T& t, const A& a)
  {
    typedef typename T::nodes_t N;
    mln_fwd_piter(N) n(t.nodes());
    for_all(n)
      if (a(t.parent(n)) < a(n))
	return false;
    return true;
  }

  template <typename T, typename A>
  inline
  void
  invariant__attribute_growing_property(const T& t, const A& a)
  {
    mln_invariant(test_attribute_growing_property(t, a));
  }




  // Make attributes grow.
  // --------------------------------

  template <typename T, typename A>
  inline
  void
  make_attribute_grow(const T& t, A& a)
  {
    typedef typename T::nodes_t N;
    mln_fwd_piter(N) n(t.nodes());
    for_all(n)
      if (a(t.parent(n)) < a(n))
	a(t.parent(n)) = a(n);

    mln_postcondition(test_attribute_growing_property(t, a));
  }



  // Back-propagate values from nodes to component sites.
  // ----------------------------------------------------

  template <typename T, typename A>
  void
  back_propagate(const T& t, A& a)
  {
    // WAS: mln_fwd_piter(T) p(t.domain());
    mln_site_piter(T) p(t);
    for_all(p)
      if (! t.is_a_node(p))
	{
	  mln_assertion(t.is_a_node(t.parent(p)));
	  a(p) = a(t.parent(p));
	}
  }




  // Attribute Extinction.
  // ---------------------

  namespace internal
  {

    template <typename T>
    struct node_pred : Function_p2b< node_pred<T> >
    {
      typedef bool result;
      
      template <typename P>
      bool operator()(const P& p) const
      {
	return t->is_a_node(p);
      }
      
      const T* t;
    };


    // Vachier's version.

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


  // Vachier's version.

  template <typename T, typename I>
  void
  extinct_attributes(const T& t, // Tree.
		     I& a, // Attribute image.
		     bool echo = false)
  {
    if (echo)
      {
	std::cout << "before:" << std::endl;
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
    S s = level::sort_psites_increasing(a | node_only);
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
	std::cout << "after:" << std::endl;
	display_tree_attributes(t, a);
      }
  }


  /*

  // Modified version.
  //
  // The extinction moves down the parent attribute.  With the 1st
  // experiments, that does not work.  Yet, it is better to first modify
  // the attributes (move down the parent values) the classically extinct.

  namespace internal
  {

    template <typename T, typename I, typename M>
    inline
    mln_value(I)
    extinct_rec__(const T& t, // tree
		  const I& a, // original attribute image
		  I& ea,      // extincted attribute image
		  M& mark,
		  const mln_psite(I)& p)
    {
      mln_invariant(mark(p) == false);
      mark(p) = true;
      if (t.parent(p) == p || mark(t.parent(p)) == true) // Stop.
	return a(t.parent(p)); // The parent attribute!
      return ea(p) = extinct_rec__(t, a, ea, mark, t.parent(p));
    }

  } // end of internal

  template <typename T, typename I>
  void
  extinct_attributes_new(const T& t, // Tree.
			 I& a) // Attribute image.
  {
    typedef mln_site(I) P;
    typedef mln_value(I) A; // Type of attributes.

    mln_ch_value(I, bool) mark;
    initialize(mark, a);
    data::fill(mark, false);

    mln_concrete(I) ea = duplicate(a);
    
    internal::node_pred<T> node_only;
    node_only.t = &t;

    typedef p_array<P> S;
    S s = level::sort_psites_increasing(a | node_only);
    mln_invariant(geom::nsites(a | t.nodes()) == s.nsites());

    mln_fwd_piter(S) p(s);
    for_all(p)
      {
	if (mark(p) == true)
	  continue;
	internal::extinct_rec__(t, a, ea, mark, p);
      }

    data::fill(a, ea);

    // debug::println(mark | t.nodes());
  }

  */


  // Move down.
  // ----------

  template <typename T, typename A>
  inline
  void
  move_down_attributes(const T& t, A& a, bool echo = false)
  {
    if (echo)
      {
	std::cout << "before:" << std::endl;
	display_tree_attributes(t, a);
      }
    typedef typename T::nodes_t N;
    mln_fwd_piter(N) n(t.nodes());
    for_all(n)
      a(n) = a(t.parent(n));

    // Test.
    for_all(n)
      mln_invariant(a(n) <= a(t.parent(n)));

    if (echo)
      {
	std::cout << "after:" << std::endl;
	display_tree_attributes(t, a);
      }
  }




  // Fuse up.
  // ----------

  template <typename T, typename A>
  inline
  void
  fuse_up_attributes(const T& t, A& a, bool echo = false)
  {
    if (echo)
      {
	std::cout << "before:" << std::endl;
	display_tree_attributes(t, a);
      }
    typedef typename T::nodes_t N;
    mln_fwd_piter(N) n(t.nodes());
    for_all(n)
      a(n) = (a(n) + a(t.parent(n))) / 2;
    
    for_all(n)
      mln_invariant(a(n) <= a(t.parent(n)));

    if (echo)
      {
	std::cout << "after:" << std::endl;
	display_tree_attributes(t, a);
      }
  }




  // Rand.
  // -----

  template <typename A, typename T>
  inline
  mln_ch_value(typename T::function, mln_result(A))
    compute_rand_attribute_on_nodes(const A& a, const T& t)
  {
    typedef typename T::function I;

    mln_ch_value(I, mln_result(A)) attr;
    initialize(attr, t.f());

    // Initialize every attribute with the corresponding pixel.
    {
      mln_piter(I) p(t.f().domain());
      for_all(p)
	attr(p) = float(std::rand() % 10000) / 1000.f;
    }

    // Propagate attribute from a site to its parent.
    {
      mln_fwd_piter(T) p(t.domain());
      for_all(p)
	if (! t.is_root(p))
	  attr(t.parent(p)) += attr(p);
    }

    return attr;
  } 



} // end of namespace mln
