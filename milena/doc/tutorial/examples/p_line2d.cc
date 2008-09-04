# include <mln/core/image2d.hh>
# include <mln/pw/all.hh>

# include <mln/core/site_set/line2d.hh>
# include <mln/core/site_set/p_set.hh>

# include <mln/core/sub_image.hh>
# include <mln/level/fill.hh>
# include <mln/debug/println.hh>



# include <mln/core/image/instant.hh>



int main()
{
  using namespace mln;

  image2d<char> ima(6, 11);
  level::fill(ima, '.');

  p_line2d line(make::point2d(4,10),
		make::point2d(1,1));
  std::cout << line << std::endl;

  level::fill(inplace(ima | line), 'x');
  debug::println(ima);

  p_set<point2d> s;
  s += line;
  mln_assertion(s.has(line.begin()) && s.has(line.end()));
  std::cout << s << std::endl;

  debug::println(pw::cst('o') | line);
  // ...


  {
    typedef image2d<char> I;
    instant_<I> ima_(ima);
    debug::println(ima_);
  }
}
