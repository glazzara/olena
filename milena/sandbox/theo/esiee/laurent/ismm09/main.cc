
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

#include <mln/morpho/gradient.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

#include <mln/accu/count.hh>
#include <mln/accu/height.hh>



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


  } // end of namespace mln::cplx2d


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

  } // internal


  template <typename T, typename A>
  void
  extinct_attributes(const T& t, // Tree.
		     A& a) // Attribute image.
  {
    mln_ch_value(A, bool) mark;
    initialize(mark, a);
    data::fill(mark, false);
    
    internal::node_pred<T> node_only;
    node_only.t = &t;

    typedef p_array<mln_site(A)> S;
    S s = level::sort_psites_increasing(a | node_only);

    mln_fwd_piter(S) p(s);
    for_all(p)
      {
	if (mark(p) == true)
	  continue;
	internal::rec(t, a, mark, p);
      }
  }


  template <typename F, typename N, typename A, typename W>
  void // util::array<unsigned>
  compute_attribute_extincted(const F& f, const N& nbh, const A& a,
			      const W& w)
  {
    typedef value::label_16 L;
    L n_basins;
    mln_ch_value(F,L) regmins = labeling::regional_minima(f, nbh, n_basins);
    
    typedef p_array<mln_psite(F)> S;
    S s = level::sort_psites_decreasing(f);
    
    typedef morpho::tree::data<F,S> tree_t;
    tree_t t(f, s, nbh);
    mln_VAR(a_ima, morpho::tree::compute_attribute_image(a, t));

    std::cout << "BEFORE:" << std::endl;
    debug::println("a_ima:", a_ima);
    debug::println("a_ima | w_line:", a_ima | (pw::value(w) == 0));
    debug::println("a_ima | basins:", a_ima | (pw::value(w) != 0));
    // debug::println("a_ima | regmins:", a_ima | (pw::value(regmins) != 0));


    extinct_attributes(t, a_ima);

    std::cout << "AFTER:" << std::endl;
    debug::println("a_ima:", a_ima);
    debug::println("a_ima | w_line:", a_ima | (pw::value(w) == 0));
    debug::println("a_ima | basins:", a_ima | (pw::value(w) != 0));
    debug::println("a_ima | regmins:", a_ima | (pw::value(regmins) != 0));
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


  // w: watershed labeling on edges.

  typedef label_16 L;
  L n_basins;
  mln_VAR( w, morpho::meyer_wst(g, cplx2d::e2e(), n_basins) );

  {
    L n_regmins;
    mln_VAR(regmins, labeling::regional_minima(g, cplx2d::e2e(), n_regmins));
    mln_invariant(n_regmins == n_basins);
    debug::println("regmins(g):", regmins);

    debug::println("w:", w);
    std::cout << "n basins = " << n_basins << std::endl
	      << std::endl;

//     mln_VAR(w_ext, cplx2d::extend_w_edges_to_all_faces(w));
//     debug::println("w_ext:", w_ext);
    
//     mln_VAR(is_line, pw::value(w_ext) == pw::cst(0));
//     debug::println("w line:", w_ext | is_line);
  }

  // accu::count< util::pix<g_t> > a_;
  accu::height<g_t> a_;

  compute_attribute_extincted(g, cplx2d::e2e(), a_,
			      w);

}
