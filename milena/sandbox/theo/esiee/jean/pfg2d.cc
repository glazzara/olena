
#include <mln/core/var.hh>
#include <mln/make/double_neighb2d.hh>




inline
bool chess(const mln::point2d& p)
{
  return p.row() % 2 == p.col() % 2;
}


int main()
{
  using namespace mln;
  
  bool u11[] =  { 1, 1, 0,
		  1, 0, 1,
		  0, 1, 1 };

  bool u1_1[] = { 0, 1, 1,
		  1, 0, 1,
		  1, 1, 0 };

  mln_VAR(nbh, make::double_neighb2d(chess, u11, u1_1));

  {
    point2d p(0, 0);
    std::cout << "N( " << p << " ) = { ";
    mln_niter_(nbh_t) n(nbh, p);
    for_all(n)
      std::cout << n << ' ';
    std::cout << " }" << std::endl;

    // N( (0, 0) ) = { (-1, -1) (-1, 0) (0, -1) (0, 1) (1, 0) (1, 1) }
  }

  {
    point2d p(0, 1);
    std::cout << "N( " << p << " ) = { ";
    mln_niter_(nbh_t) n(nbh, p);
    for_all(n)
      std::cout << n << ' ';
    std::cout << " }" << std::endl;

    // N( (0, 1) ) = { (-1, 1) (-1, 2) (0, 0) (0, 2) (1, 0) (1, 1) }
  }

}
