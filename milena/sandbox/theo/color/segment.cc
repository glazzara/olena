#include <cstdlib>

#include <mln/core/var.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/box3d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>

#include <mln/core/image/image_if.hh>
#include <mln/pw/all.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/data/paste.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/math/diff_abs.hh>

#include <mln/debug/println.hh>

#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/morpho/closing_attribute.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>
#include <mln/labeling/regional_minima.hh>


#include <mln/accu/count.hh>
#include <mln/accu/volume.hh>

#include "sum_pix.hh"


namespace mln
{

  point3d color2point(const value::rgb8& c)
  {
    point3d p(c.red(), c.green(), c.blue());
    return p;
  }


  image2d<value::rgb8> blen_image;


  template <typename B>
  void accu_blen_take(B& b, const point2d& e)
  {
    if (e.row() % 2)
      {
	b.take(color2point(blen_image(e + up)));
	b.take(color2point(blen_image(e + down)));
      }
    else
      {
	b.take(color2point(blen_image(e + left)));
	b.take(color2point(blen_image(e + right)));
      }
  }

}

#include "blen_pix.hh"




namespace mln
{

  // Functions.

  inline
  bool is_row_odd(const point2d& p)
  {
    return p.row() % 2;
  }

  inline
  bool is_square(const point2d& p)
  {
    return p.row() % 2 == 0 && p.col() % 2 == 0;
  }

  inline
  bool is_edge(const point2d& p)
  {
    return p.row() % 2 + p.col() % 2 == 1;
  }

  inline
  bool is_point(const point2d& p)
  {
    return p.row() % 2 && p.col() % 2;
  }


  // Neighborhoods.

  typedef neighb< win::multiple<window2d, bool(*)(const point2d&)> > dbl_neighb2d;
  
  const dbl_neighb2d& e2c()
  {
    static bool e2c_h[] = { 0, 1, 0,
			    0, 0, 0,
			    0, 1, 0 };
    static bool e2c_v[] = { 0, 0, 0,
			    1, 0, 1,
			    0, 0, 0 };
    static dbl_neighb2d nbh = make::double_neighb2d(is_row_odd, e2c_h, e2c_v);
    return nbh;
  }

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


  // Transforms.

  template <typename T>
  image2d<T>
  image2full(const image2d<T>& input)
  {
    image2d<T> output(2 * input.nrows() - 1,
		      2 * input.ncols() - 1);
    for (int row = 0; row < input.nrows(); ++row)
      for (int col = 0; col < input.ncols(); ++col)
	opt::at(output, 2 * row, 2 * col) = opt::at(input, row, col);
    return output;
  }
  
  template <typename T>
  image2d<T>
  full2image(const image2d<T>& input)
  {
    image2d<T> output((input.nrows() + 1) / 2,
		      (input.ncols() + 1) / 2);
    for (int row = 0; row < input.nrows(); row += 2)
      for (int col = 0; col < input.ncols(); col += 2)
	opt::at(output, row / 2, col / 2) =
	  opt::at(input, row, col);
    return output;
  }


  // Display.

  template <typename I>
  I display_edge(const I& ima, mln_value(I) bg, unsigned zoom)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
	     ncols * (zoom + 1) - 1);
    data::fill(output, bg);
    mln_VAR( edge, ima | is_edge );
    mln_piter(edge_t) p(edge.domain());
    for_all(p)
      if (p.row() % 2) // horizontal edge
	{
	  unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
	  unsigned col = (p.col() / 2) * (zoom + 1);
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row, col + i) = ima(p);
	}
      else // vertical edge
	{
	  unsigned row = (p.row() / 2) * (zoom + 1);
	  unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row + i, col) = ima(p);
	}
    return output;
  }



  // Distance.

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

  template <typename I, typename N>
  image2d<value::int_u8>
  dist(const I& input, const N& nbh)
  {
    image2d<value::int_u8> output;
    initialize(output, input);
    data::fill(output, 0);

    mln_piter(I) p(input.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      n.start();
      value::rgb8 c1 = input(n);
      n.next();
      value::rgb8 c2 = input(n);
      output(p) = dist(c1, c2);
    }

    return output;
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
      mln_fwd_piter(T) p(t.domain());
      for_all(p)
	if (! t.is_root(p))
	  acc(t.parent(p)).take(acc(p));
      // No back-propagation to non-node sites.
    }


    // Change accumulator into its result.
    {
      initialize(attr, acc);
      typedef typename T::nodes_t N;
      mln_piter(N) p(t.nodes());
      for_all(p)
      {
	mln_invariant(t.is_a_node(p));
	attr(p) = acc(p).to_result();
      }
    }

    return attr;
  } 


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
      mln_fwd_piter(T) p(t.domain());
      // Propagate attribute from a site to its parent.
      for_all(p)
  	if (t.is_a_non_root_node(p))
  	  {
  	    mln_invariant(t.is_a_node(t.parent(p)));
  	    ++nchildren(t.parent(p)); // so parent(p) is a node
  	  }
    }
    
    return nchildren;
  }




  template <typename A, typename T, typename N>
  inline
  mln_concrete(typename T::function)
  filter(const A& a, const T& t, const N& nbh,
	 unsigned n_objects,
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
	std::cout << "aborting..." << std::endl;
	return duplicate(t.f());
      }

    lambda = mln_max(mln_value(A));

    mln_ch_value(typename T::function, unsigned) nchildren = compute_nchildren(t);

    typedef p_array<mln_site(I)> S;
    S s = my_sort_increasing(a, t.nodes());

    const typename T::parent_t& par = t.parent_image();

    unsigned
      count = n_regmins_f,
      less  = 0;
    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      if (a(p) < lambda && par(p) != p)
	{
	  mln_assertion(nchildren(par(p)) > 0);
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
		  // break; // Stop iterations.
		}
	    }
	}
    }

    if (less != 0)
      std::cerr << "WARNING: less objects (" << less << ") than expected..." << std::endl
		<< std::endl;

    if (echo)
      debug::println("nchildren =", nchildren | t.nodes());

    // Filtering.
    mln_concrete(I) g;
    {
      initialize(g, t.f());
      mln_bkd_piter(T) p(t.domain());
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
	debug::println( "regmin(g)", labeling::regional_minima(g, nbh, n_regmins_g) );
      }

    return g;
  }


  template <typename A, typename I, typename N>
  void
  test_filter(A a, mln_result(A) lambda,
	      const I& f, const I& g, const N& nbh,
	      bool echo = false)
  {
    mln_concrete(I) g_ref = morpho::closing_attribute<A>(f, nbh, lambda);
    if (echo)
      debug::println("g_ref =", g_ref);

    unsigned n_regmins_g_ref;
    mln_ch_value(I, unsigned) regmin_g = labeling::regional_minima(g_ref, nbh, n_regmins_g_ref);
//     if (echo)
      std::cout << "n_regmins(g_ref) = " << n_regmins_g_ref << std::endl
		<< std::endl;
      
    if (g != g_ref)
      {
	std::cerr << "OOPS: g DIFFERS FROM ref!" << std::endl
		  << std::endl;

// 	debug::println("diff", (pw::value(g_ref) == pw::value(g)) | g.domain());

// 	debug::println("g_ref", g_ref);
// 	debug::println("g", g);
// 	debug::println("regmin_g", regmin_g);
      }

//     bool consistency = (n_regmins_g_ref + less == n_objects);
//     if (consistency == false)
//       std::cerr << "OOPS: INCONSISTENCY (BUG...)!" << std::endl
// 		<< std::endl;
  }



} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.ppm n output.ppm" << std::endl;
  std::cerr << "  n >= 2" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 4)
    usage(argv);


  {

    // Color version.
    // --------------


    image2d<rgb8> input;
    io::ppm::load(input, argv[1]);

    unsigned n_objects = atoi(argv[2]);


    // Changing input into 'f on edges'.

    image2d<int_u8> f_;
    image2d<rgb8> input_ = image2full(input);
    {
      f_ = dist(extend(input_ | is_edge, pw::value(input_)),
		e2c());
    }
    mln_VAR(f, f_ | is_edge);
    typedef f_t I;


    // Filtering f -> g.

    typedef p_array<point2d> S;
    S s = level::sort_psites_decreasing(f);

    typedef morpho::tree::data<I,S> tree_t;
    tree_t t(f, s, e2e());

    //  accu::count< util::pix<I> > a_;
    //  accu::volume<I> a_;
    //  accu::sum_pix< util::pix<I> > a_;

    blen_image = input_;
    accu::blen_pix<I> a_;

    mln_VAR(a, compute_attribute_on_nodes(a_, t));

    mln_value_(a_t) lambda;
    f_t g = filter(a, t, e2e(), n_objects, lambda);

    test_filter(a_, lambda, f, g, e2e());


    // Watershed transform.

    int_u8 nbasins;
    mln_ch_value_(f_t, int_u8) w_edges = morpho::meyer_wst(g, e2e(), nbasins);

    // io::pgm::save(display_edge(w_edges.unmorph_(), 0, 3), "temp_w_edges.pgm");

    image2d<int_u8> w_all = w_edges.unmorph_();
    {
      // edges -> squares
      mln_VAR(w_squares, w_all | is_square);
      data::paste(morpho::dilation(extend(w_squares, pw::value(w_all)),
				   c4().win()),
		  w_all);
      // edges -> points
      mln_VAR(w_points, w_all | is_point);
      data::paste(morpho::erosion(extend(w_points, pw::value(w_all)),
				  c4().win()),
		  w_all);
    }

    io::pgm::save(w_all, "temp_w_all.pgm");
  }


//   {
//     // Gray-level version.

//     image2d<int_u8> f_;
//     io::pgm::load(f_, argv[1]);

//     unsigned n_objects = atoi(argv[2]);

//     mln_VAR(f, f_ | is_edge);
//     typedef f_t I;

//     typedef p_array<point2d> S;
//     S s = level::sort_psites_decreasing(f);

//     typedef morpho::tree::data<I,S> tree_t;
//     tree_t t(f, s, e2e());

//     accu::count< util::pix<I> > a_;
//     mln_VAR(a, compute_attribute_on_nodes(a_, t));

//     f_t g = filter(a, t, e2e(), n_objects);

//     unsigned nbasins;
//     debug::println("wst =", morpho::meyer_wst(g, e2e(), nbasins));
//   }


}
