# include <mln/core/image2d.hh>
# include <mln/core/site_set/p_array.hh>
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
  
  typedef p_array<point2d> Arr1;
  typedef p_array<mln_psite_(Arr1)> Arr2;

  
  {
    mlc_bool(mln_psite_(Arr1)::proxy_level == 1)::check();
    mlc_bool(mln_piter_(Arr1)::proxy_level == 2)::check();
    mlc_bool(mln_psite_(Arr2)::proxy_level == 2)::check();
    mlc_bool(mln_piter_(Arr2)::proxy_level == 3)::check();
  }

  Arr1 arr1;

  {
    point2d p(1,1);
    arr1.append(p);
    dpoint2d dp[] = { right, right, down, down, left, left, up };
    for (unsigned i = 0; i < 7;  ++i)
      p += dp[i], arr1.append(p);

    std::cout << "arr1 = " << arr1 << std::endl;
    picture(arr1);
  }

  {

    Arr2 arr2;

    // Fill arr2 from arr1 contents.
    {
      mln_piter_(Arr1) p(arr1);
      for_all(p)
	if (p.row() % 2 && p.col() % 2)
	  arr2.append(p);
    }

    std::cout << "arr2 = " << arr2 << std::endl;
    picture(arr2);

    // Display indices.
    {
      mln_piter_(Arr2) p(arr2);
      for_all(p)
	{
 	  mln_assertion(make::point2d(p.row(), p.col()) == p);
	  std::cout << "point " << p << ": #"
		    << index_of_in(p, arr2) << " in arr2, #"
		    << index_of_in(p, arr1)  << " in arr1" << std::endl;
	}
      std::cout << std::endl;
    }

    mln_invariant(arr2 < arr1);

    {
      mln_piter_(Arr1) p1(arr1);
      mln_piter_(Arr2) p2(arr2);
      for_all_2(p1, p2)
	{
 	  mln_assertion(p2 == p1); // same as: p2.to_site() == p1.to_site()
	  p1.next(); // p1 goes twice fast as p2.
	}
    }

    {
      // Fun stuff:

      arr1[0] = point2d(0,0);
      picture(arr1);
      picture(arr2);
    }

  }

}
