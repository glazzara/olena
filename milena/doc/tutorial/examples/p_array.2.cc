# include <mln/core/image2d.hh>
# include <mln/core/p_array.hh>
# include <mln/core/window.hh>

# include <mln/debug/println.hh>
# include <mln/level/fill.hh>


template <typename A>
mln::image2d<char> picture(const A& arr)
{
  using namespace mln;

  image2d<char> ima(5, 5);
  level::fill(ima, '-');
  
  unsigned i = 0;
  mln_piter(A) p(arr);
  for_all(p)
    ima(p) = '0' + i++;

  debug::println(ima);
  return ima;
}


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


int main()
{
  using namespace mln;

  typedef p_array<point2d> Arr;
  Arr arr;

  point2d p(1,1);
  arr.append(p);
  dpoint2d dp[] = { right, right, down, down, left, left, up };
  for (unsigned i = 0; i < 7;  ++i)
    p += dp[i], arr.append(p);

  std::cout << "arr = " << arr << std::endl;
  image2d<char> ima = picture(arr);

  typedef window<util::dindex> W;
  W win;
  win.insert(-1).insert(0).insert(1);
  std::cout << "win = " << win << std::endl
	    << std::endl;

  mln_assertion(win == win);

  util::index i;
  mln_qiter_(W) j(win, i);
  int n = arr.nsites();
  for (i = 0; i < n; i = i + 1)
    {
      std::cout << "i=" << i << ": ";
      for_all(j)
	if (arr.has(j))
	  {
	    mln_invariant( ima(arr[j]) == '0' + j.to_site());
	    std::cout << j << ' ';
	  }
	else
	  std::cout << "- ";
      std::cout << std::endl;
    }

  // FIXME: j does NOT convert to int because index is NOT a proxy
  // NOTA: make index be a proxy, so equip it with every op it needs.

  for (i = 0; i < n; i = i + 1)
    {
      std::cout << "i=" << i << ": ";
      for_all(j)
	std::cout << arr[(int(j.to_site()) + n) % n] << ' ';
      std::cout << std::endl;
    }

}
