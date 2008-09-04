#include <mln/core/image/image2d.hh>

# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>


int main()
{
  mln::image2d<int> i(5 / 3,5);

  return mln::geom::nrows(i);
}
