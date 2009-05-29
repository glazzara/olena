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
# include <mln/value/int_u16.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>

# include <mln/value/rgb8.hh>
# include <mln/io/ppm/load.hh>
# include <mln/io/ppm/save.hh>

# include <mln/accu/min_max.hh>

# include <mln/fun/i2v/array.hh>
# include <mln/fun/p2v/iota.hh>

# include <mln/data/paste.hh>
# include <mln/data/fill.hh>
# include <mln/data/transform.hh>
# include <mln/extension/fill.hh>

# include <mln/morpho/meyer_wst.hh>
# include <mln/morpho/closing_area.hh>

# include <mln/opt/at.hh>

# include <mln/debug/println.hh>

# if 0
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/util/pix.hh>
# include <mln/morpho/includes.hh>
# include <mln/data/sort_psites.hh>
#endif

# include "src/distance.hh"

namespace mln
{
  template <typename I, typename N, typename Ic, typename Nc>
  struct min_tree_
  {
    typedef mln_site(I) point;
    typedef p_array<point> S;

    // in:
    const I& f;
    const N& nbh;
    const Ic& ref;
    const Nc& nbhc;

    // aux:
    S s;
    mln_ch_value(I, bool)  deja_vu;
    mln_ch_value(I, point) parent;
    mln_ch_value(I, bool) resp;
    mln_ch_value(I, point) zpar;

    // attached data:
    int lambda;
    mln_ch_value(I, int) area;
    //mln_ch_value(Ic, value::rgb8) values;
    //initialize(values, ref);
    //mln_ch_value(I, int) comp;

    min_tree_(const I& f, const N& nbh, const Ic& ref, const Nc& nbhc,
              int lambda)
      : f(f),
	nbh(nbh),
	ref(ref),
	nbhc(nbhc),
        lambda(lambda)
    {
      run();
    }

    void run()
    {
      // init
      {
	initialize(deja_vu, f);
	initialize(parent, f);
	initialize(resp, f);
	initialize(zpar, f);
	initialize(area, f);
        //initialize(comp, f);

	mln::data::fill(deja_vu, false);
	//mln::data::fill(resp, false);
        mln::data::fill(area, 0);

	s = data::sort_psites_increasing(f);
      }

      // first pass
      {
	mln_fwd_piter(S) p(s);
	mln_niter(N) n(nbh, p);
	for_all(p)
	{
	  make_set(p);
	  for_all(n)
	    if (f.has(n) && deja_vu(n))
	      do_union(n, p);
	  deja_vu(p) = true;
	}
      }

      // second pass: canonization
      {
	mln_bkd_piter(S) p(s);
	for_all(p)
	{
	  point q = parent(p);
	  if (f(parent(q)) == f(q))
          {
	    parent(p) = parent(q);
            resp(q) = false;
          }
	}
      }

      // third pass: Merging region with area < lambda
      {
	mln_fwd_piter(S) p(s);
	for_all(p)
	{
          if (resp(p) && area(p) < lambda)
          {
            resp(p) = false;
            update_data(parent(p), area(p));
          }
	}
      }
    } // end of run()

    void make_set(const point& p)
    {
      parent(p) = p;
      zpar(p) = p;
      init_data(p);
    }

    void set_parent(const point& r, const point& p)
    {
      parent(r) = p;
      merge_data(r, p);
    }

    bool is_root(const point& p) const
    {
      return parent(p) == p;
    }

    bool is_node(const point& p) const
    {
      //return is_root(p) || f(parent(p)) != f(p);
      return (is_root(p) || resp(p));
    }

    point find_root(const point& x)
    {
      if (zpar(x) == x)
	return x;
      else
	return zpar(x) = find_root(zpar(x));
    }

    point find_representative(const point& x)
    {
      if (parent(x) == x || resp(x))
	return x;
      else
	return find_representative(parent(x));
    }

    void do_union(const point& n, const point& p)
    {
       point r = find_root(n);
      if (r != p)
      {
	set_parent(r, p);
	zpar(r) = p;
      }
    }

    void init_data(const point& p)
    {
      area(p) = 1;
      resp(p) = true;
    }

    void merge_data(const point& r, const point& p)
    {
      if (f(p) == f(r))
      {
        resp(p) = false;
        area(p) += area(r);
      }
    }

    void update_data(const point& p, int val)
    {
      area(p) += val;
      if (parent(p) != p && !resp(p))
        update_data(parent(p), val);
    }

  };
}

namespace mln
{
  image2d<value::int_u16> convert_to_grey(const image2d<value::rgb8>& data)
  {
    image2d<value::int_u16> output(data.domain());
    mln_piter_(image2d<value::int_u16>) p(output.domain());
    for_all(p)
      output(p) = (int) (data(p).red() * 0.3 + data(p).green() * 0.58 + data(p).blue()) * 0.12;
    return output;
  }
}  // end of mln

namespace mln
{

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

    mln_VAR(edge, ima | is_edge);
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

  template <typename I>
  I display_edge(const I& ima, unsigned zoom)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
             ncols * (zoom + 1) - 1);

    mln_VAR( cell, ima | is_cell );
    mln_piter(cell_t) q(cell.domain());
    for_all(q)
    {
      unsigned row = (q.row() / 2) * (zoom + 1);
      unsigned col = (q.col() / 2) * (zoom + 1);
      for (unsigned i = 0; i < zoom; ++i)
        for (unsigned j = 0; j < zoom; ++j)
          opt::at(output, row + i, col + j) = ima(q);
    }

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


  namespace morpho
  {

    template <typename I, typename N>
    mln_concrete(I)
    dilation(const I& input, const N& nbh)
    {
      typedef mln_value(I) V;

      mln_concrete(I) output;
      initialize(output, input);

      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
	for_all(n)
	  if (input.has(n) && input(n) != value::rgb8(0,0,0))
	    output(p) = input(n);
      }
      return output;
    }
  } // mln::morpho


} // mln



template <typename T>
mln::image2d<T>
image2cells(const mln::image2d<T>& input)
{
  mln::image2d<T> output(2 * input.nrows() - 1,
			 2 * input.ncols() - 1);
  for (unsigned row = 0; row < input.nrows(); ++row)
    for (unsigned col = 0; col < input.ncols(); ++col)
      mln::opt::at(output, 2 * row, 2 * col) = mln::opt::at(input, row, col);
  return output;
}


template <typename T>
mln::image2d<T>
cells2image(const mln::image2d<T>& input)
{
  mln::image2d<T> output((input.nrows() + 1) / 2,
			 (input.ncols() + 1) / 2);
  for (unsigned row = 0; row < input.nrows(); row += 2)
    for (unsigned col = 0; col < input.ncols(); col += 2)
      mln::opt::at(output, row / 2, col / 2) = mln::opt::at(input, row, col);
  return output;
}


template <typename I, typename N, typename Ic, typename Nc>
unsigned min_tree(const I& f, const N& nbh, const Ic& ref, const Nc& nbhc,
                  int lambda)
{
  using namespace mln;

  min_tree_<I,N,Ic,Nc> run(f, nbh, ref, nbhc, lambda);


  mln_piter(I) p(f.domain());
  unsigned nnodes = 0;
  for_all(p)
  {
    if (run.is_node(p))
    {
      std::cout << "nodes: " << p << std::endl;
      ++nnodes;
    }
  }

  colorize colors(nnodes);
  image2d<value::rgb8> tmp(ref.domain());
  data::fill(tmp, ref);

  mln_piter(I) q(f.domain());
  unsigned int i = 0;
  for_all(q)
  {
    if (run.is_node(q))
    {
      tmp(q) = colors(i);
      i++;
    }
  }
  mln_piter(I) r(f.domain());
  for_all(r)
  {
    if (!run.is_node(r))
    {
      tmp(r) = tmp(run.find_representative(r));
    }
  }


  image2d<value::rgb8> to_display(tmp.domain());

  data::fill(to_display, value::rgb8(255, 255, 255));
  data::paste((tmp | is_edge), to_display);
  data::paste(morpho::dilation(to_display, c4()), to_display);

  io::ppm::save(display_edge(tmp, literal::white, 3),
                "edge.ppm");
  io::ppm::save(tmp, "full.ppm");
  io::ppm::save(cells2image(to_display), "colorize.ppm");

  return nnodes;
}


template <typename I>
I
do_it(I& input, int lambda, unsigned& nbasins)
{
  using namespace mln;

  /// Graph creation
  I graph;
  create_graph(input, graph, value::rgb8(0, 0, 0));

  // Initialization
  image2d<value::int_u16> ima = convert_to_grey(graph);

  // Neigbhorhood
  // e2c
  bool e2c_h[] = { 0, 1, 0,
		   0, 0, 0,
		   0, 1, 0 };
  bool e2c_v[] = { 0, 0, 0,
		   1, 0, 1,
		   0, 0, 0 };

  mln_VAR(e2c, make::double_neighb2d(is_row_odd, e2c_h, e2c_v));

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
  mln_VAR(e2e, make::double_neighb2d(is_row_odd, e2e_h, e2e_v));

  // Algorithm
  distance(extend((graph | is_edge).rw(), pw::value(graph)), e2c, ima);
  io::pgm::save(ima, "edge.pgm");

  nbasins = min_tree((ima | is_edge), e2e, graph, e2c, lambda);

  return graph;
}

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda" << std::endl;
  std::cerr << "  lambda >= 0" << std::endl;
  abort();
}

int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 3)
    usage(argv);

  int lambda = atoi(argv[2]);
  if (lambda < 0)
    usage(argv);

  image2d<value::rgb8> ima;
  io::ppm::load(ima, argv[1]);

  unsigned nbasins;
  image2d<value::rgb8> output = do_it(ima, lambda, nbasins);

  //io::ppm::save(output, argv[3]);
}
