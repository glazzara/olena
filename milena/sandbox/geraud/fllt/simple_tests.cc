#include "fllt_test.hh"

namespace mln
{
    template <typename P, typename V>
    void
    draw_tree(const image2d<V>& ima,
	      fllt_tree(P, V)& tree)
    {
      p_array<P> tmp;

      fllt_branch_iter_ind(P, V) p(tree.main_branch());
      for_all(p)
      {
	std::cout << "region mere : " << (*p).parent() << std::endl;
	std::cout << "               ^" << std::endl;
	std::cout << "               |" << std::endl;
	std::cout << "region : " << &*p
		  << " value = " << (*p).elt().value << std::endl
		  << " from " << ((*p).elt().set_id == lower<V>::id ?
				  "lower" : "upper") << " level set."
		  << std::endl
		  << " nsites = " << (*p).elt().nsites << std::endl
		  << " holes = " << (*p).elt().holes << std::endl;

	std::cout << std::endl;

	tmp.append((*p).elt().points);

	fllt_branch_iter_ind(P, V) n(fllt_branch(P, V)(tree, *p));
	for_all(n)
	  tmp.append((*n).elt().points);

	if ((*p).elt().points.nsites() > 0)
	  debug::println(ima | tmp);
	tmp.clear();

	std::cout << std::endl;
      }
    }

} // End of namespace mln.

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef fllt_tree(point2d, int_u8) tree_type;

//   int vs[8][9] = { {2,2,2,2,2,2,2,2,2},
// 		   {2,2,2,2,2,2,2,2,2},
// 		   {2,1,1,1,1,1,1,1,2},
// 		   {2,1,2,2,1,0,0,1,2},
// 		   {2,1,2,2,1,0,4,1,2},
// 		   {2,1,2,2,1,0,0,1,2},
// 		   {2,1,1,1,1,1,1,1,2},
// 		   {2,2,2,2,2,2,2,2,2} };


//   int vs[3][4] = { {1,8,2,4},
// 		   {5,6,3,8},
// 		   {9,14,0,12} };

  int vs[3][4] = { {1,1,2,1},
		   {1,1,2,2},
		   {1,1,2,1} };

//   int vs[8][9] = { {0,0,0,0,0,0,0,0,0},
// 		   {0,0,0,0,0,0,0,0,0},
// 		   {0,1,1,1,1,1,1,1,0},
// 		   {0,1,0,0,1,5,5,1,1},
// 		   {0,1,0,0,1,5,5,1,0},
// 		   {0,1,0,0,1,5,5,1,0},
// 		   {0,1,1,1,1,1,1,1,0},
// 		   {0,0,0,0,0,0,0,0,0} };

  image2d<int> lena_(make::image2d(vs));
  image2d<int_u8> lena(lena_.domain());
  data::fill(lena, lena_);

  tree_type tree = fllt(lena);
  draw_tree(lena, tree);
}
