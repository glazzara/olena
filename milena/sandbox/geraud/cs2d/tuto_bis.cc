# include <vector>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image_if_value.hh>

# include <mln/core/neighb2d.hh>
# include <mln/core/window2d.hh>
# include <mln/convert/to_window.hh>

# include <mln/debug/println.hh>
# include <mln/fun/p2v/iota.hh>
# include <mln/level/paste.hh>
# include <mln/level/fill.hh>

# include <mln/morpho/dilation.hh>
# include <mln/morpho/gradient.hh>
# include <mln/morpho/meyer_wst.hh>

# include <mln/level/transform.hh>
# include <mln/accu/mean.hh>

# include "dbl_neighb.hh"


namespace mln
{


  namespace level
  {

    template<typename I, typename R>
    mln_ch_value(I, R)
      transform(const Image<I>& input_, const std::vector<R>& v)
    {
      trace::entering("level::transform");
      const I& input = exact(input_);

      mln_ch_value(I, R) output;
      initialize(output, input);

      mln_piter(I) p(input.domain());
      for_all(p)
	output(p) = v[input(p)];

      trace::exiting("level::transform");
      return output;
    }

  } // mln::level

  namespace accu
  {

    template <typename A_, typename I, typename L, typename R>
    inline
    void
    compute(const Image<I>& input_,
	    const Image<L>& label_,
	    std::vector<R>& v)
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
	v[l] = a[l].to_result();

      trace::exiting("accu::compute");
    }

  } // mln::accu

} // mln


  struct is_cell_t :  mln::Function_p2b<is_cell_t>
  {
    typedef bool result;
    bool operator()(const mln::point2d& p) const
    {
      return p.row() % 2 == 0 && p.col() % 2 == 0;
    }
  }
  is_cell;

  struct is_edge_t :  mln::Function_p2b<is_edge_t>
  {
    typedef bool result;
    bool operator()(const mln::point2d& p) const
    {
      return p.row() % 2 + p.col() % 2 == 1;
    }
  }
  is_edge;

  struct is_point_t :  mln::Function_p2b<is_point_t>
  {
    typedef bool result;
    bool operator()(const mln::point2d& p) const
    {
      return p.row() % 2 && p.col() % 2;
    }
  }
  is_point;


  struct is_row_odd_t
  {
    bool operator()(const mln::point2d& p) const
    {
      return p.row() % 2;
    }
  } is_row_odd;



#define mln_VAR(Var, Expr) \
typeof(Expr) Var = Expr;



int main()
{
  using namespace mln;

  window2d c4 = convert::to_window(mln::c4());


  dbl_neighb_<dpoint2d, is_row_odd_t> nbh_e2c, win_p_e2c, nbh_e2e;
  {
    bool e2c_h[] = { 0, 1, 0,
		     0, 0, 0,
		     0, 1, 0 };

    bool e2c_v[] = { 0, 0, 0,
		     1, 0, 1,
		     0, 0, 0 };
    nbh_e2c
      .when_true (make::neighb2d(e2c_h))
      .when_false(make::neighb2d(e2c_v));

    win_p_e2c = nbh_e2c;
    win_p_e2c
      .insert_true(make::dpoint2d(0,0))
      .insert_false(make::dpoint2d(0,0));

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
    nbh_e2e
      .when_true (make::neighb2d(e2e_h))
      .when_false(make::neighb2d(e2e_v));
  }


  image2d<int> ima(3, 5);

  mln_VAR(cell, ima | is_cell);
  level::fill(cell, fun::p2v::iota);
  debug::println(cell);
  // 1   2   3 
  //      
  // 4   5   6 

  mln_VAR(edge, ima | is_edge);
  level::paste(morpho::gradient(edge, nbh_e2c), edge);
  //                                  ^^^^^^^
  //                         edge -> neighbooring cells
  debug::println(edge);
  //   1   1   
  // 3   3   3 
  //   1   1

  unsigned nbasins;
  mln_VAR(wst, morpho::meyer_wst(edge, nbh_e2e, nbasins));
  //                                     ^^^^^^^
  //                         edge -> neighbooring edges
  debug::println(wst);
  //   2   2   
  // 0   0   0 
  //   1   1   

  std::cout << nbasins << " bassins" << std::endl;
  // 2 bassins


  // '0' IS THE TAG VALUE FOR THE WATERSHED LINE
  // THE OTHER VALUES ARE THE REGION LABELS


  std::cout << "the watershed line = " << (wst | 0).domain() << std::endl;
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


  mln_VAR(label, wst.full());
  debug::println(label);
  // 0 2 0 2 0 
  // 0 0 0 0 0 
  // 0 1 0 1 0 

  level::paste(morpho::dilation(label | is_cell, c4), label);

  debug::println(label);
  // 2 2 2 2 2 
  // 0 0 0 0 0 
  // 1 1 1 1 1 
  debug::println(label | is_cell);
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
 

  std::vector<int> m(nbasins + 1);
  accu::compute<accu::mean>(cell, label, m);
  for (unsigned i = 1; i <= nbasins; ++i)
    std::cout << "mean value of basin " << i << " is " << m[i] << std::endl; 

  level::fill(cell, level::transform(label, m));
  debug::println(cell);
  // 2   2   2 
  //
  // 5   5   5 

  // DONE!
}
