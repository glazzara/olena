
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
		     I& a,       // Attribute image.
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

//     {
//       mln_fwd_piter(S) p(s);
//       for_all(p)
// 	std::cout << p << ' ' << a(p) << " - ";
//       std::cout << std::endl
// 		<< std::endl;
//     }

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


} // mln
