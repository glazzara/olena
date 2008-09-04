# include <mln/core/image2d.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/core/site_set/p_image.hh>
# include <mln/debug/println.hh>
# include <mln/level/fill.hh>


template <typename A>
void picture(const A& arr)
{
  using namespace mln;

  image2d<char> ima(5, 5);
  level::fill(ima, '-');
  
  unsigned i = 0;
  mln_piter(A) p(arr);
  for_all(p)
    ima(p) = '0' + i++;

  debug::println(ima);
}



int main()
{
  using namespace mln;
  
  typedef p_array<point2d> Arr;
  Arr arr;

  {
    point2d p(1,1);
    arr.append(p);
    dpoint2d dp[] = { right, right, down, down, left, left, up };
    for (unsigned i = 0; i < 7;  ++i)
      p += dp[i], arr.append(p);

    std::cout << "arr = " << arr << std::endl;
    picture(arr);
  }

  typedef image2d<bool> I;
  I ima(5, 5);
  p_image<I> s(ima);
  s += arr;
  std::cout << "ima = " << s << std::endl;
  picture(s);

  debug::println(s.image_hook_());
}
