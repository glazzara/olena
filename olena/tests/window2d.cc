#include <oln/basics2d.hh>
#include <oln/debug/print.hh>
#include <oln/io/pnm.hh>
#include <oln/morpher/add_neighborhood.hh>


namespace test
{

  template <typename I, typename W>
  void run(const oln::abstract::image<I>&  ima,
	   const oln::abstract::window<W>& win)
  {
    oln_piter(I) p(ima.topo());
    oln_qiter(I) q(p, win);
    for_all(p)
      {
	std::cout << unsigned(ima(p)) << ": ";
	for_all(q)
	  std::cout << unsigned(ima(q)) << " ";
	std::cout << std::endl;
      }
  }

}


int main()
{
  using namespace oln;

  image2d<unsigned char> ima = io::load_pgm("../img/lena32.pgm");
  debug::println(ima);

  window2d win;
  win
    .add(dpoint2d(-1, 0))
    .add(dpoint2d(-2, 0));

  test::run(ima, win);
}
