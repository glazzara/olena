# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>
# include <mln/literal/black.hh>
# include <mln/literal/white.hh>
# include <mln/literal/colors.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/data/fill.hh>


namespace mln
{

  //   0 1 2
  // 0 c c c
  // 1 c c c

  //   0 1 2 3 4
  // 0 c e c e c
  // 1 e x e x e
  // 2 c e c e c


  struct is_cell_t :  Function_v2b<is_cell_t>
  {
    typedef bool result;
    bool operator()(const point2d& p) const
    {
      return p.row() % 2 == 0 && p.col() % 2 == 0;
    }
  }
  is_cell;


  struct is_edge_t :  Function_v2b<is_edge_t>
  {
    typedef bool result;
    bool operator()(const point2d& p) const
    {
      return p.row() % 2 + p.col() % 2 == 1;
    }
  }
  is_edge;


  struct is_vertex_t :  Function_v2b<is_vertex_t>
  {
    typedef bool result;
    bool operator()(const point2d& p) const
    {
      return p.row() % 2 && p.col() % 2;
    }
  }
  is_vertex;



  void display_masks(box2d b)
  {
    box2d b_(2 * b.nrows() - 1, 2 * b.ncols() - 1);
    image2d<char> tmp(b_);
    mln_piter_(box2d) p(b_);
    for_all(p)
      if (is_cell(p))
	{
	  mln_invariant(! is_edge(p) && ! is_vertex(p));
	  tmp(p) = 'O';
	}
      else if (is_edge(p))
	{
	  mln_invariant(! is_cell(p) && ! is_vertex(p));
	  tmp(p) = p.to_point().row() % 2 ? '-' : '|';
	}
      else if (is_vertex(p))
	{
	  mln_invariant(! is_edge(p) && ! is_cell(p));
	  tmp(p) = '+';
	}
      else
	mln_invariant(0);
    debug::println(tmp);
  }


  struct colorize_t
  {
    typedef value::rgb8 result;
    result operator()(value::int_u8 i) const
    {
      return result(i,i,i);
    }
    result operator()(bool b) const
    {
      if (b)
	return literal::red;
      else
	return literal::black;
    }
  }
  colorize;


  template <typename Cs, typename F>
  image2d<mln_result(F)> pack(const Cs& cs, F f)
  {
    image2d<mln_result(F)> output(cs.b);
    mln_piter(box2d) p(cs.b);
    for_all(p)
      if (is_cell(p))
	output(p) = f(cs.cell(p));
      else if (is_edge(p))
	output(p) = f(cs.edge(p));
      else
	output(p) = f(cs.vertex(p));
    return output;
  }


  template <typename Cs, typename F>
  image2d<mln_result(F)> pack(const Cs& cs, F f, unsigned clen)
  {
    typedef mln_result(F) R;

    mln_precondition(clen % 2); // oddity

    typedef win::rectangle2d Sqr;
    Sqr sqr(clen, clen);

    typedef win::hline2d H;
    H hline(clen);

    typedef win::vline2d V;
    V vline(clen);

    //   0 1 2 3 4
    // 0 c e c e c
    // 1 e x e x e
    // 2 c e c e c

    //     0 1 2 3 4 5 6 7 8 9 10

    //  0  c c c . e . c c c . e . c c c
    //  1  c c c . e . c c c . e . c c c
    //  2  c c c . e . c c c . e . c c c
    //  3  . . . . . . . . . . . . . . .
    //  4  e e e . x . e e e . x . e e e
    //  5  . . . . . . . . . . . . . . .
    //  6  c c c . e . c c c . e . c c c
    //  7  c c c . e . c c c . e . c c c
    //  8  c c c . e . c c c . e . c c c

    box2d b(cs.nrows() * clen + 3 * (cs.nrows() - 1),
	    cs.ncols() * clen + 3 * (cs.ncols() - 1));
    image2d<R> output(b);
    data::fill(output, literal::black);

    // Cells.
    mln_piter(Cs::cell_t) c(cs.cell.domain());
    for_all(c)
      {
	R v = f(cs.cell(c));
	point2d c_ = c.to_point(), p_;
	p_.row() = clen / 2 + (clen + 3) * (c_.row() / 2);
	p_.col() = clen / 2 + (clen + 3) * (c_.col() / 2);
	mln_qiter(Sqr) q(sqr, p_);
	for_all(q)
	  output(q) = v;
      }

    // Points.
    mln_piter(Cs::vertex_t) x(cs.vertex.domain());
    for_all(x)
      {
	point2d x_ = x, q;
	q.row() = clen + 1 + (clen + 3) * (x_.row() / 2);
	q.col() = clen + 1 + (clen + 3) * (x_.col() / 2);
	output(q) = f(cs.vertex(x));
      }


    //   0 1 2 3 4
    // 0 c e c e c
    // 1 e x e x e
    // 2 c e c e c

    //     0 1 2 3 4 5 6 7 8 9 10

    //  0  c c c . e . c c c . e . c c c
    //  1  c c c . e . c c c . e . c c c
    //  2  c c c . e . c c c . e . c c c
    //  3  . . . . . . . . . . . . . . .
    //  4  e e e . x . e e e . x . e e e
    //  5  . . . . . . . . . . . . . . .
    //  6  c c c . e . c c c . e . c c c
    //  7  c c c . e . c c c . e . c c c
    //  8  c c c . e . c c c . e . c c c


    // Edges.
    mln_piter(Cs::edge_t) e(cs.edge.domain());
    for_all(e)
      {
	R v = f(cs.edge(e));
	point2d e_ = e, p_;
	if (e_.row() % 2) // Odd => horizontal.
	  {
	    p_.row() = clen + 1 + (clen + 3) * (e_.row() / 2); // vertex-like
	    p_.col() = clen / 2 + (clen + 3) * (e_.col() / 2); // cell-like
	    mln_qiter(V) q(hline, p_);
	    for_all(q) if (output.has(q))
	      output(q) = v;
	  }
	else // Even => vertical.
	  {
	    p_.row() = clen / 2 + (clen + 3) * (e_.row() / 2); // cell-like
	    p_.col() = clen + 1 + (clen + 3) * (e_.col() / 2); // vertex-like
	    mln_qiter(H) q(vline, p_);
	    for_all(q) if (output.has(q))
	      output(q) = v;
	  }
      }

    return output;
  }



  template <typename Cs, typename T>
  void fill_cells_with(Cs& cs, const image2d<T>& input)
  {
    mlc_equal(T, typename Cs::cell_value)::check();
    mln_piter(box2d) p(input.domain());
    mln_piter(Cs::cell_t) c(cs.cell.domain());
    for_all_2(p, c)
      cs.cell(c) = input(p);
  }

  template <typename Cs>
  image2d<typename Cs::cell_value>
  cell_image(const Cs& cs)
  {
    image2d<typename Cs::cell_value> output(cs.nrows(), cs.ncols());
    mln_piter(box2d) p(output.domain());
    mln_piter(Cs::cell_t) c(cs.cell.domain());
    for_all_2(p, c)
      output(p) = cs.cell(c);
    return output;
  }

} // mln


