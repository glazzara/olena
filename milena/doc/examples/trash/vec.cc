# include <mln/convert/to.hh>
# include <mln/core/alias/point2d.hh>



int main()
{
  using namespace mln;

  algebra::vec<2,float> v;
  v[0] = 5.14;
  v[1] = 1.99;
  std::cout << convert::to<point2d>(v) << std::endl;
}
