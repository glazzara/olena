# include <mln/core/image/image2d.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/core/site_set/p_array.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>

# include <mln/core/alias/neighb2d.hh>


template <typename I, typename W, typename P>
void picture(const I& ima, const W& win, const P& p)
{
  std::cout << ima(p) << ": ";
  mln_qiter(W) q(win, p);
  for_all(q)
    if (ima.has(q))
      std::cout << ima(q) << ' ';
    else 
      std::cout << "- ";
  std::cout << std::endl;
}


template <typename I, typename W>
void fill_null(I& ima, const W& win)
{
  mln_piter(I) p(ima.domain());
  mln_qiter(W) q(win, p);
  for_all(p)
    for_all(q)
      if (ima.has(q))
        ima(q) = 0;
}



int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(2, 3, 0); // no border
  debug::iota(ima);
  debug::println(ima);
  mln_invariant(ima.nsites() == 6);

  window2d win;
  win
    .insert(-1, 0)
    .insert(0, -1)
    .insert(-1,-1);
  std::cout << win << std::endl;

  {
    bool b[9] = { 1, 1, 0,
		  1, 0, 0,
		  0, 0, 0 };
    bool b2[3][3] = { { 1, 1, 0 },
		      { 1, 0, 0 },
		      { 0, 0, 0 } };
    mln_assertion(make::window2d(b) == make::window2d(b2));
    mln_assertion(make::window2d(b) == win);
  }

  {
    mln_fwd_piter_(I) p(ima.domain());
    for_all(p)
      picture(ima, win, p);
  }

  {
    typedef p_array<point2d> A;
    A arr;
    arr += ima.domain();
    mln_fwd_piter_(A) p(arr);
    for_all(p)
      picture(ima, win, p);

    // FIXME: Move this new test in a separate file.
    mln_psite_(A) c(arr, 0);
    window2d it; it.insert(0,0);
    mln_qiter_(window2d) q(it, c);
    q.start();
    q.to_site() == c.to_site();
    c.inc_index();
    mln_assertion(q.update().to_site() == c.to_site());
  }

  fill_null(ima, win);
}
