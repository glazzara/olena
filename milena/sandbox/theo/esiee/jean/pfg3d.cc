#include <mln/core/var.hh>
#include <mln/core/image/image3d.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

# include <mln/core/alias/window3d.hh>
# include <mln/core/alias/neighb3d.hh>
# include <mln/win/multiple.hh>
# include <mln/convert/to.hh>


namespace mln
{

  template <typename A, unsigned S>
  inline
  neighb< win::multiple<window3d, unsigned(*)(A)> >
  make_pfg_neighb3d(unsigned (*test)(A),
		    bool const (&case_0)[S],
		    bool const (&case_1)[S],
		    bool const (&case_2)[S],
		    bool const (&case_3)[S])
    {
      typedef win::multiple<window3d, unsigned(*)(A)> W;
      W wm(test);
      wm.set_window(0, convert::to<window3d>(case_0));
      wm.set_window(1, convert::to<window3d>(case_1));
      wm.set_window(2, convert::to<window3d>(case_2));
      wm.set_window(3, convert::to<window3d>(case_3));
      neighb<W> tmp(wm);
      return tmp;
    }


  // pfg3d is "3D perfect fusion grid" for short. 
  unsigned pfg3d_case_number(const point3d& p)
  {
    int id = 2 * (p.row() % 2 == p.col() % 2) + (p.row() % 2 == p.sli() % 2);
    mln_invariant(id >= 0 && id <= 3);
    return id;
  }

  template <typename W>
  neighb<W>
  make_neighb(const Window<W>& win)
  {
    neighb<W> tmp(exact(win));
    return tmp;
  }


  template <typename I, typename N>
  void
  show_nbh(const I& ima, const mln_psite(I)& p, const N& nbh)
  {
    std::cout << ima(p) << ": " << ' ';
    mln_niter(N) n(nbh, p);
    for_all(n)
      std::cout << ima(n) << ' ';
    std::cout << std::endl;
  }


} // mln



int main()
{
  using namespace mln;

  box3d b = make::box3d(-1,-1,-1, +1,+1,+1);
  image3d<unsigned> ima(b);
  debug::iota(ima);
  debug::println(ima);


  bool u111 [] = {   1, 1, 0, // case 3
		    1, 1, 0,
		   0, 0, 0,

		     1, 1, 0,
		    1, 0, 1,
		   0, 1, 1,

		     0, 0, 0,
		    0, 1, 1,
		   0, 1, 1 };

  bool u1_11[] = {   0, 1, 1, // case 1
		    0, 1, 1,
		   0, 0, 0,

		     0, 1, 1,
		    1, 0, 1,
		   1, 1, 0,

		     0, 0, 0,
		    1, 1, 0,
		   1, 1, 0 };

  bool u11_1[] = {   0, 0, 0, // case 2
		    0, 1, 1,
		   0, 1, 1,

		     1, 1, 0,
		    1, 0, 1,
		   0, 1, 1,

		     1, 1, 0,
		    1, 1, 0,
		   0, 0, 0 };
  
  bool u1_1_1[] = {  0, 0, 0, // case 0
		    1, 1, 0,
		   1, 1, 0,

		     0, 1, 1,
		    1, 0, 1,
		   1, 1, 0,

		     0, 1, 1,
		    0, 1, 1,
		   0, 0, 0 };

  mln_VAR( nbh, make_pfg_neighb3d(pfg3d_case_number,
				  u1_1_1, // 0
				  u1_11,  // 1
				  u11_1,  // 2
				  u111    // 3
				  ) );

  show_nbh(ima, point3d(0,0,0), nbh);
}
