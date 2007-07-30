#include <mln/core/internal/run_pset.hh>
#include <mln/core/image2d_b.hh>
#include <iostream>

using namespace mln;

template <typename Pset>
void
parc(const Pset& pset)
{
  typename Pset::fwd_piter it_(pset);

  for_all(it_)
  {
    std::cout << (typename Pset::point) it_ << std::endl;
  }

  typename Pset::bkd_piter rit_(pset);

  for_all(rit_)
  {
    std::cout << (typename Pset::point) rit_ << std::endl;
  }
}


int
main()
{
  point2d p, q, r;
  p = make::point2d(2, 4);
  q = make::point2d(18, 42);
  r = make::point2d(50, 76);

  // Psite declaration
  run_psite<point2d>  site(p, 5, 0);
  run_psite<point2d>  site2(r, 40, 0);

  // Pset test
  internal::run_pset_<point2d> ps;

  ps.insert(p, 7);
  assert(ps.npoints() == 7);

  ps.insert(q, 42);
  assert(ps.npoints() == 49);

  assert(ps.has(site));
  assert(!ps.has(site2));

  ps.insert(r, 14);
  assert(!ps.has(site2));

  // FIXME bbox test

  parc(ps);
  return 0;
}
