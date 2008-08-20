# include <mln/core/image2d.hh>
# include <mln/core/p_array.hh>
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
  Arr1 arr1;

  {
    point2d p(1,1);
    arr1.append(p);
    p = unproxy_rec(p);
  }

  {
    mln_psite_(Arr1) p(arr1, 0);
    std::cout << p << ' ' << arr1[0] << std::endl;
    point2d& v = unproxy_rec(p);
    std::cout << &(arr1[0]) << ' ' << &(unproxy_rec(p)) << std::endl;
    unproxy_rec(p).row() = 12;
    mln_assertion(arr1[0].row() != 12); // FIXME: that's normal!

    std::cout << p.get_subject() << std::endl;
    std::cout << p.to_site() << std::endl;
  }

//   {
//     mln_piter_(Arr1) p(arr1);
//     p.start();
//     std::cout << p.get_subject() << std::endl;
//     std::cout << p.to_site() << std::endl;
//   }

//   {
//     mln_psite_(Arr1) p(arr1, 0);
//     typedef typeof(mln_psite_(Arr1)::q_subject) TT;
//     TT var;
//   }


  typedef p_array<mln_psite_(Arr1)> Arr2;

//   {

//     Arr2 arr2;

//     // Fill arr2 from arr1 contents.
//     {
//       mln_piter_(Arr1) p(arr1);
//       for_all(p)
// 	if (p.row() % 2 && p.col() % 2)
// 	  arr2.append(p);
//     }

//     std::cout << "arr2 = " << arr2 << std::endl;
//     picture(arr2);

//     // Display indices.
//     {
//       mln_piter_(Arr2) p(arr2);
//       for_all(p)
// 	{
//  	  mln_assertion(make::point2d(p.row(), p.col()) == p);
// 	  std::cout << "point " << p << ": #"
// 		    << index_of_in(p, arr2) << " in arr2, #"
// 		    << index_of_in(p, arr1)  << " in arr1" << std::endl;
// 	}
//     }

//     mln_invariant(arr2 < arr1);

//     {
//       mln_piter_(Arr1) p1(arr1);
//       mln_piter_(Arr2) p2(arr2);
//       for_all_2(p1, p2)
// 	{
//  	  mln_assertion(p2 == p1); // same as: p2.to_site() == p1.to_site()
// 	  p1.next(); // p1 goes twice fast as p2.
// 	}
//     }

//   }

}
