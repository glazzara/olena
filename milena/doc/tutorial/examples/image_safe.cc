# include <mln/core/image2d.hh>
# include <mln/core/site_set/line2d.hh>
# include <mln/core/safe.hh>
# include <mln/debug/println.hh>
# include <mln/level/fill.hh>
# include <mln/level/paste.hh>
# include <mln/pw/all.hh>



int main()
{
  using namespace mln;

  typedef image2d<int> I;
  I ima(3, 3);
  level::fill(ima, 0);
  
  safe_image<I> ima_(ima);
  level::paste(pw::cst(8) | p_line2d(make::point2d(-1,-1),
				     make::point2d( 3, 3)),
	       ima_);
  debug::println(ima);
}
