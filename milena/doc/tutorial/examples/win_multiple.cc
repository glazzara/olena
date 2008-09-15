# include <mln/core/image/image2d.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/win/multiple.hh>

// # include <mln/border/fill.hh>
# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/convert/to.hh>



template <typename I, typename W>
void browse(const I& ima, const W& win)
{
  mln_fwd_piter(I) p(ima.domain());
  mln_fwd_qiter(W) q(win, p);
  for_all(p)
  {
    std::cout << ima(p) << ": ";
    for_all(q)
      if (ima.has(q))
	std::cout << ima(q) << ' ';
    std::cout << std::endl;
  }
}


struct row_oddity
{
  unsigned operator()(const mln::point2d& p) const
  {
    return p.row() % 2;
  }
};


int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(3, 3, 0); // 1);
  // border::fill(ima, 0);
  debug::iota(ima);
  debug::println(ima);

  win::multiple<window2d, row_oddity> w;

  bool vert[] = { 0, 1, 0,
		  0, 0, 0,
		  0, 1, 0 };

  convert::to<window2d>(vert);

//   w.set_window(0, convert::to<window2d>(vert));
//   bool horiz[] = { 0, 0, 0,
// 		   1, 0, 1,
// 		   0, 0, 0 };
//   w.set_window(1, convert::to<window2d>(horiz));
//   mln_assertion(w.size() == 2);

//   browse(ima, w);
}
