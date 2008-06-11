# include <mln/core/image2d.hh>


int main()
{
  using namespace mln;

  image2d<char> ima(2, 3);
  mln_invariant(ima.nsites() == 6);
}
