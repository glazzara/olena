# include <mln/core/image2d.hh>
# include <mln/core/window2d.hh>


int main()
{
  using namespace mln;

  image2d<char> ima(2, 3);
  mln_invariant(ima.nsites() == 6);

  window2d win;
  win
    .insert(-1, 0)
    .insert(0, -1)
    .insert(-1,-1);
  std::cout << win << std::endl;
}
