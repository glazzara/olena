# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/core/image/dmorph/extended.hh>
# include <mln/core/routine/extend.hh>

# include <mln/core/alias/window2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>
# include <mln/core/site_set/p_centered.hh>

# include <mln/literal/origin.hh>
# include <mln/literal/black.hh>
# include <mln/literal/white.hh>

# include <mln/value/int_u8.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>

# include <mln/value/rgb8.hh>
# include <mln/io/ppm/save.hh>

# include <mln/accu/min_max.hh>
# include <mln/accu/mean.hh>

# include <mln/fun/i2v/array.hh>
# include <mln/fun/p2v/iota.hh>

# include <mln/data/paste.hh>
# include <mln/data/fill.hh>
# include <mln/level/transform.hh>
# include <mln/extension/fill.hh>

# include <mln/morpho/meyer_wst.hh>
# include <mln/morpho/closing/volume.hh>

# include <mln/opt/at.hh>

# include <mln/debug/println.hh>



// Functions

inline
bool is_row_odd(const mln::point2d& p)
{
  return p.row() % 2;
}

inline
bool is_cell(const mln::point2d& p)
{
  return p.row() % 2 == 0 && p.col() % 2 == 0;
}

inline
bool is_edge(const mln::point2d& p)
{
  return p.row() % 2 + p.col() % 2 == 1;
}

inline
bool is_point(const mln::point2d& p)
{
  return p.row() % 2 && p.col() % 2;
}


inline
bool is_not_edge(const mln::point2d& p)
{
  return ! is_edge(p);
}



namespace mln
{

  namespace border
  {

    template <typename I>
    void
    fill(I& ima, const mln_value(I)& v)
    {
      const int nrows = ima.nrows();
      const int ncols = ima.ncols();
      for (int r = -1; r <= nrows; ++r)
	{
	  opt::at(ima, r, -1) = v;
	  opt::at(ima, r, ncols) = v;
	}
      for (int c = -1; c <= ncols; ++c)
	{
	  opt::at(ima, -1, c) = v;
	  opt::at(ima, nrows, c) = v;
	}
    }

  } // mln::border

  namespace accu
  {

    template <typename I, typename L, typename A, typename V>
    inline
    void
    compute(const Image<I>& input_,
	    const Image<L>& label_,
	    const Accumulator<A>&,
	    V& v)
    {
      trace::entering("accu::compute");

      const I& input = exact(input_);
      const L& label = exact(label_);

      const unsigned n = v.size();
      std::vector<A> a(n);

      mln_piter(I) p(input.domain());
      for_all(p)
	a[label(p)].take(input(p));

      for (unsigned l = 1; l < n; ++l)
	v(l) = a[l].to_result();

      trace::exiting("accu::compute");
    }

  } // mln::accu

  namespace morpho
  {

    template <typename I, typename N>
    mln_concrete(I)
    gradient(const I& input, const N& nbh)
    {
      mln_concrete(I) output;
      initialize(output, input);
      accu::min_max<mln_value(I)> mm;

      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
	mm.init();
	for_all(n) if (input.has(n))
	  mm.take(input(n));
	output(p) = mm.second() - mm.first();
      }
      return output;
    }

    template <typename I, typename N>
    mln_concrete(I)
    dilation(const I& input, const N& nbh)
    {
      typedef mln_value(I) V;
      // FIXME: extension::fill(input, mln_min(V));

      mln_concrete(I) output;
      initialize(output, input);
      accu::max<V> m;

      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
	m.init();
	for_all(n) if (input.has(n))
	  m.take(input(n));
	output(p) = m;
      }
      return output;
    }
    
  } // mln::morpho


  struct colorize : Function_v2v< colorize >
  {
    typedef value::rgb8 result;
    colorize(unsigned max)
      : lut(max + 1)
    {
      lut[0] = literal::black;
      for (unsigned i = 1; i <= max; ++i)
	lut[i] = result(100 + std::rand() % 150,
			100 + std::rand() % 150,
			100 + std::rand() % 150);
    }
    result operator()(unsigned i) const
    {
      return lut[i];
    }
    std::vector<result> lut;
  };


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

} // mln



template <typename T>
mln::image2d<T>
image2cells(const mln::image2d<T>& input)
{
  mln::image2d<T> output(2 * input.nrows() - 1,
			 2 * input.ncols() - 1);
  for (int row = 0; row < input.nrows(); ++row)
    for (int col = 0; col < input.ncols(); ++col)
      mln::opt::at(output, 2 * row, 2 * col) = mln::opt::at(input, row, col);
  return output;
}


template <typename T>
mln::image2d<T>
cells2image(const mln::image2d<T>& input)
{
  mln::image2d<T> output((input.nrows() + 1) / 2,
			 (input.ncols() + 1) / 2);
  for (int row = 0; row < input.nrows(); row += 2)
    for (int col = 0; col < input.ncols(); col += 2)
      mln::opt::at(output, row / 2, col / 2) =
        mln::opt::at(input, row, col);
  return output;
}




template <typename I>
void
do_it(I& ima, int lambda, const std::string& filename, unsigned& nbasins)
{
  using namespace mln;

  // e2c

  bool e2c_h[] = { 0, 1, 0,
		   0, 0, 0,
		   0, 1, 0 };

  bool e2c_v[] = { 0, 0, 0,
		   1, 0, 1,
		   0, 0, 0 };

  mln_VAR( e2c, make::double_neighb2d(is_row_odd, e2c_h, e2c_v) );

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

  // cell
  mln_VAR(cell, ima | is_cell);

  // edge
  mln_VAR(edge, extend((ima | is_edge).rw(),
		       pw::value(ima)));

  data::paste(morpho::gradient(edge, e2c), edge);
  //                                  ^^^
  //                         edge -> neighboring cells

  // 'edge' looks like:
  //
  //   1   1   
  // 3   3   3 
  //   1   1

  {
    io::pgm::save(display_edge(ima, 0, 3),
		  "temp_edge.pgm");
  }


  data::paste( morpho::closing::volume(edge, e2e, lambda), edge );
  

  image2d<unsigned> label(ima.bbox(), 0);
  //     mln_ch_value(I, unsigned) label;
  //     initialize(label, ima);


  mln_VAR(wst, label | is_edge);

  data::fill(wst, morpho::meyer_wst(edge, e2e, nbasins));
  //                                       ^^^
  //                         edge -> neighboring edges

  // 'wst' looks like:
  //
  //   2   2   
  // 0   0   0 
  //   1   1   

  colorize colors(nbasins);

  {
    image2d<value::rgb8> temp(label.domain());
    data::fill(temp, literal::white);

    data::paste( level::transform(label | is_edge,
				   colors),
		  temp );
    
    io::ppm::save(display_edge(temp, literal::white, 3),
		  "temp_label.ppm");
  }

  // YET THOSE VALUES ARE ON EDGES, NOT ON CELLS...


  mln_VAR(lab, label | is_cell);

  data::paste(morpho::dilation(extend(lab, label),
				c4()),
	       label);

  // 'lab' looks like:
  //
  // 2   2   2 
  //        
  // 1   1   1 

  io::ppm::save(level::transform(cells2image(label),
				 colors),
		filename);
}



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.ppm" << std::endl;
  std::cerr << "  lambda >= 0" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  image2d<int_u8> temp;
  io::pgm::load(temp, argv[1]);

  image2d<int_u8> ima = image2cells(temp);


  int lambda = atoi(argv[2]);
  if (lambda < 0)
    usage(argv);

  std::string filename(argv[3]);

  unsigned nbasins;
  do_it(ima, lambda, filename, nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;
}
