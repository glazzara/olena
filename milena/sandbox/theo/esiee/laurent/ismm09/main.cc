
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

    template <typename W, typename L>
    inline
    void
    e_to_labels(const W& w, const point2d& e, L& l1, L& l2)
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


  } // end of namespace mln::cplx2d



  template <typename A, typename L>
  util::array<L>
  sort_by_increasing_attributes(const util::array<A>& a, L n_basins)
  {
    typedef std::pair<A,L> pair_t;
    std::vector<pair_t> v;
    v.reserve(n_basins);
    for (L l = 1; l <= n_basins; ++l)
      v.push_back(pair_t(a[l], l));

    std::sort(v.begin(), v.end());

    util::array<L> l(n_basins);
    for (unsigned i = 0; i < n_basins; ++i)
      l[i] = v[i].second;

    return l;
  }



} // end of namespace mln



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

  // f: regular image.

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);


  // g: weights on edges.

  mln_VAR(g, cplx2d::f_to_g(f) );
  debug::println("g:", g);

  typedef mln_value_(g_t) T;                 //  <---  Type of edge values.
  typedef mln_psite_(g_t) E;                 //  <---  Type of edges.


  mln_VAR(nbh_g, cplx2d::e2e()); // Neighborhood between edges.


  // w: watershed labeling on edges.

  typedef label_16 L;                        //  <---  Type of labels.
  L n_basins;
  mln_VAR( w, morpho::meyer_wst(g, nbh_g, n_basins) );
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
				       n_basins);

  util::array<L> l = sort_by_increasing_attributes(a, n_basins);

  {
    /*
    std::cout << "l:" << std::endl;
    for (unsigned i = 0; i < l.nelements(); ++i)
      std::cout << l[i] << "(" << a[l[i]] << ") ";
    std::cout << std::endl;
    */
  }


//   {
//     // Test adjacency "e -> (l1, l2)".
//     L l1, l2;
//     mln_piter_(g_t) e(g.domain());
//     for_all(e)
//       if (w(e) == 0)
// 	{
// 	  cplx2d::e_to_labels(w, e, l1, l2);
// 	  std::cout << e << ':' << l1 << ',' << l2 << std::endl;
// 	}
//   }



  // Edges -> Priority queue.

  typedef p_priority< T, p_queue<E> > Q;
  util::array<Q> q(n_basins.next());

  {
    L l1, l2;
    mln_piter_(g_t) e(g.domain());
    for_all(e)
      if (w(e) == 0)
	{
	  cplx2d::e_to_labels(w, e,    // input
			      l1, l2); // output
	  q[l1].push(mln_max(T) - g(e), e);
	  q[l2].push(mln_max(T) - g(e), e);
	}
  }


  // Information "label l -> edge e".

  E null = E(0,0);  // Impossible value.

  util::array<E> edge(n_basins.next());
  for (L l = 0; l <= n_basins; ++l)
    edge[l] = null;


  // Initialization.

  util::array<L> lpar(n_basins.next());
  for (L l = 0; l <= n_basins; ++l)
    lpar[l] = l; // Make-Set.



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
    debug::println("all edges:", epar); // epar(e) == e so we depict the edges!
  }



}
