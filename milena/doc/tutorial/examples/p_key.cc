# include <mln/core/image/image2d.hh>
# include <mln/core/site_set/p_key.hh>
# include <mln/debug/println.hh>
# include <mln/level/fill.hh>

# include <mln/fun/v2v/inc.hh>
# include <mln/fun/v2v/dec.hh>


template <typename S>
void picture(const S& s)
{
  using namespace mln;

  image2d<char> ima(5, 5);
  level::fill(ima, '-');
  
  mln_piter(S) p(s);
  for_all(p)
    ima(p) = '0' + s.key(p);

  debug::println(ima);
}



int main()
{
  using namespace mln;
  
  // FIXME: Do not work with 'char' as a key!
  typedef p_key<unsigned, point2d> S;
  S s;

  {
    point2d p(1,1);
    s.insert(p.row(), p);
    mln_assertion(s.has(p));

    dpoint2d dp[] = { right, right, down, down, left, left, up };
    for (unsigned i = 0; i < 7;  ++i)
      p += dp[i], s.insert(p.row(), p);

    picture(s);

    std::cout << s.keys() << std::endl;
    std::cout << s << std::endl;
  }

  
  std::cout << "insert(2, (1,1))" << std::endl;
  s.insert(2, point2d(1,1));
  std::cout << s << std::endl;

  std::cout << "insert(5, (1,1))" << std::endl;
  s.insert(5, point2d(1,1));
  std::cout << s << std::endl;

  std::cout << "inc keys" << std::endl;
  s.change_keys(fun::v2v::inc<unsigned>());
  std::cout << s << std::endl;

  std::cout << "dec keys" << std::endl;
  s.change_keys(fun::v2v::dec<unsigned>());
  std::cout << s << std::endl;

  std::cout << "remove (0,0)" << std::endl;
  s.remove(point2d(0,0));
  std::cout << s << std::endl;

  std::cout << "remove (1,2)" << std::endl;
  s.remove(point2d(1,2));
  std::cout << s << std::endl;

  std::cout << "remove (1,3)" << std::endl;
  s.remove(point2d(1,3));
  std::cout << s << std::endl;

  std::cout << "remove key 0" << std::endl;
  s.remove_key(0);
  std::cout << s << std::endl;

  std::cout << "change key 2 -> 5" << std::endl;
  s.change_key(2, 5);
  std::cout << s << std::endl;

  std::cout << "remove key 3" << std::endl;
  s.remove_key(3);
  std::cout << s << std::endl;

  picture(s);

  std::cout << "clear" << std::endl;
  s.clear();
  std::cout << s << std::endl;
}
