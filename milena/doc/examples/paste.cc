#include <mln/core/image/image2d.hh>

// \{
template <typename I, typename J>
void paste(const I& data, J& dest)
{
  mln_piter(I) p(data.domain());
  for_all(p)
  dest(p) = data(p);
}
// \}

int main()
{
}
