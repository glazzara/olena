# include <mln/core/image2d.hh>
# include <mln/debug/println.hh>
# include <mln/core/p_mutable_array_of.hh>
# include <mln/core/p_run.hh>



namespace mln
{


  template <typename I, typename S>
  void
  convert_to_runs(const Image<I>& input_, Site_Set<S>& s_)
  {
    typedef mln_site(I) P;
    mlc_is(mln_element(S), p_run<P>)::check();

    const I& input = exact(input_);
    S& s = exact(s_);
    mln_precondition(input.has_data());

    mln_fwd_piter(I) p(input.domain());
    p.start();
    for (;;)
      {
	// Skip background.
	while (p.is_valid() && input(p) == false)
	  p.next();
	if (! p.is_valid()) // The end.
	  break;
	mln_invariant(input(p) == true);
	P start = p, q;
	// Go to run end.
	do
	  {
	    q = p;
	    p.next();
	  }
	while (p.is_valid() && input(p) == true && p == q + right);
	s.insert(p_run<P>(start, q));
      }
  }

} // mln



template <typename S>
void display(const S& s)
{
  mln_fwd_piter(S) p(s);
  for_all(p)
    std::cout << p << ' ';
  std::cout << std::endl;
}


int main()
{
  using namespace mln;

  bool vals[6][5] = {
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0}
  };
  image2d<bool> ima = make::image2d(vals);
  debug::println(ima);

  typedef p_mutable_array_of< p_run<point2d> > Runs;
  Runs rs;
  convert_to_runs(ima, rs);
  std::cout << rs << std::endl;

  display(rs);
}
