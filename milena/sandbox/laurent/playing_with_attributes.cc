#include "ismm2009.hh"

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>

#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>

#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/gradient.hh>
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/labeling/regional_minima.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>
#include <mln/geom/nsites.hh>

#include <mln/accu/count.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::cerr << "For Laurent's ISMM 2009 scheme: min-tree, attributes, and watershed." << std::endl;
  abort();
}



namespace mln
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


  // FIXME: HERE


  template <typename T, typename I>
  bool
  test_attribute_growing_property(const T& t, // Tree.
				  const I& a) // Attribute image.
  {
    typedef mln_site(I) P;
    typedef mln_value(I) A; // Type of attributes.

    // Test that attributes increase with parenthood.
    mln_fwd_piter(T) p(t.domain());
    for_all(p)
      if (t.is_a_non_root_node(p))
	{
	  mln_invariant(t.is_a_node(t.parent(p)));
	  if (a(t.parent(p)) < a(p))
	    return false;
	}
    return true;
  }


  template <typename T, typename I>
  void depict_tree_attributes(const T& t, // Tree.
			      const I& a) // Attribute image.
  {
    typedef mln_site(I) P;

    mln_ch_value(I, bool) deja_vu;
    initialize(deja_vu, a);
    data::fill(deja_vu, false);

    typedef typename T::nodes_t nodes_t;
    mln_fwd_piter(nodes_t) p(t.nodes());
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



  //  Attribute extinction (reference code).
  //  --------------------------------------


  template <typename T, typename I>
  mln_concrete(I)
    attribute_extinction(const T& t, // Tree.
			 const I& a, // Attribute image.
			 bool echo = false)
  {
    typedef mln_site(I) P;
    typedef mln_value(I) A; // Type of attributes.

    mln_assertion( test_attribute_growing_property(t, a) );

    if (echo)
      depict_tree_attributes(t, a);
    
    node_pred<T> node_only;
    node_only.t = &t;

    typedef p_array<P> S;
    S s = level::sort_psites_increasing(a | node_only);
    mln_invariant(geom::nsites(a | t.nodes()) == s.nsites());


    mln_ch_value(I, bool) mark;
    initialize(mark, a);
    data::fill(mark, false);


    // Modify attributes.

    mln_concrete(I) aa;
    initialize(aa, a);
    data::fill(aa, 0);

    {

      P e, par_e;
      A v;
      bool found;

      mln_fwd_piter(S) p(s);
      
      // Initialization.
      for_all(p)
	{
	  if (mark(p) == true)
	    continue;

	  // First, fetch the attribute value v from p;
	  e = p;
	  found = false;
	  while (mark(e) == false && e != t.parent(e))
	    {
	      par_e = t.parent(e);
	      if (mark(par_e) == true)
		{
		  v = a(e);
		  found = true;
		  break;
		}
	      e = par_e;
	    };

	  if (! found && e == t.parent(e))
	    {
	      // e is a root node; we get its value:
	      v = a(e);

	      if (echo)
		std::cout << "from p = " << p
			  << "  to root e = " << e
			  << "  v = " << v
			  << std::endl;

	      // we set aa from p to the root node
	      e = p;
	      do
		{
		  aa(e) = v;
		  mln_invariant(mark(e) == false);
		  mark(e) = true;
		  e = t.parent(e);
		}
	      while (e != t.parent(e));
	      // handle the root node 'e':
	      aa(e) = v;
	      mln_invariant(mark(e) == false);
	      mark(e) = true;
	      mln_invariant(a(e) == v);
	    }
	  else if (found)
	    {
	      e = p;

	      P last_e = P(0,0);

	      while (mark(e) == false)
		{
		  aa(e) = v;
		  mark(e) = true;
		  last_e = e;
		  e = t.parent(e);
		};

	      if (echo)
		{
		  if (last_e == p)
		    std::cout << "p = " << p
			      << "  par(e) = " << par_e
			      << "  so keep v = " << v
			      << std::endl;
		  else
		    std::cout << "from p = " << p
			      << "  to e = " << last_e
			      << "  with par(e) = " << par_e
			      << "  v = " << v
			      << std::endl;
		}
	    }
	}

      if (echo)
	depict_tree_attributes(t, aa);

    } // end of Modify attributes.

    return aa;
  }






  //  N E W   C O D E   = =   Attribute extinction (reference code).
  //  --------------------------------------
  
  template <typename T, typename I, typename M>
  mln_value(I)  rec(const T& t, // tree
		    I& a,       // attribute image
		    M& mark,
		    const mln_psite(I)& p)
  {
    mln_invariant(mark(p) == false);
    mark(p) = true;
    if (t.parent(p) == p || mark(t.parent(p)) == true) // Stop.
      return a(p);
    return a(p) = rec(t, a, mark, t.parent(p));
  }


  template <typename T, typename I>
  void
    NEW___extinct_attributes(const T& t, // Tree.
			     I& a, // Attribute image.
			     bool echo = false)
  {
    typedef mln_site(I) P;
    typedef mln_value(I) A; // Type of attributes.

    if (echo)
      depict_tree_attributes(t, a);


    mln_ch_value(I, bool) mark;
    initialize(mark, a);
    data::fill(mark, false);
    
    node_pred<T> node_only;
    node_only.t = &t;

    typedef p_array<P> S;
    S s = level::sort_psites_increasing(a | node_only);
    mln_invariant(geom::nsites(a | t.nodes()) == s.nsites());

    mln_fwd_piter(S) p(s);
    for_all(p)
      {
	if (mark(p) == true)
	  continue;
	rec(t, a, mark, p);
      }

    debug::println(mark | t.nodes());
  }


} // mln  ------------------------------------------------------------



template <typename I, typename N, typename L>
void do_it(const I& g, const N& nbh, L& n_labels)
{
  using namespace mln;

  // regional minima

  mln_ch_value(I,L) regmin = labeling::regional_minima(g, nbh, n_labels);
  // debug::println("regmin(g):", regmin);


  // watershed

  L n_basins;
  mln_ch_value(I,L) w = morpho::meyer_wst(g, nbh, n_basins);
  mln_invariant(n_basins == n_labels);
  // debug::println("w(g):", w);


  {
    typedef p_array<mln_site(I)> S;
    S s = level::sort_psites_decreasing(g);
    
    typedef morpho::tree::data<I,S> tree_t;
    tree_t t(g, s, nbh);
    
    accu::count< util::pix<I> > a_; // Kind of attribute.
 
    mln_ch_value(I,unsigned) a = morpho::tree::compute_attribute_image(a_, t);
    debug::println("a | nodes:", a | t.nodes());


    // Reference version.
//     mln_ch_value(I,unsigned) aa = attribute_extinction(t, a);

    // NEW version.
    NEW___extinct_attributes(t, a);
    mln_VAR(aa, a);

    debug::println("aa | nodes:", aa | t.nodes());

    // Back-propagation from nodes to components.
    {
      mln_fwd_piter(tree_t) p(t.domain());
      for_all(p)
	if (! t.is_a_node(p))
	  {
	    mln_assertion(t.is_a_node(t.parent(p)));
	    aa(p) = aa(t.parent(p));
	  }
    }

//     debug::println("a | w line:", a | (pw::value(w) == pw::cst(0)));
//     debug::println("aa | w line:", aa | (pw::value(w) == pw::cst(0)));

//     debug::println("a | w basins:", a | (pw::value(w) != pw::cst(0)));
//     debug::println("a | regmin:", a | (pw::value(regmin) != pw::cst(0)));

  }

} // end of do_it



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;


  typedef image2d<int_u8> I;
  typedef value::label_8 L;

  border::thickness = 0;

  if (argc != 2)
    usage(argv);

  I raw_f;
  io::pgm::load(raw_f, argv[1]);


  I f_ = add_edges(raw_f);
  mln_VAR(f, f_ | is_pixel);
  // debug::println("f:", f);

  mln_VAR(g, f_ | is_edge);
  data::paste( morpho::gradient(extend(g, f_),
				e2p().win()),
	       g );
  // debug::println("g:", g);

  L n;
  do_it(g, e2e(), n);

}
