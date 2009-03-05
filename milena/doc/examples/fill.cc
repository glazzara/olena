#include <mln/essential/2d.hh>

// \{
template <typename I>
void fill(I& ima, mln_value(I) v)
{
  mln_piter(I) p(ima.domain());
  for_all(p)
    ima(p) = v;
}
// \}

int main()
{
}
