#include <cstdlib>

#include <mln/core/var.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/data/fill.hh>
#include <mln/util/ord.hh>
#include <mln/make/pix.hh>
#include <mln/debug/println.hh>

#include <mln/value/rgb8.hh>
#include <mln/math/diff_abs.hh>

#include <mln/level/sort_psites.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/pw/all.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/closing/leveling.hh>

#include "change_attributes.hh"



namespace mln
{


  // Distance between 2 rgb8 colors.

  value::int_u8 dist(const value::rgb8& c1, const value::rgb8& c2)
  {
    unsigned d = 0;
    d += (math::diff_abs(c1.red(), c2.red()) + 2) / 3;
    d += (math::diff_abs(c1.green(), c2.green()) + 2) / 3;
    d += (math::diff_abs(c1.blue(), c2.blue()) + 2) / 3;
    if (d > 255)
      d = 255;
    return d;
  }

  value::int_u8 dist_(const value::rgb8& c1, const value::rgb8& c2)
  {
    unsigned d = 0;
    d += math::diff_abs(c1.red(), c2.red());
    d += math::diff_abs(c1.green(), c2.green());
    d += math::diff_abs(c1.blue(), c2.blue());
    if (d > 255)
      d = 255;
    return d;
  }


  // Sorting.


  template <typename I>
  struct my_less_
  {
    const I& ima_;
    
    inline
    my_less_(const I& ima)
      : ima_(ima)
    {
    }
    
    inline
    bool operator()(const mln_site(I)& lhs,
		    const mln_site(I)& rhs) const
    {
      return util::ord_strict(ima_(lhs), ima_(rhs))
	|| (ima_(lhs) == ima_(rhs)
	    &&
	    util::ord_strict(lhs, rhs));
    }
  };


  template <typename I, typename S>
  p_array<mln_site(I)> my_sort_increasing(const I& ima, const S& s)
  {
    p_array<mln_site(I)> v;
    convert::from_to(s, v);
    std::sort(v.hook_std_vector_().begin(), v.hook_std_vector_().end(),
	      my_less_<I>(ima));
    return v;
  }




  // Tree -> attributes (on nodes only).
  // -----------------------------------



  template <typename A, typename T>
  inline
  mln_ch_value(typename T::function, mln_result(A))
    compute_attribute_on_nodes(const A& a, const T& t)
  {
    typedef typename T::function I;

    mln_ch_value(I, A) acc;
    mln_ch_value(I, mln_result(A)) attr;

    // Initialization of 'acc'.
    {
      initialize(acc, t.f());
      data::fill(acc, a); // Transfer "dynamic data" (state) of 'a'.
    }

    // Initialize every attribute with the corresponding pixel.
    {
      mln_piter(I) p(t.f().domain());
      for_all(p)
	acc(p).take_as_init(make::pix(t.f(), p));
    }

    // Propagate attribute from a site to its parent.
    {
      mln_up_site_piter(T) p(t); // WAS: mln_fwd_piter(T) p(t.domain());
      for_all(p)
	if (! t.is_root(p))
	  acc(t.parent(p)).take(acc(p));
      // No back-propagation to non-node sites.
    }


    // Change accumulator into its result.
    {
      initialize(attr, acc);
      mln_node_piter(T) n(t);
      for_all(n)
      {
	mln_invariant(t.is_a_node(n));
	attr(n) = acc(n).to_result();
      }
    }

    return attr;
  } 






  // Tree -> attributes on every pixel.
  // Warning : it is not an attribute per flat zone!
  // -----------------------------------------------

  template <typename A, typename T>
  inline
  mln_ch_value(typename T::function, mln_result(A))
  compute_attribute_on_pixels__not_on_flat_zones(const A& a, const T& t)
  {
    typedef typename T::function I;

    mln_ch_value(I, A) acc;
    mln_ch_value(I, mln_result(A)) attr;

    // Initialization of 'acc'.
    {
      initialize(acc, t.f());
      data::fill(acc, a); // Transfer "dynamic data" (state) of 'a'.
    }

    // Initialize every attribute with the corresponding pixel.
    {
      mln_piter(I) p(t.f().domain());
      for_all(p)
	acc(p).take_as_init(make::pix(t.f(), p));
    }

    // Propagate attribute from a site to its parent.
    {
      mln_fwd_piter(T) p(t.domain());
      for_all(p)
	if (! t.is_root(p))
	  acc(t.parent(p)).take(acc(p));
    }


    // Change accumulator into its result.
    {
      initialize(attr, acc);
      mln_piter(I) p(t.f().domain()); // The main difference is here!
      for_all(p)
	attr(p) = acc(p).to_result();
    }

    return attr;
  } 




  // Tree -> nchildren (on nodes).
  // -----------------------------


  template <typename T>
  inline
  mln_ch_value(typename T::function, unsigned)
    compute_nchildren(const T& t)
  {
    typedef typename T::function I;
    mln_ch_value(I, unsigned) nchildren;
    initialize(nchildren, t.f());
    data::fill(nchildren, 0);

    {
      mln_up_node_piter(T) n(t);
      for_all(n)
  	if (! t.is_root(n))
  	  {
  	    mln_invariant(t.is_a_node(t.parent(n)));
  	    ++nchildren(t.parent(n)); // so parent(n) is a node
  	  }

      // WAS:
//       mln_fwd_piter(T) p(t.domain());
//       for_all(p)
//   	if (t.is_a_non_root_node(p))
//   	  {
//   	    mln_invariant(t.is_a_node(t.parent(p)));
//   	    ++nchildren(t.parent(p)); // so parent(p) is a node
//   	  }
    }
    
    return nchildren;
  }




  template <typename A, typename T, typename N>
  inline
  mln_concrete(typename T::function)
    run_filter(const A& a, const T& t, const N& nbh,
	       unsigned n_objects,
	       unsigned& less,
	       mln_value(A)& lambda,
	       bool echo = false)
  {
    typedef typename T::function I;

    unsigned n_regmins_f; // This value can be obtained while computing the attributes!
    mln_ch_value(I, unsigned) regmins_f = labeling::regional_minima(t.f(), nbh, n_regmins_f);
    if (echo)
      {
	debug::println("f =", t.f());
	debug::println("regmins(f) =", regmins_f);
	// debug::println("par on nodes = ", t.parent_image() | t.nodes());
	std::cout << "n regmins(f) = " << n_regmins_f << std::endl
		  << std::endl;
      }
    if (n_objects >= n_regmins_f)
      {
	std::cout << "warning: number of expected objects is greater than number of regmins!" << std::endl;
	std::cout << "aborting!" << std::endl;
	std::abort();
      }

    lambda = mln_max(mln_value(A));

    mln_ch_value(I, unsigned) nchildren = compute_nchildren(t);
    if (echo)
      debug::println("nchildren (before) =", nchildren | t.nodes());

    typedef p_array<mln_site(I)> S;
    S s = my_sort_increasing(a, t.nodes());

    const typename T::parent_t& par = t.parent_image();

    unsigned count = n_regmins_f;
    less = 0;

    // For display only.
    mln_ch_value(I, bool) filtered;
    initialize(filtered, t.f());
    data::fill(filtered, false);

    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      if (a(p) > lambda)
	break;

      if (a(p) < lambda && par(p) != p)
	{
	  mln_assertion(nchildren(par(p)) > 0);

	  filtered(p) = true; // For display only.

	  --nchildren(par(p));
	  if (nchildren(par(p)) != 0)
	    {
	      if (count <= n_objects)
		{
		  ++less; // minus 1 object wrt the expected number!
		}
	      --count;
	      if (count == n_objects)
		{
		  lambda = a(p) + 1;
		  std::cout << "lambda = " << lambda << std::endl
			    << std::endl;
		}
	    }
	}
    }

    if (echo)
      {
	back_propagate(t, filtered);
	debug::println("filtered =", filtered);
	debug::println("a < lambda = ", (pw::value(a) < pw::cst(lambda)) | a.domain());
      }


    if (less != 0)
      std::cerr << "WARNING: less objects (" << less << ") than expected..." << std::endl
		<< std::endl;

    if (echo)
      debug::println("nchildren (after) =", nchildren | t.nodes());

    // Filtering.
    mln_concrete(I) g;
    {
      initialize(g, t.f());

//       mln_bkd_piter(T) p(t.domain());

      mln_dn_site_piter(T) p(t);
      for_all(p)
	if (t.is_a_node(p) && a(p) >= lambda)
	  g(p) = t.f(p);
	else
	  g(p) = g(par(p));

      if (echo)
	debug::println("g =", g);
    }

    if (echo)
      {
	unsigned n_regmins_g;
	mln_VAR(regmin_g, labeling::regional_minima(g, nbh, n_regmins_g));
	debug::println( "regmin(g)", regmin_g);
	debug::println("a | regmin(g) = ", a | (pw::value(regmin_g) != 0));

      }

    return g;
  }



  template <typename A, typename I, typename N>
  void
  test_filter(A a, mln_result(A) lambda,
	      const I& f, const I& g, const N& nbh,
	      unsigned n_objects, unsigned less,
	      bool echo = false)
  {
    mln_concrete(I) g_ref = morpho::closing::leveling(f, nbh, A(), lambda);

    unsigned n_regmins_g_ref;
    mln_ch_value(I, unsigned) regmin_g_ref = labeling::regional_minima(g_ref, nbh, n_regmins_g_ref);
    bool consistency = (n_regmins_g_ref + less == n_objects);

    bool wrong_filtering = (g != g_ref);

    if (wrong_filtering)
      {
	std::cerr << "OOPS: g DIFFERS FROM ref!" << std::endl
		  << std::endl;
	if (echo)
	  {
	    debug::println("diff = ", (pw::value(g_ref) == pw::value(g)) | g.domain());
	    
	    debug::println("g_ref =", g_ref);
	    debug::println("regmin(g_ref) =", regmin_g_ref);
	    std::cout << "n_regmins(g_ref) = " << n_regmins_g_ref << std::endl
		      << std::endl;
	  }
      }

    if (consistency == false)
      {
	std::cerr << "OOPS: INCONSISTENCY (BUG...)!" << std::endl
		  << "      n_regmins(g_ref) = " << n_regmins_g_ref << std::endl
		  << "      n_less = "           << less << std::endl
		  << "      n_objects = "        << n_objects << std::endl
		  << std::endl;

	if (echo && ! wrong_filtering)
	  {
	    debug::println("diff = ", (pw::value(g_ref) == pw::value(g)) | g.domain());

	    debug::println("g_ref =", g_ref);
	    debug::println("regmin(g_ref) =", regmin_g_ref);
	    std::cout << "n_regmins(g_ref) = " << n_regmins_g_ref << std::endl
		      << std::endl;
	  }
      }
  }






  template <typename A, typename T, typename N>
  inline
  mln_concrete(typename T::function)
    do_filter(A& a, const T& t, const N& nbh,
	      const mln_value(A)& lambda)
  {
    typedef typename T::function I;

    const typename T::parent_t& par = t.parent_image();

    // No need for attributes back propagation since we process the
    // tree downwards: nodes are set *before* their children nodes and
    // their related (non-nodes) sites.

    mln_concrete(I) g;
    initialize(g, t.f());

    mln_dn_site_piter(T) p(t);
    for_all(p)
      if (t.is_a_node(p) && a(p) >= lambda)
	g(p) = t.f(p);
      else
	g(p) = g(par(p));
    
    return g;
  }





  // Filter facade.

  template <typename F, typename N, typename A>
  inline
  mln_concrete(F)
  filter(const F& f,  // a "gradient" of color image
	 const N& nbh,

	 const A& a_, // an attribute
	 unsigned change_kind,
	 bool do_extinction,

	 unsigned n_objects,
	 bool echo = false)
  {
    typedef p_array<mln_psite(F)> S;
    S s = level::sort_psites_decreasing(f);

    typedef morpho::tree::data<F,S> tree_t;
    tree_t t(f, s, nbh);

    mln_VAR(a, compute_attribute_on_nodes(a_, t));
    back_propagate(t, a);

    if (echo)
      {
	debug::println("par =", t.parent_image() | t.nodes());
	debug::println("a =", a | t.nodes());
	debug::println("a = ", a);
      }

    switch (change_kind)
      {
      case 0:
	break;
      case 1:
	std::cout << "move down attributes" << std::endl;
	move_down_attributes(t, a, echo);
	break;
      case 2:
	std::cout << "fuse up attributes" << std::endl;
	fuse_up_attributes(t, a, echo);
	break;
      }

    if (do_extinction)
      {
	std::cout << "do extinction" << std::endl;
	extinct_attributes(t, a, echo);
      }

    if (echo)
      debug::println("a' =", a | t.nodes());

    mln_value(a_t) lambda;
    unsigned less;
    mln_concrete(F) g = run_filter(a, t, nbh, n_objects, // input
				   less, lambda,         // output
				   echo);

    if (do_extinction == false && change_kind == 0)
      {
	std::cout << "testing filter result v. classical method" << std::endl;
	test_filter(a_, lambda, f, g, nbh,
		    n_objects, less,
		    echo);
      }

    return g;
  }



  // Granulometry.

  template <typename F, typename N, typename A>
  inline
  void
  gran_filter(const F& f, const N& nbh, const A& a_)
  {
    typedef p_array<mln_psite(F)> S;
    S s = level::sort_psites_decreasing(f);

    typedef morpho::tree::data<F,S> T;
    T t(f, s, nbh);

    mln_VAR(a, compute_attribute_on_nodes(a_, t));
    back_propagate(t, a);


    unsigned n_regmins_f; // This value can be obtained while computing the attributes!
    mln_ch_value(F, unsigned) regmins_f = labeling::regional_minima(t.f(), nbh, n_regmins_f);

    mln_ch_value(F, unsigned) nchildren = compute_nchildren(t);

    {

      typedef p_array<mln_site(F)> S;
      S s = my_sort_increasing(a, t.nodes());

      const typename T::parent_t& par = t.parent_image();
      unsigned count = n_regmins_f;

      mln_fwd_piter(S) p(s);
      p.start();
      std::ofstream file("gran.txt");
      file << a(p) << ' ' << count << std::endl;

      for_all(p)
      {
	if (par(p) != p)
	  {
	    mln_assertion(nchildren(par(p)) > 0);
	    --nchildren(par(p));
	    if (nchildren(par(p)) != 0)
	      {
		--count;
		file << a(p) << ' ' << count << std::endl;
	      }
	  }
      }
      file.close();

    }

  }



} // end of namespace mln
