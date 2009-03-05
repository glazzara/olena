# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/image/image_if.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>
# include <mln/fun/p2b/chess.hh>

# include <mln/border/fill.hh>
# include <mln/core/var.hh>
# include <mln/core/routine/extend.hh>
# include <mln/pw/all.hh>


template <typename It, typename S>
void ch_target(It it, const S& s)
{
  it.change_target(s);
}


template <typename I, typename N>
void browse(const I& ima, const N& nbh)
{
  // Relies on "ima.has(p)".
  mln_fwd_piter(I) p(ima.domain());
  mln_fwd_niter(N) n(nbh, p);
  for_all(p)
  {
    std::cout << ima(p) << ": ";
    for_all(n)
      if (ima.has(n))
	std::cout << ima(n) << ' ';
    std::cout << std::endl;
  }
}

template <typename I, typename N>
void browse_domain(const I& ima, const N& nbh)
{
  // Relies on "ima.domain().has(p)".
  mln_fwd_piter(I) p(ima.domain());
  mln_fwd_niter(N) n(nbh, p);
  for_all(p)
  {
    std::cout << ima(p) << ": ";
    for_all(n)
      if (ima.domain().has(n))
	std::cout << ima(n) << ' ';
    std::cout << std::endl;
  }
}

template <typename I>
void algo(const I& input)
{
  using namespace mln;
  mln_ch_value(I, float) output;
  initialize(output, input);
}



int main()
{
  using namespace mln;

  trace::quiet = false;

  typedef image2d<unsigned> I;
  I ima(3, 3, 1);
  // FIXME: border::fill(ima, 0);
  debug::iota(ima);

  mln_VAR(ima_, ima | fun::p2b::chess());

  debug::println(ima);
  debug::println(ima_);

  trait::image::print(ima_);
  std::cout << std::endl;

  typedef p_if<box2d, fun::p2b::chess> S;
  ch_target(mln_fwd_piter_(S)(), ima_.domain());

  // mln_VAR(ima_e, extend(ima_, pw::value(ima)));
  // mln_VAR(ima_e, extend(ima_, 8));
  mln_VAR(ima_e, extend(ima_, ima));
  debug::println(ima_e);
  browse(ima_e, c4());

  algo(ima_e);
}
