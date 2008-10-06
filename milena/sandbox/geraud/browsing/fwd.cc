# include <mln/core/image/image2d.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/accu/min.hh>

# include <mln/debug/println.hh>
# include <mln/debug/iota.hh>

# include "fwd.hh"


struct base
{
  void init()  {}
  void final() {}

  void next()  {}

  void begin() {}
  void end()   {}
};



template <typename I_>
struct iota : base
{
  typedef I_ I;

  I input;
  mln_psite(I) p;

  unsigned c;

  void init()
  {
    c = 0;
  }

  void next()
  {
    input(p) = ++c;
  }
};



template <typename I_, typename A, typename W>
struct accu_win : base
{
  typedef I_ I;

  I input;
  W win;
  mln_psite(I) p;
  mln_qiter(W) q;

  mln_accu_with(A, mln_value(I)) a;

  accu_win(const I& input,
	   const W& win)
    : input(input),
      win(win),
      p(),
      q(win, p)
  {
  }

  void next()
  {
    a.init();
    for_all(q) if (input.has(q))
      a.take(input(q));
    std::cout << a << ' ';
  }

  void final()
  {
    std::cout << std::endl;
  }
};


// generic(accu, ima)


namespace mln
{

  template <typename I>
  void line_inc(const I& ima, unsigned half)
  {
    box2d b = ima.domain();
    unsigned nrows = b.nrows(), ncols = b.ncols();
  
    for (unsigned row = 0; row < nrows; ++row)
      {
	unsigned col = 0;

	std::cout << std::endl
		  << "START : "
		  << std::endl
		  << std::endl;

	for (unsigned c = 0; c <= half; ++c)
	  std::cout << " take " << ima.at(row, c) << " ";
	std::cout << std::endl;

	std::cout << " -->  OK at " << ima.at(row, col) << std::endl;
	// ok (row, col) at line start


	std::cout << std::endl
		  << "BEGIN : "
		  << std::endl
		  << std::endl;

	unsigned col_plus = half;

	while (col < half)
	  {
	    ++col;
	    ++col_plus;
	    std::cout << " take " << ima.at(row, col_plus) << " ";
	    std::cout << " -->  OK at " << ima.at(row, col) << std::endl;
	  }

	std::cout << std::endl
		  << "MIDDLE : "
		  << std::endl
		  << std::endl;

	int col_minus = -1;

	while (col < ncols - half - 1)
	  {
	    ++col;
	    ++col_plus;
	    ++col_minus;
	    std::cout << " take "   << ima.at(row, col_plus) << " ";
	    std::cout << " untake " << ima.at(row, col_minus) << " ";
	    std::cout << " -->  OK at " << ima.at(row, col) << std::endl;
	    // ok (row, col) at line middle
	  }
	std::cout << std::endl;


	std::cout << std::endl
		  << "END : "
		  << std::endl
		  << std::endl;

	while (col < ncols - 1)
	  {
	    ++col;
	    ++col_minus;
	    std::cout << " untake " << ima.at(row, col_minus) << " ";
	    std::cout << " -->  OK at " << ima.at(row, col) << std::endl;
	  }
	std::cout << std::endl;

	abort();
      }
  }

} // mln


int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(3, 12, 0);
  debug::iota(ima);
  debug::println(ima);


  line_inc(ima, 2);

//   typedef window2d W;
//   W win;
//   win.insert(dpoint2d(0,-1));
//   win.insert(dpoint2d(0, 0));
//   win.insert(dpoint2d(0, 1));
  
//   accu_win<I, accu::min, W> f(ima, win);
//   canvas::browsing::fwd(f);

//   canvas::browsing::row_fwd(f);

//   iota<I> f;
//   f.input = ima;
//   canvas::browsing::col_fwd(f);
//   debug::println(ima);

}
