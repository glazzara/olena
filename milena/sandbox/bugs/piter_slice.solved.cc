/*
** Creating a piter onb a slice causes bugs. The iterator takes
** erroneous values.
*/

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u12.hh>

#include <mln/util/array.hh>



int main(int, char*argv[])
{
  using namespace mln;
  using value::int_u12;

  typedef image2d<int_u12> I;

  image3d<int_u12> ima(15, 25, 25);
  image2d<util::array<int_u12> > ima_arr(ima.nrows(), ima.ncols());

  box2d b = slice(ima, 0).domain();
  mln_piter_(I) p(b); // b is not a temporary object, so that's ok

  for (int i = 0; i < ima.nslices(); ++i)
  {
    I tmp_slice = duplicate(slice(ima, i));
    for_all(p)
    {
      ima_arr(p).append(tmp_slice(p));
    }
  }
}
