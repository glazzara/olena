/// Do not compile


# include <mln/algebra/vec.hh>
int main()
{
  using namespace mln::algebra;
  vec<3, float> a, b;

  // Do NOT work
  bool c = (a != b);

  // Work
  c = !(a == b);

  // Work
  c = (a == b);
}

