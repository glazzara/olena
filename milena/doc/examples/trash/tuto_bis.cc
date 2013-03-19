# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/image/extended.hh>
# include <mln/core/routine/extend.hh>

# include <mln/core/alias/window2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>
# include <mln/core/site_set/p_centered.hh>
# include <mln/literal/origin.hh>

# include <mln/accu/stat/min_max.hh>
# include <mln/accu/stat/mean.hh>

# include <mln/fun/i2v/array.hh>
# include <mln/fun/p2v/iota.hh>

# include <mln/data/paste.hh>
# include <mln/data/fill.hh>
# include <mln/data/transform.hh>
# include <mln/extension/fill.hh>

# include <mln/morpho/meyer_wst.hh>
# include <mln/morpho/gradient.hh>
# include <mln/morpho/dilation.hh>

# include <mln/debug/println.hh>



namespace mln
{

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
      mln_trace("accu::compute");

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

    }

  } // mln::accu


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


  box2d domain(3, 5);
  image2d<unsigned> ima_(domain);
  mln_VAR(ima, ima_ | domain);

  mln_VAR(cell, ima | is_cell);
  data::fill(cell, fun::p2v::iota());
  debug::println(cell);
  // 1   2   3 
  //      
  // 4   5   6 

  mln_VAR(edge, extend((ima | is_edge).rw(), ima));

  data::paste(morpho::gradient(edge, e2c.win()), edge);
  //                                  ^^^
  //                         edge -> neighboring cells
  debug::println(edge);
  //   1   1   
  // 3   3   3 
  //   1   1

  image2d<unsigned> label(domain, 0);
  data::fill(label, 9);
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
  data::fill(wst, morpho::meyer_wst(edge, e2e, nbasins));
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


  data::paste(morpho::dilation(extend(lab, label),
				c4().win()),
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
  accu::compute<accu::meta::stat::mean>(cell, label, m);
  for (unsigned i = 1; i <= nbasins; ++i)
    std::cout << "mean value of basin #" << i << " is " << m(i) << std::endl; 

//   data::fill(cell, data::transform(lab, m));
//   debug::println(cell);
//   // 2   2   2 
//   //
//   // 5   5   5 

  // DONE!

}
