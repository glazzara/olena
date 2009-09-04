
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/extend.hh>

#include <mln/core/var.hh>

#include <mln/data/paste.hh>

#include <mln/morpho/gradient.hh>


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

} // end of namespace mln
