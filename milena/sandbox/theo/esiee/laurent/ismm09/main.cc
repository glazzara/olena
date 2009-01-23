
#include <mln/core/var.hh>

#include <mln/core/image/image2d.hh>
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
      debug::println(f_ | is_pixel);

      mln_VAR(g, f_ | is_edge);
      data::paste(morpho::gradient(extend(g, f_),
 				   e2p().win()),
 		  g);
      debug::println(g);

      return g;
    }


  } // end of namespace mln::face2

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

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);

  mln_VAR(g, cplx2d::f_to_g(f) );
}
