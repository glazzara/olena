#include <mln/core/concept/image.hh>
#include <mln/literal/colors.hh>

using namespace mln;

template <typename I>
inline
void
bench_disk_perf(Image<I>& ima_)
{
  I& ima = exact(ima_);

  mln_piter(I) p(ima.domain());
  for_all(p)
    if (p.row() % 16 == 0)
    {
      ima(p) = literal::green;
    }
}
