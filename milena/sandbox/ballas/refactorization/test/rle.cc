// # include <rle_pset.hh>
# include <iostream>

# include <point2d.hh>
//# include <p_run.hh>
//# include <rle_psite.hh>
# include <rle_image.hh>


int main()
{
  using namespace mln;

  typedef point2d<int> point;

  point p1(5, 5), p2(12, 2), p3(7, 6);
  p_run<point> prun1(p1, 5);
  p_run<point> prun2(p2, 12);
  p_run<point> prun3(p3, 4);

  rle_pset<point> pset;
//   pset.insert(prun1);
//   pset.insert(prun2);
//   pset.insert(prun3);

//   rle_pset<point>::piter p(pset);
//   for (p.start(); p.is_valid(); p.next())
//     std::cout << (point) p << std::endl;

  rle_image<point, int> ima;

  ima.insert(prun1, 1);
  ima.insert(prun2, 2);
  ima.insert(prun3, 3);

  rle_image<point, int>::piter p(ima.domain());
  for (p.start(); p.is_valid(); p.next())
    std::cout << ima(p) << std::endl;

}

