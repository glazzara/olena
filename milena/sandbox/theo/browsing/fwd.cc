# include <mln/core/image/image2d.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/accu/min_h.hh>
# include <mln/accu/max_h.hh>
# include <mln/opt/at.hh>

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

  //   template <typename I>
  //   void line_inc(const I& ima, unsigned half)
  //   {
  //     box2d b = ima.domain();
  //     unsigned nrows = b.nrows(), ncols = b.ncols();
  
  //     for (unsigned row = 0; row < nrows; ++row)
  //       {
  // 	unsigned col = 0;

  // 	std::cout << std::endl
  // 		  << "START : "
  // 		  << std::endl
  // 		  << std::endl;

  // 	for (unsigned c = 0; c <= half; ++c)
  // 	  std::cout << " take " << opt::at(ima, row, c) << " ";
  // 	std::cout << std::endl;

  // 	std::cout << " -->  OK at " << opt::at(ima, row, col) << std::endl;
  // 	// ok (row, col) at line start


  // 	std::cout << std::endl
  // 		  << "BEGIN : "
  // 		  << std::endl
  // 		  << std::endl;

  // 	unsigned col_plus = half;

  // 	while (col < half)
  // 	  {
  // 	    ++col;
  // 	    ++col_plus;
  // 	    std::cout << " take " << opt::at(ima, row, col_plus) << " ";
  // 	    std::cout << " -->  OK at " << opt::at(ima, row, col) << std::endl;
  // 	  }

  // 	std::cout << std::endl
  // 		  << "MIDDLE : "
  // 		  << std::endl
  // 		  << std::endl;

  // 	int col_minus = -1;

  // 	while (col < ncols - half - 1)
  // 	  {
  // 	    ++col;
  // 	    ++col_plus;
  // 	    ++col_minus;
  // 	    std::cout << " take "   << opt::at(ima, row, col_plus) << " ";
  // 	    std::cout << " untake " << opt::at(ima, row, col_minus) << " ";
  // 	    std::cout << " -->  OK at " << opt::at(ima, row, col) << std::endl;
  // 	    // ok (row, col) at line middle
  // 	  }
  // 	std::cout << std::endl;


  // 	std::cout << std::endl
  // 		  << "END : "
  // 		  << std::endl
  // 		  << std::endl;

  // 	while (col < ncols - 1)
  // 	  {
  // 	    ++col;
  // 	    ++col_minus;
  // 	    std::cout << " untake " << opt::at(ima, row, col_minus) << " ";
  // 	    std::cout << " -->  OK at " << opt::at(ima, row, col) << std::endl;
  // 	  }
  // 	std::cout << std::endl;

  // 	abort();
  //       }
  //   }



  /*

  template <typename I>
  void line_inc(const I& ima,
		const mln_site(I)& pstart, unsigned len,
		unsigned half)
  {
    unsigned row = pstart.row();
    const unsigned col_start = pstart.col();
    const unsigned col_last = col_start + len - 1;

    std::cout << std::endl
	      << "START : "
	      << std::endl
	      << std::endl;

    unsigned col = col_start;

    for (unsigned c = 0; c <= half && c < len; ++c)
      std::cout << " take " << opt::at(ima, row, col + c) << " ";
    std::cout << std::endl;

    std::cout << " -->  OK at " << opt::at(ima, row, col) << std::endl;
    // ok (row, col) at line start

    if (col == col_last)
      return;

    std::cout << std::endl
	      << "BEGIN : "
	      << std::endl
	      << std::endl;

    unsigned col_plus = col_start + half;

    while (col < col_start + half && col < col_last)
      {
	++col;
	++col_plus;
	if (col_plus <= col_last)
	  std::cout << " take " << opt::at(ima, row, col_plus) << " ";
	std::cout << " -->  OK at " << opt::at(ima, row, col) << std::endl;
      }

    if (col == col_last)
      return;

    std::cout << std::endl
	      << "MIDDLE : "
	      << std::endl
	      << std::endl;

    int col_minus = col_start - 1;


    while (col < col_last - half)
      {
	++col;
	++col_plus;
	++col_minus;
	mln_invariant(col_plus >= col_start && col_plus <= col_last);
	std::cout << " take "   << opt::at(ima, row, col_plus) << " ";
	mln_invariant(col_minus >= col_start && col_minus <= col_last);
	std::cout << " untake " << opt::at(ima, row, col_minus) << " ";
	std::cout << " -->  OK at " << opt::at(ima, row, col) << std::endl;
	// ok (row, col) at line middle
      }
    std::cout << std::endl;

    if (col == col_last)
      return;

    std::cout << std::endl
	      << "END : "
	      << std::endl
	      << std::endl;

    while (col < col_last)
      {
	++col;
	++col_minus;
	mln_invariant(col_minus >= col_start && col_minus <= col_last);
	std::cout << " untake " << opt::at(ima, row, col_minus) << " ";
	std::cout << " -->  OK at " << opt::at(ima, row, col) << std::endl;
      }
    std::cout << std::endl;

  }

  */



  /*

  template <typename I>
  void line_inc(unsigned dir,
		const I& ima,
		const mln_site(I)& p_start, unsigned len,
		unsigned half)
  {
    typedef mln_site(I) P;
    const unsigned start = p_start[dir];
    const unsigned last = start + len - 1;

    std::cout << std::endl
	      << "START : "
	      << std::endl
	      << std::endl;

    P p = p_start;

    P q = p;
    for (unsigned c = 0; c <= half && c < len; ++c)
      {
	std::cout << " take " << ima(q) << " ";
	++q[dir];
      }
    std::cout << std::endl;

    std::cout << " -->  OK at " << ima(p) << std::endl;

    def::coord& cur = p[dir];
    if (cur == last)
      return;

    std::cout << std::endl
	      << "BEGIN : "
	      << std::endl
	      << std::endl;

    P p_plus = p_start;
    def::coord& plus = p_plus[dir];
    plus += half;

    while (cur < start + half && cur < last)
      {
	++cur;
	++plus;
	if (plus <= last)
	  std::cout << " take " << ima(p_plus) << " ";
	std::cout << " -->  OK at " << ima(p) << std::endl;
      }

    if (cur == last)
      return;

    std::cout << std::endl
	      << "MIDDLE : "
	      << std::endl
	      << std::endl;

    P p_minus = p_start;
    def::coord& minus = p_minus[dir];
    --minus;

    while (cur < last - half)
      {
	++cur;
	++plus;
	++minus;
	mln_invariant(plus >= start && plus <= last);
	std::cout << " take "   << ima(p_plus) << " ";
	mln_invariant(minus >= start && minus <= last);
	std::cout << " untake " << ima(p_minus) << " ";
	std::cout << " -->  OK at " << ima(p) << std::endl;
	// ok (row, cur) at line middle
      }
    std::cout << std::endl;

    if (cur == last)
      return;

    std::cout << std::endl
	      << "END : "
	      << std::endl
	      << std::endl;

    while (cur < last)
      {
	++cur;
	++minus;
	mln_invariant(minus >= start && minus <= last);
	std::cout << " untake " << ima(p_minus) << " ";
	std::cout << " -->  OK at " << ima(p) << std::endl;
      }
    std::cout << std::endl;

  }

*/


  template <typename I, typename A, typename O>
  void line_inc(unsigned dir,
		const I& input,
		const mln_site(I)& p_start, unsigned len,
		A a, unsigned half,
		O& output)
  {
    typedef mln_site(I) P;
    const unsigned start = p_start[dir];
    const unsigned last = start + len - 1;

    std::cout << std::endl
	      << "START : "
	      << std::endl
	      << std::endl;

    P p = p_start;
    a.init();

    P q = p;
    for (unsigned c = 0; c <= half && c < len; ++c)
      {
	a.take(input(q));
	std::cout << " take " << debug::format(input(q)) << " ";
	++q[dir];
      }
    std::cout << std::endl;

    std::cout << " -->  OK at " << debug::format(input(p)) << std::endl;
    output(p) = a;

    def::coord& cur = p[dir];
    if (cur == last)
      return;

    std::cout << std::endl
	      << "BEGIN : "
	      << std::endl
	      << std::endl;

    P p_plus = p_start;
    def::coord& plus = p_plus[dir];
    plus += half;

    while (cur < start + half && cur < last)
      {
	++cur;
	++plus;
	if (plus <= last)
	  {
	    a.take(input(p_plus));
	    std::cout << " take " << debug::format(input(p_plus)) << " ";
	  }
	std::cout << " -->  OK at " << debug::format(input(p)) << std::endl;
	output(p) = a;
      }

    if (cur == last)
      return;

    std::cout << std::endl
	      << "MIDDLE : "
	      << std::endl
	      << std::endl;

    P p_minus = p_start;
    def::coord& minus = p_minus[dir];
    --minus;

    while (cur < last - half)
      {
	++cur;
	++plus;
	++minus;
	mln_invariant(plus >= start && plus <= last);
	a.take(input(p_plus));
	std::cout << " take " << debug::format(input(p_plus)) << " ";
	mln_invariant(minus >= start && minus <= last);
	a.untake(input(p_minus));
	std::cout << " untake " << debug::format(input(p_minus)) << " ";
	std::cout << " -->  OK at " << debug::format(input(p)) << std::endl;
	output(p) = a;
	// ok (row, cur) at line middle
      }
    std::cout << std::endl;

    if (cur == last)
      return;

    std::cout << std::endl
	      << "END : "
	      << std::endl
	      << std::endl;

    while (cur < last)
      {
	++cur;
	++minus;
	mln_invariant(minus >= start && minus <= last);
	a.untake(input(p_minus));
	std::cout << " untake " << debug::format(input(p_minus)) << " ";
	std::cout << " -->  OK at " << debug::format(input(p)) << std::endl;
	output(p) = a;
      }
    std::cout << std::endl;

  }



} // mln




int main()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;
  I ima(12, 12);
  debug::iota(ima);
  debug::println(ima);


  I out(12, 12);
  point2d p_start(0, 0);
  line_inc(0, // dir
	   ima, p_start, ima.ncols(),
	   accu::min_h<int_u8>(), 4, // half
	   out);

  debug::println(out);


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
