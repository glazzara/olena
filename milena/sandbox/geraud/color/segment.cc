# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/core/image/image_if.hh>
# include <mln/core/routine/extend.hh>
# include <mln/pw/all.hh>

# include <mln/io/ppm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/ppm/save.hh>
# include <mln/math/diff_abs.hh>

# include <mln/make/double_neighb2d.hh>
# include <mln/data/paste.hh>
# include <mln/morpho/closing_volume.hh>
# include <mln/morpho/meyer_wst.hh>
# include <mln/morpho/dilation.hh>
# include <mln/morpho/erosion.hh>

# include <mln/debug/println.hh>


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


  // Transforms.

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

  template <typename T>
  image2d<T>
  image2squares(const image2d<T>& input)
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
  squares2image(const image2d<T>& input)
  {
    image2d<T> output((input.nrows() + 1) / 2,
		      (input.ncols() + 1) / 2);
    for (int row = 0; row < input.nrows(); row += 2)
      for (int col = 0; col < input.ncols(); col += 2)
	opt::at(output, row / 2, col / 2) =
	  opt::at(input, row, col);
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

  image2d<rgb8> raw_input, input;

  io::ppm::load(raw_input, argv[1]);
  input = image2squares(raw_input);

  // e2c

  bool e2c_h[] = { 0, 1, 0,
		   0, 0, 0,
		   0, 1, 0 };

  bool e2c_v[] = { 0, 0, 0,
		   1, 0, 1,
		   0, 0, 0 };

  mln_VAR( e2c, make::double_neighb2d(is_row_odd, e2c_h, e2c_v) );



  image2d<int_u8> G = dist(extend(input | is_edge, pw::value(input)),
			   e2c);

  mln_VAR(G_edges, G | is_edge);

  /*
  {
    io::pgm::save(display_edge(G, 0, 3),
		  "temp_G.pgm");
  }
  */

  // e2e

  bool e2e_h[] = { 0, 0, 1, 0, 0,
		   0, 1, 0, 1, 0,
		   0, 0, 0, 0, 0,
		   0, 1, 0, 1, 0,
		   0, 0, 1, 0, 0 };

  bool e2e_v[] = { 0, 0, 0, 0, 0,
		   0, 1, 0, 1, 0,
		   1, 0, 0, 0, 1,
		   0, 1, 0, 1, 0,
		   0, 0, 0, 0, 0 };

  mln_VAR( e2e, make::double_neighb2d(is_row_odd, e2e_h, e2e_v) );

  data::paste( morpho::closing_volume(G_edges, e2e, 50), G_edges );

  /*
  {
    io::pgm::save(display_edge(G_edges.unmorph_(), 0, 3),
		  "temp_G_closed.pgm");
  }
  */

  int_u8 nbasins;
  mln_VAR(W_edges, morpho::meyer_wst(G_edges, e2e, nbasins));

  /*
  {
    io::pgm::save(display_edge(W_edges.unmorph_(), 0, 3),
		  "temp_W_edges.pgm");
  }
  */

  mln_VAR(W_all, W_edges.unmorph_());


  mln_VAR(W_squares, W_all | is_square);
  data::paste(morpho::dilation(extend(W_squares, pw::value(W_all)),
			       c4().win()),
	      W_all);

  mln_VAR(W_points, W_all | is_point);
  data::paste(morpho::erosion(extend(W_points, pw::value(W_all)),
			      c4().win()),
	      W_all);


   {
     io::pgm::save(W_all, "temp_W_all.pgm");
   }
  
}
