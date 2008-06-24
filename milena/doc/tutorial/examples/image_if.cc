# include <mln/core/image2d.hh>
# include <mln/core/image_if.hh>
# include <mln/core/alias.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>
# include <mln/fun/p2b/chess.hh>


template <typename It, typename S>
void ch_target(It it, const S& s)
{
  it.change_target(s);
}


int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(3, 5);
  debug::iota(ima);

  debug::println(ima);
  debug::println(ima | fun::p2b::chess);

  trait::image::print(ima | fun::p2b::chess);

  typedef pset_if<box2d, fun::p2b::chess_t> S;

  ch_target(mln_fwd_piter_(S)(),
	    (ima | fun::p2b::chess).domain());
}
