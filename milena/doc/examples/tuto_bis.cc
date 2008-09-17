# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image/extended.hh>
# include <mln/core/routine/extend.hh>

# include <mln/core/alias/window2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>
# include <mln/core/site_set/p_centered.hh>
# include <mln/literal/origin.hh>

# include <mln/accu/min_max.hh>
# include <mln/accu/mean.hh>

# include <mln/fun/i2v/array.hh>
# include <mln/fun/p2v/iota.hh>

# include <mln/level/paste.hh>
# include <mln/level/fill.hh>
# include <mln/level/transform.hh>

# include <mln/morpho/meyer_wst.hh>

# include <mln/debug/println.hh>



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
	  ima.at(r, -1) = v;
	  ima.at(r, ncols) = v;
	}
      for (int c = -1; c <= ncols; ++c)
	{
	  ima.at(-1, c) = v;
	  ima.at(nrows, c) = v;
	}
    }
    
  } // mln::border

  namespace accu
  {

    template <typename A_, typename I, typename L, typename V>
    inline
    void
    compute(const Image<I>& input_,
	    const Image<L>& label_,
	    V& v)
    {
      mlc_is_a(A_, Meta_Accumulator)::check();      
      trace::entering("accu::compute");

      const I& input = exact(input_);
      const L& label = exact(label_);

      typedef mln_accu_with(A_, mln_value(I)) A;
      const unsigned n = v.size();
      std::vector<A> a(n);

      mln_piter(I) p(input.domain());
      for_all(p)
	a[label(p)].take(input(p));

      for (unsigned l = 1; l < n; ++l)
	v(l) = a[l];

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
      accu::min_max_<mln_value(I)> mm;

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
      // extension::fill(input, mln_min(V));

      mln_concrete(I) output;
      initialize(output, input);
      accu::max_<V> m;

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

} // mln



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



int main()
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



//   {
//     p_centered<e2e_t::window> wc(e2e.to_window(), literal::origin);
//     std::cout << wc << std::endl;

//     p_set<point2d> s;
//     s += wc;
//     std::cout << s << std::endl;
//   }



  image2d<int> ima(3, 5);

  mln_VAR(cell, ima | is_cell);
  level::fill(cell, fun::p2v::iota);
  debug::println(cell);
  // 1   2   3 
  //      
  // 4   5   6 



  mln_VAR(edge, extend(inplace(ima | is_edge),
		       pw::value(ima)));
  level::paste(morpho::gradient(edge, e2c), edge);
  //                                  ^^^
  //                         edge -> neighboring cells
  debug::println(edge);
  //   1   1   
  // 3   3   3 
  //   1   1


  image2d<unsigned> label(ima.bbox(), 1);
  border::fill(label, 0);
  level::fill(label, 9);
  debug::println(label);
  // 9 9 9 9 9 
  // 9 9 9 9 9 
  // 9 9 9 9 9 


  mln_VAR(wst, label | is_edge);
  debug::println(wst);
  //   9   9   
  // 9   9   9 
  //   9   9   


  unsigned nbasins;
  level::fill(wst, morpho::meyer_wst(edge, e2e, nbasins));
  //                                       ^^^
  //                         edge -> neighboring edges
  debug::println(wst);
  //   2   2   
  // 0   0   0 
  //   1   1   

  std::cout << nbasins << " bassins" << std::endl;
  // 2 bassins




  // '0' IS THE TAG VALUE FOR THE WATERSHED LINE
  // THE OTHER VALUES ARE THE REGION LABELS


  mln_VAR(wst_line, wst | (pw::value(wst) == pw::cst(0u))); // FIXME: wst | 0
  std::cout << "the watershed line = " << wst_line.domain() << std::endl
	    << std::endl;
  // the watershed line = {(1,0)(1,2)(1,4)}
  //                       ^^^^^
  //                       meaning (row = 1, col = 0)

  // Actually wst is:
  //
  //      0 1 2 3 4  <- col
  //    +-----------+
  //  0 |   2   2   |
  //  1 | 0   0   0 |
  //  2 |   1   1   |
  //    +-----------+
  //  ^
  //  |
  // row



  // YET THOSE VALUES ARE ON EDGES, NOT ON CELLS...

  debug::println(label);
  // 9 2 9 2 9 
  // 0 9 0 9 0 
  // 9 1 9 1 9 


  mln_VAR(lab, label | is_cell);
  debug::println(lab);
  // 9   9   9 
  //        
  // 9   9   9 

  level::paste(morpho::dilation(extend(lab, pw::value(label)),
				c4()),
	       label);

  debug::println(label);
  // 2 2 2 2 2 
  // 0 9 0 9 0 
  // 1 1 1 1 1 

  debug::println(lab);
  // 2   2   2 
  //        
  // 1   1   1 


  // ABOVE, WE HAVE THE LABEL IMAGE (POINTS-ONLY).
  // BELOW, THE ORIGINAL IMAGE (POINTS-ONLY).


  debug::println(cell);
  // 1   2   3 
  //        
  // 4   5   6 


  // NOW WE WANT TO MODIFY THE INPUT IMAGE TO FLATTEN REGIONS...


  fun::i2v::array<int> m(nbasins + 1);
  accu::compute<accu::mean>(cell, label, m);
  for (unsigned i = 1; i <= nbasins; ++i)
    std::cout << "mean value of basin #" << i << " is " << m(i) << std::endl; 

  level::fill(cell, level::transform(lab, m));
  debug::println(cell);
  // 2   2   2 
  //
  // 5   5   5 


  // DONE!
}
